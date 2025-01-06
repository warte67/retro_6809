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
    virtual void OnInit();                          // initialize
    virtual void OnQuit();                          // shutdown
    virtual void OnActivate();                      // activate
    virtual void OnDeactivate();                    // deactivate
    virtual void OnEvent(SDL_Event* evnt);          // handle events
    virtual void OnUpdate(float fElapsedTime);      // update
    virtual void OnRender();                        // render

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
    void _update_debug_screen();
    // void _update_mouse_pos(); 

    void DumpMemory(int col, int row, Word addr);
    bool CoordIsValid(int x, int y);
    void _correct_mouse_coords(int& mx, int& my);

    void MouseStuff();
    void KeyboardStuff();

    void DrawCursor(float fElapsedTime);


    // hardware registers
    Word _dbg_brk_addr  = 0;    // DBG_BRK_ADDR
    enum _DBG_FLAGS {
        DBGF_DEBUG_ENABLE       = 0x80, // - bit 7: Debug Enable
        DBGF_SINGLE_STEP_ENABLE = 0x40, // - bit 6: Single Step Enable
        DBGF_CLEAR_ALL_BRKPT    = 0x20, // - bit 5: Clear All Breakpoints
        DBGF_UPDATE_BRKPT       = 0x10, // - bit 4: Update Breakpoint at DEBUG_BRK_ADDR
        DBGF_FIRQ               = 0x08, // - bit 3: FIRQ  (on low to high edge)
        DBGF_IRQ                = 0x04, // - bit 2: IRQ   (on low to high edge)
        DBGF_NMI                = 0x02, // - bit 1: NMI   (on low to high edge)
        DBGF_RESET              = 0x01  // - bit 0: RESET (on low to high edge)       
    };
    Byte _dbg_flags     = DBGF_DEBUG_ENABLE;    // DBG_FLAGS

    // debug display
    int _dbg_width = DEBUG_WIDTH;
    int _dbg_height = DEBUG_HEIGHT;
    int _dbg_window_width = DEBUG_WINDOW_WIDTH;
    int _dbg_window_height = DEBUG_WINDOW_HEIGHT;

	Uint32 _dbg_window_flags = SDL_WINDOW_RESIZABLE;
    Uint32 _dbg_renderer_flags = SDL_RENDERER_VSYNC_DISABLED;

    SDL_Window*   _dbg_window   = nullptr;
    SDL_Renderer* _dbg_renderer = nullptr;
    SDL_Texture*  _dbg_texture  = nullptr;

    std::vector <Word> mem_bank = { SSTACK_TOP - 0x0048, VIDEO_START, USER_RAM, 0xFE00 };


    // debugger stuctures
    enum CSR_AT {
        CSR_AT_NONE, CSR_AT_ADDRESS, CSR_AT_DATA, CSR_AT_REGISTER
    };    
    int csr_x = 0;
    int csr_y = 0;
    int csr_at = CSR_AT::CSR_AT_NONE;   
    char mouse_wheel = 0; 
    bool bIsCursorVisible = false;
    bool bIsMouseOver = false;

    const bool* keybfr = SDL_GetKeyboardState(NULL);

    struct D_GLYPH 
    { 
        Byte attr;
        Byte chr; 
    };
    std::vector<D_GLYPH> _db_bfr{0};

};

// END: Debug.hpp