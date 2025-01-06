/*** Tests.hpp ******************************************
 *      _____       _            _              
 *     |_   _|__ __| |_ ___     | |_  _ __ _ __ 
 *       | |/ -_|_-<  _(_-<  _  | ' \| '_ \ '_ \
 *       |_|\___/__/\__/__/ (_) |_||_| .__/ .__/
 *                                   |_|  |_|   
 *  
 *********************************/

#pragma once

#include "IDevice.hpp"

class Tests : public IDevice 
{
public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    Tests();
    virtual ~Tests();

public: // VIRTUAL METHODS
    virtual Byte OnRead(Word offset);               // read
    virtual void OnWrite(Word offset, Byte data);   // write
    // =============================================
    virtual int  OnAttach(int nextAddr);            // attach to the memory map
    virtual void OnInit();                          // initialize
    virtual void OnQuit();                          // shutdown
    virtual void OnActivate();                      // activate
    virtual void OnDeactivate();                    // deactivate
    virtual void OnEvent(SDL_Event* evnt);          // handle events
    virtual void OnUpdate(float fElapsedTime);      // update
    virtual void OnRender();                        // render

public: // PUBLIC ACCESSORS
    enum TestFlags {
        TEST_NONE           = 0b0000'0000,
        TEST_ENABLE         = 0b1000'0000,
        TEST_ANIM_BG        = 0b0100'0000,
        TEST_FLAG_5         = 0b0010'0000,
        TEST_FLAG_4         = 0b0001'0000,
        TEST_FLAG_3         = 0b0000'1000,
        TEST_FLAG_2         = 0b0000'0100,
        TEST_ANIM_FG        = 0b0000'0010,
        TEST_INC_VID_MODES  = 0b0000'0001,
    };

private: // PRIVATE MEMBERS

    // Byte _tests_one = TEST_NONE;
    // Byte _tests_one = TEST_ENABLE;
    Byte _tests_one = TEST_ENABLE | TEST_ANIM_FG;
    // Byte _tests_one = TEST_ENABLE | TEST_ANIM_FG | TEST_ANIM_BG;
    // Byte _tests_one = TEST_ENABLE | TEST_INC_VID_MODES;
    // Byte _tests_one = 0;
    Byte _tests_two = 0b0000'0000;

};

// END: NewDevice.hpp