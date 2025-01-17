#pragma once
/**  XK_Keycodes.hpp  ***********************************************************
 * 
 *     __   ___  __  _  __                        _             _                 
 *     \ \ / / |/ / | |/ /                       | |           | |                
 *      \ V /| ' /  | ' / ___ _   _  ___ ___   __| | ___  ___  | |__  _ __  _ __  
 *       > < |  <   |  < / _ \ | | |/ __/ _ \ / _` |/ _ \/ __| | '_ \| '_ \| '_ \ 
 *      / . \| . \  | . \  __/ |_| | (_| (_) | (_| |  __/\__ \_| | | | |_) | |_) |
 *     /_/ \_\_|\_\ |_|\_\___|\__, |\___\___/ \__,_|\___||___(_)_| |_| .__/| .__/ 
 *              ______         __/ |                                 | |   | |    
 *             |______|       |___/                                  |_|   |_| 
 * 
 *   NOTES:
 *       To fit into 32 8-bit keyboard flag registers, the
 *       number of allowed keys must not exceed 256.
 *       SDL currently defines 290. I think exclusive support
 *       for the standard US/UK 102 key keyboard is needed.
 *   
 *       In accordance with the above statement. Key scan codes
 *       should be limited to no more than 7 bits worth of indicies.
 *       The sign bit on the index will represent KEYUP or KEYDOWN
 *       states.
 *   
 *       The bulk of the work this class will be doing is the keycode
 *       conversion between, SDL_scancodes, VK_CODES, and XK_CODES.
 *       Ultimately, the XK_CODES will be mostly based on Windows
 *       VK_CODES, but with a few exceptions.
 * 
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *   
 *********************************************************************************/
#pragma once 

namespace XKey
{
    typedef enum
    {
        NONE        = 0x00,        // INVALID, NULL, or UNASSIGNED key types
        NUMLOCK     = 0x01,        // NUMLOCK key
        SCROLL      = 0x02,        // SCROLL LOCK key
        BREAK       = 0x03,        // [CTRL]+[BREAK] the "BREAK KEY" (also CTRL-C)      
        L_SHIFT     = 0x04,        // LEFT-SHIFT key
        R_SHIFT     = 0x05,        // RIGHT-SHIFT key
        L_CONTROL   = 0x06,        // LEFT-CONTROL key
        R_CONTROL   = 0x07,        // RIGHT-CONTROL key
        BACKSPACE   = 0x08,        // BACKSPACE key
        TAB         = 0x09,        // TAB key
        L_ALT       = 0x0a,        // LEFT-ALT key
        R_ALT       = 0x0b,        // RIGHT-ALT key
        GUI         = 0x0c,        // WINDOWS key 
        RETURN      = 0x0d,        // ENTER key
        SEMICOLON   = 0x0e,        // [;][:] key
        EQUAL       = 0x0f,        // [=][+] key
        SHIFT       = 0x10,        // SHIFT key             (NOTE: Can use LSHIFT and RSHIFT)
        CONTROL     = 0x11,        // CONTROL key           (NOTE: Can use LCONTROL and RCONTROL)
        ALT         = 0x12,        // ALT key               (NOTE: Can use LALT and RALT)
        PAUSE       = 0x13,        // PAUSE key
        CAPITAL     = 0x14,        // CAPS LOCK key
        BACKSLASH   = 0x15,        // [\][|] key        
        QUOTE       = 0x16,        // [']["] key
        SLASH       = 0x17,        // [/][?] key
        TILDE       = 0x18,        // [`][~] key
        L_BRACKET   = 0x19,        // [[][{] key
        R_BRACKET   = 0x1a,        // []][}] key
        ESCAPE      = 0x1b,        // ESCAPE key
        SPACE       = 0x20,        // SPACEBAR key
        PAGEUP      = 0x21,        // PAGE UP key
        PAGEDOWN    = 0x22,        // PAGE DOWN key
        END         = 0x23,        // END key
        HOME        = 0x24,        // HOME key
        LEFT        = 0x25,        // LEFT ARROW key
        UP          = 0x26,        // UP ARROW key
        RIGHT       = 0x27,        // RIGHT key
        DOWN        = 0x28,        // DOWN key
        CONTEXT     = 0x29,        // CONTEXT key (windows contextual menu)
        PRINTSCRN   = 0x2c,        // PRINT SCREEN Bkey
        INSERT      = 0x2d,        // INSERT key   
        DELETE      = 0x2e,        // DELETE key   
        NUM_0       = 0x30,        // 0 key        
        NUM_1       = 0x31,        // 1 key        
        NUM_2       = 0x32,        // 2 key        
        NUM_3       = 0x33,        // 3 key        
        NUM_4       = 0x34,        // 4 key        
        NUM_5       = 0x35,        // 5 key        
        NUM_6       = 0x36,        // 6 key        
        NUM_7       = 0x37,        // 7 key        
        NUM_8       = 0x38,        // 8 key        
        NUM_9       = 0x39,        // 9 key        
        A           = 0x41,        // A key        
        B           = 0x42,        // B key        
        C           = 0x43,        // C key        
        D           = 0x44,        // D key        
        E           = 0x45,        // E key        
        F           = 0x46,        // F key        
        G           = 0x47,        // G key        
        H           = 0x48,        // H key        
        I           = 0x49,        // I key        
        J           = 0x4a,        // J key        
        K           = 0x4b,        // K key        
        L           = 0x4c,        // L key        
        M           = 0x4d,        // M key
        N           = 0x4e,        // N key
        O           = 0x4f,        // O key
        P           = 0x50,        // P key
        Q           = 0x51,        // Q key
        R           = 0x52,        // R key
        S           = 0x53,        // S key
        T           = 0x54,        // T key
        U           = 0x55,        // U key
        V           = 0x56,        // V key
        W           = 0x57,        // W key
        X           = 0x58,        // X key
        Y           = 0x59,        // Y key
        Z           = 0x5a,        // Z key
        L_WIN       = 0x5b,        // LEFT WINDOWS key
        R_WIN       = 0x5c,        // RIGHT WINDOWS key
        COMMA       = 0x5d,        // [,][<] key
        MINUS       = 0x5e,        // [-][_] key
        PERIOD      = 0x5f,        // [.][>] key
        NUMPAD_0    = 0x60,        // NUM PAD 0 key
        NUMPAD_1    = 0x61,        // NUM PAD 1 key
        NUMPAD_2    = 0x62,        // NUM PAD 2 key
        NUMPAD_3    = 0x63,        // NUM PAD 3 key
        NUMPAD_4    = 0x64,        // NUM PAD 4 key
        NUMPAD_5    = 0x65,        // NUM PAD 5 key
        NUMPAD_6    = 0x66,        // NUM PAD 6 key
        NUMPAD_7    = 0x67,        // NUM PAD 7 key
        NUMPAD_8    = 0x68,        // NUM PAD 8 key
        NUMPAD_9    = 0x69,        // NUM PAD 9 key
        NP_MULT     = 0x6a,        // MULTIPLY key
        NP_ADD      = 0x6b,        // ADD key
        NP_ENTER    = 0x7c,        // NUM PAD ENTER key
        NP_SUB      = 0x6d,        // SUBTRACT key
        NP_DECIMAL  = 0x6e,        // DECIMAL key
        NP_DIVIDE   = 0x6f,        // DEVIDE key
        F1          = 0x70,        // F1  key
        F2          = 0x71,        // F2  key
        F3          = 0x72,        // F3  key
        F4          = 0x73,        // F4  key
        F5          = 0x74,        // F5  key
        F6          = 0x75,        // F6  key
        F7          = 0x76,        // F7  key
        F8          = 0x77,        // F8  key
        F9          = 0x78,        // F9  key
        F10         = 0x79,        // F10 key
        F11         = 0x7a,        // F11 key
        F12         = 0x7b,        // F12 key

        //R_WIN        = 0x7f,

        MAX
    } XK;
};



//_XK_KEYCODES_HPP_