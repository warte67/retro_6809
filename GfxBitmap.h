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


#endif	// __GFXBITMAP_H__
