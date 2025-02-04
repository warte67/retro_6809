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
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
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

    virtual int  OnAttach(int nextAddr) override;       // attach to the memory map
    virtual void OnInit() override;                     // initialize
    virtual void OnQuit() override;                     // shutdown
    virtual void OnActivate() override;                 // activate
    virtual void OnDeactivate() override;               // deactivate
    virtual void OnEvent(SDL_Event* evnt) override;     // handle events
    virtual void OnUpdate(float fElapsedTime) override; // update
    virtual void OnRender() override;                   // render
    // virtual bool OnTest() override;                     // Unit Tests

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
    void ContinueSingleStep();\
    inline bool IsDebugActive() { return s_bIsDebugActive; }
    inline bool IsCursorVisible() { return bIsCursorVisible; }
    inline void SetDebugActive(bool value) { s_bIsDebugActive = value; }


    SDL_WindowID Get_Window_ID() { return SDL_GetWindowID( _dbg_window ); }
    SDL_Window* Get_SDL_Window() { return _dbg_window; }

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

private: // PRIVATE MEMBERS

    void _clear_texture(SDL_Texture* texture, Byte r, Byte g, Byte b, Byte a);
    void _setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte palette_index);
    void OutGlyph(int col, int row, Byte glyph, Byte color_attr);
    int OutText(int col, int row, std::string text, Byte color_attr);
    std::string _hex(Uint32 n, Uint8 d);
    void _update_debug_screen();
    // void _update_mouse_pos(); 

    void DrawMemoryFrame(int col, int row);

    void DumpMemory(int col, int row, Word addr);
    void DrawCpu(int x, int y);
    void DrawCode(int col, int row);

    void Old_DrawCode(int col, int row);

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
    void cbExit();
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
        // REGISTER               value, y_pos, x_min, x_max
        { EDIT_REGISTER::EDIT_NONE,  0,    0,      0,     0     },
        { EDIT_REGISTER::EDIT_CC,    0,    1,     44,    45     },
        { EDIT_REGISTER::EDIT_D,     0,    2,     44,    47     },
        { EDIT_REGISTER::EDIT_A,     0,    2,     52,    53     },
        { EDIT_REGISTER::EDIT_B,     0,    2,     60,    61     },
        { EDIT_REGISTER::EDIT_X,     0,    3,     44,    47     },
        { EDIT_REGISTER::EDIT_Y,     0,    3,     52,    55     },
        { EDIT_REGISTER::EDIT_U,     0,    3,     60,    63     },
        { EDIT_REGISTER::EDIT_PC,    0,    4,     44,    47     },
        { EDIT_REGISTER::EDIT_S,     0,    4,     52,    55     },
        { EDIT_REGISTER::EDIT_DP,    0,    4,     61,    62     },
        { EDIT_REGISTER::EDIT_BREAK, 0,   43,     48,    51     },
    };
    REGISTER_NODE nRegisterBeingEdited = { EDIT_NONE,0,0,0,0 };

    struct BUTTON_NODE {
        std::string text;		// button text
        SDL_Scancode key;		// shortcut key scancode
        Uint16 x_min;			// button left
        Uint16 x_max;			// button right
        Uint16 y_min;           // button top
        Uint16 y_max;           // button bottom
        Byte clr_index;			// color index
        void (Debug::* cbFunction)();	// button callback
    };

    enum BTN_ID { 
        CLEAR_BREAKS_ID = 0, 
        RESET_ID, 
        NMI_ID, 
        IRQ_ID, 
        FIRQ_ID, 
        RUN_STOP_ID, 
        EXIT_ID, 
        STEP_INTO_ID, 
        STEP_OVER_ID, 
        ADD_BREAK_ID, 
        HIDE_ID,
        LAST_ID 
    };
    std::vector<BUTTON_NODE> vButton = {       //   X1  X2  Y1  Y2  CLR  CB
        {"Clear Breaks",        SDL_SCANCODE_C,		40, 56, 39, 41, 0x5, &Debug::cbClearBreaks},
        {"Reset ",			    SDL_SCANCODE_R,		02, 10, 42, 44, 0xD, &Debug::cbReset },         // Reset
        {"NMI ",                SDL_SCANCODE_N,		28, 36, 42, 44, 0xD, &Debug::cbNMI },
        {"IRQ" ,                SDL_SCANCODE_I,		20, 27, 42, 44, 0xD, &Debug::cbIRQ },
        {"FIRQ ",			    SDL_SCANCODE_F,		11, 19, 42, 44, 0xD, &Debug::cbFIRQ },
        {"RUN! ",               SDL_SCANCODE_D,		02, 10, 39, 41, 0xB, &Debug::cbRunStop },       // Run / Stop
        {"EXIT",			    SDL_SCANCODE_X,		72, 77, 01, 03, 0x1, &Debug::cbExit },
        {"STEP_INTO ",		    SDL_SCANCODE_SPACE,	11, 23, 39, 41, 0x9, &Debug::cbStepIn },
        {"STEP_OVER ",		    SDL_SCANCODE_O,		24, 36, 39, 41, 0x9, &Debug::cbStepOver },
        {"Add Breakpoint",	    SDL_SCANCODE_B,		40, 56, 42, 44, 0x5, &Debug::cbAddBrk },
        {"HIDE",			    SDL_SCANCODE_H,		66, 71, 01, 03, 0x1, &Debug::cbHide },          
    };

    // breakpoint window stuff
    int _bkp_window_len = 12;
    int _bkp_window_width = 14;
    int _bkp_window_xmin = 58;  //71;
    int _bkp_window_ymin = 39;  //41;
    int _bkp_window_xmax = _bkp_window_xmin +_bkp_window_width;
    int _bkp_window_ymax = _bkp_window_ymin + _bkp_window_len;
    
    // std::vector<Word> mem_bank = { static_cast<Word>(MAP(SSTACK_TOP) - 0x0048), MAP(VIDEO_START), 0xFE00, 0x0000 };
    std::vector<Word> mem_bank = { 0x0000, 0x0400, 0x2400, 0xFE00 };


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