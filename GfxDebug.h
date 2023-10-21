// GfxDebug.h
//
#ifndef __GFXDEBUG_H__
#define __GFXDEBUG_H__
#pragma once


#include "IGfxDevice.h"
#include "types.h"

class Gfx;

class GfxDebug : public IGfxDevice
{
	friend class Gfx;

public:
	GfxDebug(Gfx* _gfx) : IGfxDevice(_gfx) {}
	virtual ~GfxDebug() {}

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

protected:

private:
	SDL_Texture* _debug_texture = nullptr;
	int _tex_width = 0;
	int _tex_height = 0;
};




#endif // __GFXDEBUG_H__
