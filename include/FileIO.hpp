/*** FileIO.hpp *******************************************
 *      ______ _ _      _____ ____        _                 
 *     |  ____(_) |    |_   _/ __ \      | |                
 *     | |__   _| | ___  | || |  | |     | |__  _ __  _ __  
 *     |  __| | | |/ _ \ | || |  | |     | '_ \| '_ \| '_ \ 
 *     | |    | | |  __/_| || |__| |  _  | | | | |_) | |_) |
 *     |_|    |_|_|\___|_____\____/  (_) |_| |_| .__/| .__/ 
 *                                             | |   | |    
 *                                             |_|   |_|   
 *  
 *   A filesystem that acts to emulate a Raspberry PI Pico
 *		Implementing File Input / Output with the onboad 
 *      flash ram, USB storage and the SDCARD.
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#pragma once

#include "IDevice.hpp"

class FileIO : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    FileIO();
    virtual ~FileIO();

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

// END: FileIO.hpp