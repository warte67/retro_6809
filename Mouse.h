#ifndef __MOUSE_H__
#define __MOUSE_H__
#pragma once

#include "IGfxDevice.h"
#include "types.h"

class Gfx;

class Mouse : public IGfxDevice
{
	friend class Gfx;

public:
	Mouse(Gfx* _gfx) : IGfxDevice(_gfx) {}
	virtual ~Mouse() {}

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

};


#endif // __MOUSE_H__

