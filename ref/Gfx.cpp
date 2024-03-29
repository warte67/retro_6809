/////
// Gfx.cpp
//
// Graphics Device
////////

#include <sstream>

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
        case DSP_EMUFLAGS:      return _dsp_emuflags;
        case DSP_GMODE:         return _dsp_gmode;
        case DSP_TBASE+0:       return _dsp_tbase >> 8;
        case DSP_TBASE+1:       return _dsp_tbase & 0xFF;
        case DSP_TXT_COLS:      return _texture_width / 8;      // read-only
        case DSP_TXT_ROWS:      return _texture_height / 8;     // read-only
        case DSP_TXT_PITCH+0:   return _texture_width >> 8;
        case DSP_TXT_PITCH+1:   return _texture_width & 0xFF;

        case DSP_GBASE:         return (_dsp_gbase >> 8) & 0xFF;
        case DSP_GBASE+1:       return _dsp_gbase & 0xFF;
        case DSP_SCN_WIDTH+0:   return _texture_width >> 8;     // read-only
        case DSP_SCN_WIDTH+1:   return _texture_width & 0xFF;   // read-only
        case DSP_SCN_HEIGHT+0:  return _texture_height >> 8;    // read-only
        case DSP_SCN_HEIGHT+1:  return _texture_height & 0xFF;  // read-only
        case DSP_SCN_PITCH:     return _dsp_gpitch >> 8;
        case DSP_SCN_PITCH+1:   return _dsp_gpitch & 0xFF;     

        case DSP_GADDR+0:   return _dsp_gaddr >> 8;
        case DSP_GADDR+1:   return _dsp_gaddr & 0xFF;
        case DSP_GDATA:     return _gfxDisplayBuffer[_dsp_gaddr];

        // color palete registers
        case DSP_PAL_IDX:   return _dsp_pal_idx;
        case DSP_PAL_CLR+1: return (_palette[_dsp_pal_idx].color >> 8) & 0xFF;
        case DSP_PAL_CLR+0: return _palette[_dsp_pal_idx].color  & 0xFF;

        // text glyph definition data registers
        case DSP_GLYPH_IDX:     return _dsp_glyph_idx;
        case DSP_GLYPH_DATA+0:  return _dsp_glyph_data[_dsp_glyph_idx][0];
        case DSP_GLYPH_DATA+1:  return _dsp_glyph_data[_dsp_glyph_idx][1];
        case DSP_GLYPH_DATA+2:  return _dsp_glyph_data[_dsp_glyph_idx][2];
        case DSP_GLYPH_DATA+3:  return _dsp_glyph_data[_dsp_glyph_idx][3];
        case DSP_GLYPH_DATA+4:  return _dsp_glyph_data[_dsp_glyph_idx][4];
        case DSP_GLYPH_DATA+5:  return _dsp_glyph_data[_dsp_glyph_idx][5];
        case DSP_GLYPH_DATA+6:  return _dsp_glyph_data[_dsp_glyph_idx][6];
        case DSP_GLYPH_DATA+7:  return _dsp_glyph_data[_dsp_glyph_idx][7];
    }

    return 0xCC;
}

void Gfx::write(Word offset, Byte data, bool debug)
{
    // printf("WRITE GFX: $%04X = $%02X\n", offset, data);

    switch (offset)
    {
        case DSP_GMODE:
        {
            if (_dsp_gmode == data)
                return; // no change
            // screen needs to be refreshed
            _dsp_gmode = data;      
            Bus::IsDirty(true);
            break;
        }       

        case DSP_EMUFLAGS:
        {
            if (_dsp_emuflags == data)
                return; // no change            
            // fullscreen / windowed toggle only
            if ((_dsp_emuflags & 0x80) != (data & 0x80))
            {
                if (data & 0x80)
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
            break;
        }
        

        // text base address registers
        case DSP_TBASE+0:
            _dsp_tbase = (_dsp_tbase & 0xff00) | (data << 0);
            return;
        case DSP_TBASE+1:        
            _dsp_tbase = (_dsp_tbase & 0x00ff) | (data << 8);
            return;

        case DSP_TXT_PITCH+0:   
            _dsp_tbase = (_dsp_tbase & 0xff00) | (data << 0);
            return;        
        case DSP_TXT_PITCH+1:   
            _dsp_tbase = (_dsp_tbase & 0x00ff) | (data << 8);
            return;        

        // graphics base address registers
        case DSP_GBASE+0:
            _dsp_gbase = (_dsp_gbase & 0xff00) | (data << 0);
            return;
        case DSP_GBASE+1:        
            _dsp_gbase = (_dsp_gbase & 0x00ff) | (data << 8);
            return;

        case DSP_SCN_PITCH+0:   
            _dsp_gpitch = (_dsp_gpitch & 0xff00) | (data << 0);
            return;        
        case DSP_SCN_PITCH+1:   
            _dsp_gpitch = (_dsp_gpitch & 0x00ff) | (data << 8);
            return;     

        // graphics buffer I/O registers
        case DSP_GADDR+0:
            _dsp_gaddr = (_dsp_gaddr & 0xff00) | (data << 0);
            return;
        case DSP_GADDR+1:        
            _dsp_gaddr = (_dsp_gaddr & 0x00ff) | (data << 8);
            return;
        case DSP_GDATA:
            _gfxDisplayBuffer[_dsp_gaddr] = data;
            return;

        // color palete registers
        case DSP_PAL_IDX:   _dsp_pal_idx = data;  return;
        case DSP_PAL_CLR+1: { 
            Word c = _palette[_dsp_pal_idx].color & 0x00ff;
            _palette[_dsp_pal_idx].color = c | ((Word)data << 8);
            return;         }
        case DSP_PAL_CLR:   {
            Word c = _palette[_dsp_pal_idx].color & 0xff00;
            _palette[_dsp_pal_idx].color = c | data;
            return;         }

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
    DisplayEnum("DSP_EMUFLAGS", nextAddr, " (Byte) Auxillary Emulation Flags");
    DisplayEnum("", 0, "DSP_EMUFLAGS: A000.0000");
    DisplayEnum("", 0, "A=0:WINDOWED 1:FULLSCREEN");
    nextAddr += 1;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_GMODE", nextAddr, " (Byte) Display Mode Register");
    DisplayEnum("", 0, "DSP_GMODE: ABCC.DDEE");
    DisplayEnum("", 0, "A = VSYNC     0:off       1:on");
    DisplayEnum("", 0, "B = gfx_enable 	0:off		1:on");
    DisplayEnum("", 0, "C = bit depth 00:1bpp    01:2bpp  10:4bpp 11:8bpp (text00:mono)");
    DisplayEnum("", 0, "D = h_scan    00:1x      01:2x    10:4x           (text: lsb only)");
    DisplayEnum("", 0, "E = v_scan    00:1x      01:2x    10:4x           (text: lsb only)");
    nextAddr++;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_TBASE", nextAddr, " (Word) Text Glyph Base Address");
    DisplayEnum("", 0, "DSP_TBASE:  ($0400 default)");
    DisplayEnum("", 0, "NOTE: While this can be changed to any valid 16-bit address, ");
    DisplayEnum("", 0, "    care should be taken to ensure the screen remains within ");
    DisplayEnum("", 0, "    the text buffer ($0400-$1800).");
    nextAddr += 2;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_TXT_COLS", nextAddr, " (Byte) READ-ONLY Text Screen Columns");
    nextAddr += 1;
    DisplayEnum("DSP_TXT_ROWS", nextAddr, " (Byte) READ-ONLY Text Screens Rows");
    nextAddr += 1;
    DisplayEnum("DSP_TXT_PITCH", nextAddr, " (Word) Text Screen Pitch");
    nextAddr += 2;
    
    DisplayEnum("", 0, "");
    DisplayEnum("DSP_TXT_PXLOFS", nextAddr, " (Byte) Text Screen Pixel Offset");
    DisplayEnum("", 0, "DSP_TXT_PXLOFS: AAAA.BBBB");
    DisplayEnum("", 0, "A = horizontal pixel offset (signed 4-bit)");
    DisplayEnum("", 0, "B = vertical pixel offset (signed 4-bit)");
    DisplayEnum("", 0, "Smooth scrolling affects only text modes.");
    nextAddr++;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_GBASE", nextAddr, " (Word) Graphics Base Address");
    DisplayEnum("", 0, "DSP_GBASE: Can be used for page swapping ($0000 default)");
    DisplayEnum("", 0, "Note: This is the base address to begin displaying pixel");
    DisplayEnum("", 0, "    graphics information within the external serial QSPI RAM.");
    nextAddr += 2;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_SCN_WIDTH", nextAddr, " (Word) READ-ONLY Screen Pixel Width");
    nextAddr += 2;
    DisplayEnum("DSP_SCN_HEIGHT", nextAddr, " (Word) READ-ONLY Screen Pixel Height");
    nextAddr += 2;
    DisplayEnum("DSP_SCN_PITCH", nextAddr, " (Word) Graphics Screen Pitch");
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
		// ...
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




		// // fill out the remaining entries with random junk for now
		// Word color = 0x0010;
		// while (_palette.size() < 256)
		// {
		// 	PALETTE ent;
		// 	color = rand() % 0x10000;
		// 	//color = 0;
		// 	ent.color = color;
		// 	_palette.push_back(ent);
		// }

        // Temp: Fill the rest of the palette with black
		PALETTE blank { 0 };
		while (_palette.size() < 256)
			_palette.push_back(blank);
	}  

    // clear the graphis buffer
    for (int t=0; t<65536; t++)
        _gfxDisplayBuffer[t] = 0;

    // clear the text buffer to white on black spaces
    for (int t=SCREEN_BUFFER; t<SCREEN_BUFFER+0x1200; t+=2)
    {
        Bus::write(t, 32);        
        Bus::write(t+1, 0xF0);       
    }
    
    // initialize the font glyph buffer
    for (int i=0; i<256; i++)
        for (int r=0; r<8; r++)
            _dsp_glyph_data[i][r] = font8x8_system[i][r];

    // // output a test string
    // std::string sMessage = "Hello World...";
    // Word addr = _dsp_tbase;
    // for (auto &a : sMessage)
    // {
    //     Bus::write(addr, a);
    //     Bus::write(addr+1, 0xF0);		// F: yellow
    //     addr+=2;
    // }
    // Bus::write(_dsp_tbase+3, 0xE4);
    // Bus::write(_dsp_tbase+5, 0xA2);
}
void Gfx::_statusText(void)
{
	static Byte fg = 0x01;
	static Byte bg = 0x01;

    std::string sMessage = "";
	std::ostringstream osMsg;
	osMsg << "W:" << _texture_width;
	osMsg << " H:" << _texture_height;
	// osMsg << " BPP:" << _bpp;

	// new line
	while (osMsg.str().length() < (_texture_width / 8))
		osMsg << " ";
	osMsg << "BPP:" << _bpp;
	// new line
	while (osMsg.str().length() < ((_texture_width / 8)*2))
		osMsg << " ";
	osMsg << "aspect:" << _aspect;		
	// new line
	while (osMsg.str().length() < ((_texture_width / 8)*3))
		osMsg << " ";
	osMsg << "fps:" << Bus::GetInstance()->FPS();		
	// new line
	while (osMsg.str().length() < ((_texture_width / 8)*4))
		osMsg << " ";		

	// osMsg << "text:" << ((_texture_width / 8) * (_texture_height / 8)) * 2 << " bytes";
	osMsg << (_texture_width/8) << "x" << ((float)_texture_height/8.0f) << ":";
	osMsg << ((_texture_width / 8) * (_texture_height / 8)) * 2 << " bytes";


	// new line
	while (osMsg.str().length() < ((_texture_width / 8)*5))
		osMsg << " ";		
	osMsg << "gfx:" << ((_texture_width * _texture_height) / 8) * _bpp << " bytes";

	// new line
	while (osMsg.str().length() < ((_texture_width / 8)*6))
		osMsg << " ";		

		

	// output the string
	sMessage = osMsg.str();
	// clear the text buffer
    for (int t=SCREEN_BUFFER; t<HDW_REGS; t+=2)
    {
        Bus::write(t, 32);        
        Bus::write(t+1, 0xf0);       
    }

	// output the text to the bufffer
    Word addr = _dsp_tbase;
    for (auto &a : sMessage)
    {
        Bus::write(addr, a);
        //Bus::write(addr+1, (color & 0xf0) | 0x02);		// F: yellow
        Bus::write(addr+1, (bg<<4) | fg);       
        addr+=2;
    }	
	static Byte skip=0;
	if (skip++ > 16)
	{
		bg++;
		bg%=16;
		skip = 0;
		if (bg==0)	
		{
			fg++;
			fg%=16;
		}		
	}

	// render one line of transparent text
	std::string sHello = "Hello World!";
    addr = _dsp_tbase + (_texture_width / 4)*7;
	static Byte color = 0xf0;
	if (skip==0)
		color = (rand() % 16) << 4;	//0xf0;
    for (auto &a : sHello)
    {
        Bus::write(addr, a);
        Bus::write(addr+1, color);		// F: yellow
        addr+=2;
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
	{
        _window_flags = SDL_WINDOW_OPENGL | 
                        SDL_WINDOW_RESIZABLE;    
		if (_timing_width == 512)
		{
			_window_width = _timing_width * 2.5;
			_window_height = _timing_height * 2.5;		
		}
	}
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

    // create the background graphics texture	
    // _bg_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888,
	// 		SDL_TEXTUREACCESS_STREAMING, _texture_width, _texture_height);
    _bg_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB4444,
			SDL_TEXTUREACCESS_STREAMING, _texture_width, _texture_height);


    printf("SIZE  W:%d   H:%d\n", _texture_width, _texture_height);            
}

void Gfx::OnDeactivate()
{
    printf("Gfx::OnDeactivate()\n");
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

    // // on deactivate tests 
    // Bus::write_word(DSP_GADDR, 0x1234);
    // printf("read DSP_GADDR: $%04X\n", Bus::read_word(DSP_GADDR));
    // Bus::write(DSP_GDATA, 0x56);
    // printf("read DSP_GDATA: $%02X\n", Bus::read(DSP_GDATA));    
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
                    Byte data = Bus::read(DSP_EMUFLAGS);
                    if (data & 0x80)
                        data &= ~0x80;
                    else
                        data |= 0x80;
                    Bus::write(DSP_EMUFLAGS, data);
                }
            }     
			// TOGGLE VSYNC
			if (evnt->key.keysym.sym == SDLK_BACKSPACE)
			{
				Byte data = Bus::read(DSP_GMODE);
				if (data & 0x80)
				{
					data &= ~0x80;
					Bus::write(DSP_GMODE, data);
				}
				else
				{
					data |= 0x80;
					Bus::write(DSP_GMODE, data);
				}
			}

            // DISPLAY TESTS
			// SPACEBAR TOGGLES EXTENDED GRAPHICS
            if (evnt->key.keysym.sym == SDLK_SPACE)
            {
                Byte data = Bus::read(DSP_GMODE);
				if (data & 0x40)
				{
					data &= ~0x40;
					Bus::write(DSP_GMODE, data);
				}
				else
				{
                    data |= 0x40;
                    Bus::write(DSP_GMODE, data);					
				}
            }
            // number keys change pixel size

            if (evnt->key.keysym.sym >= SDLK_0 && evnt->key.keysym.sym <= SDLK_8)
            // if (evnt->key.keysym.sym >= SDLK_1 && evnt->key.keysym.sym <= SDLK_6)
            // if (evnt->key.keysym.sym >= SDLK_1 && evnt->key.keysym.sym <= SDLK_5)
            {
                Byte key =  evnt->key.keysym.sym - SDLK_0;                
                printf("key: %d\n", key);
                Byte data = Bus::read(DSP_GMODE) & 0xF0;
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

						// case 1:     data |= 0b00000000;      break;
						// case 2:     data |= 0b00000001;      break;
						// case 3:     data |= 0b00000100;      break;
						// case 4:     data |= 0b00000101;      break;
						// case 5:     data |= 0b00001000;      break;
						// case 6:     data |= 0b00001001;      break;

						// case 1:     data |= 0b00000000;      break;		// 1x / 1x
						// case 2:     data |= 0b00000001;      break;		// 1x / 2x
						// case 3:     data |= 0b00000100;      break;		// 2x / 1x
						// case 4:     data |= 0b00000101;      break;		// 2x / 2x
						// case 5:     data |= 0b00001001;      break;		// 4x / 2x
                }
                Bus::write(DSP_GMODE, data);
            }
            // [+] and [-] change bit depth
            if (evnt->key.keysym.sym == SDLK_EQUALS || evnt->key.keysym.sym == SDLK_MINUS)
            {
                // C = bit depth 00:1bpp    01:2bpp  10:4bpp 11:8bpp (text00:mono)
                Byte data = Bus::read(DSP_GMODE);
                Byte b = (data >> 4) & 0x03;
                // PLUS
                if (evnt->key.keysym.sym == SDLK_EQUALS)
                {
                    b++;
                    if (b>3)
                        b=3;
                }
                // MINUS
                if (evnt->key.keysym.sym == SDLK_MINUS)
                {
                    b--;
                    if (b>3)    //unsigned
                        b=0;
                }
                data &= 0b11001111;
                data |= (b << 4);
                Bus::write(DSP_GMODE, data);
            }
			// [ and ] change aspect ratio
			if (evnt->key.keysym.sym == SDLK_LEFTBRACKET || evnt->key.keysym.sym == SDLK_RIGHTBRACKET)
			{
				if (evnt->key.keysym.sym == SDLK_LEFTBRACKET)
				{
					if (_aspect == 1.33333333333f)
						_aspect = 1.77777777778f;						
					else if (_aspect == 1.77777777778f)
						_aspect = 1.6f;
					else if (_aspect == 1.6f)
						_aspect = 1.45454545f;
					else if (_aspect == 1.45454545f)
						_aspect = 1.33333333333f;				}
				else
				{
					if (_aspect == 1.33333333333f)
						_aspect = 1.45454545f;
					else if (_aspect == 1.45454545f)
						_aspect = 1.6f;
					else if (_aspect == 1.6f)
						_aspect = 1.77777777778f;
					else if (_aspect == 1.77777777778f)
						_aspect = 1.33333333333f;
				}

				_timing_height = _timing_width / _aspect;   //384;
				_window_width = _timing_width * 2;
				_window_height = _timing_height * 2;		
				Bus::IsDirty(true);
			}
			if (evnt->key.keysym.sym == SDLK_BACKSLASH)
			{
				if (_timing_width == 512)
					_timing_width = 640;
				else
					_timing_width = 512;

				// if (_timing_width == 640)
				// 	_timing_width = 512;
				// else
				// 	_timing_width = 640;

				_timing_height = _timing_width / _aspect;   //384;
				_window_width = _timing_width * 2;
				_window_height = _timing_height * 2;		
				Bus::IsDirty(true);
			}
        }
        break;
    }
} 

void Gfx::_updateGraphics(float fElapsedTime)
{
	// enabled?
	if (!_dsp_gfx_enable)
		return;

    // stream to the background texture
    Word pixel_index = 0;
    void *pixels;
    int pitch;
    if (SDL_LockTexture(_bg_texture, NULL, &pixels, &pitch) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
        Bus::Error("");
    }
    else
    {
        for (int y = 0; y < _texture_height; y++)
        {
            for (int x = 0; x < _texture_width; )
            {
                // figure out how to use graphics pitch with this algorhythm
                // _dsp_gpitch

                // 256 color mode
                if (_bpp == 8)
                {
                    Byte index = _gfxDisplayBuffer[pixel_index++];
                    _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                }
                // 16 color mode
                else if (_bpp == 4)
                {
                    Byte data = _gfxDisplayBuffer[pixel_index++];
                    Byte index = (data >> 4);
                    _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    index = (data & 0x0f);
                    _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                }
                // 4 color mode
                else if (_bpp == 2)
                {
                    Byte data = _gfxDisplayBuffer[pixel_index++];
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
                else if (_bpp == 1)
                {
                    Byte data = _gfxDisplayBuffer[pixel_index++];
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
        SDL_UnlockTexture(_bg_texture); 
    }
}
void Gfx::_updateTiles(float fElapsedTime) {}

void Gfx::_updateTextScreen(float fElapsedTime) 
{
    void *pixels;
    int pitch;
    if (SDL_LockTexture(_bg_texture, NULL, &pixels, &pitch) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
        Bus::Error("");
    }
    else
    {
        for (int y = 0; y < _texture_height; y+=8)
        {
            for (int x = 0; x < _texture_width; x+=8)
            {
                Word addr = _dsp_tbase + ((y>>3) * _dsp_tpitch) + ((x>>3)<<1);
                Byte ch = Bus::read(addr);
                Byte at = Bus::read(addr+1);
                Byte fg = at >> 4;
                Byte bg = at & 0x0f;
				for (int v=0; v<8; v++)
				{    
					for (int h=0; h<8; h++)
					{
						int index = bg;
						if (_dsp_glyph_data[ch][v] & (1 << 7-h))
							index = fg;
						_setPixel_unlocked(pixels, pitch, x+h, y+v, index);   
					}
				}
            }
        }
        SDL_UnlockTexture(_bg_texture); 
    }
}            


inline void Gfx::_setPixel(int x, int y, Byte color_index, bool bIgnoreAlpha)
{

    void *pixels;
    int pitch;
    Uint8 clr_index = 0;    
    if (SDL_LockTexture(_bg_texture, NULL, &pixels, &pitch) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
        Bus::Error("");
    }
    else
    {
        _setPixel_unlocked(pixels, pitch, x, y, color_index, bIgnoreAlpha);
        SDL_UnlockTexture(_bg_texture);
    }    
}
inline void Gfx::_setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte color_index, bool bIgnoreAlpha)
{
    //Uint32 *dst = (Uint32*)((Uint8*)pixels + (y * pitch) + (x*4));    // why is this (x*4) and not simply x?
    Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));		// because data size is two bytes 
    bool ALPHA_BLEND = true;
    if (ALPHA_BLEND)
    {       
        // int ret = ((p1 * (256-a))) + (p2 * (a+1)) >> 8;
        Uint16 pixel = *dst;	// 0xARGB
		Byte r1 = (pixel & 0x0f00) >> 8;
		Byte g1 = (pixel & 0x00f0) >> 4;
		Byte b1 = (pixel & 0x000f) >> 0;

        Byte a2 = alf(color_index);
        Byte r2 = red(color_index);
        Byte g2 = grn(color_index);
        Byte b2 = blu(color_index);
        if (bIgnoreAlpha)
            a2 = 15;

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

void Gfx::OnUpdate(float fElapsedTime)
{
    // TEMPORARY RANDOM NOISE
    const float _texDelay = 0.01f;
    static float _texAcc = fElapsedTime;
    _texAcc += fElapsedTime;
    if (_texAcc > fElapsedTime + _texDelay)
    {
		static Byte clr = 0;
        _texAcc -= _texDelay;
        for (int t=0; t<65536; t++)
            //_gfxDisplayBuffer[t] = rand() % 256;
            _gfxDisplayBuffer[t] = clr++;
		clr++;      
    }

	// update the text buffer
	_statusText();

    // update the window title bar
    Bus* bus = Bus::GetInstance();
    std::string sTitle = "      (fps:";
    sTitle += std::to_string(bus->FPS());
    sTitle += ")";
    sTitle += "  Retro_68009 - ";
    sTitle += "  W:";
    sTitle += std::to_string(_texture_width);
    sTitle += "  H:";
    sTitle += std::to_string(_texture_height);
    sTitle += "  bpp: ";
    sTitle += std::to_string(_bpp);
    SDL_SetWindowTitle(_window, sTitle.c_str());

    // display the background graphics (OR the tile graphics)
    if (true)
        _updateGraphics(fElapsedTime);
    else
        _updateTiles(fElapsedTime);

    // display text glyphs
    _updateTextScreen(fElapsedTime);
    
}

void Gfx::_decode_gmode()
{
    // VSYNC (A000.0000)
    _vsync = false;
    if (_dsp_gmode & 0b10000000)
        _vsync = true;

    // timing (0B00.0000)
    // _timing_width = 512;
    // _timing_height = _timing_width / _aspect;   //384;
    // if (_dsp_gmode & 0b01000000)
    // {
    //     _timing_width = 640;
    //     _timing_height = _timing_width / _aspect;   //400;
    // }
    // _window_width = _timing_width * 2;
    // _window_height = _timing_height * 2;

	// BYPASS TIMING BIT:
	_timing_width = 512;
	_timing_height = _timing_width / _aspect; 
	float _delta = 2.0f;
	if (!_dsp_emuflags)
		_delta = 2.5f;
    _window_width = _timing_width * _delta;
    _window_height = _timing_height * _delta;


	// timing changed to graphics enable bit (0B00.0000)
	// B = gfx_enable 	0:off	1:on 
	_dsp_gfx_enable = false;
	if (_dsp_gmode & 0b01000000)
		_dsp_gfx_enable = true;
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

    default:     
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

        // To prevent text address overflow and speed problems
        // restrict the 640x400 mode to 640x200 regardless of bpp.  
        // if (_timing_width > 512 && _pixel_width == 1)
        //     _pixel_height = 2;  
        break;
    }

    // texture size
    _texture_width = _timing_width / _pixel_width;
    _texture_height = _timing_height / _pixel_height;

    // text screen pitch default
    _dsp_tpitch = _texture_width >> 2; 

    // graphics pitch default
    _dsp_gpitch = _texture_width;

    // WINDOWED or FULLSCREEN
    _fullscreen = false;
    if (_dsp_emuflags)      // for now:  non-zero = fullscreen
        _fullscreen = true;

    // restrict bits-per-pixel based on memory usage    
    while ((_texture_width * _texture_height) / (8/_bpp) > (64*1024))
        _bpp >>=1;
    int size = (_texture_width * _texture_height) / (8/_bpp);
    _g_top_addr = size;

    // printf("%d-bpp graphics size: $%04X (%d)\n", _bpp, size, size);
    // int num_sprites = (0x10000 - _g_top_addr)/sizeof(Sprite);
    // printf("Room for %d sprites.\n",  num_sprites);
}


void Gfx::OnRender() 
{    
    // clear the window    
    SDL_SetRenderTarget(_renderer, NULL);
	SDL_SetRenderDrawColor(_renderer, 16, 16, 16, 255);	// border color
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
    
    // The Bus object presents after everything renders
    // SDL_RenderPresent(_renderer);    
}




/********************        // if (_timing_width > 512 && _pixel_width == 1)
        //     _pixel_height = 2;  
 
    
     DSP_GMODE = 0x1801,        //  (Byte) Display Mode Register
    // DSP_GMODE: ABCC.DDEE
    // A = VSYNC     0:off      1:on
    // B = gfx_enable 	0:off	1:on 
    // C = bit depth 00:1bpp    01:2bpp  10:4bpp 11:8bpp (text00:mono)
    // D = h_scan    00:1x      01:2x    10:4x           (text: lsb only)
    // E = v_scan    00:1x      01:2x    10:4x           (text: lsb only)
    
	// possibly restrict the vertical scan to 1x and 2x only due to refresh rates on the pico


	ToDo:
		- Select tile or pixel mode	(change B above to select pixel/tile)

		Add Gfx Mode enable flag (in painters algorhythm order):
		0) Pixel/Tile Layer Enable (Gfx Modes)
		1) Text Layer Enable
		2-4) Additional  Text Layers Enable (optional?)
		5) Sprite Layer Enable
		6) Debug Layer Enable
		7) Cursor Layer Enable



*********************/