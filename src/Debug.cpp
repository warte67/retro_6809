/*** Device.cpp *******************************************
 *      _____       _                                   
 *     |  __ \     | |                                  
 *     | |  | | ___| |__  _   _  __ _   ___ _ __  _ __  
 *     | |  | |/ _ \ '_ \| | | |/ _` | / __| '_ \| '_ \ 
 *     | |__| |  __/ |_) | |_| | (_| || (__| |_) | |_) |
 *     |_____/ \___|_.__/ \__,_|\__, (_)___| .__/| .__/ 
 *                               __/ |     | |   | |    
 *                              |___/      |_|   |_| 
 *  
 ************************************/

#include "Debug.hpp"
#include "Bus.hpp"
#include "Memory.hpp"



/****************
 * Read / Write *
 ***************/ 

Byte Debug::OnRead(Word offset) 
{ 
    Byte data = IDevice::OnRead(offset);
    // DBG_BRK_ADDR          = 0xFE05,   // (Word) Address of current breakpoint
    // DBG_FLAGS             = 0xFE07,   // (Byte) Debug Specific Hardware Flags:
    //           - bit 7: Debug Enable
    //           - bit 6: Single Step Enable
    //           - bit 5: Clear All Breakpoints
    //           - bit 4: Update Breakpoint at DEBUG_BRK_ADDR
    //           - bit 3: FIRQ  (on low (0) to high (1) edge)
    //           - bit 2: IRQ   (on low (0) to high (1) edge)
    //           - bit 1: NMI   (on low (0) to high (1) edge)
    //           - bit 0: RESET (on low (0) to high (1) edge)

    if (MAP(DBG_BRK_ADDR) == offset) { 
        data = _dbg_brk_addr; 
    } 
    else if (MAP(DBG_FLAGS) == offset) { 
        data = _dbg_flags; 
    }

    return data; 
} // END: Debug::OnRead()

void Debug::OnWrite(Word offset, Byte data) 
{ 
    IDevice::OnWrite( offset, data);
} // END: Debug::OnWrite()



/***************************
* Constructor / Destructor *
***************************/

Debug::Debug() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Debug Created" << clr::RETURN;
    _device_name = "DEBUG_DEVICE"; 
} // END: Debug()

Debug::~Debug() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Debug Destroyed" << clr::RETURN; 
} // END: ~Debug()



/******************
* Virtual Methods *
******************/



int  Debug::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnAttach() Entry" << clr::RETURN;
    if (nextAddr == 0) { ; } // stop the compiler from complaining
    
    Word old_address=nextAddr;
    this->heading = "Debug Hardware Registers:";
    register_node new_node;
    new_node = { "DBG_BRK_ADDR", nextAddr,  { "(Word) Address of current breakpoint"} }; nextAddr+=2;
    mapped_register.push_back(new_node);

    new_node = { "DBG_FLAGS", nextAddr,  {  "(Byte) Debug Specific Hardware Flags:",
                                            "- bit 7: Debug Enable",
                                            "- bit 6: Single Step Enable",
                                            "- bit 5: Clear All Breakpoints",
                                            "- bit 4: Update Breakpoint at DEBUG_BRK_ADDR",
                                            "- bit 3: FIRQ  (on low {0} to high {1} edge)",
                                            "- bit 2: IRQ   (on low {0} to high {1} edge)",
                                            "- bit 1: NMI   (on low {0} to high {1} edge)",
                                            "- bit 0: RESET (on low {0} to high {1} edge)",
                                            "" } }; // nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "DBG_END", nextAddr,  { "End of Debug Registers"} }; nextAddr++;
    mapped_register.push_back(new_node);

    new_node = { "DBG_TOP", nextAddr,  { "Top of Debug Registers", "---"} }; // nextAddr++;
    mapped_register.push_back(new_node);

    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnAttach() Exit" << clr::RETURN;    
    _size = nextAddr - old_address;
    return _size; 
}



bool Debug::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnInit() Entry" << clr::RETURN;
        
    // create the debugger window
    _dbg_window = SDL_CreateWindow("Debugger", 
        _dbg_window_width, 
        _dbg_window_height, 
        _dbg_window_flags); 
    SDL_ShowWindow(_dbg_window);

    // create the renderer
    _dbg_renderer = SDL_CreateRenderer(_dbg_window, NULL);    
    SDL_SetRenderLogicalPresentation(_dbg_renderer, 
        _dbg_width, 
        _dbg_height, 
        SDL_LOGICAL_PRESENTATION_STRETCH);

    // create the main debug texture
    _dbg_texture = SDL_CreateTexture(_dbg_renderer, 
        SDL_PIXELFORMAT_ARGB4444,         
        SDL_TEXTUREACCESS_STREAMING, 
        _dbg_width, _dbg_height);
    SDL_SetTextureScaleMode(_dbg_texture, SDL_SCALEMODE_NEAREST);        

    // create the character buffer
    _db_bfr.clear();
    for (unsigned int t=0; t<DEBUG_BUFFER_SIZE; t++)
    {
        D_GLYPH dg;
        dg.attr = 0x0f;
        dg.chr = 'A';
        _db_bfr.push_back(dg);
    }
    Byte at=0x0f;
    for (auto& c : _db_bfr)
    {
        c.attr = at;
        c.chr = ' ';
    }

    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnInit() Exit" << clr::RETURN;
    return true;
}



bool Debug::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnQuit() Entry" << clr::RETURN;
     
    if (_dbg_texture) 
    { // destroy the texture
        SDL_DestroyTexture(_dbg_texture);
        _dbg_texture = nullptr;
    }   
    
    if (_dbg_renderer)
    { // destroy the renderer
        SDL_DestroyRenderer(_dbg_renderer);
        _dbg_renderer = nullptr;
    }

    if (_dbg_window)
    { // destroy the debugger window
        SDL_DestroyWindow(_dbg_window);
        _dbg_window = nullptr;
    }    
    
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnQuit() Exit" << clr::RETURN;
    return true;
}



bool Debug::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnActivate() Entry" << clr::RETURN;
  

    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnActivate() Exit" << clr::RETURN;
    return true;
}



bool Debug::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnDeactivate() Entry" << clr::RETURN;

    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnDeactivate() Exit" << clr::RETURN;
    return true;
}


bool Debug::OnEvent(SDL_Event* evnt)
{
    // if not a debug event, just return now
    if (evnt->window.windowID != SDL_GetWindowID(_dbg_window)) { return true; }

    // if the debugger is not active, just return
    if (!(_dbg_flags & _DBG_FLAGS::DBGF_DEBUG_ENABLE)) { return true; }

    // SDL_ConvertEventToRenderCoordinates(_dbg_renderer, evnt);

    switch (evnt->type) 
    {
        // handle default events SDL_QUIT and ALT-X quits
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:  
            SDL_MinimizeWindow(_dbg_window);
            break;

        case SDL_EVENT_MOUSE_MOTION:
        {
            // Mouse_X = (int)evnt->motion.x;
            // Mouse_Y = (int)evnt->motion.y;
        } break;
    }
    return true;
}


bool Debug::OnUpdate(float fElapsedTime)
{
    if (fElapsedTime==0.0f) { ; } // stop the compiler from complaining

    // if the debugger is not active, just return
    // return true;


    const float delay = 1.0f / 30.0f;
    static float delayAcc = fElapsedTime;
    delayAcc += fElapsedTime;
    if (delayAcc >= delay)
    {
        delayAcc -= delay;

        if (_dbg_flags & _DBG_FLAGS::DBGF_DEBUG_ENABLE)
        { 
            // update internals
            //_update_mouse_pos(); 

            // render the debugger
            // clear the text buffer
            for (auto &c : _db_bfr)
            {
                c.attr = 0xf0;
                c.chr = ' ';
            }

            // call update functions
            // MouseStuff();
            // KeyboardStuff();

            DumpMemory(1,  1, mem_bank[0]);
            DumpMemory(1, 11, mem_bank[1]);
            DumpMemory(1, 21, mem_bank[2]);
            DumpMemory(1, 31, mem_bank[3]);

            // DrawCpu(39, 1);
            // DrawCode(39, 6);

            // DrawButtons();    
            // HandleButtons();
            // DrawBreakpoints();

            // if (!EditRegister(fElapsedTime))
            //     DrawCursor(fElapsedTime);

            // instruction text
            OutText(1, 51, "[SPACE]    Step", 0x80);
            OutText(1, 52, "[ALT-X]    Quit", 0x80);
            OutText(1, 53, "[ALT-D] ~ Debug", 0x80);
            OutText(1, 54, "[ALT-R] RunStop", 0x80);
            OutText(1, 55, "[ALT-ENTER] Toggles between Fullscreen and Windowed", 0x80);



            // TESTING ...
                int mx, my;
                _correct_mouse_coords(mx, my);

                std::string s = "Mouse          - X: " + std::to_string(mx) + " Y: " + std::to_string(my);
                OutText(1, 63, s, 0x80);

                std::string q = "Window -- width: " + std::to_string(_dbg_window_width) + " height: " + std::to_string(_dbg_window_height);
                OutText(1, 65, q, 0x80);

                static int delta = 0;
                std::string d = "Delta: " + std::to_string(delta++);
            OutText(1, 68, d, 0x80);
            // END: ... TESTING


            // _clear_texture(_dbg_texture, rand()%15, rand()%15, rand()%15, 15);
            _update_debug_screen();
        } else {
            // _clear_texture(_dbg_texture, 1, 2, 0, 15);
        }
    }
    return true;
}


bool Debug::OnRender()
{
    // if the debugger is not active, just return
    // ...

    if (_dbg_renderer)
    {
        // clear the background
        SDL_SetRenderDrawColor(_dbg_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_dbg_renderer);

        // render the debug texture        
        SDL_RenderTexture(_dbg_renderer, _dbg_texture, NULL, NULL);

        // update the debugger window display
        SDL_RenderPresent(_dbg_renderer);
    }
    return true;
}


void Debug::_clear_texture(SDL_Texture* texture, Byte r, Byte g, Byte b, Byte a)
{
    r &= 0x0f; g &= 0x0f; b &= 0x0f; a &= 0x0f;
    void *pixels;
    int pitch;
    if (!SDL_LockTexture(texture, NULL, &pixels, &pitch)) {
        Bus::Error(SDL_GetError());
    } else {
        for (int y = 0; y < _dbg_height; y++) {
            for (int x = 0; x < _dbg_width; x++) {
                Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));
                *dst = ( (a<<12) | (r<<8) | (g<<4) | (b<<0) );
            }
        }
        SDL_UnlockTexture(texture); 
    }    
}


void Debug::_setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte palette_index)
{
    palette_index &= 0x0f;
    Byte r = _debug_palette[palette_index].r;
    Byte g = _debug_palette[palette_index].g;
    Byte b = _debug_palette[palette_index].b;
    Byte a = _debug_palette[palette_index].a;  
    Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));
    *dst = ( (a<<12) | (r<<8) | (g<<4) | (b<<0) );
}

void Debug::OutGlyph(int col, int row, Byte glyph, Byte color_attr)
{
    Word index = row*(DEBUG_WIDTH/8) + col;
    _db_bfr[index].chr = glyph;
    _db_bfr[index].attr = color_attr;
}

int Debug::OutText(int col, int row, std::string text, Byte color_attr)
{
    int pos = 0;
    for (auto& a : text)
    {
        OutGlyph(col++, row, a, color_attr);
        pos++;
    }
    return pos;
}

std::string Debug::_hex(Uint32 n, Uint8 d)
{
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4)
        s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
}

void Debug::_update_debug_screen() 
{
    void *pixels;
    int pitch;

    if (!SDL_LockTexture(_dbg_texture, NULL, &pixels, &pitch)) {
        // SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
        Bus::Error(SDL_GetError(), __FILE__, __LINE__);
    }
    else
    {
        Byte width = DEBUG_WIDTH / 8;
        // int i=0;
        Word i_size = _db_bfr.size();
        for (Word i=0; i<i_size; i++)
        {
            Byte ch = _db_bfr[i].chr;
            Byte at = _db_bfr[i].attr;
			Byte fg = at >> 4;
			Byte bg = at & 0x0f;
            int x = (i % width) * 8;
            int y = (i / width) * 8;
			for (int v = 0; v < 8; v++)
			{
				for (int h = 0; h < 8; h++)
				{
					int color = bg;
                    Byte gd = GPU::GetGlyphData(ch, v);                    
					if (gd & (1 << (7 - h)))
						color = fg;
					_setPixel_unlocked(pixels, pitch, x + h, y + v, color);
				}
			}
        }
        SDL_UnlockTexture(_dbg_texture); 
    }
} 


void Debug::DumpMemory(int col, int row, Word addr)
{
    const bool use_debug_read = false;
    int line = 0;
    int width = 16;
    for (int ofs = addr; ofs < addr + 0x48; ofs += 8)
    {
        int c = col;
        std::string out = _hex(ofs, 4) + " ";
        for (int b = 0; b < width; b++)
            out += _hex(Memory::Read(ofs + b, use_debug_read), 2) + " ";

        c += OutText(col, row + line, out.c_str(), 0xe0);

        bool characters = true;
        if (characters)
        {
            for (int b = 0; b < width; b++)
            {
                Byte data;
                data = Memory::Read(ofs + b, use_debug_read);
                OutGlyph(c++, row + line, data, 0xd0);
            }
        }
        line++;
    }
}

bool Debug::CoordIsValid(int x, int y)
{
    if (y > 0 && y < 30 && y != 10 && y != 20)
    {
        // at an address
        if (x > 0 && x < 5)
        {
            csr_at = CSR_AT::CSR_AT_ADDRESS;
            return true;
        }
        // at a data entry
        if (x > 5 && x < 29 && (x + 1) % 3)
        {
            csr_at = CSR_AT::CSR_AT_DATA;
            return true;
        }
    }
    return false;
}


void Debug::MouseStuff()
{
    if (!bIsMouseOver)  return;

    // C6809* cpu = Bus::GetC6809();
    int mx, my;
    _correct_mouse_coords(mx, my);
    Uint32 btns = SDL_GetRelativeMouseState(NULL, NULL);

    // // mouse wheel
    // if (mouse_wheel)
    // {
    //     // scroll memory banks
    //     if (mx > 0 && mx < 29)
    //     {
    //         if (my > 0 && my < 10)	mem_bank[0] -= mouse_wheel * 8;
    //         if (my > 10 && my < 20)	mem_bank[1] -= mouse_wheel * 8;
    //         if (my > 20 && my < 30)	mem_bank[2] -= mouse_wheel * 8;
    //         bIsCursorVisible = false;
    //     }
    //     // Scroll the Code
    //     if (mx > 38 && mx < 64 && my > 5 && my < 30)
    //     {
    //         if (bMouseWheelActive == false)
    //         {
    //             mousewheel_offset = -25;
    //             bMouseWheelActive = true;
    //         }
    //         s_bSingleStep = true;	// scrollwheel enters into single step mode
    //         nRegisterBeingEdited.reg = Debug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
    //         mousewheel_offset -= mouse_wheel * 1;		// slow scroll
    //         if (SDL_GetModState() & KMOD_CTRL)	// is CTRL down?
    //             mousewheel_offset -= mouse_wheel * 3;	// faster scroll			
    //     }
    //     // scroll the break point display window (bottom right corner)
    //     if (mx >= 55 && my >= 33)
    //     {
    //         // printf("mouse_wheel: %d\n", mouse_wheel);
    //         mw_brk_offset -= mouse_wheel;
    //     }
    //     // reset the wheel
    //     mouse_wheel = 0;
    // }

    // // left mouse button clicked?
    // static bool last_LMB = false;
    // if ((btns & 1) && !last_LMB)
    // {
    //     // left-clicked on breakpoint
    //     if (mx >= 55 && my >= 33)
    //     {
    //         int index = (my - 33) + mw_brk_offset;
    //         // build a vector of active breakpoints
    //         std::vector<Word> breakpoints;
    //         for (auto& bp : mapBreakpoints)
    //             if (bp.second)
    //                 breakpoints.push_back(bp.first);
    //         if ((unsigned)index < breakpoints.size())
    //             printf("LEFT CLICK: $%04X\n", breakpoints[index]);
    //         //mapBreakpoints[breakpoints[index]] = false;
    //     }

    //     // click to select
    //     if (btns & 1)
    //     {
    //         if (CoordIsValid(mx, my))
    //         {
    //             csr_x = mx;
    //             csr_y = my;
    //             bIsCursorVisible = true;
    //         }
    //         else
    //             bIsCursorVisible = false;
    //         //printf("MX:%d  MY:%d\n", mx, my);
    //     }
    //     // condition code clicked?
    //     if (my == 1)
    //     {
    //         if (mx == 48) cpu->setCC_E(!cpu->getCC_E());
    //         if (mx == 49) cpu->setCC_F(!cpu->getCC_F());
    //         if (mx == 50) cpu->setCC_H(!cpu->getCC_H());
    //         if (mx == 51) cpu->setCC_I(!cpu->getCC_I());
    //         if (mx == 52) cpu->setCC_N(!cpu->getCC_N());
    //         if (mx == 53) cpu->setCC_Z(!cpu->getCC_Z());
    //         if (mx == 54) cpu->setCC_V(!cpu->getCC_V());
    //         if (mx == 55) cpu->setCC_C(!cpu->getCC_C());
    //     }
    //     // Register Clicked?
    //     bool bFound = false;
    //     for (auto& a : register_info)
    //     {
    //         if (a.y_pos == my && mx >= a.x_min && mx <= a.x_max)
    //         {
    //             // begin editing a register
    //             nRegisterBeingEdited.reg = a.reg;
    //             nRegisterBeingEdited.value = a.value;
    //             nRegisterBeingEdited.y_pos = a.y_pos;
    //             nRegisterBeingEdited.x_min = a.x_min;
    //             nRegisterBeingEdited.x_max = a.x_max;
    //             csr_x = mx;
    //             csr_y = my;
    //             bFound = true;
    //         }
    //     }
    //     if (!bFound)
    //         nRegisterBeingEdited.reg = EDIT_NONE;
    //     // left-click on code line toggles breakpoint
    //     if (mx > 38 && mx < 64 && my > 5 && my < 30 && s_bSingleStep)
    //     {
    //         Word offset = sDisplayedAsm[my - 6];
    //         (mapBreakpoints[offset]) ?
    //             mapBreakpoints[offset] = false :
    //             mapBreakpoints[offset] = true;
    //     }
    // }
    // last_LMB = (btns & 1);
    // // right mouse button clicked
    // static bool last_RMB = false;
    // if (btns & 4 && !last_RMB)
    // {
    //     // right-clicked on breakpoint
    //     if (mx >= 55 && my >= 33)
    //     {
    //         int index = (my - 33) + mw_brk_offset;
    //         // build a vector of active breakpoints
    //         std::vector<Word> breakpoints;
    //         for (auto& bp : mapBreakpoints)
    //             if (bp.second)
    //                 breakpoints.push_back(bp.first);
    //         if ((unsigned)index < breakpoints.size())
    //         {
    //             //printf("RIGHT CLICK: $%04X\n", breakpoints[index]);
    //             mapBreakpoints[breakpoints[index]] = false;
    //         }
    //     }

    //     // on PC register
    //     if (my == 4 && mx > 42 && mx < 47)
    //     {
    //         s_bSingleStep = !s_bSingleStep;
    //         if (!s_bSingleStep)
    //             nRegisterBeingEdited.reg = Debug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
    //     }
    //     // right-click on code line toggles breakpoint and resumes execution
    //     if (mx > 38 && mx < 64 && my > 5 && my < 30 && s_bSingleStep)
    //     {
    //         Word offset = sDisplayedAsm[my - 6];
    //         (mapBreakpoints[offset]) ?
    //             mapBreakpoints[offset] = false :
    //             mapBreakpoints[offset] = true;
    //         if (mapBreakpoints[offset] == true)
    //             s_bSingleStep = false;
    //     }
    // }
    // last_RMB = (btns & 4);
}

void Debug::_correct_mouse_coords(int& mx, int& my)
{ 
    float mouse_x, mouse_y;
    SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_GetWindowSize(_dbg_window, &_dbg_window_width, &_dbg_window_height);
    float ax = (float)_dbg_width / (float)_dbg_window_width;
    float ay =  (float)_dbg_height / (float)_dbg_window_height;
    mx = (int)(mouse_x * ax)/8;
    my = (int)(mouse_y * ay)/8;
    return;
}

// END: Debug.cpp
