/*** Gfx.hpp *******************************************
 *       _____    __              _                     
 *      / ____|  / _|            | |                    
 *     | |  __  | |_  __  __     | |__    _ __    _ __  
 *     | | |_ | |  _| \ \/ /     | '_ \  | '_ \  | '_ \ 
 *     | |__| | | |    >  <   _  | | | | | |_) | | |_) |
 *      \_____| |_|   /_/\_\ (_) |_| |_| | .__/  | .__/ 
 *                                       | |     | |    
 *                                       |_|     |_|    
 *
 * This is the base class for all graphics devices. It provides the
 * basic methods for reading and writing to the device's memory.
 * 
 ************************************/

#pragma once

#include "IDevice.hpp"

class Gfx : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    Gfx();
    virtual ~Gfx();

public: // VIRTUAL METHODS
    virtual Byte OnRead(Word offset);               // read
    virtual void OnWrite(Word offset, Byte data);   // write
    // =============================================
    virtual int  OnAttach(int nextAddr);            // attach to the memory map
    virtual bool OnInit();                          // initialize
    virtual bool OnQuit();                          // shutdown
    virtual bool OnActivate();                      // activate
    virtual bool OnDeactivate();                    // deactivate
    virtual bool OnEvent(SDL_Event* evnt);          // handle events
    virtual bool OnUpdate(float fElapsedTime);      // update
    virtual bool OnRender();                        // render

public: // PUBLIC ACCESSORS
    // ... 

private: // PRIVATE MEMBERS
    // ...

};

// END: Gfx.hpp