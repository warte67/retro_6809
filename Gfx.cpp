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
	if (offset == DSP_EXT)
		return _dsp_ext;
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
	if (offset == DSP_EXT)
	{
		printf("Write to DSP_EXT: 0x%02x\n", data);
		_dsp_ext = data;
		Bus::Inst().write(DSP_EXT, _dsp_ext, true);
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
    DisplayEnum("", 0, "    BB:00 = Standard Graphics 1-bpp (2-color mode)");
    DisplayEnum("", 0, "    BB:01 = Standard Graphics 2-bpp (4-color mode)");
    DisplayEnum("", 0, "    BB:10 = Standard Graphics 4-bpp (16-color mode)");
    DisplayEnum("", 0, "    BB:11 = Standard Graphics 8-bpp (256-color mode)");    
    DisplayEnum("", 0, "    RR:00 = 16:9  aspect (1.777778)");
    DisplayEnum("", 0, "    RR:01 = 16:10 aspect (1.600000)");
    DisplayEnum("", 0, "    RR:10 = 16:11 aspect (1.454545)");
    DisplayEnum("", 0, "    RR:11 = 16:12 aspect (1.333333)");    
    DisplayEnum("", 0, "    HH:00 = 4x Horizontal Overscan Multiplier");
    DisplayEnum("", 0, "    HH:01 = 3x Horizontal Overscan Multiplier");
    DisplayEnum("", 0, "    HH:10 = 2x Horizontal Overscan Multiplier");
    DisplayEnum("", 0, "    HH:11 = 1x Horizontal Overscan Multiplier");    
    DisplayEnum("", 0, "    VV:00 = 4x Vertical Overscan Multiplier");
    DisplayEnum("", 0, "    VV:01 = 3x Vertical Overscan Multiplier");
    DisplayEnum("", 0, "    VV:10 = 2x Vertical Overscan Multiplier");
    DisplayEnum("", 0, "    VV:11 = 1x Vertical Overscan Multiplier");
    nextAddr++;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_EXT", nextAddr, " (Byte) Extended Graphics Register");
    DisplayEnum("", 0, "DSP_EXT: ABCD.EFGG");
    DisplayEnum("", 0, "     AA:00 = Extended Graphics 1bpp (2-color mode) ");
    DisplayEnum("", 0, "     AA:01 = Extended Graphics 2bpp (4-color mode) ");
    DisplayEnum("", 0, "     AA:10 = Extended Graphics 4bpp (16-color mode) ");
    DisplayEnum("", 0, "     AA:11 = Extended Graphics 4bpp (16-color mode) ");
    DisplayEnum("", 0, "     B:0   = Extended Graphics: DISABLED ");
    DisplayEnum("", 0, "     B:1   = Extended Graphics: ENABLED ");
    DisplayEnum("", 0, "     C:0   = Extended Extended Mode: BITMAP ");
    DisplayEnum("", 0, "     C:1   = Extended Extended Mode: TILES ");
    DisplayEnum("", 0, "     D:0   = Standard Graphics: DISABLED ");
    DisplayEnum("", 0, "     D:1   = Standard Graphics: ENABLED ");
    DisplayEnum("", 0, "     E:0   = Standard Display Mode: TEXT ");
    DisplayEnum("", 0, "     E:1   = Standard Display Mode: BITMAP ");
    DisplayEnum("", 0, "     F:0   = VSYNC OFF ");
    DisplayEnum("", 0, "     F:1   = VSYNC ON ");
    DisplayEnum("", 0, "     B:0   = Fullscreen Enabled( emulator only ) ");
    DisplayEnum("", 0, "     B:1   = Windowed Enabled ( emulator only ) ");
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
	_decode_dsp_gres();
	_decode_dsp_ext();

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





void Gfx::_decode_dsp_gres()
{
	// shortcut to the Bus instance and DSP_GRES data
	Bus& bus = Bus::Inst();
	Byte data = bus.read(DSP_GRES);

	// _aspect = Aspect Ratio
	Byte a = (data >> 4) & 0x03;
	switch (a)
	{
		case 0: _aspect = 16.0f/9.0f;  break;	// 16:9 
		case 1: _aspect = 16.0f/10.0f; break;	// 16:10
		case 2: _aspect = 16.0f/11.0f; break;	// 16:11
		case 3: _aspect = 16.0f/12.0f; break;	// 16:12
	}

	// _h_scan = Horizontal Overscan Multiplier
	_h_scan = (3-((data >> 2) & 0x03))+1;

	// _v_scan = Vertical Overscan Multiplier
	_v_scan = (3-((data >> 0) & 0x03))+1;

	// _std_bpp = Standard Graphics Bits-Per-Pixel
	_std_bpp = (data >> 6) & 0x03;
	switch (_std_bpp)
	{
		case 0: _std_bpp = 1; break;	// 2-colors
		case 1: _std_bpp = 2; break;	// 4-colors
		case 2: _std_bpp = 4; break;	// 16-colors
		case 3: _std_bpp = 8; break;	// 256-colors
	}
	// Adjust to fit within Standard Buffer
	float real_width = _base_texture_width * (5-_h_scan);
	float real_height = (_base_texture_width / _aspect) * (5-_v_scan);	
	float div = 9 - _std_bpp;		// bpp divisor
	float req_buffer_size = (real_width * real_height) / div;

	if (req_buffer_size > std_buffer_size)
		printf ("    ERROR: Buffer Overrun... Making Adjustments\n");
	while (req_buffer_size > std_buffer_size)
	{		
		_std_bpp >>= 1;					// _std_bpp changed
		if (_std_bpp == 0) break;
		printf("    -->_std_bpp: %d\n", _std_bpp);
		div = 9 - _std_bpp;		// bpp divisor
		req_buffer_size = (real_width * real_height) / div;

		// ENCODE THE CHANGES (_std_bpp: XX00.0000)
		Byte d = data;
		d &= 0x3f;
		d |= ((_std_bpp)-1) << 6;
		bus.write(DSP_GRES, d, true);
	}

	// bpp error buffer too big
	if (_std_bpp == 0)	
	{
		printf ("    ERROR: Buffer Overrun... Making Adjustments\n");
		_std_bpp = 1;	// back to 2-colors
		// try reducing vertical resolution
		while (req_buffer_size > std_buffer_size)
		{
			_v_scan++;					// _v_scan changed
			real_height = (_base_texture_width / _aspect) * (5-_v_scan);	
			printf("    -->real_height: %4.2f\n", real_height);
			div = 9 - _std_bpp;		// bpp divisor
			req_buffer_size = (real_width * real_height) / div;	

			// ENCODE THE CHANGES (_v_scan: 0000.00XX)
			Byte d = data;
			d &= 0xFC;
			d |= ((_v_scan-1) & 0x03);
			bus.write(DSP_GRES, d, true);			
		}
	}

	// SDL Texture Size
	_texture_width = (int)real_width;
	_texture_height = (int)real_height;

	// output debugging text
	printf("DSP_GRES decoded:\n");
	printf("  Aspect Ratio: %f\n", _aspect);
	printf("  Horizontal Overscan: %dx\n", _h_scan);
	printf("  Vertical Overscan: %dx\n", _v_scan);
	printf("  Real Width: %3.2f\n", real_width);
	printf("  Real Height: %3.2f\n", real_height);
	printf("  Standard Graphics BPP: %d\n", _std_bpp);
	printf("  Buffer Size: %3.2fK\n", req_buffer_size / 1024.0f);
	
}


void Gfx::_decode_dsp_ext()
{
	// shortcut to the Bus instance and DSP_EXT data
	Bus& bus = Bus::Inst();
	Byte data = bus.read(DSP_EXT);

	// _ext_bpp = Extended Graphics Bits_Per_Pixel
	_ext_bpp = (data >> 6) & 0x03;
	switch (_ext_bpp)
	{
		case 0: _ext_bpp = 1; break;	// 2-colors
		case 1: _ext_bpp = 2; break;	// 4-colors
		case 2: _ext_bpp = 4; break;	// 16-colors
		case 3: _ext_bpp = 8; break;	// 256-colors
	}	
	// adjust _ext_bpp for extended buffer overrun
	// ...

	// _extended_graphics_enable = Extended Graphics Enable
	_extended_graphics_enable = data & 0b00100000;

	// _extended_display_mode = Extended Mode: BITMAP / TILES
	_extended_display_mode = data & 0b00010000;

	// _standard_graphics_enable = Standard Graphics Enable
	_standard_graphics_enable = data & 0b00001000;

	// _standard_display_mode = Standard Display Mode 
	_standard_display_mode = data & 0b00000100;

	// _vsync = Vertical Sync
	_vsync = data & 0b00000010;

	// _windowed = Emulation Mode: 0:FULLSCREEN or 1:WINDOWED 
	_windowed = data & 0b00000001;

	// SDL Window Flags
	_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;  
	if (_windowed)
	{
		_window_width = 1280;
		_window_height = _window_width / _aspect;
		_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	}

	// SDL Renderer Flags
	_renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
	if (_vsync)
		_renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC;

	// output debugging text
	printf("DSP_EXT decoded:\n");
	printf("  Extended Graphics BPP: %d\n", _ext_bpp);
	printf("  Extended Graphics Enable: %d ", _extended_graphics_enable);
	(_extended_graphics_enable) ? printf("(ENABLED)\n") : printf("(DISABLED)\n");
	printf("  Extended Display Mode: %d ", _extended_display_mode);
	(_extended_display_mode) ? printf("(TILES)\n") : printf("(BITMAP)\n");
	printf("  Standard Graphics Enable: %d ", _standard_graphics_enable);
	(_standard_graphics_enable) ? printf("(ENABLED)\n") : printf("(DISABLED)\n");
	printf("  Extended Display Mode: %d ", _extended_display_mode);
	(_extended_display_mode) ? printf("(BITMAP)\n") : printf("(TEXT)\n");
	printf("  Vertical Sync: %d ", _vsync);
	(_vsync) ? printf("(ON)\n") : printf("(OFF)\n");
	printf("  Emulation Mode: %d ", _windowed);
	(_windowed) ? printf("(WINDOWED)\n") : printf("(FULLSCREEN)\n");
}