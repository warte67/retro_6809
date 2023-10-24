// GfxDebug.h
//
#ifndef __GFXDEBUG_H__
#define __GFXDEBUG_H__
#pragma once


#include "IGfxDevice.h"
#include "types.h"

class Gfx;
class C6809;

class GfxDebug : public IGfxDevice
{
	friend class Gfx;

public:
	GfxDebug(Gfx* _gfx) : IGfxDevice(_gfx) { _deviceName = "GfxDebug"; }
	virtual ~GfxDebug() {}

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

	void OutGlyph(int col, int row, Byte glyph,
		Byte red = 255, Byte grn = 255, Byte blu = 255, bool bDropShadow = false);
	int OutText(int col, int row, std::string text,
		Byte red = 255, Byte grn = 255, Byte blu = 255, bool bDropshadow = false);
	void DumpMemory(int col, int row, Word addr);
	void DrawCpu(int x, int y);

protected:

	bool _bIsDebugActive = true;
	
	// protected helpers
	void _onWindowResize();	// called when the viewing window is scaled
	std::string _hex(Uint32 n, Uint8 d);

private:
	SDL_Texture* _debug_texture = nullptr;
	std::vector<SDL_Texture*> glyph_textures;
	int _tex_width = 0;
	int _tex_height = 0;

	std::vector <Word> mem_bank = { SSTACK_TOP - 0x0048, SCREEN_BUFFER, HDW_REGS };
};




#endif // __GFXDEBUG_H__
