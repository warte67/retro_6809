/*** GPU.hpp *******************************************
 *     _____ _____  _    _       _                 
 *    / ____|  __ \| |  | |     | |                
 *   | |  __| |__) | |  | |     | |__  _ __  _ __  
 *   | | |_ |  ___/| |  | |     | '_ \| '_ \| '_ \ 
 *   | |__| | |    | |__| |  _  | | | | |_) | |_) |
 *    \_____|_|     \____/  (_) |_| |_| .__/| .__/ 
 *                                    | |   | |    
 *                                    |_|   |_|    
 *   
 * This is the base class for the primary graphics devices. It provides the
 * basic methods for reading and writing to the device's memory.
 * 
 ************************************/

#pragma once

#include "IDevice.hpp"

class GPU : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    GPU();
    virtual ~GPU();

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
    void RenderPresent() {
        SDL_RenderPresent(pRenderer);
    }
    SDL_Window* GetWindow() { return pWindow; }         // get the SDL window
    SDL_Renderer* GetRenderer() { return pRenderer; }   // get the SDL renderer

private: // PRIVATE MEMBERS
    // internal hardware register states:
    Byte _gfx_mode = 0b00000010;    // default: text, max resolution, 16 colors
    Byte _gfx_emu =  0b00000000;    // default: no debug

    bool _change_gfx_mode(Byte data);
    bool _change_emu_mode(Byte data);

	// SDL stuff
	SDL_Window* pWindow = nullptr;
	SDL_Renderer* pRenderer = nullptr;
	// Uint32 window_flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	Uint32 window_flags = SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE;
    // Uint32 renderer_flags = SDL_RENDERER_ACCELERATED_2D | SDL_RENDERER_PRESENTVSYNC;
    Uint32 renderer_flags = SDL_RENDERER_VSYNC_ADAPTIVE;    
};

/*** NOTES: ****************************************
 
    Total Video Resolution:    400x240 with borders
    Actual Video Resolution:   320x200
    Top Border:                20 pixels
    Bottom Border:             20 pixels
    Left Border:               40 pixels
    Right Border:              40 pixels

    4 color modes:             2 colors, 4 colors, 16 colors, 256 colors
    Overscan Horiz and Vert:   1x, 2x, 4x
    Horizontal Resolutions:    320, 160, 80
    Vertical Resolutions:      200, 100, 50

    2-bits for color
    2-bits for Vscan
    2-bits for Hscan
    1-bit for text/bitmap mode
    1-bit reserved
  
 ****************************************/



// END: Gfx.hpp