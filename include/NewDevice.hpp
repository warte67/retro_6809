/*** NewDevice.hpp *******************************************
 *      _   _                     _____                   _                     _                     
 *     | \ | |                   |  __ \                 (_)                   | |                    
 *     |  \| |   ___  __      __ | |  | |   ___  __   __  _    ___    ___      | |__    _ __    _ __  
 *     | . ` |  / _ \ \ \ /\ / / | |  | |  / _ \ \ \ / / | |  / __|  / _ \     | '_ \  | '_ \  | '_ \ 
 *     | |\  | |  __/  \ V  V /  | |__| | |  __/  \ V /  | | | (__  |  __/  _  | | | | | |_) | | |_) |
 *     |_| \_|  \___|   \_/\_/   |_____/   \___|   \_/   |_|  \___|  \___| (_) |_| |_| | .__/  | .__/ 
 *                                                                                     | |     | |    
 *                                                                                     |_|     |_|    
 * This file serves meerly as a template as a
 * starting point for new devices.
 * 
 ************************************/

#pragma once

#include "IDevice.hpp"

class NewDevice : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    NewDevice();
    virtual ~NewDevice();

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

// END: NewDevice.hpp