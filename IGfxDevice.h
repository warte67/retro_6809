// IGfxDevice.h
//
#ifndef __IGFXDEVICE_H__
#define __IGFXDEVICE_H__
#pragma once	// redundant

#include "Device.h"

class Gfx;

class IGfxDevice : public Device
{
public:
	IGfxDevice(Gfx* _gfx) : m_gfx(_gfx) {}
	virtual ~IGfxDevice() {}

	// virtuals
	virtual Byte read(Word offset, bool debug = false) = 0;
	virtual void write(Word offset, Byte data, bool debug = false) = 0;
	//virtual Word read_word(Word offset, bool debug = false) = 0;
	//virtual void write_word(Word offset, Word data, bool debug = false) = 0;
	virtual Word OnAttach(Word nextAddr) = 0;
	virtual void OnInit() = 0;
	virtual void OnQuit() = 0;
	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;
	virtual void OnEvent(SDL_Event* evnt) = 0;
	virtual void OnUpdate(float fElapsedTime) = 0;
	virtual void OnRender() = 0;

protected:
	Gfx* m_gfx = nullptr;
private:
};




#endif //__IGFXDEVICE_H__