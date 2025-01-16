/*** Mouse.hpp *******************************************
 *      __  __                        _                 
 *     |  \/  |                      | |                
 *     | \  / | ___  _   _ ___  ___  | |__  _ __  _ __  
 *     | |\/| |/ _ \| | | / __|/ _ \ | '_ \| '_ \| '_ \ 
 *     | |  | | (_) | |_| \__ \  __/_| | | | |_) | |_) |
 *     |_|  |_|\___/ \__,_|___/\___(_)_| |_| .__/| .__/ 
 *                                         | |   | |    
 *                                         |_|   |_|    
 * 
 * This file serves meerly as a template as a
 * starting point for new devices.
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#pragma once

#include "IDevice.hpp"

class Mouse : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    Mouse();
    virtual ~Mouse();

public: // VIRTUAL METHODS

    virtual int  OnAttach(int nextAddr);            // attach to the memory map
    virtual void OnInit();                          // initialize
    virtual void OnQuit();                          // shutdown
    virtual void OnActivate();                      // activate
    virtual void OnDeactivate();                    // deactivate
    virtual void OnEvent(SDL_Event* evnt);          // handle events
    virtual void OnUpdate(float fElapsedTime);      // update
    virtual void OnRender();                        // render

public: // PUBLIC ACCESSORS
    Uint8 red(Uint8 index) { Uint8 c = _csr_palette[index].r;  return c; }
    Uint8 grn(Uint8 index) { Uint8 c = _csr_palette[index].g;  return c; }
    Uint8 blu(Uint8 index) { Uint8 c = _csr_palette[index].b;  return c; }
    Uint8 alf(Uint8 index) { Uint8 c = _csr_palette[index].a;  return c; }

private: // PRIVATE MEMBERS

    // palette stuff
    union PALETTE {
        Word color;
        struct {
            Uint8 b : 4;		// blue
            Uint8 g : 4;		// green
            Uint8 r : 4;		// red
            Uint8 a : 4;		// alpha
        };
    };
    Byte m_palette_index = 0x00;   // DSP_PAL_IDX
    std::vector<PALETTE> _csr_palette = {};

    // starting cursor image
    std::vector<std::string> csr_buffer = 
    {
        "0000            ",
        "08F0000         ",
        "018FFF00000     ",
        "0018FFFFFF0     ",
        " 0118FFF000     ",
        " 01118000       ",
        " 001100         ",
        "  0110          ",
        "  0100          ",
        "  010           ",
        "  000           ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                "
    };
    std::vector<Byte> csr_data;

    // private helpers
    void _show_SDL_cursor(bool b);      // hides or shows the SDL and Hardware cursors 
    void _display_SDL_cursor();         // displays the cursor bitmap image
    void _render_csr_buffer();
    void _setPixel_cursor(void* pixels, int pitch, int x, int y, Byte color_index);

    // SDL Stuff
    // SDL_Texture* _cursor_texture = nullptr;

    // internal state
    int mouse_x = 0;			// horizontal mouse cursor position
    int mouse_y = 0;			// vertical mouse cursor position
    char mouse_wheel = 0;		// signed mouse wheel delta
    Uint8 mouse_x_offset = 0;	// mouse cursor offset x
    Uint8 mouse_y_offset = 0;	// mouse cursor offset y
    Byte button_flags = 0;		// bits 0-4: button states, bit 5: cursor enable, bits 6-7: number of clicks
    bool _bCsrIsDirty = true;	// internal flag
    // bool _bUseSdlCursor = ENABLE_SDL_MOUSE_CURSOR;  // was _bCsrIsVisible

    // cursor stuff
    Byte bmp_offset = 0;
    Byte cursor_buffer[16][16];     // indexed 16-color bitmap data for the mouse cursor
};

// END: Mouse.hpp