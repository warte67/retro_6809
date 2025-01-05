/*** Debug.hpp *******************************************
 *      _____       _                   _                 
 *     |  __ \     | |                 | |                
 *     | |  | | ___| |__  _   _  __ _  | |__  _ __  _ __  
 *     | |  | |/ _ \ '_ \| | | |/ _` | | '_ \| '_ \| '_ \ 
 *     | |__| |  __/ |_) | |_| | (_| |_| | | | |_) | |_) |
 *     |_____/ \___|_.__/ \__,_|\__, (_)_| |_| .__/| .__/ 
 *                               __/ |       | |   | |    
 *                              |___/        |_|   |_|                                                                                      | |     | |    
 *
 ************************************/

#pragma once

#include "IDevice.hpp"


class Debug : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    Debug();
    virtual ~Debug();

public: // VIRTUAL METHODS
    virtual Byte OnRead(Word offset);               // read
    virtual void OnWrite(Word offset, Byte data);   // write
    // =============================================
    virtual int  OnAttach(int nextAddr);            // attach to the memory map
    virtual bool OnInit();                          // initialize
    virtual bool OnQuit();                          // shutdown
    virtual bool OnActivate();                      // activate
    virtual bool OnDeactivate();                    // deactivate
    virtual bool OnEvent(SDL_Event* evnt);          // handle events
    virtual bool OnUpdate(float fElapsedTime);      // update
    virtual bool OnRender();                        // render

public: // PUBLIC ACCESSORS
    // ... 

private: // PRIVATE MEMBERS

    void _clear_texture(SDL_Texture* texture, Byte r, Byte g, Byte b, Byte a);
    void _setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte r, Byte g, Byte b, Byte a);

    // hardware registers
    Word _dbg_brk_addr  = 0;    // DBG_BRK_ADDR
    Byte _dbg_flags     = 0;    // DBG_FLAGS
    // DBG_FLAGS    = 0xFE07,   // (Byte) Debug Specific Hardware Flags:
    //                          // - bit 7: Debug Enable
    //                          // - bit 6: Single Step Enable
    //                          // - bit 5: Clear All Breakpoints
    //                          // - bit 4: Update Breakpoint at DEBUG_BRK_ADDR
    //                          // - bit 3: FIRQ  (on low to high edge)
    //                          // - bit 2: IRQ   (on low to high edge)
    //                          // - bit 1: NMI   (on low to high edge)
    //                          // - bit 0: RESET (on low to high edge)

    int _dbg_logical_width = 1024;
    int _dbg_logical_height = 640;
    int _dbg_window_width = 640*2.125;
    int _dbg_window_height = 400*2.125;

	Uint32 _dbg_window_flags = SDL_WINDOW_RESIZABLE;
    Uint32 _dbg_renderer_flags = SDL_RENDERER_VSYNC_DISABLED;

    SDL_Window*   _dbg_window   = nullptr;
    SDL_Renderer* _dbg_renderer = nullptr;
    SDL_Texture*  _dbg_texture  = nullptr;

};

// END: Debug.hpp