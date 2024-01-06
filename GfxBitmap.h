// GfxBitmap.h
//

#ifndef __GFXBITMAP_H__
#define __GFXBITMAP_H__
#pragma once

#include "IGfxDevice.h"
#include "types.h"

class Gfx;

class GfxBitmap : public IGfxDevice
{
	friend Gfx;

public:
	GfxBitmap(Gfx* _gfx) : IGfxDevice(_gfx) { _deviceName = "GfxBitmap"; }
	virtual ~GfxBitmap() {}

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


#endif	// __GFXBITMAP_H__
