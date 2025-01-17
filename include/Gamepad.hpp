/*** Gamepad.hpp *******************************************
 *       _____                                      _       _                 
 *      / ____|                                    | |     | |                
 *     | |  __  __ _ _ __ ___   ___ _ __   __ _  __| |     | |__  _ __  _ __  
 *     | | |_ |/ _` | '_ ` _ \ / _ \ '_ \ / _` |/ _` |     | '_ \| '_ \| '_ \ 
 *     | |__| | (_| | | | | | |  __/ |_) | (_| | (_| |  _  | | | | |_) | |_) |
 *      \_____|\__,_|_| |_| |_|\___| .__/ \__,_|\__,_| (_) |_| |_| .__/| .__/ 
 *                                 | |                           | |   | |    
 *                                 |_|                           |_|   |_|
 * 
 *      Supports Gamepad Controller input as well as most Joystick Controllers.
 * 
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#pragma once

#include "IDevice.hpp"

class Gamepad : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    Gamepad();
    virtual ~Gamepad();

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
    // ... 

private: // PRIVATE MEMBERS
    // ...

};

// END: Gamepad.hpp