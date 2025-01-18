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

/**********************

CategoryJoystick: https://wiki.libsdl.org/SDL3/CategoryJoystick

CategoryGamepad: https://wiki.libsdl.org/SDL3/CategoryGamepad


To Do:

    Add a condition flag byte for each of the two controllers based
    on whether they are a gamepad or a joystick and other related
    SDL3 based information.

    Flags:
        0000'0000: Not Connected
        0000'1111: Controller Type
        0001'0000: (reserved)
        0010'0000: (reserved)
        0100'0000: Is a Gamepad
        1000'0000: Is a Joystick

SDL_GamepadType SDL_GetRealGamepadType(SDL_Gamepad *gamepad);
typedef enum SDL_GamepadType
{
    SDL_GAMEPAD_TYPE_UNKNOWN = 0,                   // 0
    SDL_GAMEPAD_TYPE_STANDARD,                      // 1
    SDL_GAMEPAD_TYPE_XBOX360,                       // 2
    SDL_GAMEPAD_TYPE_XBOXONE,                       // 2
    SDL_GAMEPAD_TYPE_PS3,                           // 3
    SDL_GAMEPAD_TYPE_PS4,                           // 4
    SDL_GAMEPAD_TYPE_PS5,                           // 5
    SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO,           // 6
    SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT,   // 7
    SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT,  // 8
    SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR,   // 9       
    SDL_GAMEPAD_TYPE_COUNT
} SDL_GamepadType;



SDL_JoystickType SDL_GetJoystickType(SDL_Joystick *joystick);
typedef enum SDL_JoystickType
{
    SDL_JOYSTICK_TYPE_UNKNOWN,          // 0
    SDL_JOYSTICK_TYPE_GAMEPAD,          // 1
    SDL_JOYSTICK_TYPE_WHEEL,            // 2
    SDL_JOYSTICK_TYPE_ARCADE_STICK,     // 3
    SDL_JOYSTICK_TYPE_FLIGHT_STICK,     // 4
    SDL_JOYSTICK_TYPE_DANCE_PAD,        // 5
    SDL_JOYSTICK_TYPE_GUITAR,           // 6
    SDL_JOYSTICK_TYPE_DRUM_KIT,         // 7
    SDL_JOYSTICK_TYPE_ARCADE_PAD,       // 8
    SDL_JOYSTICK_TYPE_THROTTLE,         // 9
    SDL_JOYSTICK_TYPE_COUNT             // 10
} SDL_JoystickType;




const char * SDL_GetGamepadName(SDL_Gamepad *gamepad);
const char * SDL_GetJoystickName(SDL_Joystick *joystick);


 ********************/



// END: Joystick.hpp