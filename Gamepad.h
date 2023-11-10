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
    Gamepad() { _deviceName = "Gamepad"; }
    virtual ~Gamepad() {}

    // virtuals
    virtual Word OnAttach(Word nextAddr) override;
    virtual void OnInit() override;
    virtual void OnQuit() override;
    virtual void OnActivate() override;
    virtual void OnDeactivate() override;
    virtual void OnEvent(SDL_Event* evnt) override;
    virtual void OnUpdate(float fElapsedTime) override;

    virtual Byte read(Word offset, bool debug = false);
    virtual void write(Word offset, Byte data, bool debug = false);

protected:
    void OpenControllers();
    void CloseControllers();
    void InitButtonStates();
    Word EncodeButtonRegister(int id);
    void EncodeAxesRegister(int id);

	struct STATE {
		SDL_GameController* controller = nullptr;
		SDL_Joystick* joystick = nullptr;
		bool bIsActive = false;
		bool bIsGamepad = false;
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


private:

	// Bus* bus = nullptr;
	// Memory* memory = nullptr;

	bool bGamepadWasInit = false;
	STATE state[2];
	std::map<Word, Byte> joysBtnMap;
	std::map<Word, Byte> gpadBtnMap;
};




#endif // __GAMEPAD_H__