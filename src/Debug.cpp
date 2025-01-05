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
        SDL_LOGICAL_PRESENTATION_DISABLED);

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
    if (!(SDL_GetWindowFlags(_dbg_window) & SDL_WINDOW_INPUT_FOCUS)) 
        return true;

    switch (evnt->type) 
    {
        // handle default events SDL_QUIT and ALT-X quits

        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:  
            SDL_MinimizeWindow(_dbg_window);
            break;

    }
    return true;
}


bool Debug::OnUpdate(float fElapsedTime)
{
    if (fElapsedTime==0.0f) { ; } // stop the compiler from complaining

    // if the debugger is not active, just return
    // ...

    // _clear_texture(_dbg_texture, 1, 2, 0, 15);

    // restrict the debugger update rate to reduce CPU usage    
    static float reference_time = 0.025f;
    static float delta_time = 99999.0f;
    if (delta_time > (fElapsedTime + reference_time) ) {
        delta_time = fElapsedTime;
    } else {
        delta_time += fElapsedTime;
        return true;
    }


    _updateDebugScreen();

    // // clear the debugger texture
    // _clear_texture(_dbg_texture, 1, 2, 0, 15);
    //
    // // fill the debugger texture with colored bars
    // void *pixels;
    // int pitch;
    // Byte r = 1, g = 2, b = 0, a = 15;
    // if (!SDL_LockTexture(_dbg_texture, NULL, &pixels, &pitch)) {
    //     Bus::Error(SDL_GetError());
    // } else {
    //     for (int y = 0; y < _dbg_height; y++) {
    //         for (int x = 0; x < _dbg_width; x++) {
    //             Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));
    //             *dst = ( (a<<12) | (r<<8) | (g<<4) | (b<<0) );
    //             r++; r&=0x0f;
    //             if (r==0) { g++; g&=0x0f; }
    //             if (r==0 && g==0) { b++; b&=0x0f; }                
    //         }
    //     }
    //     SDL_UnlockTexture(_dbg_texture); 
    // }          

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

void Debug::_updateDebugScreen() 
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
			Byte bg = at >> 4;
			Byte fg = at & 0x0f;
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

// END: Debug.cpp
