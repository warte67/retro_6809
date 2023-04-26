/////
// Gfx.cpp
//
// Graphics Device
////////

#include "Bus.h"
#include "Gfx.h"

Byte Gfx::read(Word offset, bool debug)
{
    // if (offset - _base < _size)
    // 	return memory[(Word)(offset - _base)];

    // printf("READ GFX: $%04X = ???\n", offset);
    switch(offset)
    {
        case DSP_GMODE:
            return _dsp_gmode;

        case DSP_EMUFLAGS:
            return _dsp_emuflags;
    }

    return 0xCC;
}

void Gfx::write(Word offset, Byte data, bool debug)
{
    // if (debug)
    //     if (offset - _base < _size)
    //         memory[(Word)(offset -_base)] = data;

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
            // // screen needs to be refreshed
            // _dsp_gmode = data;      
            // Bus::IsDirty(false);
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

void Gfx::OnInit() {}
void Gfx::OnQuit() {}

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
    // create the desktop texture
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA4444,
			SDL_TEXTUREACCESS_TARGET, _texture_width, _texture_height);
    SDL_SetRenderTarget(_renderer, _texture);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);  //clear to blue for now
    SDL_RenderClear(_renderer);
    SDL_SetRenderTarget(_renderer, NULL);
}

void Gfx::OnDeactivate()
{
    printf("Gfx::OnDeactivate()\n");
    // shutdown SDL based devices
    // destroy texture
    if (_texture)
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
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
                if ((SDL_GetModState() & KMOD_SHIFT))
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
        }
        break;
    }
}

void Gfx::OnUpdate(float fElapsedTime)
{
    Bus* bus = Bus::GetInstance();
    std::string sTitle = "Retro_68009 - FPS: ";
    sTitle += std::to_string(bus->FPS());
    SDL_SetWindowTitle(_window, sTitle.c_str());

    // fill the Gfx screen with static
    SDL_SetRenderTarget(_renderer, _texture);
    for (int t=0; t<10000; t++)
    {
        int x = rand() % (_texture_width);
        int y = rand() % (_texture_height);
        Uint8 r = rand() % 256;
        Uint8 g = rand() % 256;
        Uint8 b = rand() % 256;
        SDL_SetRenderDrawColor(_renderer, r, g, b, 255);
        SDL_RenderDrawPoint(_renderer, x, y);
        SDL_RenderCopy(_renderer, NULL, NULL, NULL);
    }
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

    // texture size
    _texture_width = _timing_width / _pixel_width;
    _texture_height = _timing_height / _pixel_height;

    /////////////////////////////////////////////////////
    // auxillary emulation flags

    // WINDOWED or FULLSCREEN
    _fullscreen = false;
    if (_dsp_emuflags)      // for now:  non-zero = fullscreen
        _fullscreen = true;
}


void Gfx::OnRender() 
{
    SDL_SetRenderTarget(_renderer, NULL);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, NULL, NULL, NULL);    
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
    SDL_RenderCopy(_renderer, _texture, NULL, &dest);

    // The Bus object presents after everything renders
    // SDL_RenderPresent(_renderer);    
}
