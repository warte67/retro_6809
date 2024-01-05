// GfxDebug.h
//
#ifndef __GFXDEBUG_H__
#define __GFXDEBUG_H__
#pragma once

#include <map>
#include <list>
#include "IGfxDevice.h"
#include "types.h"

class Gfx;
class C6809;
class Keyboard;

class GfxDebug : public IGfxDevice
{
	friend class Gfx;
	friend class Keyboard;

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
	void DrawCode(int col, int row);
	void DrawButtons();
	void DrawBreakpoints();
	void HandleButtons();
	bool EditRegister(float fElapsedTime);
	void DrawCursor(float fElapsedTime);

	bool SingleStep();
	void ContinueSingleStep();
	bool CoordIsValid(int x, int y);

	void MouseStuff();
	void KeyboardStuff();


protected:

	inline static bool _bIsDebugActive = ENABLE_DEBUG;
	inline static bool bSingleStep = DEBUG_SINGLE_STEP;	// false;

	
	// protected helpers
	void _onWindowResize();	// called when the viewing window is scaled
	std::string _hex(Uint32 n, Uint8 d);
	void _correctMouseCoords(int& mx, int& my);


	enum CSR_AT {
		CSR_AT_NONE, CSR_AT_ADDRESS, CSR_AT_DATA, CSR_AT_REGISTER
	};
	enum EDIT_REGISTER {
		EDIT_NONE,
		EDIT_CC, EDIT_D, EDIT_A, EDIT_B, EDIT_X,
		EDIT_Y, EDIT_U, EDIT_PC, EDIT_S, EDIT_DP,
		EDIT_BREAK,
	};
	struct REGISTER_NODE {
		EDIT_REGISTER reg = EDIT_NONE;
		Uint16 value = 0;
		Uint16 y_pos = 0;
		Uint16 x_min = 0;
		Uint16 x_max = 0;
	};
	std::vector<REGISTER_NODE> register_info = {
		{ EDIT_REGISTER::EDIT_NONE, 0, 0, 0, 0 },
		{ EDIT_REGISTER::EDIT_CC, 0, 1, 43, 44 },
		{ EDIT_REGISTER::EDIT_D, 0, 2, 42, 45 },
		{ EDIT_REGISTER::EDIT_A, 0, 2, 51, 52 },
		{ EDIT_REGISTER::EDIT_B, 0, 2, 57, 58 },
		{ EDIT_REGISTER::EDIT_X, 0, 3, 43, 46 },
		{ EDIT_REGISTER::EDIT_Y, 0, 3, 51, 54 },
		{ EDIT_REGISTER::EDIT_U, 0, 3, 59, 62 },
		{ EDIT_REGISTER::EDIT_PC, 0, 4, 43, 46 },
		{ EDIT_REGISTER::EDIT_S, 0, 4, 51, 54 },
		{ EDIT_REGISTER::EDIT_DP, 0, 4, 60, 61 },
		{ EDIT_REGISTER::EDIT_BREAK, 0, 33, 50, 53 },
	};
	REGISTER_NODE nRegisterBeingEdited = { EDIT_NONE,0,0,0,0 };

	struct BUTTON_NODE {
		std::string text;		// button text
		SDL_Scancode key;		// shortcut key scancode
		// color attribute
		// ...
		Uint16 x_min;			// button left
		Uint16 x_max;			// button right
		Uint16 y_pos;
		Byte clr_index;			// color index
		void (GfxDebug::* cbFunction)();	// button callback
	};
	std::vector<BUTTON_NODE> vButton = {	
		{" Clr Breaks",		SDL_SCANCODE_C,		51, 62, 31, 0xC, &GfxDebug::cbClearBreaks},
		{"Reset",			SDL_SCANCODE_R,		43, 49, 31, 0xB, &GfxDebug::cbReset },
		{"NMI",				SDL_SCANCODE_N,		37, 41, 31, 0xD, &GfxDebug::cbNMI },
		{"IRQ",				SDL_SCANCODE_I,		31, 35, 31, 0xD, &GfxDebug::cbIRQ },
		{" FIRQ",			SDL_SCANCODE_F,		24, 29, 31, 0xD, &GfxDebug::cbFIRQ },
		{" RUN",			SDL_SCANCODE_D,		17, 22, 31, 0xB, &GfxDebug::cbRunStop },
		{" EXIT",			SDL_SCANCODE_H,		17, 22, 33, 0xB, &GfxDebug::cbHide },
		{"STEP_INTO",		SDL_SCANCODE_SPACE,	24, 34, 33, 0x9, &GfxDebug::cbStepIn },
		{"STEP_OVER",		SDL_SCANCODE_O,		36, 46, 33, 0x9, &GfxDebug::cbStepOver },
		{"ADD BRK",			SDL_SCANCODE_B,		48, 54, 33, 0xC, &GfxDebug::cbAddBrk },
	};

	// button callbacks
	void cbClearBreaks();
	void cbReset();
	void cbNMI();
	void cbIRQ();
	void cbFIRQ();
	void cbRunStop();
	void cbHide();
	void cbStepIn();
	void cbStepOver();
	void cbAddBrk();

private:
	SDL_Texture* _debug_texture = nullptr;
	std::vector<SDL_Texture*> glyph_textures;
	int _tex_width = 0;
	int _tex_height = 0;

	std::vector <Word> mem_bank = { SSTACK_TOP - 0x0048, STD_VID_MIN, STD_VID_MAX };
	std::vector <Word> sDisplayedAsm;
	std::map<Word, bool> mapBreakpoints;	
	std::list<Word> asmHistory;		// track last several asm addresses

	int csr_x = 0;
	int csr_y = 0;
	int csr_at = CSR_AT::CSR_AT_NONE;
	char mouse_wheel = 0;
	bool bIsStepPaused = true;
	bool bIsCursorVisible = false;
	Word mousewheel_offset = 0;		// applies to code scroll
	bool bMouseWheelActive = false; // applies to code scroll
	int mw_brk_offset = 0;			// mouse wheel adjusts the offset

	bool bEditingBreakpoint = false;
	Word new_breakpoint = 0;		// working copy to be edited
	Word reg_brk_addr = 0x0000;	// break point hardware register
	Byte reg_flags = 0x00;			// debug flags hardware register
};




#endif // __GFXDEBUG_H__
