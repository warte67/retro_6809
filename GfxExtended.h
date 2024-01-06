// GfxExtended.h
//

#ifndef __GFXEXTENDED_H__
#define __GFXEXTENDED_H__
#pragma once

#include "IGfxDevice.h"
#include "types.h"

class Gfx;

class GfxExtended : public IGfxDevice
{
	friend Gfx;

public:
	GfxExtended(Gfx* _gfx) : IGfxDevice(_gfx) { _deviceName = "GfxExtended"; }
	virtual ~GfxExtended() {}

	// virtuals
	virtual Byte read(Word offset, bool debug = false);
	virtual void write(Word offset, Byte data, bool debug = false);
	virtual Word OnAttach(Word nextAddr);
	virtual void OnInit();
	virtual void OnQuit();
	virtual void OnActivate();
	virtual void OnDeactivate();
	virtual void OnEvent(SDL_Event* evnt);
	virtual void OnUpdate(float fElapsedTime);
	virtual void OnRender();

private:

};

#endif // __GFXEXTENDED_H__