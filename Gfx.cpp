/////
// Gdx.cpp
// 
////////

#include "Bus.h"
#include "Gfx.h"
#include "font8x8_system.h"

// extern const Byte font8x8_system[256][8];

Byte Gfx::read(Word offset, bool debug )		// is debug completely unused in the call?
{
	// handle GfxMouse reads
	if (offset >= CSR_BEGIN && offset <= CSR_END)
		return m_mouse->read(offset, debug);
	// handle GfxDebug reads
	if (offset >= DBG_BEGIN && offset <= DBG_END)
		return m_debug->read(offset, debug);

	Bus& bus = Bus::Inst();
	Byte data = 0xCC;

	// handle Gfx Reads
	switch (offset)
	{
		case STD_VID_MAX:		data = _std_vid_max >> 8; break;
		case STD_VID_MAX+1:		data = _std_vid_max & 0xFF; break;
		case DSP_GRES: 			data = _dsp_gres; break;
		case DSP_EXT:			data = _dsp_ext; break;
		case DSP_ERR:			{ Byte err = _dsp_err;	_dsp_err = 0;	data = err; break;} 
		case DSP_TXT_COLS:      data = _texture_width / 8; break;
		case DSP_TXT_ROWS:      data = _texture_height / 8; break;

        // color palete registers
		case DSP_PAL_IDX:   	data = _dsp_pal_idx; break;
		case DSP_PAL_CLR + 1: 	data = (_palette[_dsp_pal_idx].color >> 8) & 0xFF; break;
		case DSP_PAL_CLR + 0: 	data = _palette[_dsp_pal_idx].color & 0xFF; break;

        // text glyph definition data registers
        case DSP_GLYPH_IDX:     data = _dsp_glyph_idx; break;
        case DSP_GLYPH_DATA+0:  data = _dsp_glyph_data[_dsp_glyph_idx][0]; break;
        case DSP_GLYPH_DATA+1:  data = _dsp_glyph_data[_dsp_glyph_idx][1]; break;
        case DSP_GLYPH_DATA+2:  data = _dsp_glyph_data[_dsp_glyph_idx][2]; break;
        case DSP_GLYPH_DATA+3:  data = _dsp_glyph_data[_dsp_glyph_idx][3]; break;
        case DSP_GLYPH_DATA+4:  data = _dsp_glyph_data[_dsp_glyph_idx][4]; break;
        case DSP_GLYPH_DATA+5:  data = _dsp_glyph_data[_dsp_glyph_idx][5]; break;
        case DSP_GLYPH_DATA+6:  data = _dsp_glyph_data[_dsp_glyph_idx][6]; break;
		case DSP_GLYPH_DATA + 7:  data = _dsp_glyph_data[_dsp_glyph_idx][7]; break;

	}

	bus.write(offset, data, true);
	return data;	//data;
}

void Gfx::write(Word offset, Byte data, bool debug)
{
	// handle GfxMouse writes
	if (offset >= CSR_BEGIN && offset <= CSR_END) 
	{ m_mouse->write(offset, data, debug); return; }
	// handle GfxDebug writes
	if (offset >= DBG_BEGIN && offset <= DBG_END)
	{ m_debug->write(offset, data, debug); return; }

	// handle Gfx Writes
	switch (offset)
	{
		case DSP_GRES: 	{
			_dsp_gres = data;
			Bus::Inst().write(DSP_GRES, _dsp_gres, true);
			Bus::Inst().IsDirty(true);		
			return;	
		}
		case DSP_EXT:	{
			_dsp_ext = data;
			Bus::Inst().write(DSP_EXT, _dsp_ext, true);
			Bus::Inst().IsDirty(true);
			return;
		}
		case DSP_ERR:	{
			_dsp_err = data;
			Bus::Inst().write(DSP_ERR, _dsp_err, true);
			return;
		}
		// color palete registers
		case DSP_PAL_IDX:	{ _dsp_pal_idx = data;  return; }
		case DSP_PAL_CLR:	{
			Word c = _palette[_dsp_pal_idx].color & 0xff00;
			_palette[_dsp_pal_idx].color = c | data;
			return;
		}
		case DSP_PAL_CLR+1:	{
			Word c = _palette[_dsp_pal_idx].color & 0x00ff;
			_palette[_dsp_pal_idx].color = c | ((Word)data << 8);
			return; 
		}
        // text glyph definition data registers
        case DSP_GLYPH_IDX: _dsp_glyph_idx = data; return;
        case DSP_GLYPH_DATA+0:  _dsp_glyph_data[_dsp_glyph_idx][0] = data; return;
        case DSP_GLYPH_DATA+1:  _dsp_glyph_data[_dsp_glyph_idx][1] = data; return;
        case DSP_GLYPH_DATA+2:  _dsp_glyph_data[_dsp_glyph_idx][2] = data; return;
        case DSP_GLYPH_DATA+3:  _dsp_glyph_data[_dsp_glyph_idx][3] = data; return;
        case DSP_GLYPH_DATA+4:  _dsp_glyph_data[_dsp_glyph_idx][4] = data; return;
        case DSP_GLYPH_DATA+5:  _dsp_glyph_data[_dsp_glyph_idx][5] = data; return;
        case DSP_GLYPH_DATA+6:  _dsp_glyph_data[_dsp_glyph_idx][6] = data; return;
        case DSP_GLYPH_DATA+7:  _dsp_glyph_data[_dsp_glyph_idx][7] = data; return; 		
	
	}
}


Word Gfx::OnAttach(Word nextAddr) 
{ 
    int size = 0;
    Word old_addr = nextAddr;

    DisplayEnum("", 0, "");
    DisplayEnum("STD_VID_MAX", nextAddr, " (Word) Standard Video Buffer Max");
    nextAddr+=2;

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

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_ERR", nextAddr, " (Byte) Display Sub-System Error Code Register");
    DisplayEnum("", 0, "DSP_ERR: ABCD.EFGH");
    DisplayEnum("", 0, "     A:0   = Standard Buffer Overflow ");
    DisplayEnum("", 0, "     B:0   = Extended Buffer Overflow ");
    DisplayEnum("", 0, "     C:0   = Reserved ");
    DisplayEnum("", 0, "     D:0   = Reserved ");
    DisplayEnum("", 0, "     E:0   = Reserved ");
    DisplayEnum("", 0, "     F:0   = Reserved ");
    DisplayEnum("", 0, "     G:0   = Reserved ");
    DisplayEnum("", 0, "     H:0   = Reserved ");
	nextAddr++;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_TXT_COLS", nextAddr, " (Byte) READ-ONLY Text Screen Columns");
    nextAddr += 1;
    DisplayEnum("DSP_TXT_ROWS", nextAddr, " (Byte) READ-ONLY Text Screens Rows");
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

	// ADD THE GFX DEVICES:
	
	// add the debug registers
	m_debug = new GfxDebug(this);
	nextAddr += Attach(m_debug, nextAddr);
	//_gfx_devices.push_back(m_debug);

	// add the mouse registers
	m_mouse = new GfxMouse(this);
	nextAddr += Attach(m_mouse, nextAddr);
	//_gfx_devices.push_back(m_mouse);

	DisplayEnum("GFX_END", nextAddr, "End of GFX Device Registers");

	return nextAddr - old_addr;
}


void Gfx::OnInit() 
{
	// initialize the default color palette
	if (_palette.size() == 0)
	{
		std::vector<PALETTE> ref = {
			{ 0x0000 },		// 0: transparent black
			{ 0xFFFF },		// 1: white
			{ 0xF007 },		// 2: dk blue
			{ 0xF600 },		// 5: dk red
			{ 0xF140 },		// 4: dk green
			{ 0xF840 },		// 3: brown
			{ 0xF406 },		// 6: purple          
			{ 0xF046 },		// 7: deep sea           	
			{ 0xF555 },		// 8: gray
			{ 0xF22F },		// 9: blue
			{ 0xFd00 },		// A: red
			{ 0xF4F6 },		// B: lt green
			{ 0xFED0 },		// C: yellow
			{ 0xF85b },		// D: Lt Purple
			{ 0xF59f },		// E: lt sky
			{ 0xF000 },		// F: black

			// { 0xF057 },		// 4: dk cyan
			// { 0xF050 },		// 3: dk green
			// { 0xF607 },		// 6: dk magenta
			// { 0xF650 },		// 7: brown			
			// { 0xFAAA },		// 8: lt gray
			// { 0xF666 },		// 9: dk gray
			// { 0xF00F },		// A: blue
			// { 0xF0F0 },		// B: green
			// { 0xF0FF },		// C: cyan
			// { 0xFF00 },		// D: red
			// { 0xFF0F },		// E: magenta
			// { 0xFFF0 },		// F: yellow


			{ 0xF000 },		// N: opaque black
			{ 0xFEEE },		// N: lt silver
			{ 0xF999 },		// N: md silver
			{ 0xF444 },		// N: dk silver
			{ 0xF817 },		// N:
			{ 0xFa35 },		// N:
			{ 0xFc66 },		// N:
			{ 0xFe94 },		// N: 
			{ 0xFed0 },		// N: yellow
			{ 0xF9d5 },		// N: 	
			{ 0xF4d8 },		// N;
			{ 0xF2cb },		// N:
			{ 0xF0bc },		// N:
			{ 0xF09c },		// N:
			{ 0xF36b },		// N:
			{ 0xF639 },		// N:
		};
        for (auto &p : ref)
			_palette.push_back(p);        
        // ToDo: define the rest of the color palette
		for (int r = 0; r < 16; r++)
		{
			PALETTE ent = { 0x000f };
			ent.r = r;
            _palette.push_back(ent);
		}        
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.g = t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.b = t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.r = t;
			ent.b = t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.r = t;
			ent.g = t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.r = t;
			ent.g = t;
			ent.b = t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.r = 15 - t;
			ent.b = t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.r = 15 - t;
			ent.g = t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.g = 15 - t;
			ent.b = t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.r = 15 - t;
			ent.g = 15 - t;
			ent.b = t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.r = 15 - t;
			ent.g = t;
			ent.b = 15 - t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.r = t;
			ent.g = 15 - t;
			ent.b = 15 - t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.r = 15 - t;
			ent.g = t;
			ent.b = 15 - t;
			_palette.push_back(ent);
		}
		for (int t = 0; t < 16; t++)
		{
			PALETTE ent = { 0x000f };
			ent.r = 15 - t;
			ent.g = 15 - t;
			ent.b = t;
			_palette.push_back(ent);
		}
        // Temp: Fill the rest of the palette with black
		PALETTE blank { 0 };
		while (_palette.size() < 256)
			_palette.push_back(blank);
	}  

    // // clear the graphics buffer
    // for (int t=0; t<65536; t++)
    //     _gfxDisplayBuffer[t] = 0;

    // clear the text buffer to white on black spaces
    for (int t=SCREEN_BUFFER; t<SCREEN_BUFFER+std_buffer_size; t+=2)
    {
        write(t, 32);        
        write(t+1, 0xF0);       
    }    
    // initialize the font glyph buffer
    for (int i=0; i<256; i++)
        for (int r=0; r<8; r++)
            _dsp_glyph_data[i][r] = font8x8_system[i][r];

    // // output a test string
    // std::string sMessage = "Hello World...";
    // Word addr = SCREEN_BUFFER;
    // for (auto &a : sMessage)
    // {
    //     write(addr, a);
    //     write(addr+1, 0xF0);		// F: yellow
    //     addr+=2;
    // }

	// init the other graphics devices
	for (auto& d : _gfx_devices)
		d->OnInit();
}

Word Gfx::Attach(IGfxDevice* dev, Word _lastAddress, Word size)
{
	if (dev != nullptr)
	{
		if (size == 0)
			size = dev->OnAttach((Word)_lastAddress);
		else
			dev->DisplayEnum(dev->Name(), _lastAddress, "");
		dev->Base(_lastAddress);
		dev->Size(size);
		_lastAddress += size;
		_gfx_devices.push_back(dev);
	}
	if (size > 65536)
		Bus::Error("Memory allocation beyond 64k boundary!");
	return size;
}

void Gfx::OnQuit() 
{
	// remove the mouse device
	if (m_mouse)
	{
		delete m_mouse;
		m_mouse = nullptr;
	}
	// clear the _gfx_devices vector
	_gfx_devices.clear();

	// run OnQuit() for the other graphics devices
	for (auto& d : _gfx_devices)
		d->OnQuit();
}

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

	// run OnActivate() for the other graphics devices
	for (auto& d : _gfx_devices)
		d->OnActivate();
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

	// run OnDeactivate() for the other graphics devices
	for (auto& d : _gfx_devices)
		d->OnDeactivate();
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

				m_debug->_onWindowResize();
            }
			break;
		}
	}	

	// run OnEvent() for the other graphics devices
	for (auto& d : _gfx_devices)
		d->OnEvent(evnt);
}

void Gfx::OnUpdate(float fElapsedTime) 
{
	// restrict updates based on elapsed time
	const float _delay = 0.01f;
    static float _acc = fElapsedTime;
    _acc += fElapsedTime;
    if (_acc > fElapsedTime + _delay)
    {
        _acc -= _delay;

		// clear the window to the border color
		SDL_SetRenderTarget(_renderer, NULL);
		SDL_SetRenderDrawColor(_renderer, 32, 32, 32, 255);	// border color
		SDL_RenderClear(_renderer);

		// do nothing extra anymore
		// video buffer is being updated via asm
		// the 6809 CPU should nowbe working

		// // palette test
		// write(DSP_PAL_IDX, 1);
		// Word clr = read_word(DSP_PAL_CLR)+1;
		// write(DSP_PAL_CLR, clr);


		// update the display textures
		_display_extended();
		_display_standard();
	}

	// run OnUpdate() for the other graphics devices
	for (auto& d : _gfx_devices)
		d->OnUpdate(fElapsedTime);
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

	// run OnRender() for the other graphics devices
	for (auto& d : _gfx_devices)
		d->OnRender();
}

void Gfx::OnPresent() 
{
	// swap display buffers / present
	SDL_RenderPresent(_renderer);  	
}






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
	auto get_div = [](Byte bpp) {
		switch (bpp)
		{
			case 1: return 8;
			case 2: return 4;
			case 4: return 2;
			case 8: return 1;
		}	
		return 8;
	};
	float real_width = _base_texture_width * (5-_h_scan);
	float real_height = (_base_texture_width / _aspect) * (5-_v_scan);	
	float div = get_div(_std_bpp);
	float req_buffer_size = (real_width * real_height) / div;


	// text mode
	Byte tm = bus.read(DSP_EXT) & 0b00000100;
	if (tm == 0)	
	{
		req_buffer_size = ((real_width/8) * (real_height/8)) * 2;
		_std_bpp = 1;
	}
	else
	{
		// adjust for the bitmap mode
		if (req_buffer_size > std_buffer_size)
			printf ("    ERROR: Buffer Overrun... Making Adjustments\n");
		while (req_buffer_size > std_buffer_size)
		{		
			_std_bpp >>= 1;					// _std_bpp changed
			if (_std_bpp == 0) break;
			printf("    -->_std_bpp: %d\n", _std_bpp);
			div = get_div(_std_bpp);
			req_buffer_size = (real_width * real_height) / div;
			// ENCODE THE CHANGES (_std_bpp: XX00.0000)
			Byte d = data;
			d &= 0x3f;
			d |= ((_std_bpp)-1) << 6;
			bus.write(DSP_GRES, d, true);
			// set the error bit
			d = bus.read(DSP_ERR) | 0x80;
			bus.write(DSP_ERR, d);
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
				div = get_div(_std_bpp);
				req_buffer_size = (real_width * real_height) / div;	
				// ENCODE THE CHANGES (_v_scan: 0000.00XX)
				Byte d = data;
				d &= 0xFC;
				d |= ((_v_scan-1) & 0x03);
				bus.write(DSP_GRES, d, true);			
			}
		}
	}	

	// SDL Texture Size
	_texture_width = (int)real_width;
	_texture_height = (int)real_height;

	// [STD_VID_MAX] video buffer end
	_std_vid_max = (SCREEN_BUFFER + (int)req_buffer_size)-1;

	// output debugging text
	printf("DSP_GRES decoded:\n");
	printf("  Aspect Ratio: %f\n", _aspect);
	printf("  Horizontal Overscan: %dx\n", _h_scan);
	printf("  Vertical Overscan: %dx\n", _v_scan);
	printf("  Real Width: %3.2f\n", real_width);
	printf("  Real Height: %3.2f\n", real_height);
	printf("  Standard Graphics BPP: %d\n", _std_bpp);
	printf("  Buffer Size: %3.2fK\n", req_buffer_size / 1024.0f);	
	printf("  Buffer Top: $%04X\n", read_word(STD_VID_MAX));
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
	printf("  Standard Display Mode: %d ", _standard_display_mode);
	(_extended_display_mode) ? printf("(BITMAP)\n") : printf("(TEXT)\n");
	printf("  Vertical Sync: %d ", _vsync);
	(_vsync) ? printf("(ON)\n") : printf("(OFF)\n");
	printf("  Emulation Mode: %d ", _windowed);
	(_windowed) ? printf("(WINDOWED)\n") : printf("(FULLSCREEN)\n");
}


// helpers
void Gfx::_display_standard()
{
	// there is no palette during the first cycle
	if (_palette.size()==0)
		return;

	if (_standard_graphics_enable)
	{
		// graphics mode
		if (_standard_display_mode)
		{
			Bus& bus = Bus::Inst();				
			Word pixel_index = SCREEN_BUFFER;
			void *pixels;
			int pitch;
			if (SDL_LockTexture(_std_texture, NULL, &pixels, &pitch) < 0)
				Bus::Error("Failed to lock texture: ");	
			else
			{
				for (int y = 0; y < _texture_height; y++)
				{
					for (int x = 0; x < _texture_width; )
					{
						// 256 color mode
						if (_std_bpp == 8)
						{
							Byte index = bus.read(pixel_index++);
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
						}
						// 16 color mode
						else if (_std_bpp == 4)
						{
							Byte data = bus.read(pixel_index++);
							Byte index = (data >> 4);
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data & 0x0f);
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
						}
						// 4 color mode
						else if (_std_bpp == 2)
						{
							Byte data = bus.read(pixel_index++);
							Byte index = (data >> 6) & 0x03;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data >> 4) & 0x03;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data >> 2) & 0x03;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data >> 0) & 0x03;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
						}
						// 2 color mode
						else if (_std_bpp == 1)
						{
							Byte data = bus.read(pixel_index++);
							Byte index = (data >> 7) & 1;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true); 
							index = (data >> 6) & 1;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data >> 5) & 1;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data >> 4) & 1;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data >> 3) & 1;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data >> 2) & 1;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data >> 1) & 1;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data >> 0) & 1;
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
						}
					}
				}
				SDL_UnlockTexture(_std_texture); 
			}
		} // end: graphics mode
		else
		{ // text mode
			_updateTextScreen();
		} // end: text mode
		SDL_SetRenderTarget(_renderer, _render_target);
		SDL_RenderCopy(_renderer, _std_texture, NULL, NULL);		
	}
}

void Gfx::_updateTextScreen() 
{
	Bus& bus = Bus::Inst();
    void *pixels;
    int pitch;
    if (SDL_LockTexture(_std_texture, NULL, &pixels, &pitch) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
        Bus::Error("");
    }
    else
    {
		Word end = bus.read_word(STD_VID_MAX);
		Word addr = SCREEN_BUFFER;
		for (  ; addr <= end; addr+=2)
		{
			Byte ch = bus.read(addr, true);
			Byte at = bus.read(addr+1, true);
			Byte fg = at >> 4;
			Byte bg = at & 0x0f;
			Word index = addr - SCREEN_BUFFER;
			Byte width = _texture_width / 8;
			int x = ((index / 2) % width) * 8;
			int y = ((index / 2) / width) * 8;			
			for (int v=0; v<8; v++)
			{    
				for (int h=0; h<8; h++)
				{
					int color = bg;
					if (_dsp_glyph_data[ch][v] & (1 << 7-h))
						color = fg;
					_setPixel_unlocked(pixels, pitch, x+h, y+v, color);   
				}
			}
		}

        SDL_UnlockTexture(_std_texture); 
    }
} 


void Gfx::_display_extended()
{
	// STILL JUST TESTING, NEED PALETTE COLORS
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
		// clear the display to solid color
		SDL_SetRenderTarget(_renderer, _render_target);
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);
	}
}

void Gfx::_setPixel(int x, int y, Byte color_index, 
						SDL_Texture* _texture, bool bIgnoreAlpha)
{
    void *pixels;
    int pitch;
    Uint8 clr_index = 0;    
    if (SDL_LockTexture(_texture, NULL, &pixels, &pitch) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
        Bus::Error("");
    }
    else
    {
        _setPixel_unlocked(pixels, pitch, x, y, color_index, bIgnoreAlpha);
        SDL_UnlockTexture(_texture);
    }    
}

void Gfx::_setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte color_index, bool bIgnoreAlpha)
{
    Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));		// because data size is two bytes 
    bool ALPHA_BLEND = true;
    if (ALPHA_BLEND)
    {       
        // int ret = ((p1 * (256-a))) + (p2 * (a+1)) >> 8;
        Uint16 pixel = *dst;	// 0xARGB
		Byte r1 = (pixel & 0x0f00) >> 8;
		Byte g1 = (pixel & 0x00f0) >> 4;
		Byte b1 = (pixel & 0x000f) >> 0;
		//
        Byte a2 = alf(color_index);
        Byte r2 = red(color_index);
        Byte g2 = grn(color_index);
        Byte b2 = blu(color_index);
        if (bIgnoreAlpha)
            a2 = 15;
		//
        Byte r = ((r1 * (16-a2))) + (r2 * (a2+1)) >> 4;
        Byte g = ((g1 * (16-a2))) + (g2 * (a2+1)) >> 4;
        Byte b = ((b1 * (16-a2))) + (b2 * (a2+1)) >> 4;

        if (alf(color_index) != 0 || bIgnoreAlpha)
        {
            *dst = (
                0xF000 | 
                (r<<8) | 
                (g<<4) | 
                (b)
            );          
		}	
    }
    else
    {        
        // simple non-zero alpha channel
        if (alf(color_index) != 0 || bIgnoreAlpha)
        {
            *dst = 
            (
                0xF000 |
                (red(color_index)<<8) |
                (grn(color_index)<<4) |
                blu(color_index)
            );    
        }
    }    
}
