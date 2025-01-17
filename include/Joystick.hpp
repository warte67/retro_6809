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
    virtual void OnActivate() override;                      // activate
    virtual void OnDeactivate() override;                    // deactivate
    virtual void OnEvent(SDL_Event* evnt) override;          // handle events
    virtual void OnUpdate(float fElapsedTime) override;      // update

    // not used
    void OnRender() override {};    

 
public: // PUBLIC ACCESSORS
    // ... 

private: // PRIVATE MEMBERS

    void OpenControllers();
    void CloseControllers();
    void InitButtonStates();
    Word EncodeButtonRegister(int id);
    void EncodeAxesRegister(int id);

    struct STATE {
        SDL_Gamepad* controller = nullptr;
        SDL_Joystick* joystick = nullptr;
        bool bIsActive = false;
        bool bIsJoystick = false;
        std::string name = "";
    };

    enum GPAD_BTN {
        BTN_A,
        BTN_B,
        BTN_X,
        BTN_Y,
        BTN_BACK,
        BTN_START,
        BTN_LS,
        BTN_RS,
        BTN_DPAD_UP,
        BTN_DPAD_DOWN,
        BTN_DPAD_LEFT,
        BTN_DPAD_RIGHT,
        BTN_GUIDE,
    };

    enum JOYS_BTN {
        BTN_1,
        BTN_2,
        BTN_3,
        BTN_4,
        BTN_5,
        BTN_6,
        BTN_7,
        BTN_8,
        BTN_9,
        BTN_10,
        BTN_11,
        BTN_12,
        BTN_HAT_UP,
        BTN_HAT_DOWN,
        BTN_HAT_LEFT,
        BTN_HAT_RIGHT,
    };

    bool bJoystickWasInit = false;
    STATE state[2];
    std::unordered_map<Word, Byte> joysBtnMap;
    std::unordered_map<Word, Byte> gpadBtnMap; 

};

/**********************

CategoryJoystick: https://wiki.libsdl.org/SDL3/CategoryJoystick

CategoryGamepad: https://wiki.libsdl.org/SDL3/CategoryGamepad





 ********************/



// END: Joystick.hpp