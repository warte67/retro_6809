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
		return _dsp_gmode;
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
		_dsp_gmode = data;
		Bus::Inst().write(DSP_GMODE, _dsp_gmode, true);
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
    DisplayEnum("", 0, "    BB:00 = Extended Graphics 1-bpp (2-color mode)");
    DisplayEnum("", 0, "    BB:01 = Extended Graphics 2-bpp (4-color mode)");
    DisplayEnum("", 0, "    BB:10 = Extended Graphics 4-bpp (16-color mode)");
    DisplayEnum("", 0, "    BB:11 = Extended Graphics 8-bpp (256-color mode)");    
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
    DisplayEnum("", 0, "DSP_GMODE: ABCD.EFGG");
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
	DisplayEnum("", 0, "    GG:00 = Extended Graphics 1-bpp (2-colors)");
	DisplayEnum("", 0, "    GG:01 = Extended Graphics 2-bpp (4-colors)");
	DisplayEnum("", 0, "    GG:10 = Extended Graphics 4-bpp (16-colors)");
	DisplayEnum("", 0, "    GG:11 = Extended Graphics 8-bpp (256-colors)");
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

	// decode the gfx registers
	_decode_gfx();

	// testing
	// bus.write(DSP_GRES, 0b10111111);	

	// create the window
    _window = SDL_CreateWindow("Retro_6809",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               (int)_window_width,
                               (int)_window_height,
                               _window_flags);		
							   //SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	// create the renderer
    _renderer = SDL_CreateRenderer(_window, -1, _renderer_flags);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

	// create the extended graphics texture
	_ext_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB4444,
	 		SDL_TEXTUREACCESS_STREAMING, _texture_width, _texture_height);

	// create the standard graphics texture
	_std_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB4444,
	 		SDL_TEXTUREACCESS_STREAMING, _texture_width, _texture_height);

	// create the render target texture
	_render_target = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB4444,
			SDL_TEXTUREACCESS_TARGET, _texture_width, _texture_height);

}

void Gfx::OnDeactivate()
{
	if (_render_target != nullptr)
	{
		SDL_DestroyTexture(_render_target);
		_render_target = nullptr;
	}
	if (_std_texture != nullptr)
	{
		SDL_DestroyTexture(_std_texture);
		_std_texture = nullptr;
	}	
	if (_ext_texture != nullptr)
	{
		SDL_DestroyTexture(_ext_texture);
		_ext_texture = nullptr;
	}	
	if (_renderer != nullptr)
	{
		SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;
	}
	if (_window != nullptr) 
	{
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}
}

void Gfx::OnEvent(SDL_Event* evnt) 
{
    switch(evnt->type)
    {
        case SDL_WINDOWEVENT:  
        {
            if (evnt->window.event == SDL_WINDOWEVENT_RESIZED)
            {
                // printf("Window %d resized to %dx%d\n",
                //         evnt->window.windowID, evnt->window.data1,
                //         evnt->window.data2);                
                _window_width = evnt->window.data1;
                _window_height = evnt->window.data2;
            }
        }
        break; 
	}	
}

void Gfx::OnUpdate(float fElapsedTime) 
{
	// clear the window    
    SDL_SetRenderTarget(_renderer, NULL);
	SDL_SetRenderDrawColor(_renderer, 16, 16, 16, 255);	// border color
    SDL_RenderClear(_renderer);

	// fill extended buffer with noise for testing
	if (_extended_graphics_enable)
	{
		void *pixels;
		int pitch;
		if (SDL_LockTexture(_ext_texture, NULL, &pixels, &pitch) < 0)
			Bus::Error("Failed to lock texture: ");	
		else
		{
			for (int t=0; t<1000; t++)
			{
				int x = std::rand() % (_texture_width);
				int y = std::rand() % (_texture_height);

				Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));		// because data size is two bytes 

				*dst = ( 
					0xF000	|							// alpha
					(std::rand() % 0xF) << 8 |			// red
					(std::rand() % 0xF) << 4 |			// green
					(std::rand() % 0xF)					// blue
				);    
			}
			SDL_UnlockTexture(_ext_texture);
		}
		SDL_SetRenderTarget(_renderer, _render_target);
		SDL_RenderCopy(_renderer, _ext_texture, NULL, NULL);
	}
	else
	{
		SDL_SetRenderTarget(_renderer, _render_target);
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);
	}
}
void Gfx::OnRender() 
{
    SDL_SetRenderTarget(_renderer, NULL);

    // build the destination rectangle according to current aspect ratio
    int ww = _window_width;
    int wh = _window_height;
    float fh = (float)_window_height;
    float fw = fh * _aspect;

    if (fw > ww)
    {
        fw = ww;
        fh = fw / _aspect;
    }
    SDL_Rect dest = 
    { 
        int(_window_width / 2 - (int)fw / 2), 
        int(wh / 2 - (int)fh / 2), 
        (int)fw, 
        (int)fh 
    };
    // render the background graphics layer
    SDL_RenderCopy(_renderer, _render_target, NULL, &dest);
}

void Gfx::OnPresent() 
{
	// swap display buffers / present
	SDL_RenderPresent(_renderer);  	
}


// void Gfx::_setPixel(int x, int y, Byte color_index, bool bIgnoreAlpha)
// {

//     void *pixels;
//     int pitch;
//     Uint8 clr_index = 0;    
//     if (SDL_LockTexture(_bg_texture, NULL, &pixels, &pitch) < 0) {
//         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
//         Bus::Error("");
//     }
//     else
//     {
//         _setPixel_unlocked(pixels, pitch, x, y, color_index, bIgnoreAlpha);
//         SDL_UnlockTexture(_bg_texture);
//     }    
// }

// void Gfx::_setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte color_index, bool bIgnoreAlpha)
// {
//     //Uint32 *dst = (Uint32*)((Uint8*)pixels + (y * pitch) + (x*4));    // why is this (x*4) and not simply x?
//     Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));		// because data size is two bytes 
//     bool ALPHA_BLEND = true;
//     if (ALPHA_BLEND)
//     {       
//         // int ret = ((p1 * (256-a))) + (p2 * (a+1)) >> 8;
//         Uint16 pixel = *dst;	// 0xARGB
// 		Byte r1 = (pixel & 0x0f00) >> 8;
// 		Byte g1 = (pixel & 0x00f0) >> 4;
// 		Byte b1 = (pixel & 0x000f) >> 0;
// 		//
//         Byte a2 = alf(color_index);
//         Byte r2 = red(color_index);
//         Byte g2 = grn(color_index);
//         Byte b2 = blu(color_index);
//         if (bIgnoreAlpha)
//             a2 = 15;
// 		//
//         Byte r = ((r1 * (16-a2))) + (r2 * (a2+1)) >> 4;
//         Byte g = ((g1 * (16-a2))) + (g2 * (a2+1)) >> 4;
//         Byte b = ((b1 * (16-a2))) + (b2 * (a2+1)) >> 4;

//         if (alf(color_index) != 0 || bIgnoreAlpha)
//         {
//             *dst = (
//                 0xF000 | 
//                 (r<<8) | 
//                 (g<<4) | 
//                 (b)
//             );          
// 		}	
//     }
//     else
//     {        
//         // simple non-zero alpha channel
//         if (alf(color_index) != 0 || bIgnoreAlpha)
//         {
//             *dst = 
//             (
//                 0xF000 |
//                 (red(color_index)<<8) |
//                 (grn(color_index)<<4) |
//                 blu(color_index)
//             );    
//         }
//     }    
// }





void Gfx::_decode_gfx()
{
	// shortcut to the Bus instance
	Bus& bus = Bus::Inst();

	// setup window flags
	_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	_fullscreen = true;
	if (bus.read(DSP_GMODE) & 0x40)
	{
		_fullscreen = false;
		_window_width = 1280;
		_window_height = _window_width / _aspect;
	}

	if (_fullscreen)
		_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;  

	// setup aspect
	int asp = (bus.read(DSP_GRES) & 0x30) >> 4;
	printf("asp: 0x%02x\n", asp);	// *** DEBUGGING ***
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
	_window_height = _window_width / _aspect;

	// renderer flags
	_vsync = bus.read(DSP_GMODE) & 0x80;
	_renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
    if (_vsync)
        _renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC;         

	// scan multiplier
	_h_scan = ((bus.read(DSP_GRES) & 0x0C) >> 2)+1;
	_v_scan = ((bus.read(DSP_GRES) & 0x03) >> 0)+1;
	_texture_width = _base_texture_width * _h_scan;
	_texture_height = (_base_texture_width / _aspect) * _v_scan;

	// bits per pixel
	switch (bus.read(DSP_GRES) >> 6)
	{
		case 0: _ext_bpp = 1; break;
		case 1: _ext_bpp = 2; break;
		case 2: _ext_bpp = 4; break;
		case 3: _ext_bpp = 8; break;
	}
	switch (bus.read(DSP_GMODE) & 0x03)
	{
		case 0: _std_bpp = 1; break;
		case 1: _std_bpp = 2; break;
		case 2: _std_bpp = 4; break;
		case 3: _std_bpp = 8; break;
	}

	// adjust bits-per-pixel to fit within the video buffer
	// ...
    // while ((_texture_width * _texture_height) / (8/_bpp) > (64*1024))
    //     _bpp >>=1;
    // int size = (_texture_width * _texture_height) / (8/_bpp);



	// graphics enable bits
	_extended_graphics_enable = bus.read(DSP_GMODE) & 0x20;
	_extended_tilemode_enable = bus.read(DSP_GMODE) & 0x10;
	_standard_graphics = bus.read(DSP_GMODE) & 0x08;
	_standard_bitmap = bus.read(DSP_GMODE) & 0x04;

	// output debugging text
	printf("Aspect: %f\n", _aspect);
	printf("_window_width: %d\n", (int)_window_width);
	printf("_window_height: %d\n", (int)_window_height);
	printf("h_scan: %d\n", _h_scan);
	printf("v_scan: %d\n", _v_scan);
	printf("texture width: %d\n", _texture_width);
	printf("texture height: %d\n", _texture_height);
	printf("VSYNC: %d\n", _vsync);
	printf("Extended BPP: %d\n", _ext_bpp);
	printf("Standard BPP: %d\n", _std_bpp);
	printf("Extended Graphics Enabled: %d\n", _extended_graphics_enable);
	printf("Extended Tilemode Enabled: %d\n", _extended_tilemode_enable);
	printf("Standard Graphics Enabled: %d\n", _standard_graphics);

	if (_standard_bitmap)
		printf("Standard Graphics Mode: Bitmap\n");
	else
		printf("Standard Graphics Mode: Text\n");
	
}
