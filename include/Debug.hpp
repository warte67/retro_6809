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

#include <list>
#include <unordered_map>
#include <map>

#include "types.hpp"
#include "Memory.hpp"
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

    bool SingleStep();
    void ContinueSingleStep();


private: // PRIVATE MEMBERS

    void _clear_texture(SDL_Texture* texture, Byte r, Byte g, Byte b, Byte a);
    void _setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte palette_index);
    void OutGlyph(int col, int row, Byte glyph, Byte color_attr);
    int OutText(int col, int row, std::string text, Byte color_attr);
    std::string _hex(Uint32 n, Uint8 d);
    void _update_debug_screen();
    // void _update_mouse_pos(); 

    void DumpMemory(int col, int row, Word addr);
    void DrawCpu(int x, int y);
    void DrawCode(int col, int row);
    void _display_previous_instructions(int col, int& row);
    void _display_single_instruction(int col, int& row, Word &nextAddress);
    void _display_next_instructions(int col, int& row, Word &nextAddress);

    void DrawButtons();
    void HandleButtons();
    void DrawBreakpoints();
    bool EditRegister(float fElapsedTime);


    bool CoordIsValid(int x, int y);
    void _correct_mouse_coords(int& mx, int& my);

    void MouseStuff();
    void KeyboardStuff();

    void DrawCursor(float fElapsedTime);


    // hardware registers
    enum _DBG_FLAGS : Byte {
        DBGF_DEBUG_ENABLE       = 0x80, // - bit 7: Debug Enable
        DBGF_SINGLE_STEP_ENABLE = 0x40, // - bit 6: Single Step Enable
        DBGF_CLEAR_ALL_BRKPT    = 0x20, // - bit 5: Clear All Breakpoints
        DBGF_UPDATE_BRKPT       = 0x10, // - bit 4: Update Breakpoint at DEBUG_BRK_ADDR
        DBGF_FIRQ               = 0x08, // - bit 3: FIRQ  (on low to high edge)
        DBGF_IRQ                = 0x04, // - bit 2: IRQ   (on low to high edge)
        DBGF_NMI                = 0x02, // - bit 1: NMI   (on low to high edge)
        DBGF_RESET              = 0x01  // - bit 0: RESET (on low to high edge)       
    };

    // debug display
    int _dbg_width = DEBUG_WIDTH;
    int _dbg_height = DEBUG_HEIGHT;
    int _dbg_window_width = DEBUG_WINDOW_WIDTH;
    int _dbg_window_height = DEBUG_WINDOW_HEIGHT;

	Uint32 _dbg_window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    Uint32 _dbg_renderer_flags = SDL_RENDERER_VSYNC_DISABLED;

    SDL_Window*   _dbg_window   = nullptr;
    SDL_Renderer* _dbg_renderer = nullptr;
    SDL_Texture*  _dbg_texture  = nullptr;


    // button callbacks
    void cbClearBreaks();
    void cbReset();
    void cbNMI();
    void cbIRQ();
    void cbFIRQ();
    void cbRunStop();
    void cbHide();
    void cbStepIn();
    void cbStepOver();
    void cbAddBrk();

    // debugger stuctures
    enum CSR_AT {
        CSR_AT_NONE, CSR_AT_ADDRESS, CSR_AT_DATA, CSR_AT_REGISTER
    };    
    enum EDIT_REGISTER {
        EDIT_NONE,
        EDIT_CC, EDIT_D, EDIT_A, EDIT_B, EDIT_X,
        EDIT_Y, EDIT_U, EDIT_PC, EDIT_S, EDIT_DP,
        EDIT_BREAK,
    };
    struct REGISTER_NODE {
        EDIT_REGISTER reg = EDIT_NONE;
        Uint16 value = 0;
        Uint16 y_pos = 0;
        Uint16 x_min = 0;
        Uint16 x_max = 0;
    };
    std::vector<REGISTER_NODE> register_info = {
        { EDIT_REGISTER::EDIT_NONE,  0,  0,  0,  0   },
        { EDIT_REGISTER::EDIT_CC,    0,  1, 75, 76 },
        { EDIT_REGISTER::EDIT_D,     0,  2, 75, 78 },
        { EDIT_REGISTER::EDIT_A,     0,  2, 83, 84 },
        { EDIT_REGISTER::EDIT_B,     0,  2, 91, 92 },
        { EDIT_REGISTER::EDIT_X,     0,  3, 75, 78 },
        { EDIT_REGISTER::EDIT_Y,     0,  3, 83, 86 },
        { EDIT_REGISTER::EDIT_U,     0,  3, 91, 94 },
        { EDIT_REGISTER::EDIT_PC,    0,  4, 75, 78 },
        { EDIT_REGISTER::EDIT_S,     0,  4, 83, 86 },
        { EDIT_REGISTER::EDIT_DP,    0,  4, 92, 93 },
        { EDIT_REGISTER::EDIT_BREAK, 0, 43, 50, 53 },
    };
    REGISTER_NODE nRegisterBeingEdited = { EDIT_NONE,0,0,0,0 };

    struct BUTTON_NODE {
        std::string text;		// button text
        SDL_Scancode key;		// shortcut key scancode
        Uint16 x_min;			// button left
        Uint16 x_max;			// button right
        Uint16 y_pos;
        Byte clr_index;			// color index
        void (Debug::* cbFunction)();	// button callback
    };
    std::vector<BUTTON_NODE> vButton = {	
        {" Clr Breaks",		SDL_SCANCODE_C,		51, 62, 41, 0xC, &Debug::cbClearBreaks},
        {"Reset",			SDL_SCANCODE_R,		43, 49, 41, 0xB, &Debug::cbReset },
        {"NMI",				SDL_SCANCODE_N,		37, 41, 41, 0xD, &Debug::cbNMI },
        {"IRQ",				SDL_SCANCODE_I,		31, 35, 41, 0xD, &Debug::cbIRQ },
        {" FIRQ",			SDL_SCANCODE_F,		24, 29, 41, 0xD, &Debug::cbFIRQ },
        {" RUN",			SDL_SCANCODE_D,		17, 22, 41, 0xB, &Debug::cbRunStop },
        {" EXIT",			SDL_SCANCODE_H,		17, 22, 43, 0xB, &Debug::cbHide },
        {"STEP_INTO",		SDL_SCANCODE_SPACE,	24, 34, 43, 0x9, &Debug::cbStepIn },
        {"STEP_OVER",		SDL_SCANCODE_O,		36, 46, 43, 0x9, &Debug::cbStepOver },
        {"ADD BRK",			SDL_SCANCODE_B,		48, 54, 43, 0xC, &Debug::cbAddBrk },
    };

    std::vector<Word> mem_bank = { static_cast<Word>(MAP(SSTACK_TOP) - 0x0048), MAP(VIDEO_START), 0xFE00, 0x0000 };


    std::vector<int> sDisplayedAsm = std::vector<int>(34, -1);  // 0-33 are valid, otherwise invalid or not displayed

    std::unordered_map<Word, bool> mapBreakpoints;	
    std::list<Word> asmHistory;		// track last several asm addresses

    int csr_x = 0;
    int csr_y = 0;
    int csr_at = CSR_AT::CSR_AT_NONE;   
    char mouse_wheel = 0; 
    bool bIsStepPaused = true;
    bool bIsCursorVisible = false;
    int mousewheel_offset = 0;		// applies to code scroll
    bool bMouseWheelActive = false; // applies to code scroll
    int mw_brk_offset = 0;			// mouse wheel adjusts the offset    
    bool bIsMouseOver = false;

    bool bEditingBreakpoint = false;
    Word new_breakpoint = 0;		// working copy to be edited


    // SYS_DBG_BRK_ADDR               // (Word) Address of current debug breakpoint
    Word _dbg_brk_addr  = 0;          
    // SYS_DBG_FLAGS                  // (Byte) Debug Specific Hardware Flags
    Byte _dbg_flags     = DBGF_DEBUG_ENABLE;
                                      // - bit 7: Debug Enable
                                      // - bit 6: Single Step Enable
                                      // - bit 5: Clear All Breakpoints
                                      // - bit 4: Update Breakpoint at DEBUG_BRK_ADDR
                                      // - bit 3: FIRQ  (on low {0} to high {1} edge)
                                      // - bit 2: IRQ   (on low {0} to high {1} edge)
                                      // - bit 1: NMI   (on low {0} to high {1} edge)
                                      // - bit 0: RESET (on low {0} to high {1} edge)
                                      // 



    inline static bool s_bIsDebugActive = DEBUG_STARTS_ACTIVE;
    inline static bool s_bSingleStep = DEBUG_SINGLE_STEP;
    inline static bool s_bIsStepPaused = true;        

    const bool* keybfr = SDL_GetKeyboardState(NULL);

    struct D_GLYPH 
    { 
        Byte attr;
        Byte chr; 
    };
    std::vector<D_GLYPH> _db_bfr{0};

};

// END: Debug.hpp