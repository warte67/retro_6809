/**** Gamepad.h ***************************************
 *
 * The Gamepad / Joystick device interface.
 *
 * Copyright (C) 2023 by Jay Faries
 ************************************/

#pragma once
#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__


#include "Device.h"



class Gamepad : public Device
{
public:
    Gamepad() 
    { 
        _deviceName = "Gamepad"; 
        printf("Gamepad::Gamepad();\n");
    }
    virtual ~Gamepad() 
    {
        printf("Gamepad::~Gamepad();\n");
    }

    // virtuals
    virtual Word OnAttach(Word nextAddr) override;
    virtual void OnInit() override;
    virtual void OnQuit() override;
    virtual void OnActivate() override;
    virtual void OnDeactivate() override;
    virtual void OnEvent(SDL_Event* evnt) override;
    virtual void OnUpdate(float fElapsedTime) override;
    // virtual void OnRender() override;

    virtual Byte read(Word offset, bool debug = false);
    virtual void write(Word offset, Byte data, bool debug = false);
    // virtual Word read_word(Word offset, bool debug = false);
    // virtual void write_word(Word offset, Word data, bool debug = false);

    // helpers
    // virtual void DisplayEnum(std::string sToken, Word ofs, std::string sComment);

};




#endif // __GAMEPAD_H__