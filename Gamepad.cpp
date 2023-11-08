/**** Gamepad.cpp ***************************************
 *
 * The Gamepad / Joystick device interface.
 * 
 *		Notes:
 *			Tested specifically with Logitech F310 gamepad controllers
 *			and Extreme 3D Pro joysticks. Other controller devices will
 *			likely work, but have not yet been tested.
 *
 * Copyright (C) 2023 by Jay Faries
 ************************************/

#include "Bus.h"
#include "Gamepad.h"

Byte Gamepad::read(Word offset, bool debug)
{
	Byte data = Bus::Read(offset, true);
    // write statically and return
    Bus::Write(offset, data, true);
    return data;
}

void Gamepad::write(Word offset, Byte data, bool debug)
{
	// simply write statically
    Bus::Write(offset, data, true);
}

Word Gamepad::OnAttach(Word nextAddr)
{
    int size = 0;
    Word old_addr = nextAddr;

    DisplayEnum("", 0, "");
    DisplayEnum("JOYS_BEGIN",   nextAddr, "Start of the Game Controller Register space");
    nextAddr += 0;

    DisplayEnum("JOYS_1_BTN",   nextAddr, "  (Word) button bits: room for up to 16 buttons  (realtime)");
    nextAddr += 2;
    DisplayEnum("JOYS_1_DBND",  nextAddr, "  (Byte) PAD 1 analog deadband; default is 5   (read/write)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_LTX",   nextAddr, "  (char) PAD 1 LThumb-X position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_LTY",   nextAddr, "  (char) PAD 1 LThumb-Y position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_RTX",   nextAddr, "  (char) PAD 1 RThumb-X position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_RTY",   nextAddr, "  (char) PAD 1 RThumb-Y position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_Z1",    nextAddr, "  (char) PAD 1 left analog trigger (0 - 127)     (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_Z2",    nextAddr, "  (char) PAD 1 right analog trigger (0 - 127)    (realtime)");
    nextAddr += 1;

    DisplayEnum("", 0, "");

    DisplayEnum("JOYS_2_BTN",   nextAddr, "  (Word) button bits: room for up to 16 buttons  (realtime)");
    nextAddr += 2;                           
    DisplayEnum("JOYS_2_DBND",  nextAddr, "  (Byte) PAD 2 analog deadband; default is 5   (read/write)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_LTX",   nextAddr, "  (char) PAD 2 LThumb-X position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_LTY",   nextAddr, "  (char) PAD 2 LThumb-Y position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_RTX",   nextAddr, "  (char) PAD 2 RThumb-X position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_RTY",   nextAddr, "  (char) PAD 2 RThumb-Y position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_Z1",    nextAddr, "  (char) PAD 2 left analog trigger (0 - 127)     (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_Z2",    nextAddr, "  (char) PAD 2 right analog trigger (0 - 127)    (realtime)");
    nextAddr += 1;

    DisplayEnum("JOYS_END", nextAddr, "End of the Game Controller Register space");
    nextAddr += 0;

    return nextAddr - old_addr;
}

void Gamepad::OnInit()
{
    // printf("Gamepad::OnInit();\n");
	InitButtonStates();
	OnActivate();
}

void Gamepad::OnQuit()
{
    // printf("Gamepad::OnQuit();\n");
	OnDeactivate();
}

void Gamepad::OnActivate()
{
    // printf("Gamepad::OnActivate();\n");
	if (!bGamepadWasInit)
	{
		int ret = SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
		if (ret < 0)
		{
			std::string msg = "SDL could not initialize gamepads and/or joysticks! SDL_Error: ";
			msg += SDL_GetError();
			Bus::Error(msg.c_str());
			return;
		}
		// set the default analog dead band (dead zone)
		Bus::Write(JOYS_1_DBND, 5, true);	// bus->debug_write(JOYS_1_DBND, 5);
		Bus::Write(JOYS_2_DBND, 5, true);	// bus->debug_write(JOYS_2_DBND, 5);
		OpenControllers();
		bGamepadWasInit = true;
	}
}

void Gamepad::OnDeactivate()
{
    // printf("Gamepad::OnDeactivate();\n");
	if (bGamepadWasInit)
	{
		CloseControllers();
		SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
		bGamepadWasInit = false;
	}
}

void Gamepad::OnEvent(SDL_Event* evnt)
{
    switch (evnt->type)
    {
    case SDL_CONTROLLERDEVICEADDED:
    case SDL_CONTROLLERDEVICEREMOVED:
    case SDL_CONTROLLERDEVICEREMAPPED:
    case SDL_JOYDEVICEADDED:
    case SDL_JOYDEVICEREMOVED:
        CloseControllers();
        OpenControllers();
        break;
    }
}

void Gamepad::OnUpdate(float fElapsedTime)
{
	// update button registers
	if (state[0].bIsActive)
		Bus::Write_Word(JOYS_1_BTN, EncodeButtonRegister(0), true);
	else
		Bus::Write_Word(JOYS_1_BTN, 0xffff, true);
	if (state[1].bIsActive)
		Bus::Write_Word(JOYS_2_BTN, EncodeButtonRegister(1), true);
	else
		Bus::Write_Word(JOYS_2_BTN, 0xffff, true);
	EncodeAxesRegister(0);
	EncodeAxesRegister(1);
}



// ******************************************************************

void Gamepad::InitButtonStates()
{
	Byte btn = 0;
	gpadBtnMap.clear();
	joysBtnMap.clear();

	gpadBtnMap[SDL_CONTROLLER_BUTTON_A] = btn++;			// GPAD_BTN::BTN_A
	gpadBtnMap[SDL_CONTROLLER_BUTTON_B] = btn++;			// GPAD_BTN::BTN_B
	gpadBtnMap[SDL_CONTROLLER_BUTTON_X] = btn++;			// GPAD_BTN::BTN_X
	gpadBtnMap[SDL_CONTROLLER_BUTTON_Y] = btn++;			// GPAD_BTN::BTN_Y
	gpadBtnMap[SDL_CONTROLLER_BUTTON_BACK] = btn++;			// GPAD_BTN::BTN_BACK
	gpadBtnMap[SDL_CONTROLLER_BUTTON_START] = btn++;		// GPAD_BTN::BTN_START
	gpadBtnMap[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = btn++;	// GPAD_BTN::BTN_LS
	gpadBtnMap[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = btn++;	// GPAD_BTN::BTN_RS
	gpadBtnMap[SDL_CONTROLLER_BUTTON_DPAD_UP] = btn++;		// GPAD_BTN::BTN_DPAD_UP
	gpadBtnMap[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = btn++;	// GPAD_BTN::BTN_DPAD_DOWN
	gpadBtnMap[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = btn++;	// GPAD_BTN::BTN_DPAD_LEFT
	gpadBtnMap[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = btn++;	// GPAD_BTN::BTN_DPAD_RIGHT
	gpadBtnMap[SDL_CONTROLLER_BUTTON_GUIDE] = btn++;		// GPAD_BTN::BTN_GUIDE

	btn = 0;
	joysBtnMap[JOYS_BTN::BTN_1] = btn++;
	joysBtnMap[JOYS_BTN::BTN_2] = btn++;
	joysBtnMap[JOYS_BTN::BTN_3] = btn++;
	joysBtnMap[JOYS_BTN::BTN_4] = btn++;
	joysBtnMap[JOYS_BTN::BTN_5] = btn++;
	joysBtnMap[JOYS_BTN::BTN_6] = btn++;
	joysBtnMap[JOYS_BTN::BTN_7] = btn++;
	joysBtnMap[JOYS_BTN::BTN_8] = btn++;
	joysBtnMap[JOYS_BTN::BTN_HAT_UP] = btn++;
	joysBtnMap[JOYS_BTN::BTN_HAT_DOWN] = btn++;
	joysBtnMap[JOYS_BTN::BTN_HAT_LEFT] = btn++;
	joysBtnMap[JOYS_BTN::BTN_HAT_RIGHT] = btn++;
	joysBtnMap[JOYS_BTN::BTN_9] = btn++;
	joysBtnMap[JOYS_BTN::BTN_10] = btn++;
	joysBtnMap[JOYS_BTN::BTN_11] = btn++;
	joysBtnMap[JOYS_BTN::BTN_12] = btn++;
}


void Gamepad::OpenControllers()
{
	int num_joysticks = SDL_NumJoysticks();
	for (int index = 0; index < num_joysticks; index++)
	{
		if (SDL_IsGameController(index)) // is a gamepad
		{
			state[index].bIsGamepad = true;
			state[index].name = SDL_GameControllerNameForIndex(index);
			state[index].controller = SDL_GameControllerOpen(index);
			if (state[index].controller == nullptr)
			{
				std::string msg = "SDL could not open a gamepad! SDL_Error: ";
				msg += SDL_GetError();
				Bus::Error(msg.c_str());
				return;
			}
			state[index].bIsActive = true;
		}
		else // is a joystick
		{
			state[index].bIsGamepad = false;
			state[index].joystick = SDL_JoystickOpen(index);
			if (state[index].joystick == nullptr)
			{
				std::string msg = "SDL could not open a joystick! SDL_Error: ";
				msg += SDL_GetError();
				Bus::Error(msg.c_str());
				return;
			}
			state[index].name = SDL_JoystickName(state[index].joystick);
			state[index].bIsActive = true;
		}
	}
}

void Gamepad::CloseControllers()
{
	state[0].bIsActive = false;
	Bus::Write_Word(JOYS_1_BTN, 0xffff, true);	// bus->debug_write_word(JOYS_1_BTN, 0xffff);
	state[1].bIsActive = false;
	Bus::Write_Word(JOYS_2_BTN, 0xffff, true);	// bus->debug_write_word(JOYS_2_BTN, 0xffff);
}


Word Gamepad::EncodeButtonRegister(int id)
{
	id %= 2;
	Word ret = 0xffff;
	if (state[id].bIsActive)
	{
		ret = 0;
		// encode only the first 16 buttons
		for (int btn = 0; btn < 16; btn++)
		{
			// gamepad?
			if (state[id].bIsGamepad)
			{
				if (SDL_GameControllerGetButton(state[id].controller, (SDL_GameControllerButton)btn))
					ret |= (1 << gpadBtnMap[btn]);
			}
			else // joystick
			{
				if (SDL_JoystickGetButton(state[id].joystick, btn))
					ret |= (1 << joysBtnMap[btn]);
				// encode hat
				Byte hat = SDL_JoystickGetHat(state[id].joystick, 0);
				if (hat == SDL_HAT_UP || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_LEFTUP)
					ret |= 0x0100;
				if (hat == SDL_HAT_DOWN || hat == SDL_HAT_RIGHTDOWN || hat == SDL_HAT_LEFTDOWN)
					ret |= 0x0200;
				if (hat == SDL_HAT_LEFT || hat == SDL_HAT_LEFTUP || hat == SDL_HAT_LEFTDOWN)
					ret |= 0x0400;
				if (hat == SDL_HAT_RIGHT || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_RIGHTDOWN)
					ret |= 0x0800;
			}
		}
	}
	return ret;
}


void Gamepad::EncodeAxesRegister(int id)
{
	id %= 2;
	if (state[id].bIsActive)
	{
		Sint8 deadband = read(JOYS_1_DBND);
		if (id == 1)
			Sint8 deadband = read(JOYS_2_DBND);

		if (state[id].bIsGamepad)	// is gamepad
		{
			Sint8 LTX = (SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_LEFTX) & 0xff00) >> 8;
			if (LTX < 0 && LTX > -deadband)	LTX = 0;
			if (LTX > 0 && LTX < deadband)	LTX = 0;
			Sint8 LTY = (SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_LEFTY) & 0xff00) >> 8;
			if (LTY < 0 && LTY > -deadband)	LTY = 0;
			if (LTY > 0 && LTY < deadband)	LTY = 0;
			Sint8 RTX = (SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_RIGHTX) & 0xff00) >> 8;
			if (RTX < 0 && RTX > -deadband)	RTX = 0;
			if (RTX > 0 && RTX < deadband)	RTX = 0;
			Sint8 RTY = (SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_RIGHTY) & 0xff00) >> 8;
			if (RTY < 0 && RTY > -deadband)	RTY = 0;
			if (RTY > 0 && RTY < deadband)	RTY = 0;
			Sint8 Z1 = SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) >> 8;
			Sint8 Z2 = SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) >> 8;

			if (id == 0)
			{
				Bus::Write(JOYS_1_LTX, LTX, true);	// bus->debug_write(JOYS_1_LTX, LTX);
				Bus::Write(JOYS_1_LTY, LTY, true);	// bus->debug_write(JOYS_1_LTY, LTY);
				Bus::Write(JOYS_1_RTX, RTX, true);	// bus->debug_write(JOYS_1_RTX, RTX);
				Bus::Write(JOYS_1_RTY, RTY, true);	// bus->debug_write(JOYS_1_RTY, RTY);
				Bus::Write(JOYS_1_Z1,   Z1, true);	// bus->debug_write(JOYS_1_Z1, Z1);
				Bus::Write(JOYS_1_Z2,   Z2, true);	// bus->debug_write(JOYS_1_Z2, Z2);
			}
			else
			{
				Bus::Write(JOYS_2_LTX, LTX, true);	// bus->debug_write(JOYS_2_LTX, LTX);
				Bus::Write(JOYS_2_LTY, LTY, true);	// bus->debug_write(JOYS_2_LTY, LTY);
				Bus::Write(JOYS_2_RTX, RTX, true);	// bus->debug_write(JOYS_2_RTX, RTX);
				Bus::Write(JOYS_2_RTY, RTY, true);	// bus->debug_write(JOYS_2_RTY, RTY);
				Bus::Write(JOYS_2_Z1,   Z1, true);	// bus->debug_write(JOYS_2_Z1, Z1);
				Bus::Write(JOYS_2_Z2,   Z2, true);	// bus->debug_write(JOYS_2_Z2, Z2);
			}
		}
		else  // is joystick
		{
			Sint8 LTX = (SDL_JoystickGetAxis(state[id].joystick, 0) & 0xff00) >> 8;
			if (LTX < 0 && LTX > -deadband)	LTX = 0;
			if (LTX > 0 && LTX < deadband)	LTX = 0;
			Sint8 LTY = (SDL_JoystickGetAxis(state[id].joystick, 1) & 0xff00) >> 8;
			if (LTY < 0 && LTY > -deadband)	LTY = 0;
			if (LTY > 0 && LTY < deadband)	LTY = 0;
			// RTX maps the hat
			Byte hat = SDL_JoystickGetHat(state[id].joystick, 0);
			Sint8 RTX = 0;
			Sint8 RTY = 0;
			if (hat == SDL_HAT_UP || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_LEFTUP)
				RTY = -128;
			if (hat == SDL_HAT_DOWN || hat == SDL_HAT_RIGHTDOWN || hat == SDL_HAT_LEFTDOWN)
				RTY = 127;
			if (hat == SDL_HAT_LEFT || hat == SDL_HAT_LEFTUP || hat == SDL_HAT_LEFTDOWN)
				RTX = -128;
			if (hat == SDL_HAT_RIGHT || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_RIGHTDOWN)
				RTX = 127;
			// throttle lever
			Uint8 Z1 = SDL_JoystickGetAxis(state[id].joystick, 3) >> 8;
			Z1 = 255 - (Z1 + 128);

			// rudder axis
			Sint8 Z2 = SDL_JoystickGetAxis(state[id].joystick, 2) >> 8;
			if (Z2 < 0 && Z2 > -deadband * 3)	Z2 = 0;
			if (Z2 > 0 && Z2 < deadband * 3)	Z2 = 0;

			if (id == 0)
			{
				Bus::Write(JOYS_1_LTX, LTX, true);	// bus->debug_write(JOYS_1_LTX, LTX);
				Bus::Write(JOYS_1_LTY, LTY, true);	// bus->debug_write(JOYS_1_LTY, LTY);
				Bus::Write(JOYS_1_RTX, RTX, true);	// bus->debug_write(JOYS_1_RTX, RTX);
				Bus::Write(JOYS_1_RTY, RTY, true);	// bus->debug_write(JOYS_1_RTY, RTY);
				Bus::Write(JOYS_1_Z1,   Z1, true);	// bus->debug_write(JOYS_1_Z1, Z1);
				Bus::Write(JOYS_1_Z2,   Z2, true);	// bus->debug_write(JOYS_1_Z2, Z2);
			}
			else
			{
				Bus::Write(JOYS_2_LTX, LTX, true);	// bus->debug_write(JOYS_2_LTX, LTX);
				Bus::Write(JOYS_2_LTY, LTY, true);	// bus->debug_write(JOYS_2_LTY, LTY);
				Bus::Write(JOYS_2_RTX, RTX, true);	// bus->debug_write(JOYS_2_RTX, RTX);
				Bus::Write(JOYS_2_RTY, RTY, true);	// bus->debug_write(JOYS_2_RTY, RTY);
				Bus::Write(JOYS_2_Z1, Z1, true);	// bus->debug_write(JOYS_2_Z1, Z1);
				Bus::Write(JOYS_2_Z2, Z2, true);	// bus->debug_write(JOYS_2_Z2, Z2);
			}
		}
	}
}


