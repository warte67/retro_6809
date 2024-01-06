// GfxText.h
//

#ifndef __GFXTEXT_H__
#define __GFXTEXT_H__
#pragma once

#include "IGfxDevice.h"
#include "types.h"

class Gfx;

class GfxText : public IGfxDevice
{
	friend class Gfx;

public:
	GfxText(Gfx* _gfx) : IGfxDevice(_gfx) { _deviceName = "GfxText"; }
	virtual ~GfxText() {}

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


#endif // __GFXTEXT_H__

