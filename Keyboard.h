/**** Keyboard.h ***************************************
 *
 *   Acts to emulate a Raspberry PI Pico #2
 *		Dispatches Keyboard Events to PICO #1
 *		Emulates the Keyboard Hardware Registers
 *
 *  Copyright (C) 2023 by Jay Faries
 ************************************/

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#pragma once	// redundant header guard

#include <map>
#include <queue>
#include <array>

#include "Device.h"
#include "XK_Keycodes.h"

class Keyboard : public Device
{
public:
	Keyboard()			{ _deviceName = "Keyboard"; }
	virtual ~Keyboard() {}

	// virtuals
	virtual Word OnAttach(Word nextAddr) override;
	virtual void OnInit() override;
	virtual void OnQuit() override;
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
	virtual void OnEvent(SDL_Event* evnt) override;
	//virtual void OnUpdate(float fElapsedTime) override;
	//virtual void OnRender() override;

	virtual Byte read(Word offset, bool debug = false) override;
	virtual void write(Word offset, Byte data, bool debug = false) override;

	int charQueueLen();
	Byte charScanQueue();
	Byte charPopQueue();
	XKey::XK TranslateSDLtoXKey(SDL_Scancode k);
	XKey::XK AscToXKey(Byte asc);
	char XKeyToAscii(XKey::XK xKey);
	bool IsKeyDown(Byte xk);


protected:

	std::map<XKey::XK, int> keyMap;
	std::vector<std::tuple<XKey::XK, char, char>> xkToAsciiMap;
	std::queue <Byte> charQueue;		// ascii character queue
	std::array <Byte, 256> editBuffer;	// internal line edit character buffer
	Byte edt_bfr_csr = 0;				// internal edit character buffer cursor position

protected:
	bool _bWasInit = false;
};


#endif	// __KEYBOARD_H__

