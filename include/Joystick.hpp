/*** Joystick.hpp *******************************************
 *           _                 _   _      _          _                 
 *          | |               | | (_)    | |        | |                
 *          | | ___  _   _ ___| |_ _  ___| | __     | |__  _ __  _ __  
 *      _   | |/ _ \| | | / __| __| |/ __| |/ /     | '_ \| '_ \| '_ \ 
 *     | |__| | (_) | |_| \__ \ |_| | (__|   <   _  | | | | |_) | |_) |
 *      \____/ \___/ \__, |___/\__|_|\___|_|\_\ (_) |_| |_| .__/| .__/ 
 *                    __/ |                               | |   | |    
 *                   |___/                                |_|   |_|  
 * 
 *      Supports Joystick Controller input as well as most Gamepad Controllers.
 * 
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/
#pragma once

#include <unordered_map>

#include "IDevice.hpp"

class Joystick : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    Joystick();
    virtual ~Joystick();

public: // VIRTUAL METHODS

    virtual int  OnAttach(int nextAddr) override;            // attach to the memory map
    virtual void OnInit() override;                          // initialize
    virtual void OnQuit() override;                          // shutdown
    virtual void OnEvent(SDL_Event* evnt) override;          // handle events
    virtual void OnUpdate(float fElapsedTime) override;      // update

    // not used
    void OnActivate() override {};
    void OnDeactivate() override {};
    void OnRender() override {};    
 
public: // PUBLIC ACCESSORS
    // ... 

private: // PRIVATE MEMBERS

    void OpenControllers();
    void CloseControllers();
    void InitButtonStates();
    Word EncodeButtonRegister(int id);
    void EncodeAxesRegister(int id);
    void EncodeConditionFlags(int id);

    struct STATE {
        SDL_Gamepad* gamepad = nullptr;
        SDL_Joystick* joystick = nullptr;
        bool bIsActive = false;
        bool bIsJoystick = false;
        // int  num_buttons = 0;
        std::string name = "";
    };

    // Gamepad Buttons Bits (16-max allowed)
    enum GPAD_BTN {
        BTN_A,          // 0
        BTN_B,          // 1
        BTN_X,          // 2
        BTN_Y,          // 3
        BTN_LS,         // 4
        BTN_RS,         // 5
        BTN_BACK,       // 6
        BTN_START,      // 7
        BTN_MISC1,      // 8
        BTN_MISC2,      // 9
        BTN_MISC3,      // A
        BTN_GUIDE,      // B
        BTN_DPAD_UP,    // C
        BTN_DPAD_DOWN,  // D
        BTN_DPAD_LEFT,  // E
        BTN_DPAD_RIGHT, // F
    };

    // Joystick Buttons Bits (16-max allowed)
    enum JOYS_BTN {
        BTN_1,          // 0    
        BTN_2,          // 1
        BTN_3,          // 2
        BTN_4,          // 3
        BTN_5,          // 4
        BTN_6,          // 5
        BTN_7,          // 6
        BTN_8,          // 7
        BTN_9,          // 8
        BTN_10,         // 9
        BTN_11,         // A
        BTN_12,         // B
        BTN_HAT_UP,     // C    
        BTN_HAT_DOWN,   // D        
        BTN_HAT_LEFT,   // E    
        BTN_HAT_RIGHT,  // F    
    };

    bool bJoystickWasInit = false;
    STATE state[2];
    std::unordered_map<Word, Byte> joysBtnMap;
    std::unordered_map<Word, Byte> gpadBtnMap; 

};


// END: Joystick.hpp