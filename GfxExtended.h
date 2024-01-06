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
	virtual Byte read(Word offset, bool debug = false) override;
	virtual void write(Word offset, Byte data, bool debug = false) override;
	virtual Word OnAttach(Word nextAddr) override;
	virtual void OnInit() override;
	virtual void OnQuit() override;
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
	virtual void OnEvent(SDL_Event* evnt) override;
	virtual void OnUpdate(float fElapsedTime) override;
	virtual void OnRender() override;

private:

};

#endif // __GFXEXTENDED_H__