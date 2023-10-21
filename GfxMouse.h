#ifndef __GFXMOUSE_H__
#define __GFXMOUSE_H__
#pragma once

#include "IGfxDevice.h"
#include "types.h"

class Gfx;

class GfxMouse : public IGfxDevice
{
	friend class Gfx;

public:
	GfxMouse(Gfx* _gfx) : IGfxDevice(_gfx) {}
	virtual ~GfxMouse() {}

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

	int mouse_x = 0;
	int mouse_y = 0;

private:
	SDL_Texture* _mouse_texture = nullptr;
	int _tex_width = 0;
	int _tex_height = 0;

};


#endif // __GFXMOUSE_H__

