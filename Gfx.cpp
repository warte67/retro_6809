/////
// Gfx.cpp
//
// Graphics Device
////////

#include "Bus.h"
#include "Gfx.h"
#include "font8x8_system.h"

extern Byte font8x8_system[256][8];

Byte Gfx::read(Word offset, bool debug)
{
    // printf("READ GFX: $%04X = ???\n", offset);
    switch(offset)
    {
        // standard graphics sub-system hardware registers
        case DSP_GMODE:     return _dsp_gmode;
        case DSP_PXLOFS:    return _dsp_pxlofs;
        case DSP_TBASE:     return (_dsp_tbase >> 8) & 0xFF;
        case DSP_TBASE+1:   return _dsp_tbase & 0xFF;
        case DSP_GBASE:     return (_dsp_gbase >> 8) & 0xFF;
        case DSP_GBASE+1:   return _dsp_gbase & 0xFF;
        case DSP_GADDR:     return (_dsp_gaddr >> 8) & 0xFF;
        case DSP_GADDR+1:   return _dsp_gaddr & 0xFF;
        case DSP_GDATA:     return _dsp_data;

        // color palete registers (TODO: make these based on array)
        case DSP_PAL_IDX:       return _dsp_pal_idx;
        case DSP_PAL_CLR:       return (_dsp_pal_clr >> 8) & 0xFF;
        case DSP_PAL_CLR+1:     return _dsp_pal_clr & 0xFF;

        // text glyph definition data registers (TODO: move to text glyph mode)
        case DSP_GLYPH_IDX:     return _dsp_glyph_idx;
        case DSP_GLYPH_DATA:    return 0;   //(_dsp_glhph_data[_dsp_glyph_idx] >> 8) & 0xFF;
        case DSP_GLYPH_DATA+1:  return 0;   //_dsp_glhph_data[_dsp_glyph_idx] & 0xFF;

        // auxillary emulation flags
        case DSP_EMUFLAGS:  return _dsp_emuflags;
    }

    return 0xCC;
}

void Gfx::write(Word offset, Byte data, bool debug)
{
    printf("WRITE GFX: $%04X = $%02X\n", offset, data);

    switch (offset)
    {
        case DSP_GMODE:
        {
            if (_dsp_gmode == data)
                return; // no change
            // screen needs to be refreshed
            _dsp_gmode = data;      
            Bus::IsDirty(true);
        }
        break;

        case DSP_EMUFLAGS:
        {
            if (_dsp_emuflags == data)
                return; // no change
            // fullscreen / windowed toggle only
            if ((_dsp_emuflags & 0x01) != (data & 0x01))
            {
                if (data & 0x01)
                    _fullscreen = true;
                else
                    _fullscreen = false;
                Uint32 flags = 0;
                if (_fullscreen)
                    flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
                SDL_SetWindowFullscreen(_window, flags);   
                _dsp_emuflags = data;                 
                return;
            }
        }
        break;
    }
}

Word Gfx::OnAttach(Word nextAddr)
{
    int size = 0;
    Word old_addr = nextAddr;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_GMODE", nextAddr, " (Byte) Display Mode Register");
    DisplayEnum("", 0, "DSP_GMODE: ABCC.DDEE");
    DisplayEnum("", 0, "A = VSYNC     0:off       1:on");
    DisplayEnum("", 0, "B = timing    0:512x384   1:640x400");
    DisplayEnum("", 0, "C = bit depth 00:1bpp    01:2bpp  10:4bpp 11:8bpp (text00:mono)");
    DisplayEnum("", 0, "D = h_scan    00:1x      01:2x    10:4x           (text: lsb only)");
    DisplayEnum("", 0, "E = v_scan    00:1x      01:2x    10:4x           (text: lsb only)");
    nextAddr++;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_PXLOFS", nextAddr, " (Byte) Display Pixel Offset");
    DisplayEnum("", 0, "DSP_PXLOFS: AAAA.BBBB");
    DisplayEnum("", 0, "A = horizontal pixel offset (signed 4-bit)");
    DisplayEnum("", 0, "B = vertical pixel offset (signed 4-bit)");
    DisplayEnum("", 0, "Smooth scrolling affects only text modes.");
    nextAddr++;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_TBASE", nextAddr, " (Word) Text Glyph Base Address");
    DisplayEnum("", 0, "DSP_TBASE:  ($0400 default)");
    DisplayEnum("", 0, "NOTE: While this can be changed to any valid 16-bit address, ");
    DisplayEnum("", 0, "    care should be taken to ensure the screen remains within ");
    DisplayEnum("", 0, "    the text buffer ($0400-$1800).");
    nextAddr += 2;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_GBASE", nextAddr, " (Word) Graphics Base Address");
    DisplayEnum("", 0, "DSP_GBASE: Can be used for page swapping ($0000 default)");
    DisplayEnum("", 0, "Note: This is the base address to begin displaying pixel");
    DisplayEnum("", 0, "    graphics information within the external serial QSPI RAM.");
    nextAddr += 2;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_GADDR", nextAddr, " (Word) Pixel Address");
    DisplayEnum("", 0, "DSP_GADDR: $0000-$FFFF (16-bit address)");
    DisplayEnum("", 0, "Note: This is the 16-bit address register for the graphics modes.");
    DisplayEnum("", 0, "    Write to set or read to retrieve the current register value.");
    nextAddr += 2;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_GDATA", nextAddr, " (Byte) Pixel Data");
    DisplayEnum("", 0, "DSP_GDATA: $00-$FF (8-bit data)");
    DisplayEnum("", 0, "Note: Current pixel data stored at where the address register points.");
    DisplayEnum("", 0, "    Can be used for both read and write operations.");
    nextAddr += 1;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_PAL_IDX", nextAddr, " (Byte) Color Palette Index");
    DisplayEnum("", 0, "DSP_PAL_IDX: 0-255");
    DisplayEnum("", 0, "Note: Use this register to set the index into theColor Palette. ");
    DisplayEnum("", 0, "  Set this value prior referencing color data (DSP_PAL_CLR).");
    nextAddr += 1;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_PAL_CLR", nextAddr, " (Word) Indexed Color Palette Data");
    DisplayEnum("", 0, "DSP_PAL_CLR: Color Data A4R4G4B4 format");
    DisplayEnum("", 0, "Note: This is the color data for an individual palette entry.");
    DisplayEnum("", 0, "    Write to DSP_PAL_IDX with the index within the color palette");
    DisplayEnum("", 0, "    prior to reading or writing the color data in the DSP_PAL_CLR register.");
    nextAddr += 2;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_GLYPH_IDX", nextAddr, " (Byte) Text Glyph Index");
    DisplayEnum("", 0, "DSP_GLYPH_IDX: 0-256");
    DisplayEnum("", 0, "Note: Set this register to index a specific text glyph. Set this value");
    DisplayEnum("", 0, "    prior to updating glyph pixel data.");
    nextAddr += 1;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_GLYPH_DATA", nextAddr, " (8-Bytes) Text Glyph Pixel Data Array");
    DisplayEnum("", 0, "DSP_GLYPH_DATA: 8 rows of binary encoded glyph pixel data");
    DisplayEnum("", 0, "Note: Each 8x8 text glyph is composed of 8 bytes. The first byte in this ");
    DisplayEnum("", 0, "    array represents the top line of 8 pixels. Each array entry represents");
    DisplayEnum("", 0, "    a row of 8 pixels. ");
    nextAddr += 8;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_EMUFLAGS", nextAddr, " (Byte) Auxillary Emulation Flags");
    DisplayEnum("", 0, "DSP_EMUFLAGS: 0000.000A");
    DisplayEnum("", 0, "A=0:WINDOWED 1:FULLSCREEN");
    nextAddr += 1;
    /*
    DSP_SPRITE & DSP_TILES (varies):
        Sprites and Tiles will not be immediately implemented, but will
        utilize unused serial chip RAM.

    DSP_TILE_WIDTH, DSP_TILE_PITCH, DSP_TILE_BASE:
        Will later be used to determine base offset, map width, and pitch
        within the external serial chip RAM (or TONI’s Graphics Buffer).
    */
    return nextAddr - old_addr;
}

void Gfx::OnInit() 
{
    if (_gfxDisplayBuffer.size() == 0)
        _gfxDisplayBuffer.resize(65536);

	// initialize the default color palette
	if (_palette.size() == 0)
	{
		for (int t = 0; t < 16; t++)
			_palette.push_back({0x00});
		std::vector<PALETTE> ref = {
			{ 0x0000 },	// 0000 0000.0000 0000		0
			{ 0x005F },	// 0000 0000.0101 1111		1
			{ 0x050F },	// 0000 0101.0000 1111		2
			{ 0x055F },	// 0000 0101.0101 1111		3
			{ 0x500F },	// 0101 0000.0000 1111		4
			{ 0x505F },	// 0101 0000.0101 1111		5
			{ 0x631F },	// 0101 0101.0000 1111		6		{ 0x550F }
			{ 0xAAAF },	// 1010 1010.1010 1111		7
			{ 0x555F },	// 0101 0101.0101 1111		8
			{ 0x00FF },	// 0000 0000.1111 1111		9
			{ 0x0F0F },	// 0000 1111.0000 1111		a
			{ 0x0FFF },	// 0000 1111.1111 1111		b
			{ 0xF00F },	// 1111 0000.0000 1111		c
			{ 0xF0FF },	// 1111 0000.1111 1111		d
			{ 0xFF0F },	// 1111 1111.0000 1111		e
			{ 0xFFFF },	// 1111 1111.1111 1111		f
		};
        int t=0;
        for (auto &p : ref)
            _palette[t++] = p;
        
        // ToDo: define the rest of the color palette
        // ...

        // Temp: Fill the rest of the palette with black
		PALETTE blank { 0 };
		while (_palette.size() < 256)
			_palette.push_back(blank);
	}        
}
void Gfx::OnQuit() 
{    
    _palette.clear();
    _gfxDisplayBuffer.clear();
}

void Gfx::OnActivate()
{
    printf("Gfx::OnActivate()\n");
    // decode current settings
    _decode_gmode();

    // create a new window
    if (_fullscreen)
        _window_flags = SDL_WINDOW_OPENGL | 
                        SDL_WINDOW_FULLSCREEN_DESKTOP;              
    else // WINDOWED
        _window_flags = SDL_WINDOW_OPENGL | 
                        SDL_WINDOW_RESIZABLE;    
    _window = SDL_CreateWindow("Retro_6809",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               _window_width,
                               _window_height,
                               _window_flags);
    // create a new renderer
    _render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
    if (_vsync)
        _render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC;         
    _renderer = SDL_CreateRenderer(_window, -1, _render_flags);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    // // create the main target texture
    // _main_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888,
	// 		SDL_TEXTUREACCESS_TARGET, _texture_width, _texture_height);

    // create the background graphics texture
    _bg_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, _texture_width, _texture_height);

    // // create the text layer texture
    // _txt_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888,
	// 		SDL_TEXTUREACCESS_STREAMING, _texture_width, _texture_height);

/*
    // create the font glyphs
	if (_glyph_texture.size() == 0)
	{
		for (int t = 0; t < 256; t++)
		{
			SDL_Texture* glyph = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA4444,
				SDL_TEXTUREACCESS_TARGET, 8, 8);
			SDL_SetRenderTarget(_renderer, glyph);
			SDL_SetTextureBlendMode(glyph, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
			SDL_RenderClear(_renderer);
			for (int y = 0; y < 8; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					Byte bitMask = 1 << (7 - x);
					//if (font8x8_system[t][y] & bitMask)
					if (font8x8_system[t][y] & bitMask)
					{
						SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
						SDL_RenderDrawPoint(_renderer, x, y);
					}
				}
			}
			_glyph_texture.push_back(glyph);
		}
	}
*/
   
    for (int i=0; i<256; i++)
    {
        SDL_Texture* temp = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, 8, 8);
        SDL_Color sdl_c;
        Uint32 *dst;
        void *pixels;
        int pitch;            
        if (SDL_LockTexture(temp, NULL, &pixels, &pitch) < 0) 
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
            Bus::Error("");
        }
        for (int r=0; r<8; r++)
        {
            dst = (Uint32*)((Uint8*)pixels + r * pitch);
            Byte data = font8x8_system[i][r];
            for (int b=0; b<8; b++)
            {
                if (data & (1 << (7 - b)))
                {
                    //printf("1");       
                    sdl_c.a = 255;      
                    sdl_c.r = 255;      
                    sdl_c.g = 255;      
                    sdl_c.b = 255;      
                }
                else
                {
                    //printf(" ");    
                    // transparent
                    sdl_c.a = 0;    
                    sdl_c.r = 255;      
                    sdl_c.g = 0;      
                    sdl_c.b = 0;                               
                }
                *dst++ = 
                (
                    sdl_c.a<<24 |
                    (sdl_c.r<<16) |
                    (sdl_c.g<<8) |
                    sdl_c.b
                );                
            }
            //printf("\n");
        }
        //printf("\n");
        SDL_UnlockTexture(temp); 
        _glyph_texture.push_back(temp);
    }

}

void Gfx::OnDeactivate()
{
    printf("Gfx::OnDeactivate()\n");

	// destroy the glyph textures
	for (auto& a : _glyph_texture)
	{
    	SDL_DestroyTexture(a);    
        // delete a;
    }
	_glyph_texture.clear();

    // shutdown SDL based devices
    // // destroy text texture
    // if (_main_texture)
    // {
    //     SDL_DestroyTexture(_main_texture);
    //     _main_texture = nullptr;
    // }
    // // destroy text texture
    // if (_txt_texture)
    // {
    //     SDL_DestroyTexture(_txt_texture);
    //     _txt_texture = nullptr;
    // }
    // destroy bg texture
    if (_bg_texture)
    {
        SDL_DestroyTexture(_bg_texture);
        _bg_texture = nullptr;
    }
    // destroy renderer
    if (_renderer != nullptr)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    // destroy window
    if (_window != nullptr)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

void Gfx::OnEvent(SDL_Event *evnt) 
{
    switch(evnt->type)
    {
        case SDL_WINDOWEVENT:  
        {
            if (evnt->window.event == SDL_WINDOWEVENT_RESIZED)
            {
                printf("Window %d resized to %dx%d\n",
                        evnt->window.windowID, evnt->window.data1,
                        evnt->window.data2);
                
                _window_width = evnt->window.data1;
                _window_height = evnt->window.data2;
            }
        }
        break; 

        case SDL_KEYDOWN:
        {
            // FULLSCREEN TOGGLE
            if (evnt->key.keysym.sym == SDLK_RETURN)
            {                
                if ((SDL_GetModState() & KMOD_SHIFT))
                {
                    Byte data = Bus::memory()->read(DSP_EMUFLAGS);
                    if (data & 0b00000001)
                        data &= 0b11111110;
                    else
                        data |= 0b00000001;
                    Bus::memory()->write(DSP_EMUFLAGS, data);
                }
            }            
            // DISPLAY TESTS (SPACE and ALT-SPACE)
            if (evnt->key.keysym.sym == SDLK_SPACE)
            {
                Byte data = Bus::memory()->read(DSP_GMODE);
                if (SDL_GetModState() & KMOD_SHIFT)
                {
                    Byte data = Bus::memory()->read(DSP_GMODE);
                    data |= 0x40;
                    Bus::memory()->write(DSP_GMODE, data);
                }
                else
                {                    
                    Byte data = Bus::memory()->read(DSP_GMODE);
                    data &= ~0x40;
                    Bus::memory()->write(DSP_GMODE, data);
                }
            }
            // number keys change pixel size
            if (evnt->key.keysym.sym >= SDLK_0 && evnt->key.keysym.sym <= SDLK_8)
            {
                Byte key =  evnt->key.keysym.sym - SDLK_0;                
                printf("key: %d\n", key);
                Byte data = Bus::memory()->read(DSP_GMODE) & 0xF0;
                switch(key)
                {
                    case 0:     data |= 0b00000000;      break;
                    case 1:     data |= 0b00000001;      break;
                    case 2:     data |= 0b00000010;      break;
                    case 3:     data |= 0b00000100;      break;
                    case 4:     data |= 0b00000101;      break;
                    case 5:     data |= 0b00000110;      break;
                    case 6:     data |= 0b00001000;      break;
                    case 7:     data |= 0b00001001;      break;
                    case 8:     data |= 0b00001010;      break;
                }
                Bus::memory()->write(DSP_GMODE, data);
            }
        }
        break;
    }
} 

void Gfx::_updateGraphics(float fElapsedTime)
{
    // TEMPORARY RANDOM NOISE
    const float _texDelay = 0.01f;
    static float _texAcc = fElapsedTime;
    _texAcc += fElapsedTime;
    if (_texAcc > fElapsedTime + _texDelay)
    {
        _texAcc -= _texDelay;
        for (int t=0; t<65536; t++)
            _gfxDisplayBuffer[t] = rand() % 16;
    }
    //printf("fElapsedTime: %f\n", fElapsedTime);

    // steam to the background texture
    Word pixel_index = 0;
    SDL_Color sdl_c;
    Uint8 *src;
    Uint32 *dst;
    void *pixels;
    int pitch;
    Uint8 clr_index = 0;
    if (SDL_LockTexture(_bg_texture, NULL, &pixels, &pitch) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
        Bus::Error("");
    }
    else
    {
        for (int row = 0; row < _texture_height; ++row) 
        {
            dst = (Uint32*)((Uint8*)pixels + row * pitch);
            //for (int col = 0; col < _texture_width; ++col) 
            for (int col = 0; col < _texture_width; ++col) 
            {
                clr_index = _gfxDisplayBuffer[pixel_index++];
                *dst++ = 
                (
                    0xFF000000 |
                    (red(clr_index)<<16) |
                    (grn(clr_index)<<8) |
                    blu(clr_index)
                );
            }
        }
        SDL_UnlockTexture(_bg_texture);    
    }
}
void Gfx::_updateTiles(float fElapsedTime) {}
void Gfx::_updateTextGlyphs(float fElapsedTime) 
{
    // SDL_SetRenderTarget(_renderer, _txt_texture);
    // SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);
    // SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    // SDL_RenderClear(_renderer);

    // SDL_Rect dst = {0, 0, 8, 8};
    // SDL_RenderCopy(_renderer, _glyph_texture[0], NULL, &dst);
}
void Gfx::_updateSprites(float fElapsedTime) {}

void Gfx::OnUpdate(float fElapsedTime)
{
    // update the window title bar
    Bus* bus = Bus::GetInstance();
    std::string sTitle = "Retro_68009 - FPS: ";
    sTitle += std::to_string(bus->FPS());
    SDL_SetWindowTitle(_window, sTitle.c_str());

    // display the background graphics (OR the tile graphics)
    if (true)
        _updateGraphics(fElapsedTime);
    else
        _updateTiles(fElapsedTime);

    // overlay the text glyphs
    _updateTextGlyphs(fElapsedTime);

    // overlay sprites
    _updateSprites(fElapsedTime);
}

void Gfx::_decode_gmode()
{
    // VSYNC (A000.0000)
    _vsync = false;
    if (_dsp_gmode & 0b10000000)
        _vsync = true;

    // timing (0B00.0000)
    _timing_width = 512;
    _timing_height = _timing_width / _aspect;   //384;
    if (_dsp_gmode & 0b01000000)
    {
        _timing_width = 640;
        _timing_height = _timing_width / _aspect;   //400;
    }
    _window_width = _timing_width * 2;
    _window_height = _timing_height * 2;

    // bits per pixel (00CC.0000)
    _bpp = 1;
    Byte b = (_dsp_gmode >> 4) & 0x03;
    _bpp <<= b;

    // h_scan (0000.DD00)
    switch ((_dsp_gmode >> 2) & 0x03)
    {
    case 00:
        _pixel_width = 4;
        break;
    case 01:
        _pixel_width = 2;
        break;
    default:    // SDL_RenderClear(_renderer);
    // SDL_RenderCopy(_renderer, NULL, NULL, NULL);    
        _pixel_width = 1;
        break;
    }

    // v_scan (0000.00EE)
    switch (_dsp_gmode & 0x03)
    {
    case 00:
        _pixel_height = 4;
        break;
    case 01:
        _pixel_height = 2;
        break;
    default:
        _pixel_height = 1;
        break;
    }
    // SDL_SetRenderTarget(_renderer, _txt_texture);
    // SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);
    // SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    // SDL_RenderClear(_renderer);

    // SDL_Rect dst = {0, 0, 8, 8};
    // SDL_RenderCopy(_renderer, _glyph_texture[0], NULL, &dst);
    // auxillary emulation flags

    // WINDOWED or FULLSCREEN
    _fullscreen = false;
    if (_dsp_emuflags)      // for now:  non-zero = fullscreen
        _fullscreen = true;
}


void Gfx::OnRender() 
{    
    // clear the window    
    SDL_SetRenderTarget(_renderer, NULL);
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, NULL, NULL, NULL);

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
    //SDL_SetRenderTarget(_renderer, NULL);
    SDL_RenderCopy(_renderer, _bg_texture, NULL, &dest);
    
    // render the text glyph layer
    // SDL_SetRenderTarget(_renderer, _main_texture);
    // SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);
    // SDL_RenderCopy(_renderer, _txt_texture, NULL, NULL);//&dest);
    
    // render the sprite layer
    // ...

    // render the final result
    // SDL_SetRenderTarget(_renderer, NULL);
    // SDL_RenderCopy(_renderer, _main_texture, NULL, &dest);


    // The Bus object presents after everything renders
    // SDL_RenderPresent(_renderer);    
}
