/////
// Gdx.cpp
// 
////////

#include "Bus.h"
#include "Gfx.h"


Byte Gfx::read(Word offset, bool debug )
{
	if (offset == DSP_GRES)
		return _dsp_gres;
	if (offset == DSP_GMODE)
		return _dsp_gres;
	return 0xCC;	//data;
}

void Gfx::write(Word offset, Byte data, bool debug)
{
	if (offset == DSP_GRES)
	{
		printf("Write to DSP_GRES: 0x%02x\n", data);
		_dsp_gres = data;
		Bus::Inst().write(DSP_GRES, _dsp_gres, true);
		Bus::Inst().IsDirty(true);
	}
	if (offset == DSP_GMODE)
	{
		printf("Write to DSP_GMODE: 0x%02x\n", data);
		_dsp_gres = data;
		Bus::Inst().write(DSP_GRES, _dsp_gres, true);
		Bus::Inst().IsDirty(true);
	}
}


Word Gfx::OnAttach(Word nextAddr) 
{ 
    int size = 0;
    Word old_addr = nextAddr;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_GRES", nextAddr, " (Byte) Screen Resolution Register");
    DisplayEnum("", 0, "DSP_GRES: BBRR.HHVV");
    DisplayEnum("", 0, "    BB:00 = 1-bpp (2-colors)");
    DisplayEnum("", 0, "    BB:01 = 2-bpp (4-colors)");
    DisplayEnum("", 0, "    BB:10 = 4-bpp (16-colors)");
    DisplayEnum("", 0, "    BB:11 = 8-bpp (256-colors)");    
    DisplayEnum("", 0, "    RR:00 = 16:9  aspect (1.777778)");
    DisplayEnum("", 0, "    RR:01 = 16:10 aspect (1.600000)");
    DisplayEnum("", 0, "    RR:10 = 16:11 aspect (1.454545)");
    DisplayEnum("", 0, "    RR:11 = 16:12 aspect (1.333333)");    
    DisplayEnum("", 0, "    HH:00 = 1x Horizontal Multiplier");
    DisplayEnum("", 0, "    HH:01 = 2x Horizontal Multiplier");
    DisplayEnum("", 0, "    HH:10 = 3x Horizontal Multiplier");
    DisplayEnum("", 0, "    HH:11 = 4x Horizontal Multiplier");    
    DisplayEnum("", 0, "    VV:00 = 1x Vertical Multiplier");
    DisplayEnum("", 0, "    VV:01 = 2x Vertical Multiplier");
    DisplayEnum("", 0, "    VV:10 = 3x Vertical Multiplier");
    DisplayEnum("", 0, "    VV:11 = 4x Vertical Multiplier");
    nextAddr++;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_GMODE", nextAddr, " (Byte) Graphics Mode Register");
    DisplayEnum("", 0, "DSP_GRES: ABCD.EFGG");
    DisplayEnum("", 0, "    A:0 = VSYNC OFF");
    DisplayEnum("", 0, "    A:1 = VSYNC ON");
    DisplayEnum("", 0, "    B:0 = Fullscreen Enabled (emulator only)");
    DisplayEnum("", 0, "    B:1 = Windowed Enabled (emulator only)");
    DisplayEnum("", 0, "    C:0 = Extended Graphics DISABLED");
    DisplayEnum("", 0, "    C:1 = Extended Graphics ENABLED");
    DisplayEnum("", 0, "    D:0 = Extended Bitmap Mode Active");
    DisplayEnum("", 0, "    D:1 = Extended Tile Graphics Active");
    DisplayEnum("", 0, "    E:0 = Standard Text / Bitmap DISABLED");
    DisplayEnum("", 0, "    E:1 = Standard Text / Bitmap ENABLED");
    DisplayEnum("", 0, "    F:0 = Standard Text Mode ENABLED");
	DisplayEnum("", 0, "    F:1 = Standard Bitmap Mode ENABLED");
	DisplayEnum("", 0, "    GG:00 = Standard Graphics 1-bpp (2-colors)");
	DisplayEnum("", 0, "    GG:01 = Standard Graphics 2-bpp (4-colors)");
	DisplayEnum("", 0, "    GG:10 = Standard Graphics 4-bpp (16-colors)");
	DisplayEnum("", 0, "    GG:11 = Standard Graphics 8-bpp (256-colors)");
    nextAddr++;

	// add more gfx registers here
	// ....

	return nextAddr - old_addr;
}




void Gfx::OnInit() {}
void Gfx::OnQuit() {}

void Gfx::OnActivate() 
{
	printf("void Gfx::OnActivate() \n");

	Bus& bus = Bus::Inst();


	// testing
	// bus.write(DSP_GRES, 0b10111111);


	// move to gmode decoder function
		// set up fullscreen/windowed
		bool bFullscreen = false;
		if (bus.read(DSP_GRES) & 0b01000000)
			bFullscreen = true;
		_window_flags = SDL_WINDOW_OPENGL | 
								SDL_WINDOW_RESIZABLE;
		if (bFullscreen)
			_window_flags = SDL_WINDOW_OPENGL | 
							SDL_WINDOW_FULLSCREEN_DESKTOP;  

		// set up aspect
		int asp = (bus.read(DSP_GRES) & 0x30) >> 4;
		printf("asp: 0x%02x\n", asp);
		switch (asp)
		{
			case 0:
				_aspect = 1.777778f;
				break;
			case 1:
				_aspect = 1.6f;
				break;
			case 2:
				_aspect = 1.454545f;
				break;
			case 3:
				_aspect = 1.333333f;
				break;
		}
		printf("Aspect: %f\n", _aspect);
		float window_width = 1280.0f;
		float window_height = window_width / _aspect;
	// ENDIF: move to 
	

    _window = SDL_CreateWindow("Retro_6809",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               (int)window_width,
                               (int)window_height,
                               _window_flags);		


}

void Gfx::OnDeactivate()
{
	if (_window != nullptr)
	{
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}
}


void Gfx::OnEvent(SDL_Event* evnt) {}
void Gfx::OnUpdate(float fElapsedTime) 
{
	std::string sTitle = "Retro_6809    FPS: ";

	//SDL_SetWindowTitle(_window, "text");	 // crashes hard!
}
void Gfx::OnRender() {}

void Gfx::OnPresent() 
{
	// swap display buffers / present
	// if (m_gfx)
	//     SDL_RenderPresent(m_gfx->GetRenderer());  	
}


