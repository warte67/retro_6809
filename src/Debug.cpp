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
#include "C6809.hpp"



/****************
 * Read / Write *
 ***************/ 

Byte Debug::OnRead(Word offset) 
{ 
    Byte data = IDevice::OnRead(offset);

    if (MAP(SYS_DBG_BRK_ADDR) == offset) { 
        data = _dbg_brk_addr; 
    } 
    else if (MAP(SYS_DBG_FLAGS) == offset) { 
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
    _device_name = "SYS_DEVICE"; 
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
    this->heading = "System and Debug Hardware Registers:";
    register_node new_node;

    new_node = { "SYS_BEGIN", nextAddr,  { "Start of System Registers"} };
    mapped_register.push_back(new_node);

    new_node = { "SYS_STATE", nextAddr,  { "(Byte) System State Register",
        "SYS_STATE: ABCD.SSSS                          ",
        "- bit  7   = Error: Standard Buffer Overflow  ",
        "- bit  6   = Error: Extended Buffer Overflow  ",
        "- bit  5   = Error: Reserved                  ",
        "- bit  4   = Error: Reserved                  ",  
        "- bits 0-3 = CPU Speed (0-15):                ", 
        "   0 ($0)  = CPU Clock  25 khz.               ",
        "   1 ($1)  = CPU Clock  50 khz.               ",
        "   2 ($2)  = CPU Clock 100 khz.               ",
        "   3 ($3)  = CPU Clock 200 khz.               ",
        "   4 ($4)  = CPU Clock 333 khz.               ",
        "   5 ($5)  = CPU Clock 416 khz.               ",
        "   6 ($6)  = CPU Clock 500 khz.               ",
        "   7 ($7)  = CPU Clock 625 khz.               ",
        "   8 ($8)  = CPU Clock 769 khz.               ",
        "   9 ($9)  = CPU Clock 833 khz.               ",
        "  10 ($A)  = CPU Clock 1.0 mhz.               ",
        "  11 ($B)  = CPU Clock 1.4 mhz.               ",
        "  12 ($C)  = CPU Clock 2.0 mhz.               ",
        "  13 ($D)  = CPU Clock 3.3 mhz.               ",
        "  14 ($E)  = CPU Clock 5.0 mhz.               ",
        "  15 ($F)  = CPU Clock ~10.0 mhz. (unmetered) ", 
        ""} }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "SYS_SPEED", nextAddr,  { "(Word) Average CPU Clock Speed (Read Only)"} }; nextAddr+=2;
    mapped_register.push_back(new_node);

    new_node = { "SYS_CLOCK_DIV", nextAddr,  
        { "(Byte) 70 hz Clock Divider Register (Read Only)",
        "- bit 7: 0.546875 hz",
        "- bit 6: 1.09375 hz",
        "- bit 5: 2.1875 hz",
        "- bit 4: 4.375 hz",
        "- bit 3: 8.75 hz",
        "- bit 2: 17.5 hz",
        "- bit 1: 35.0 hz",
        "- bit 0: 70.0 hz",
        ""} }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "SYS_TIMER", nextAddr,  { "(Word) Increments at 0.546875 hz"} }; nextAddr+=2;
    mapped_register.push_back(new_node);

    new_node = { "SYS_DBG_BRK_ADDR", nextAddr,  { "(Word) Address of current debug breakpoint"} }; nextAddr+=2;
    mapped_register.push_back(new_node);

    new_node = { "SYS_DBG_FLAGS", nextAddr,  {  "(Byte) Debug Specific Hardware Flags:",
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

    new_node = { "SYS_END", nextAddr,  { "End of System Registers"} }; nextAddr++;
    mapped_register.push_back(new_node);

    new_node = { "SYS_TOP", nextAddr,  { "Top of System Registers", ""} }; // nextAddr++;
    mapped_register.push_back(new_node);

    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnAttach() Exit" << clr::RETURN;    
    _size = nextAddr - old_address;
    return _size; 
}



void Debug::OnInit()
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

    // get the keybuffer
    keybfr = SDL_GetKeyboardState(NULL);

    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnInit() Exit" << clr::RETURN;
}



void Debug::OnQuit()
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
}



void Debug::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnActivate() Entry" << clr::RETURN;
  

    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnActivate() Exit" << clr::RETURN;
}



void Debug::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnDeactivate() Entry" << clr::RETURN;

    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnDeactivate() Exit" << clr::RETURN;
}


void Debug::OnEvent(SDL_Event* evnt)
{
    if (SDL_GetWindowFromEvent(evnt) != _dbg_window) { return; }

    switch (evnt->type) 
    {
        // handle default events SDL_QUIT and ALT-X quits        
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:  
        {
            SDL_MinimizeWindow(_dbg_window);
            break;
        }

        case SDL_EVENT_KEY_DOWN:
        {
            // if debugger is active
            if (_dbg_flags & _DBG_FLAGS::DBGF_DEBUG_ENABLE)
            {
                if (evnt->key.key == SDLK_ESCAPE)
                    bIsCursorVisible = false;
                if (bIsCursorVisible)
                {
                    if (evnt->key.key == SDLK_LEFT || evnt->key.key == SDLK_BACKSPACE)
                        if (csr_x > 1)
                            while (!CoordIsValid(--csr_x, csr_y));
                    if (evnt->key.key == SDLK_RIGHT)
                        if (csr_x < 52)
                            while (!CoordIsValid(++csr_x, csr_y));
                    if (evnt->key.key == SDLK_UP)
                    {
                        if (csr_y == 1)			mem_bank[0] -= 16;
                        else if (csr_y == 11)	mem_bank[1] -= 16;
                        else if (csr_y == 21)	mem_bank[2] -= 16;
                        else if (csr_y == 31)	mem_bank[3] -= 16;
                        else if (csr_y > 1)		while (!CoordIsValid(csr_x, --csr_y));
                    }
                    if (evnt->key.key == SDLK_DOWN)
                    {
                        if (csr_y == 9)			mem_bank[0] += 16;
                        else if (csr_y == 19)	mem_bank[1] += 16;
                        else if (csr_y == 29)	mem_bank[2] += 16;
                        else if (csr_y == 39)	mem_bank[3] += 16;
                        else if (csr_y < 40)	while (!CoordIsValid(csr_x, ++csr_y));
                    }
                    if (evnt->key.key == SDLK_RETURN)
                        bIsCursorVisible = false;
                }    
            }
            break;
        }


        case SDL_EVENT_MOUSE_WHEEL:
        {
            mouse_wheel = evnt->wheel.y;
            break;            
        }

        case SDL_EVENT_WINDOW_MINIMIZED:
        {
            //s_bIsDebugActive = false;
            break;
        }
        case SDL_EVENT_WINDOW_RESTORED:
        {
            //s_bIsDebugActive = true;
            break;
        }
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
        {
            bIsMouseOver = true;
            break;
        }
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
        {
            bIsMouseOver = false;
            break;
        }
        case SDL_EVENT_WINDOW_FOCUS_LOST:
        {
            bIsCursorVisible = false;
            break;
        }   

    // SDL_EVENT_WINDOW_SHOWN = 0x202,     /**< Window has been shown */
    // SDL_EVENT_WINDOW_HIDDEN,            /**< Window has been hidden */
    // SDL_EVENT_WINDOW_EXPOSED,           /**< Window has been exposed and should be redrawn, and can be redrawn directly from event watchers for this event */
    // SDL_EVENT_WINDOW_MOVED,             /**< Window has been moved to data1, data2 */
    // SDL_EVENT_WINDOW_RESIZED,           /**< Window has been resized to data1xdata2 */
    // SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,/**< The pixel size of the window has changed to data1xdata2 */
    // SDL_EVENT_WINDOW_METAL_VIEW_RESIZED,/**< The pixel size of a Metal view associated with the window has changed */
    // SDL_EVENT_WINDOW_MINIMIZED,         /**< Window has been minimized */
    // SDL_EVENT_WINDOW_MAXIMIZED,         /**< Window has been maximized */
    // SDL_EVENT_WINDOW_RESTORED,          /**< Window has been restored to normal size and position */
    // SDL_EVENT_WINDOW_MOUSE_ENTER,       /**< Window has gained mouse focus */
    // SDL_EVENT_WINDOW_MOUSE_LEAVE,       /**< Window has lost mouse focus */
    // SDL_EVENT_WINDOW_FOCUS_GAINED,      /**< Window has gained keyboard focus */
    // SDL_EVENT_WINDOW_FOCUS_LOST,        /**< Window has lost keyboard focus */
    // SDL_EVENT_WINDOW_CLOSE_REQUESTED,   /**< The window manager requests that the window be closed */
    // SDL_EVENT_WINDOW_HIT_TEST,          /**< Window had a hit test that wasn't SDL_HITTEST_NORMAL */
    // SDL_EVENT_WINDOW_ICCPROF_CHANGED,   /**< The ICC profile of the window's display has changed */
    // SDL_EVENT_WINDOW_DISPLAY_CHANGED,   /**< Window has been moved to display data1 */
    // SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED, /**< Window display scale has been changed */
    // SDL_EVENT_WINDOW_SAFE_AREA_CHANGED, /**< The window safe area has been changed */
    // SDL_EVENT_WINDOW_OCCLUDED,          /**< The window has been occluded */
    // SDL_EVENT_WINDOW_ENTER_FULLSCREEN,  /**< The window has entered fullscreen mode */
    // SDL_EVENT_WINDOW_LEAVE_FULLSCREEN,  /**< The window has left fullscreen mode */
    // SDL_EVENT_WINDOW_DESTROYED,         /**< The window with the associated ID is being or has been destroyed. If this message is being handled
         
    }
}


void Debug::OnUpdate(float fElapsedTime)
{
    if (fElapsedTime==0.0f) { ; } // stop the compiler from complaining

    // if the debugger is not active, just return
    // return true;


    const float delay = 1.0f / 60.0f;
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
            MouseStuff();
            KeyboardStuff();

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
                DrawCursor(fElapsedTime);

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

            // END: ... TESTING


            // _clear_texture(_dbg_texture, rand()%15, rand()%15, rand()%15, 15);
            _update_debug_screen();
        } else {
            // _clear_texture(_dbg_texture, 1, 2, 0, 15);
        }
    }
}


void Debug::OnRender()
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
    for (int ofs = addr; ofs < addr + 0x90; ofs += 16)
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
    if (y > 0 && y < 40 && y != 10 && y != 20 && y != 30)
    {
        // at an address
        if (x > 0 && x < 5)
        {
            csr_at = CSR_AT::CSR_AT_ADDRESS;
            return true;
        }
        // at a data entry
        if (x > 5 && x < 53 && (x + 1) % 3)
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

    // mouse wheel
    if (mouse_wheel)
    {
        // scroll memory banks
        if (mx > 0 && mx < 70)
        {
            if (my > 0 && my < 10)	mem_bank[0] -= mouse_wheel * 8;
            if (my > 10 && my < 20)	mem_bank[1] -= mouse_wheel * 8;
            if (my > 20 && my < 30)	mem_bank[2] -= mouse_wheel * 8;
            if (my > 30 && my < 40)	mem_bank[3] -= mouse_wheel * 8;
            bIsCursorVisible = false;
        }
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

        // reset the mouse wheel
        mouse_wheel = 0;
    }

    // // left mouse button clicked?
    static bool last_LMB = false;
    if ((btns & 1) && !last_LMB)
    {
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
        // click to select
        if (btns & 1)
        {
            if (CoordIsValid(mx, my))
            {
                csr_x = mx;
                csr_y = my;
                bIsCursorVisible = true;
            }
            else
                bIsCursorVisible = false;
            // printf("MX:%d  MY:%d\n", mx, my);
        }
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
    }
    last_LMB = (btns & 1);
    // right mouse button clicked
    static bool last_RMB = false;
    if (btns & 4 && !last_RMB)
    {
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
    }
    last_RMB = (btns & 4);
}

void Debug::_correct_mouse_coords(int& mx, int& my)
{ 
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_GetWindowSize(_dbg_window, &_dbg_window_width, &_dbg_window_height);
    float ax = (float)_dbg_width / (float)_dbg_window_width;
    float ay =  (float)_dbg_height / (float)_dbg_window_height;
    mx = (int)(mouse_x * ax)/8;
    my = (int)(mouse_y * ay)/8;
    return;
}

void Debug::DrawCursor(float fElapsedTime)
{
    if (!bIsCursorVisible)	return;
    
    // C6809* cpu = Bus::GetC6809();
    std::string ch = " ";

    // which character is under the cursor?
    switch (csr_at)
    {
        case CSR_AT::CSR_AT_ADDRESS:
        {
            Word addr = 0;
            if (csr_y < 10)      addr = mem_bank[0] + ((csr_y - 1 ) * 16);
            else if (csr_y < 20) addr = mem_bank[1] + ((csr_y - 11) * 16);
            else if (csr_y < 30) addr = mem_bank[2] + ((csr_y - 21) * 16);
            else if (csr_y < 40) addr = mem_bank[3] + ((csr_y - 31) * 16);
            Byte digit = csr_x - 1;
            Byte num = 0;
            if (digit == 0)	num = ((addr & 0xf000) >> 12);
            if (digit == 1)	num = ((addr & 0x0f00) >> 8);
            if (digit == 2)	num = ((addr & 0x00f0) >> 4);
            if (digit == 3)	num = ((addr & 0x000f) >> 0);
            ch = _hex(num, 1);
            break;
        }
        case CSR_AT::CSR_AT_DATA:
        {
            Byte ofs = ((csr_x - 5) / 3) + ((csr_y - 1) * 16);
            Byte digit = ((csr_x + 1) % 3) - 1;
            Byte num = 0;
            Word addr = mem_bank[0];
            if (csr_y > 10 && csr_y < 20) { ofs += 96 ; addr = mem_bank[1]; }
            if (csr_y > 20 && csr_y < 30) { ofs -= 64; addr = mem_bank[2]; }
            if (csr_y > 30 && csr_y < 40) { ofs -= 224; addr = mem_bank[3]; }

            Byte data = Memory::Read(addr + ofs);//, true);
            if (digit == 0) num = (data & 0xf0) >> 4;
            if (digit == 1) num = (data & 0x0f) >> 0;
            ch = _hex(num, 1);
            break;
        }
    }
    // display the reversed character
    static Byte attr = 0;

    const float delay = 1.0f / 60.0f;
    static float delayAcc = fElapsedTime;
    delayAcc += fElapsedTime;
    if (delayAcc >= delay)
    {
        delayAcc -= delay;
        attr++;
        attr &= 0x0f;
    }
    // OutGlyph(csr_x, csr_y, ch[0], (rand() % 16)<<0);
    OutGlyph(csr_x, csr_y, ch[0], attr);    
}


void Debug::KeyboardStuff()
{
    if (!bIsCursorVisible)	return;

    SDL_Keycode hx[] = { SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
                         SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7,
                         SDL_SCANCODE_8, SDL_SCANCODE_9, SDL_SCANCODE_A, SDL_SCANCODE_B,
                         SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E, SDL_SCANCODE_F };
    char d[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    static bool state[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    // check for valid key presses
    bool bKeyPressed = false;
    Byte ch = 0;
    for (int t = 0; t < 16; t++)
    {
        if (state[t] == 0 && keybfr[hx[t]])
        {
            state[t] = 1;
            bKeyPressed = true;
            ch = d[t];
        }
        // reset the key
        if (state[t] == 1 && !keybfr[hx[t]])	state[t] = 0;
    }

    // respond to [DEL]
    // ...

    if (!bKeyPressed)	return;

    switch (csr_at)
    {
        case CSR_AT::CSR_AT_ADDRESS:
        {
            Word addr = 0;
            if (csr_y < 10)      addr = mem_bank[0] + ((csr_y - 1)  * 16);
            else if (csr_y < 20) addr = mem_bank[1] + ((csr_y - 11) * 16);
            else if (csr_y < 30) addr = mem_bank[2] + ((csr_y - 21) * 16);
            else if (csr_y < 40) addr = mem_bank[3] + ((csr_y - 31) * 16);
            Byte digit = csr_x - 1;
            if (digit == 0)	addr = (addr & 0x0fff) | (ch << 12);
            if (digit == 1)	addr = (addr & 0xf0ff) | (ch << 8);
            if (digit == 2)	addr = (addr & 0xff0f) | (ch << 4);
            if (digit == 3)	addr = (addr & 0xfff0) | (ch << 0);
            if (csr_y < 10)			mem_bank[0] = addr - ((csr_y - 1)  * 16);
            else if (csr_y < 20)	mem_bank[1] = addr - ((csr_y - 11) * 16);
            else if (csr_y < 30)	mem_bank[2] = addr - ((csr_y - 21) * 16);
            else if (csr_y < 40)	mem_bank[3] = addr - ((csr_y - 31) * 16);
            if (csr_x < 5)	while (!CoordIsValid(++csr_x, csr_y));
            break;
        }
        case CSR_AT::CSR_AT_DATA:
        {
            Byte ofs = ((csr_x - 5) / 3) + ((csr_y - 1) * 16);
            Byte digit = ((csr_x + 1) % 3) - 1;
            Word addr = mem_bank[0];
            if (csr_y > 10 && csr_y < 20) { ofs += 96 ; addr = mem_bank[1]; }
            if (csr_y > 20 && csr_y < 30) { ofs -= 64; addr = mem_bank[2]; }
            if (csr_y > 30 && csr_y < 40) { ofs -= 224; addr = mem_bank[3]; }
            Byte data = Memory::Read(addr + ofs);//,true);
            if (digit == 0)		data = (data & 0x0f) | (ch << 4);
            if (digit == 1)		data = (data & 0xf0) | (ch << 0);
            Memory::Write(addr + ofs, data);
            if (csr_x < 53)		while (!CoordIsValid(++csr_x, csr_y));
            break;
        }
    }    
}


bool Debug::SingleStep()
{
    // do nothing if singlestep is disabled
    if (!s_bSingleStep)
        return true;
    // wait for space
    if (s_bIsStepPaused)
        return false;
    return true;
}

void Debug::ContinueSingleStep() {
    // C6809* cpu = Bus::Inst().m_cpu;
    C6809* cpu = Bus::GetC6809();
    // if breakpoint reached... enable singlestep
    if (mapBreakpoints[cpu->getPC()] == true)
    {
        s_bIsDebugActive = true;
        s_bSingleStep = true;
    }
    // continue from paused state?
    s_bIsStepPaused = s_bSingleStep;
}





// END: Debug.cpp
