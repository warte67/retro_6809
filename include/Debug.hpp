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
#include "font8x8_system.hpp"

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

    // palette stuff
    union PALETTE {
        Word color;
        struct {
            Uint8 b : 4;
            Uint8 g : 4;
            Uint8 r : 4;
            Uint8 a : 4;
        };
    };
    Uint8 red(Uint8 index) { Uint8 c = _debug_palette[index].r;  return c; }
    Uint8 grn(Uint8 index) { Uint8 c = _debug_palette[index].g;  return c; }
    Uint8 blu(Uint8 index) { Uint8 c = _debug_palette[index].b;  return c; }
    Uint8 alf(Uint8 index) { Uint8 c = _debug_palette[index].a;  return c; }  

    std::vector<PALETTE> _debug_palette = {    
        { 0xF000 },		// 0: black
        { 0xF555 },		// 1: dk gray
        { 0xF007 },		// 2: dk blue
        { 0xF600 },		// 3: dk red
        { 0xF140 },		// 4: dk green
        { 0xF840 },		// 5: brown
        { 0xF406 },		// 6: purple          
        { 0xF046 },		// 7: deep sea
        { 0xF888 },		// 8: lt gray
        { 0xF22F },		// 9: blue
        { 0xFd00 },		// A: red
        { 0xF4F6 },		// B: lt green
        { 0xFED0 },		// C: yellow
        { 0xF85b },		// D: Lt Purple
        { 0xF59f },		// E: lt sky
        { 0xFFFF }		// F: white
    };

private: // PRIVATE MEMBERS

    void _clear_texture(SDL_Texture* texture, Byte r, Byte g, Byte b, Byte a);
    void _setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte palette_index);
    void OutGlyph(int col, int row, Byte glyph, Byte color_attr);
    int OutText(int col, int row, std::string text, Byte color_attr);
    std::string _hex(Uint32 n, Uint8 d);
    void _updateDebugScreen();

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

    int _dbg_width = DEBUG_WIDTH;
    int _dbg_height = DEBUG_HEIGHT;
    int _dbg_window_width = DEBUG_WINDOW_WIDTH;
    int _dbg_window_height = DEBUG_WINDOW_HEIGHT;

	Uint32 _dbg_window_flags = SDL_WINDOW_RESIZABLE;
    Uint32 _dbg_renderer_flags = SDL_RENDERER_VSYNC_DISABLED;

    SDL_Window*   _dbg_window   = nullptr;
    SDL_Renderer* _dbg_renderer = nullptr;
    SDL_Texture*  _dbg_texture  = nullptr;



    struct D_GLYPH 
    { 
        Byte attr;
        Byte chr; 
    };
    std::vector<D_GLYPH> _db_bfr{0};

};

// END: Debug.hpp