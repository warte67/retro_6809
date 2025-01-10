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

#include <deque>
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

    // system registers
    if (offset == MAP(SYS_STATE)) 
    {
        Byte err = C6809::s_sys_state & 0xF0;
        C6809::s_sys_state &= 0x0F;
        data = C6809::s_sys_state | err; 
    } 
    else if (offset == MAP(SYS_SPEED) + 0) 
    {
        data = Bus::GetCpuSpeed() >> 8;
    } 
    else if (offset == MAP(SYS_SPEED) + 1) 
    {
        data = Bus::GetCpuSpeed() & 0xFF;
    } 
    else if (offset == MAP(SYS_CLOCK_DIV)) 
    {
        data = Bus::GetClockDiv();
    } 
    else if (offset == MAP( SYS_TIMER) + 0) 
    {
        data = Bus::GetClockTimer() >> 8;
    } 
    else if (offset == MAP(SYS_TIMER) + 1) 
    {
        data = Bus::GetClockTimer() & 0xff;
    } 
    else if (offset == MAP(SYS_DBG_BRK_ADDR)) 
    { 
        data = _dbg_brk_addr; 
    } 
    else if (offset == MAP(SYS_DBG_FLAGS) ) 
    {
        (s_bIsDebugActive) ? _dbg_flags |= DBGF_DEBUG_ENABLE : _dbg_flags &= ~DBGF_DEBUG_ENABLE; // Enable
        (s_bSingleStep)     ? _dbg_flags |= DBGF_SINGLE_STEP_ENABLE : _dbg_flags &= ~DBGF_SINGLE_STEP_ENABLE; // Single-Step
        _dbg_flags &= ~DBGF_CLEAR_ALL_BRKPT;     // zero for Clear all Breakpoints
        (mapBreakpoints[_dbg_brk_addr]) ? _dbg_flags |= DBGF_UPDATE_BRKPT : _dbg_flags &= ~DBGF_UPDATE_BRKPT;
        _dbg_flags &= ~DBGF_FIRQ;     // FIRQ
        _dbg_flags &= ~DBGF_IRQ;     // IRQ
        _dbg_flags &= ~DBGF_NMI;     // NMI
        _dbg_flags &= ~DBGF_RESET;     // RESET
        data = _dbg_flags;              
    }

    return data; 
} // END: Debug::OnRead()

void Debug::OnWrite(Word offset, Byte data) 
{ 
    if ( offset == MAP(SYS_STATE) ) 
    {  
        C6809::s_sys_state = data;
    } 
    else if ( offset == MAP(SYS_TIMER) + 0 ) 
    {
        Bus::SetClockTimer( (data << 8) );
    } 
    else if ( offset == MAP(SYS_TIMER) + 1 ) 
    {
        Bus::SetClockTimer( (data << 0) );
    } 
    else if ( offset == MAP(SYS_DBG_BRK_ADDR) + 0 ) 
    {
        _dbg_brk_addr = (_dbg_brk_addr & 0x00ff) | (data << 8);
    } 
    else if ( offset == MAP(SYS_DBG_BRK_ADDR) + 1 ) 
    {
        _dbg_brk_addr = (_dbg_brk_addr & 0xff00) | (data << 0);
    }
    else if ( offset == MAP(SYS_DBG_FLAGS) )
    {
        _dbg_flags = data;
        (_dbg_flags & DBGF_DEBUG_ENABLE) ? s_bIsDebugActive = true : s_bIsDebugActive = false;
        (_dbg_flags & DBGF_SINGLE_STEP_ENABLE) ? s_bSingleStep = true : s_bSingleStep = false;
        if (_dbg_flags & DBGF_CLEAR_ALL_BRKPT)  cbClearBreaks();
        (_dbg_flags & DBGF_UPDATE_BRKPT) ? mapBreakpoints[_dbg_brk_addr] = true : mapBreakpoints[_dbg_brk_addr] = false;
        if (_dbg_flags & DBGF_FIRQ)   cbFIRQ();
        if (_dbg_flags & DBGF_IRQ)   cbIRQ();
        if (_dbg_flags & DBGF_NMI)   cbNMI();
        if (_dbg_flags & DBGF_RESET)   cbReset();
        // activate or deactivate the debugger
        if (s_bIsDebugActive)   // activate the debugger
        {
            SDL_ShowWindow( Bus::GetGPU()->GetWindow() );
            // reset the visited memory 
            // C6809* cpu = Bus::GetC6809();
            // cpu->ClearVisited_Memory();
            SDL_RaiseWindow( _dbg_window );
        }
        else                    // deactivate the debugger     
        {      
            SDL_HideWindow(_dbg_window);
            SDL_RaiseWindow(Bus::GetGPU()->GetWindow());
        }        
    }

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

    if (DEBUG_STARTS_ACTIVE) {
        _dbg_flags |= DBGF_DEBUG_ENABLE;
        SDL_ShowWindow(_dbg_window);
    } else {
        _dbg_flags &= ~DBGF_DEBUG_ENABLE;
        SDL_HideWindow(_dbg_window);
    }

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
    // if (SDL_GetWindowFromEvent(evnt) != _dbg_window) { return; }
    if (s_bIsDebugActive == false) { return; }

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
                {
                    bIsCursorVisible = false;
                    bMouseWheelActive = false;
                }
                if (bIsCursorVisible)
                {
                    if (evnt->key.key == SDLK_LEFT || evnt->key.key == SDLK_BACKSPACE)
                        if (csr_x > 1)
                            while (!CoordIsValid(--csr_x, csr_y));
                    if (evnt->key.key == SDLK_RIGHT)
                        if (csr_x < 28)
                            while (!CoordIsValid(++csr_x, csr_y));
                    if (evnt->key.key == SDLK_UP)
                    {
                        if (csr_y == 1)			mem_bank[0] -= 8;
                        else if (csr_y == 10)	mem_bank[1] -= 8;
                        else if (csr_y == 19)	mem_bank[2] -= 8;
                        else if (csr_y == 28)	mem_bank[3] -= 8;
                        else if (csr_y > 1)		while (!CoordIsValid(csr_x, --csr_y));
                    }
                    if (evnt->key.key == SDLK_DOWN)
                    {
                        if (csr_y == 8)			mem_bank[0] += 8;
                        else if (csr_y == 17)	mem_bank[1] += 8;
                        else if (csr_y == 26)	mem_bank[2] += 8;
                        else if (csr_y == 35)	mem_bank[3] += 8;
                        else if (csr_y < 35)	while (!CoordIsValid(csr_x, ++csr_y));
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
    // if the debugger is not active, just return
    if (!( _dbg_flags & DBGF_DEBUG_ENABLE))   { return; }   

    // only update once every so often
    const float delay = 1.0f / 30.0f;
    static float delayAcc = fElapsedTime;
    delayAcc += fElapsedTime;
    if (delayAcc >= delay)
    {
        delayAcc -= delay;

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

        DrawMemoryFrame(0,0);
        DumpMemory(1,  1, mem_bank[0]);
        DumpMemory(1, 10, mem_bank[1]);
        DumpMemory(1, 19, mem_bank[2]);
        DumpMemory(1, 28, mem_bank[3]);

        DrawCpu(40, 1);     // was (39,1)
        DrawCode(40, 6);    // was (39,6)

        DrawButtons();    
        HandleButtons();
        DrawBreakpoints();

        if (!EditRegister(fElapsedTime))
            DrawCursor(fElapsedTime);

        // instruction text
        OutText(1, 51, "[SPACE]    Step", 0x80);
        OutText(1, 52, "[ALT-X]    Quit", 0x80);
        OutText(1, 53, "[ALT-D] ~ Debug", 0x80);
        OutText(1, 54, "[ALT-R] RunStop", 0x80);
        OutText(1, 55, "[ALT-ENTER] Toggles between Fullscreen and Windowed", 0x80);



        // TESTING ...
            int mx, my;
            int row = 45;
            _correct_mouse_coords(mx, my);

            std::string s = "Mouse          - X: " + std::to_string(mx) + " Y: " + std::to_string(my);
            OutText(1, row++, s, 0x80);

            std::string q = "Window -- width: " + std::to_string(_dbg_window_width) + " height: " + std::to_string(_dbg_window_height);
            OutText(1, row++, q, 0x80);

            std::string r = "s_bSingleStep: " + std::to_string(s_bSingleStep);
            OutText(1, row++, r, 0x80);

        // END: ... TESTING


        // _clear_texture(_dbg_texture, rand()%15, rand()%15, rand()%15, 15);
        _update_debug_screen();
    } else {
        // _clear_texture(_dbg_texture, 1, 2, 0, 15);
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


void Debug::DrawMemoryFrame(int col, int row)
{
    // 0xb3 = top left
    // 0xb5 = horizontal (both top and bottom)
    // 0xb6 = top right
    // 0xb9 = bottom left
    // 0xba = vertical (both left and right)
    // 0xbb = left side and right horizontal
    // 0xbc = bottom right
    // 0xBE = right side and left horizontal

    // BLUE Memory Dump Frames
    std::string s = "";
    Byte clr = 0x70;
    s += (char)0xb3; s += std::string(37, (char)0xb5); s += (char)0xb6;
    OutText(col, row++, s, clr);
    for (int r = 0; r < 3; r++)
    {
        s = "";
        s += (char)0xba; s += std::string(37, (char)0x20); s += (char)0xba;
        for (int t = 0; t < 8; t++)
            OutText(col, row++, s, clr);
        s = "";
        s += (char)0xbb; s += std::string(37, (char)0xb5); s += (char)0xbe;
        OutText(col, row++, s, clr);
    }
    s = "";
    s += (char)0xba; s += std::string(37, (char)0x20); s += (char)0xba;
    for (int t = 0; t < 8; t++)
        OutText(col, row++, s, clr);
    s = "";
    s += (char)0xb9; s += std::string(37, (char)0xb5); s += (char)0xbc;
    OutText(col, row++, s, clr);

    // GREEN Register Display Frame
    // ...

    // GRAY Code Display Frame
    // ...

    // RED Breakpoint Display Frame
    // ...

    // BUTTONS:  
    // 0xA1 = top left
    // 0xA2 = top right
    // 0xA3 = top horizontal
    // 0xA4 = bottom left
    // 0xA5 = left vertical
    // 0xA8 = bottom right
    // 0xAC = bottom horizontal

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


void Debug::DrawCpu(int x, int y)
{
    C6809* cpu = Bus::GetC6809();

    int RamX = x, RamY = y;
    // Condition Codes
    RamX += OutText(RamX, RamY, "CC($", 0xB0);
    RamX += OutText(RamX, RamY, _hex(cpu->getCC(), 2).c_str(), 0xC0);
    RamX += OutText(RamX, RamY, "): ", 0xB0);
    if (cpu->getCC_E())		RamX += OutText(RamX, RamY, "E", 0xC0);
    else RamX += OutText(RamX, RamY, "e", 0xB0);
    if (cpu->getCC_F())		RamX += OutText(RamX, RamY, "F", 0xC0);
    else RamX += OutText(RamX, RamY, "f", 0xB0);
    if (cpu->getCC_H())		RamX += OutText(RamX, RamY, "H", 0xC0);
    else RamX += OutText(RamX, RamY, "h", 0xB0);
    if (cpu->getCC_I())		RamX += OutText(RamX, RamY, "I", 0xC0);
    else RamX += OutText(RamX, RamY, "i", 0xB0);
    if (cpu->getCC_N())		RamX += OutText(RamX, RamY, "N", 0xC0);
    else RamX += OutText(RamX, RamY, "n", 0xB0);
    if (cpu->getCC_Z())		RamX += OutText(RamX, RamY, "Z", 0xC0);
    else RamX += OutText(RamX, RamY, "z", 0xB0);
    if (cpu->getCC_V())		RamX += OutText(RamX, RamY, "V", 0xC0);
    else RamX += OutText(RamX, RamY, "v", 0xB0);
    if (cpu->getCC_C())		RamX += OutText(RamX, RamY, "C", 0xC0);
    else RamX += OutText(RamX, RamY, "c", 0xB0);
    RamX = x; RamY++;	// carraige return(ish)

    // D = (A<<8) | B & 0x00FF
    RamX += OutText(RamX, RamY, " D:$", 0xB0);
    RamX += OutText(RamX, RamY, _hex(cpu->getD(), 4), 0xC0);
    RamX += OutText(RamX, RamY, " A:$", 0xB0);
    RamX += OutText(RamX, RamY, _hex(cpu->getA(), 2), 0xC0);
    RamX += OutText(RamX, RamY, "   B:$", 0xB0);
    RamX += OutText(RamX, RamY, _hex(cpu->getB(), 2), 0xC0);
    // RamX += OutText(RamX, RamY, ")", 0xB0);
    RamX = x; RamY++;	// carraige return(ish)

    // X
    RamX += OutText(RamX, RamY, " X:$", 0xB0);
    RamX += OutText(RamX, RamY, _hex(cpu->getX(), 4), 0xC0);
    // Y
    RamX += OutText(RamX, RamY, " Y:$", 0xB0);
    RamX += OutText(RamX, RamY, _hex(cpu->getY(), 4), 0xC0);
    // U
    RamX += OutText(RamX, RamY, " U:$", 0xB0);
    RamX += OutText(RamX, RamY, _hex(cpu->getU(), 4), 0xC0);
    RamX = x; RamY++;	// carraige return(ish)
    // PC
    RamX += OutText(RamX, RamY, "PC:$", 0xB0);
    RamX += OutText(RamX, RamY, _hex(cpu->getPC(), 4), 0xC0);
    // S
    RamX += OutText(RamX, RamY, " S:$", 0xB0);
    RamX += OutText(RamX, RamY, _hex(cpu->getS(), 4), 0xC0);
    // DP
    RamX += OutText(RamX, RamY, " DP:$", 0xB0);
    RamX += OutText(RamX, RamY, _hex(cpu->getDP(), 2), 0xC0);
    RamX = x; RamY++;	// carraige return(ish)
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
    int width = 8;
    for (int ofs = addr; ofs < addr + 0x40; ofs += 8)
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
    if (y > 0 && y < 36 && y != 9 && y != 18 && y != 27)
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

    // mouse wheel
    if (mouse_wheel)
    {
        // scroll memory banks
        if (mx > 0 && mx < 38)
        {
            if (my > 0 && my < 9)	mem_bank[0] -= mouse_wheel * 8;
            if (my > 9 && my < 18)	mem_bank[1] -= mouse_wheel * 8;
            if (my > 18 && my < 27)	mem_bank[2] -= mouse_wheel * 8;
            if (my > 27 && my < 36)	mem_bank[3] -= mouse_wheel * 8;
            bIsCursorVisible = false;
        }

        // Scroll the Code
        if (mx > 39 && mx < 80 && my > 5 && my < 36)
        {
            if (bMouseWheelActive == false)
            {
                mousewheel_offset = -18;
                bMouseWheelActive = true;
            }
            s_bSingleStep = true;	// scrollwheel enters into single step mode
            nRegisterBeingEdited.reg = Debug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
            if ( (SDL_GetModState() & SDL_KMOD_CTRL) || (SDL_GetModState() & SDL_KMOD_SHIFT) )
                mousewheel_offset -= mouse_wheel * 1;	// fine scroll	
            else
                mousewheel_offset -= mouse_wheel * 3;		// fast scroll
        }


        // scroll the break point display window (bottom right corner)
        if (mx >= 71 && my >= 41 && mx < 100 && my < 48)
        {
            mw_brk_offset -= mouse_wheel;
        }

        // reset the mouse wheel
        mouse_wheel = 0;
    }

    // left mouse button clicked?
    static bool last_LMB = false;
    if ((btns & 1) && !last_LMB)
    {
        // left-clicked on breakpoint
        if (mx >= 71 && my >= 41)
        {
            // int index = (my - 41) + mw_brk_offset;
            // build a vector of active breakpoints
            std::vector<Word> breakpoints;
            for (auto& bp : mapBreakpoints)
                if (bp.second)
                    breakpoints.push_back(bp.first);
            // if ((unsigned)index < breakpoints.size())
            //     printf("LEFT CLICK: $%04X\n", breakpoints[index]);
            //mapBreakpoints[breakpoints[index]] = false;
        }
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
        }
        // condition code clicked?
        if (my == 1)
        {
            C6809* cpu = Bus::GetC6809();
            if (mx == 49) cpu->setCC_E(!cpu->getCC_E());
            if (mx == 50) cpu->setCC_F(!cpu->getCC_F());
            if (mx == 51) cpu->setCC_H(!cpu->getCC_H());
            if (mx == 52) cpu->setCC_I(!cpu->getCC_I());
            if (mx == 53) cpu->setCC_N(!cpu->getCC_N());
            if (mx == 54) cpu->setCC_Z(!cpu->getCC_Z());
            if (mx == 55) cpu->setCC_V(!cpu->getCC_V());
            if (mx == 56) cpu->setCC_C(!cpu->getCC_C());
        }
        // Register Clicked?
        bool bFound = false;
        for (auto& a : register_info)
        {
            if (a.y_pos == my && mx >= a.x_min && mx <= a.x_max)
            {
                // begin editing a register
                nRegisterBeingEdited.reg = a.reg;
                nRegisterBeingEdited.value = a.value;
                nRegisterBeingEdited.y_pos = a.y_pos;
                nRegisterBeingEdited.x_min = a.x_min;
                nRegisterBeingEdited.x_max = a.x_max;
                csr_x = mx;
                csr_y = my;
                bFound = true;
            }
        }
        if (!bFound)
            nRegisterBeingEdited.reg = EDIT_NONE;

        // left-click on code line toggles breakpoint
        if (mx > 39 && mx < 80 && my > 5 && my < 36 && s_bSingleStep)
        {
            if (sDisplayedAsm[my - 6] >= 0)
            {
                Word offset = sDisplayedAsm[my - 6];
                (mapBreakpoints[offset]) ?
                    mapBreakpoints[offset] = false :
                    mapBreakpoints[offset] = true;
            }
        }
    }
    last_LMB = (btns & 1);
    // right mouse button clicked
    static bool last_RMB = false;
    if (btns & 4 && !last_RMB)
    {
        // right-clicked on breakpoint
        if (mx >= 39 && my >= 41)
        {
            int index = (my - 41) + mw_brk_offset;
            // build a vector of active breakpoints
            std::vector<Word> breakpoints;
            for (auto& bp : mapBreakpoints)
                if (bp.second)
                    breakpoints.push_back(bp.first);
            if ((unsigned)index < breakpoints.size())
            {
                //printf("RIGHT CLICK: $%04X\n", breakpoints[index]);
                mapBreakpoints[breakpoints[index]] = false;
            }
        }

        // on PC register
        if (my == 4 && mx > 74 && mx < 79)
        {
            s_bSingleStep = !s_bSingleStep;
            if (!s_bSingleStep)
                nRegisterBeingEdited.reg = Debug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
        }
        // right-click on code line toggles breakpoint and resumes execution
        if (mx > 39 && mx < 80 && my > 5 && my < 36 && s_bSingleStep)
        {            
            if (sDisplayedAsm[my - 6] >= 0)
            {
                Word offset = sDisplayedAsm[my - 6];
                (mapBreakpoints[offset]) ?
                    mapBreakpoints[offset] = false :
                    mapBreakpoints[offset] = true;
                if (mapBreakpoints[offset] == true)
                    s_bSingleStep = false;
            }
        }
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
            if (csr_y < 10)      addr = mem_bank[0] + ((csr_y - 1 ) * 8);
            else if (csr_y < 19) addr = mem_bank[1] + ((csr_y - 10) * 8);
            else if (csr_y < 28) addr = mem_bank[2] + ((csr_y - 19) * 8);
            else if (csr_y < 36) addr = mem_bank[3] + ((csr_y - 28) * 8);
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
            Byte ofs = ((csr_x - 5) / 3) + ((csr_y - 1) * 8);
            Byte digit = ((csr_x + 1) % 3) - 1;
            Byte num = 0;
            Word addr = mem_bank[0];
            if (csr_y >  9 && csr_y < 18) { ofs -= 72 ; addr = mem_bank[1]; }
            if (csr_y > 18 && csr_y < 27) { ofs -= 144; addr = mem_bank[2]; }
            if (csr_y > 27 && csr_y < 36) { ofs -= 216; addr = mem_bank[3]; }

            Byte data = Memory::Read(addr + ofs);//, true);
            if (digit == 0) num = (data & 0xf0) >> 4;
            if (digit == 1) num = (data & 0x0f) >> 0;
            ch = _hex(num, 1);
            break;
        }
    }
    // display the reversed character
    static Byte attr = 0;

    const float delay = 1.0f / 120.0f;
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


void Debug::DrawCode(int col, int row) {

    if (bMouseWheelActive) {
        Old_DrawCode(col, row);
        return;
    }

    C6809* cpu = Bus::GetC6809();
    Word nextAddress = cpu->getPC();

    // Reset the displayed disassembly buffer
    for (auto &d : sDisplayedAsm) { d = -1; }

    // Display previous several instructions
    int top_rows = row + 13;
    _display_previous_instructions(col, top_rows);

    // Display current instruction
    int lower_rows = row + 14;
    _display_single_instruction(col, lower_rows, nextAddress);

    // Display next several instructions
    _display_next_instructions(col, lower_rows, nextAddress);

}


void Debug::_display_previous_instructions(int col, int& row) 
{
    C6809* cpu = Bus::GetC6809();
    
    int threshold = 100;
    int count = 14;
    int start_addr = (cpu->getPC()-1);              // + mousewheel_offset;
    for (int i = start_addr; i > 0; i--)
    {
        if (cpu->WasVisited_Memory(i))
        {
            bool atBreak = false;
            if (mapBreakpoints[i])	atBreak = true;
            Word throw_away;
            std::string code = cpu->disasm(i, throw_away);
            sDisplayedAsm[row-6] = i;
            OutText(col, row--, code, atBreak ? 0x50 : 0x80);
            count--;
            if (count == 0) break;
        }
        if (threshold-- < 0) break;
    }

}

void Debug::_display_single_instruction(int col, int& row, Word &nextAddress) 
{
    C6809* cpu = Bus::GetC6809();
    // current address
    Word currentAddress = nextAddress;

    // display the current instruction
    bool atBreak = mapBreakpoints[currentAddress];    
    std::string code = cpu->disasm(currentAddress, nextAddress);   // this updates nextAddress 
    sDisplayedAsm[row-6] = currentAddress;
    OutText(col, row++, code, atBreak ? 0xA0 : 0xF0);
}

void Debug::_display_next_instructions(int col, int& row, Word& nextAddress) 
{
    C6809* cpu = Bus::GetC6809();

    // Display the next several instructions
    int count = 0;
    while (count < 15) {
        bool atBreak = mapBreakpoints[nextAddress];

        // Disassemble the current instruction and update nextAddress to the next instruction
        Word currentAddress = nextAddress;
        std::string code = cpu->disasm(nextAddress, nextAddress);

        // Output the disassembled instruction
        //if (cpu->WasVisited_Memory(currentAddress)) {
            sDisplayedAsm[row-6] = currentAddress;
            OutText(col, row++, code, atBreak ? 0x30 : 0x10);
        //}

        // Increment count
        ++count;
    }
}


void Debug::Old_DrawCode(int col, int row)
{
    C6809* cpu = Bus::GetC6809();

    std::string code = "";
    int line = 0;
    Word next = 0;
    sDisplayedAsm.clear();

    if (bMouseWheelActive)
    {
        Word cpu_PC = cpu->getPC();
        Word offset = cpu_PC + mousewheel_offset;
        int max_lines = 30;
        while (line < max_lines)
        {
            if (offset < cpu_PC)
            {
                bool atBreak = false;
                if (mapBreakpoints[offset])	atBreak = true;
                sDisplayedAsm.push_back(offset);
                code = cpu->disasm(offset, offset);
                if (atBreak)
                    OutText(col, row + line, code, 0xA0);    // red
                else
                    OutText(col, row + line, code, 0x40);    // dk green
                line++;
            }
            if (offset == cpu_PC && line < max_lines)
            {
                bool atBreak = false;
                if (mapBreakpoints[offset])	atBreak = true;
                sDisplayedAsm.push_back(offset);
                code = cpu->disasm(offset, offset);
                if (atBreak)
                    OutText(col, row + line, code, 0x50);  // brown
                else
                    OutText(col, row + line, code, 0xC0);    // yellow
                line++;
            }
            if (offset > cpu_PC && line < max_lines)
            {
                bool atBreak = false;
                if (mapBreakpoints[offset])	atBreak = true;
                sDisplayedAsm.push_back(offset);
                code = cpu->disasm(offset, offset);
                if (atBreak)
                    OutText(col, row + line, code, 0x30);   // dk red
                else
                    OutText(col, row + line, code, 0xB0);   // lt green
                line++;
            }

        }

    }
    else
    {
        // draw the last several lines
        for (auto& a : asmHistory)
        {
            if (a != cpu->getPC())
            {
                bool atBreak = false;
                if (mapBreakpoints[a])	atBreak = true;
                sDisplayedAsm.push_back(a);
                code = cpu->disasm(a, next);
                if (atBreak)
                    OutText(col, row + line++, code, 0x30);      // 0x30 dk red
                else
                    OutText(col, row + line++, code, 0x10);    // 0x10 DK gray
            }
        }
        // draw the current line
        sDisplayedAsm.push_back(cpu->getPC());
        code = cpu->disasm(cpu->getPC(), next);
        if (mapBreakpoints[cpu->getPC()])
            OutText(col, row + line++, code, 0xA0);              // 0xA0 red
        else
            OutText(col, row + line++, code, 0xF0);            // 0xF0 white
        // create a history of addresses to display in the future
        static Word last = cpu->getPC();
        if (last != cpu->getPC())
        {
            last = cpu->getPC();
            asmHistory.push_back(cpu->getPC());
            while (asmHistory.size() > 12)
                asmHistory.pop_front();
        }
        // draw the next several future lines
        while (line < 24)
        {
            bool atBreak = false;
            if (mapBreakpoints[next])	atBreak = true;
            sDisplayedAsm.push_back(next);
            code = cpu->disasm(next, next);
            if (atBreak)
                OutText(col, row + line++, code, 0X30);          // 0X30 DK RED
            else
                OutText(col, row + line++, code,0X80);        // 0X80 LT GRAY
        }
    }
}




void Debug::DrawButtons()
{
    // C6809* cpu = Bus::GetC6809();

    // change the run/stop according to the single step state
    if (s_bSingleStep)
    {
        vButton[5].text = " RUN!";
        vButton[5].x_min = 02;
        vButton[5].clr_index = 0xB;
    }
    else {
        vButton[5].text = " STOP";
        vButton[5].x_min = 02;
        vButton[5].clr_index = 0xA;
    }
    if (bEditingBreakpoint)
    {
        vButton[9].text = "[$";
        vButton[9].text += _hex(new_breakpoint, 4);
        vButton[9].text += "]";
        vButton[9].x_min = 48;
        vButton[9].clr_index = 0xC;
    }
    else
    {
        vButton[9].text = "ADD BRK";
        vButton[9].x_min = 48;
        vButton[9].clr_index = 0xC;
    }
    // draw the buttons
    for (auto& a : vButton)
    {
        int x1 = a.x_min;
        int x2 = a.x_max;
        int y1 = a.y_min;
        int y2 = a.y_max;
        int p = ((a.clr_index % 16)<<4);

    // BUTTONS:  
    // 0xA1 = top left
    // 0xA2 = top right
    // 0xA3 = top horizontal
    // 0xA4 = bottom left
    // 0xA5 = left vertical
    // 0xA8 = bottom right
    // 0xAA = right vertical
    // 0xAC = bottom horizontal

        OutGlyph(x1, y1, 0xA1, p);          // top left
        for (int x = x1+1; x < x2; x++)
            OutGlyph(x, y1, 0xA3, p);       // top horizontal
        OutGlyph(x2, y1, 0xA2, p);          // top right
        for (int y = y1+1; y < y2; y++)
        {
            OutGlyph(x1, y, 0xA5, p);        // left vertical
            for (int x = x1+1; x < x2; x++)
                OutGlyph(x, y, 0x8F, p);     // solid
            OutGlyph(x2  , y, 0xAA, p);      // right vertical
        }
        OutGlyph(x1, y2, 0xA4, p);          // bottom left
        for (int x = x1+1; x < x2; x++)
            OutGlyph(x, y2, 0xAC, p);       // bottom horizontal
        OutGlyph(x2, y2, 0xA8, p);          // bottom right

        // foreground
        int sX = x1 + ((x2 - x1) / 2) - (int)a.text.size() / 2;
        OutText(sX, y1+1, a.text, a.clr_index % 16);
    }
}

void Debug::HandleButtons()
{
    // C6809* cpu = Bus::GetC6809();
    int mx, my;
    _correct_mouse_coords(mx, my);
    Uint32 btns = SDL_GetRelativeMouseState(NULL, NULL);

    static bool last_LMB = false;
    if (btns & 1 && !last_LMB)
    {
        for (auto& a : vButton)
        {
            if (my >= a.y_min && my <= a.y_max && mx >= a.x_min && mx <= a.x_max)
            {
                if (a.cbFunction)	(this->*a.cbFunction)();
            }
        }
    }
    last_LMB = (btns & 1);
}

void Debug::DrawBreakpoints()
{
    // C6809* cpu = Bus::GetC6809();

    int x = 71, y = 41;		// y <= 38
    // Uint8 ci = 0x0C;

    // build a vector of active breakpoints
    std::vector<Word> breakpoints;
    for (auto& bp : mapBreakpoints)
        if (bp.second)
            breakpoints.push_back(bp.first);
    // standard display
    if (breakpoints.size() < 8)
    {
        for (Word t = 0; t < breakpoints.size(); t++)
        {
            std::string strBkpt = "[$";
            strBkpt += _hex(breakpoints[t], 4);
            strBkpt += "]";
            OutText(x, y, strBkpt, 0xA0);
            y++;
        }
    }
    // oversized, mousewheel scrollable, display
    else
    {
        if (mw_brk_offset < 0)							
            mw_brk_offset = 0;
        if ((unsigned)mw_brk_offset + 7 > breakpoints.size())		
            mw_brk_offset = breakpoints.size() - 7;

        int index = mw_brk_offset;
        for (int t = 0; t < 7; t++)
        {
            std::string strBkpt;

            if (t == 0 && mw_brk_offset > 0)
                strBkpt = "[ ... ]";
            else if (t == 6 && (unsigned)index != breakpoints.size() - 1)
                strBkpt = "[ ... ]";
            else
            {
                strBkpt = "[$";
                strBkpt += _hex(breakpoints[index], 4);
                strBkpt += "]";
            }
            OutText(x, y, strBkpt, 0xA0);
            y++;
            index++;
        }
    }
}

bool Debug::EditRegister(float fElapsedTime)
{
    if (nRegisterBeingEdited.reg == EDIT_REGISTER::EDIT_NONE)
        return false;

    C6809* cpu = Bus::GetC6809();

    static float delay = 0.0625f;
    static float delayAcc = fElapsedTime;
    static int ci = 9;
    delayAcc += fElapsedTime;
    std::string ch = " ";

    if (delayAcc > delay + fElapsedTime)
    {
        delayAcc -= delay;
        ci++;
        if (ci > 15)	ci = 9;
    }
    // OutGlyph(csr_x, csr_y, 0x8f, (rand() % 16)<<4);

    Word data = 0;
    switch (nRegisterBeingEdited.reg) {
        case EDIT_CC:	data = (Word)cpu->getCC() << 8; break;
        case EDIT_D:	data = cpu->getD(); break;
        case EDIT_A:	data = (Word)cpu->getA() << 8; break;
        case EDIT_B:	data = (Word)cpu->getB() << 8; break;
        case EDIT_X:	data = cpu->getX(); break;
        case EDIT_Y:	data = cpu->getY(); break;
        case EDIT_U:	data = cpu->getU(); break;
        case EDIT_PC:	data = cpu->getPC(); s_bSingleStep = true;  break;
        case EDIT_S:	data = cpu->getS(); break;
        case EDIT_DP:	data = (Word)cpu->getDP() << 8; break;
        case EDIT_BREAK: data = new_breakpoint; break;
        case EDIT_NONE: break;
    }
    Byte digit = csr_x - nRegisterBeingEdited.x_min;
    Byte num = 0;
    if (digit == 0)	num = ((data & 0xf000) >> 12);
    if (digit == 1)	num = ((data & 0x0f00) >> 8);
    if (digit == 2)	num = ((data & 0x00f0) >> 4);
    if (digit == 3)	num = ((data & 0x000f) >> 0);
    ch = _hex(num, 1);
    // OutGlyph(csr_x, csr_y, ch[0], 0);
    OutGlyph(csr_x, csr_y, ch[0], (rand() % 16)<<0);

    // respond to numeric keys
    SDL_Keycode hx[] = { SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
                         SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7,
                         SDL_SCANCODE_8, SDL_SCANCODE_9, SDL_SCANCODE_A, SDL_SCANCODE_B,
                         SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E, SDL_SCANCODE_F };
    // char k[] = { '0', '1', '2', '3', '4', '5', '6', '7',
    //              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    char d[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    static bool state[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    // const Uint8* keybfr = SDL_GetKeyboardState(NULL);    // already defined
    Word n = 0;
    bool bKeyPressed = false;
    for (int t = 0; t < 16; t++)
    {
        if (state[t] == 0 && keybfr[hx[t]])
        {
            n = d[t];
            bKeyPressed = true;
            state[t] = 1;
        }
        // reset the key
        if (state[t] == 1 && !keybfr[hx[t]])	state[t] = 0;
    }

    if (bKeyPressed)
    {
        //printf("pressed\n");
        if (digit == 0)		data = (data & 0x0fff) | (n << 12);
        if (digit == 1)		data = (data & 0xf0ff) | (n << 8);
        if (digit == 2)		data = (data & 0xff0f) | (n << 4);
        if (digit == 3)		data = (data & 0xfff0) | (n << 0);

        switch (nRegisterBeingEdited.reg) {
            case EDIT_CC:	cpu->setCC(data >> 8);  break;
            case EDIT_D:	cpu->setD(data);		break;
            case EDIT_A:	cpu->setA(data >> 8);	break;
            case EDIT_B:	cpu->setB(data >> 8);	break;
            case EDIT_X:	cpu->setX(data);		break;
            case EDIT_Y:	cpu->setY(data);		break;
            case EDIT_U:	cpu->setU(data);		break;
            case EDIT_PC:	cpu->setPC(data);		bMouseWheelActive = false;  break;
            case EDIT_S:	cpu->setS(data);		break;
            case EDIT_DP:	cpu->setDP(data >> 8);  break;
            case EDIT_BREAK: new_breakpoint = data; break;
            case EDIT_NONE: break;
        }
        if (csr_x < nRegisterBeingEdited.x_max)
            csr_x++;
    }
    SDL_Keycode ex[] = { SDL_SCANCODE_LEFT , SDL_SCANCODE_RIGHT , SDL_SCANCODE_RETURN , SDL_SCANCODE_ESCAPE };
    static bool bEx[] = { 0, 0, 0, 0 };
    for (int t = 0; t < 4; t++)
    {
        if (bEx[t] == 0 && keybfr[ex[t]])
        {
            // left
            if (ex[t] == SDL_SCANCODE_LEFT)
                if (csr_x > nRegisterBeingEdited.x_min)
                    csr_x--;
            // right
            if (ex[t] == SDL_SCANCODE_RIGHT)
                if (csr_x < nRegisterBeingEdited.x_max)
                    csr_x++;
            // enter updates and saves the register
            if (ex[t] == SDL_SCANCODE_RETURN || ex[t] == SDL_SCANCODE_ESCAPE)
            {
                if (nRegisterBeingEdited.reg == EDIT_REGISTER::EDIT_BREAK)
                {
                    mapBreakpoints[new_breakpoint] = true;
                    nRegisterBeingEdited.reg = EDIT_REGISTER::EDIT_NONE;
                    bEditingBreakpoint = false;
                }
                nRegisterBeingEdited.reg = EDIT_REGISTER::EDIT_NONE;
            }
            bEx[t] = 1;
        }
        else if (bEx[t] == 1 && !keybfr[ex[t]])
            bEx[t] = 0;
    }
    return true;
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
            if (csr_y < 9)       addr = mem_bank[0] + ((csr_y -  1) * 8);
            else if (csr_y < 18) addr = mem_bank[1] + ((csr_y - 10) * 8);
            else if (csr_y < 27) addr = mem_bank[2] + ((csr_y - 19) * 8);
            else if (csr_y < 36) addr = mem_bank[3] + ((csr_y - 28) * 8);
            Byte digit = csr_x - 1;
            if (digit == 0)	addr = (addr & 0x0fff) | (ch << 12);
            if (digit == 1)	addr = (addr & 0xf0ff) | (ch << 8);
            if (digit == 2)	addr = (addr & 0xff0f) | (ch << 4);
            if (digit == 3)	addr = (addr & 0xfff0) | (ch << 0);
            if (csr_y < 9)			mem_bank[0] = addr - ((csr_y -  1) * 8);
            else if (csr_y < 18)	mem_bank[1] = addr - ((csr_y - 10) * 8);
            else if (csr_y < 27)	mem_bank[2] = addr - ((csr_y - 19) * 8);
            else if (csr_y < 36)	mem_bank[3] = addr - ((csr_y - 28) * 8);
            if (csr_x < 5)	while (!CoordIsValid(++csr_x, csr_y));
            break;
        }
        case CSR_AT::CSR_AT_DATA:
        {
            Byte ofs = ((csr_x - 5) / 3) + ((csr_y - 1) * 8);
            Byte digit = ((csr_x + 1) % 3) - 1;
            Word addr = mem_bank[0];
            if (csr_y >  9 && csr_y < 18) { ofs -= 72 ; addr = mem_bank[1]; }
            if (csr_y > 18 && csr_y < 27) { ofs -= 144; addr = mem_bank[2]; }
            if (csr_y > 27 && csr_y < 36) { ofs -= 216; addr = mem_bank[3]; }
            Byte data = Memory::Read(addr + ofs);//,true);
            if (digit == 0)		data = (data & 0x0f) | (ch << 4);
            if (digit == 1)		data = (data & 0xf0) | (ch << 0);
            Memory::Write(addr + ofs, data);
            if (csr_x < 28)		while (!CoordIsValid(++csr_x, csr_y));
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



// ********************
// * Button Callbacks *
// ********************

void Debug::cbClearBreaks()
{
    if (mapBreakpoints.size() > 0)
        mapBreakpoints.clear();
}
void Debug::cbReset()
{
    C6809* cpu = Bus::GetC6809();
    cpu->reset();
    mousewheel_offset = 0;
    bMouseWheelActive = false;
    s_bSingleStep = true;
    s_bIsStepPaused = true;
}
void Debug::cbNMI()
{
    C6809* cpu = Bus::GetC6809();
    cpu->nmi();
    s_bIsStepPaused = false;
}
void Debug::cbIRQ()
{
    C6809* cpu = Bus::GetC6809();
    cpu->irq();
    s_bIsStepPaused = false;
}
void Debug::cbFIRQ()
{    
    C6809* cpu = Bus::GetC6809();
    cpu->firq();
    s_bIsStepPaused = false;
}
void Debug::cbRunStop()
{
    (s_bSingleStep) ? s_bSingleStep = false : s_bSingleStep = true;
    s_bIsStepPaused = s_bSingleStep;
    nRegisterBeingEdited.reg = Debug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
    bMouseWheelActive = false;

    mousewheel_offset = -18;        // approx. middle of the dispayed listing
}
void Debug::cbHide()
{
    bMouseWheelActive = false;
    s_bSingleStep = false;
    s_bIsStepPaused = s_bSingleStep;
    nRegisterBeingEdited.reg = Debug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits

    s_bIsDebugActive = false;
    SDL_MinimizeWindow(_dbg_window);
}
void Debug::cbStepIn()  //F11
{
    s_bSingleStep = true;
    s_bIsStepPaused = false;
    nRegisterBeingEdited.reg = Debug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
    bMouseWheelActive = false;
}
void Debug::cbStepOver() //F10
{
    s_bSingleStep = true;
    s_bIsStepPaused = false;
    nRegisterBeingEdited.reg = Debug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
    bMouseWheelActive = false;
}
void Debug::cbAddBrk()
{
    printf("Add Breakpoint\n");
    bEditingBreakpoint = true;
    nRegisterBeingEdited.reg = Debug::EDIT_REGISTER::EDIT_BREAK;
}


// END: Debug.cpp
