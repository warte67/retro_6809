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
    NewDevice() { _device_name = "NewDevice"; }
    virtual ~NewDevice() {}

public: // VIRTUAL METHODS
    virtual Byte OnRead(Word offset);
    virtual void OnWrite(Word offset, Byte data);

    virtual int  OnAttach(int nextAddr) = 0;   
    virtual bool OnInit() = 0;
    virtual bool OnQuit() = 0;
    virtual bool OnActivate() = 0;
    virtual bool OnDeactivate() = 0;
    virtual bool OnEvent(SDL_Event* evnt) = 0;
    virtual bool OnUpdate(float fElapsedTime) = 0;
    virtual bool OnRender() = 0;

public: // PUBLIC ACCESSORS
    // ... 

private: // PRIVATE MEMBERS
    // ...

};

// END: NewDevice.hpp