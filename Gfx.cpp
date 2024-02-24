/////
// Gdx.cpp
// 
////////

#include <iostream>

#include "Bus.h"
#include "C6809.h"
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
	// default data value
	Byte data = 0xCC;
	// handle screen memory
	if (offset >= STD_VID_MIN && offset <= _std_vid_max)
	{
		if (_standard_display_mode)
			return m_gfx_bitmap->read(offset, debug);
		else
			return m_gfx_text->read(offset, debug);
	}
	// handle Gfx Reads
	switch (offset)
	{
		case STD_VID_MAX:		data = _std_vid_max >> 8; break;
		case STD_VID_MAX+1:		data = _std_vid_max & 0xFF; break;

		case SYS_STATE: {
			// Byte err = bus._sys_state & 0xF0;
			Byte err = Bus::_sys_state & 0xF0;
			Bus::_sys_state &= 0x0F;
			data = Bus::_sys_state | err; 
			break;
		}
		case SYS_SPEED + 0:		data = Bus::_sys_cpu_speed >> 8; break;
		case SYS_SPEED + 1:		data = Bus::_sys_cpu_speed & 0xFF; break;
		case SYS_CLOCK_DIV:		data = Bus::_clock_div; break;
		case SYS_TIMER + 0:		data = Bus::_clock_timer >> 8; break;
		case SYS_TIMER + 1:		data = Bus::_clock_timer & 0xff; break;

		case EMU_FLAGS:			data = _emu_flags; break;
		case DSP_RES:			data = _dsp_res; break;
		case DSP_MODE:			data = _dsp_mode; break;

//		case DSP_GRES: 			data = _dsp_gres; break;	// DEPRICATED
//		case DSP_EXT:			data = _dsp_ext; break;		// DEPRICATED


		// case DSP_ERR:			{ Byte err = _dsp_err;	_dsp_err = 0;	data = err; break;} 
		case DSP_TXT_COLS:      data = _texture_width / 8; break;
		case DSP_TXT_ROWS:      data = _texture_height / 8; break;

        // color palete registers
		case DSP_PAL_IDX:   	data = _dsp_pal_idx; break;
		case DSP_PAL_CLR + 0: 	data = (_palette[_dsp_pal_idx].color >> 8) & 0xFF; break;
		case DSP_PAL_CLR + 1: 	data = _palette[_dsp_pal_idx].color & 0xFF; break;

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

	//bus.write(offset, data, true);
	Bus::Write(offset, data, true);
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
	// dispatch video memory writes
	if (offset >= STD_VID_MIN && offset <= _std_vid_max)
	{
		if (_standard_display_mode)
			m_gfx_bitmap->write(offset, data, debug);
		else
			m_gfx_text->write(offset, data, debug);
	}

	// handle Gfx Writes
	switch (offset)
	{
		case SYS_STATE: { 
			Bus::_sys_state = data;
			break;
		}
		case SYS_TIMER + 0: {
			Bus::_clock_timer = (data << 8) | (Bus::_clock_timer & 0x0f);
			break;
		}
		case SYS_TIMER + 1: {
			Bus::_clock_timer = (data << 0) | (Bus::_clock_timer & 0xf0);
			break;
		}
		case EMU_FLAGS: {
			_emu_flags = data; 
			_decode_display();
			Bus::IsDirty(true);	
			break;
		}
		case DSP_RES: {
			_dsp_res = data;
			_decode_display();
			Bus::IsDirty(true);		
			break;
		}
		case DSP_MODE: {
			_dsp_mode = data;
			_decode_display();
			Bus::IsDirty(true);	
			break;
		}
//														// DEPRECATED
//															case DSP_GRES: 	{
//																_dsp_gres = data;
//																_decode_dsp_gres();
//																Bus::IsDirty(true);		
//																break;
//															}
//															case DSP_EXT:	{
//																_dsp_ext = data;
//																_decode_dsp_ext();
//																Bus::IsDirty(true);
//																break;
//															}
//														// END DEPRECATED

		// color palete registers
		case DSP_PAL_IDX:	{ _dsp_pal_idx = data;  break; }
		case DSP_PAL_CLR+1:	{
			Word c = _palette[_dsp_pal_idx].color & 0xff00;
			_palette[_dsp_pal_idx].color = c | data;
			data |= _palette[_dsp_pal_idx].color;
			Bus::IsDirty(true);
			break;
		}
		case DSP_PAL_CLR+0:	{
			Word c = _palette[_dsp_pal_idx].color & 0x00ff;
			_palette[_dsp_pal_idx].color = c | ((Word)data << 8);
			data = _palette[_dsp_pal_idx].color;
			Bus::IsDirty(true);
			break;
		}
        // text glyph definition data registers
        case DSP_GLYPH_IDX: _dsp_glyph_idx = data; break;
        case DSP_GLYPH_DATA+0:  _dsp_glyph_data[_dsp_glyph_idx][0] = data; break;
        case DSP_GLYPH_DATA+1:  _dsp_glyph_data[_dsp_glyph_idx][1] = data; break;
        case DSP_GLYPH_DATA+2:  _dsp_glyph_data[_dsp_glyph_idx][2] = data; break;
        case DSP_GLYPH_DATA+3:  _dsp_glyph_data[_dsp_glyph_idx][3] = data; break;
        case DSP_GLYPH_DATA+4:  _dsp_glyph_data[_dsp_glyph_idx][4] = data; break;
        case DSP_GLYPH_DATA+5:  _dsp_glyph_data[_dsp_glyph_idx][5] = data; break;
        case DSP_GLYPH_DATA+6:  _dsp_glyph_data[_dsp_glyph_idx][6] = data; break;
        case DSP_GLYPH_DATA+7:  _dsp_glyph_data[_dsp_glyph_idx][7] = data; break; 		
	
	}
	Bus::Write(offset, data, true);
}


Word Gfx::OnAttach(Word nextAddr) 
{ 
    int size = 0;
    Word old_addr = nextAddr;

	// begin graphics device register allocation

	// const Word VID_BUFFER_SIZE = 6 * 1024;  // moved to types.h
	// DisplayEnum("", 0, "");
	DisplayEnum("STD_VID_MIN", nextAddr, "Start of Standard Video Buffer Memory");
	nextAddr += VID_BUFFER_SIZE;

	DisplayEnum("STD_VID_MAX", nextAddr, " (Word) Standard Video Buffer Max");
    nextAddr+=2;

	// DisplayEnum("", 0, "");
	DisplayEnum("SYS_STATE", nextAddr, " (Byte) System State Register");
	DisplayEnum("", 0, "SYS_STATE: ABCD.SSSS");
	DisplayEnum("", 0, "     A:0   = Error: Standard Buffer Overflow ");
	DisplayEnum("", 0, "     B:0   = Error: Extended Buffer Overflow ");
	DisplayEnum("", 0, "     C:0   = Error: Reserved ");
	DisplayEnum("", 0, "     D:0   = Error: Reserved ");
	DisplayEnum("", 0, "     S:$0  = CPU Clock  25 khz.");
	DisplayEnum("", 0, "     S:$1  = CPU Clock  50 khz.");
	DisplayEnum("", 0, "     S:$2  = CPU Clock 100 khz.");
	DisplayEnum("", 0, "     S:$3  = CPU Clock 200 khz.");
	DisplayEnum("", 0, "     S:$4  = CPU Clock 333 khz.");
	DisplayEnum("", 0, "     S:$5  = CPU Clock 416 khz.");
	DisplayEnum("", 0, "     S:$6  = CPU Clock 500 khz.");
	DisplayEnum("", 0, "     S:$7  = CPU Clock 625 khz.");
	DisplayEnum("", 0, "     S:$8  = CPU Clock 769 khz.");
	DisplayEnum("", 0, "     S:$9  = CPU Clock 833 khz.");
	DisplayEnum("", 0, "     S:$A  = CPU Clock 1.0 mhz.");
	DisplayEnum("", 0, "     S:$B  = CPU Clock 1.4 mhz.");
	DisplayEnum("", 0, "     S:$C  = CPU Clock 2.0 mhz.");
	DisplayEnum("", 0, "     S:$D  = CPU Clock 3.3 mhz.");
	DisplayEnum("", 0, "     S:$E  = CPU Clock 5.0 mhz.");
	DisplayEnum("", 0, "     S:$F  = CPU Clock ~10.0 mhz. (unmetered)");
	nextAddr++;

	DisplayEnum("", 0, "");
	DisplayEnum("SYS_SPEED", nextAddr, " (Word) Approx. Average CPU Clock Speed");
	nextAddr+=2;

	// DisplayEnum("", 0, "");
	DisplayEnum("SYS_CLOCK_DIV", nextAddr, " (Byte) 60 hz Clock Divider Register (Read Only) ");
	DisplayEnum("", 0, "SYS_CLOCK_DIV:");
	DisplayEnum("", 0, "     bit 7: 0.46875 hz");
	DisplayEnum("", 0, "     bit 6: 0.9375 hz");
	DisplayEnum("", 0, "     bit 5: 1.875 hz");
	DisplayEnum("", 0, "     bit 4: 3.75 hz");
	DisplayEnum("", 0, "     bit 3: 7.5 hz");
	DisplayEnum("", 0, "     bit 2: 15.0 hz");
	DisplayEnum("", 0, "     bit 1: 30.0 hz");
	DisplayEnum("", 0, "     bit 0: 60.0 hz");
	nextAddr++;

	DisplayEnum("", 0, "");
	DisplayEnum("SYS_TIMER", nextAddr, " (Word) Increments at 0.46875 hz");

	nextAddr += 2;
	DisplayEnum("EMU_FLAGS", nextAddr, " (Byte) Emulation Flags ");
	DisplayEnum("", 0, "EMU_FLAGS: xxxx.xxFG");
	DisplayEnum("", 0, "     x     = reserved ");
	DisplayEnum("", 0, "     F:0   = VSYNC OFF ");
	DisplayEnum("", 0, "     F:1   = VSYNC ON ");
	DisplayEnum("", 0, "     G:0   = Fullscreen Enabled ( emulator only ) ");
	DisplayEnum("", 0, "     G:1   = Windowed Enabled ( emulator only ) ");

	nextAddr++;

    DisplayEnum("", 0, "");
    DisplayEnum("DSP_RES", nextAddr, " (Byte) Display Resolution Timing 0-255");
	nextAddr++;

    // DisplayEnum("", 0, "");
    DisplayEnum("DSP_MODE", nextAddr, " (Byte) Graphics Display Mode");
    DisplayEnum("", 0, "DSP_MODE: ABCD.EEFF");
    DisplayEnum("", 0, "     A:0   = Extended Bitmap Mode ");
    DisplayEnum("", 0, "     A:1   = Extended Tile Mode ");
    DisplayEnum("", 0, "     B:1   = Standard Bitmap Mode ");
    DisplayEnum("", 0, "     B:0   = Standard Text Mode ");
    DisplayEnum("", 0, "     C:1   = Extended Graphics Enabled ");
    DisplayEnum("", 0, "     C:0   = Extended Graphics Disabled ");
    DisplayEnum("", 0, "     D:1   = Standard Graphics Enabled ");
    DisplayEnum("", 0, "     D:0   = Standard Graphics Disabled ");
    DisplayEnum("", 0, "    EE:00  = Extended Graphics   2-Color ");
    DisplayEnum("", 0, "    EE:01  = Extended Graphics   4-Color ");
    DisplayEnum("", 0, "    EE:10  = Extended Graphics  16-Color ");
    DisplayEnum("", 0, "    EE:11  = Extended Graphics 256-Color ");
    DisplayEnum("", 0, "    FF:00  = Standard Graphics   2-Color ");
    DisplayEnum("", 0, "    FF:01  = Standard Graphics   4-Color ");
    DisplayEnum("", 0, "    FF:10  = Standard Graphics  16-Color ");
    DisplayEnum("", 0, "    FF:11  = Standard Graphics 256-Color ");
    nextAddr++;

//		// BEGIN DEPRECIATED REGISTERS...
//			DisplayEnum("", 0, "");
//			DisplayEnum("DSP_GRES", nextAddr, " (Byte) Screen Resolution Register (DEPRECATED)");
//			DisplayEnum("", 0, "DSP_GRES: BBRR.HHVV");
//			DisplayEnum("", 0, "    BB:00 = Standard Graphics 1-bpp (2-color mode)");
//			DisplayEnum("", 0, "    BB:01 = Standard Graphics 2-bpp (4-color mode)");
//			DisplayEnum("", 0, "    BB:10 = Standard Graphics 4-bpp (16-color mode)");
//			DisplayEnum("", 0, "    BB:11 = Standard Graphics 8-bpp (256-color mode)");    
//			DisplayEnum("", 0, "    RR:00 = 16:9  aspect (1.777778)");
//			DisplayEnum("", 0, "    RR:01 = 16:10 aspect (1.600000)");
//			DisplayEnum("", 0, "    RR:10 = 16:11 aspect (1.454545)");
//			DisplayEnum("", 0, "    RR:11 = 16:12 aspect (1.333333)");    
//			DisplayEnum("", 0, "    HH:00 = 4x Horizontal Overscan Multiplier");
//			DisplayEnum("", 0, "    HH:01 = 3x Horizontal Overscan Multiplier");
//			DisplayEnum("", 0, "    HH:10 = 2x Horizontal Overscan Multiplier");
//			DisplayEnum("", 0, "    HH:11 = 1x Horizontal Overscan Multiplier");    
//			DisplayEnum("", 0, "    VV:00 = 4x Vertical Overscan Multiplier");
//			DisplayEnum("", 0, "    VV:01 = 3x Vertical Overscan Multiplier");
//			DisplayEnum("", 0, "    VV:10 = 2x Vertical Overscan Multiplier");
//			DisplayEnum("", 0, "    VV:11 = 1x Vertical Overscan Multiplier");
//			nextAddr++;
//			DisplayEnum("", 0, "");
//			DisplayEnum("DSP_EXT", nextAddr, " (Byte) Extended Graphics Register (DEPRECATED)");
//			DisplayEnum("", 0, "DSP_EXT: AABC.DEFG");
//			DisplayEnum("", 0, "     AA:00 = Extended Graphics 1bpp (2-color mode) ");
//			DisplayEnum("", 0, "     AA:01 = Extended Graphics 2bpp (4-color mode) ");
//			DisplayEnum("", 0, "     AA:10 = Extended Graphics 4bpp (16-color mode) ");
//			DisplayEnum("", 0, "     AA:11 = Extended Graphics 4bpp (16-color mode) ");
//			DisplayEnum("", 0, "     B:0   = Extended Graphics: DISABLED ");
//			DisplayEnum("", 0, "     B:1   = Extended Graphics: ENABLED ");
//			DisplayEnum("", 0, "     C:0   = Extended Extended Mode: BITMAP ");
//			DisplayEnum("", 0, "     C:1   = Extended Extended Mode: TILES ");
//			DisplayEnum("", 0, "     D:0   = Standard Graphics: DISABLED ");
//			DisplayEnum("", 0, "     D:1   = Standard Graphics: ENABLED ");
//			DisplayEnum("", 0, "     E:0   = Standard Display Mode: TEXT ");
//			DisplayEnum("", 0, "     E:1   = Standard Display Mode: BITMAP ");
//			DisplayEnum("", 0, "     F:0   = VSYNC OFF ");
//			DisplayEnum("", 0, "     F:1   = VSYNC ON ");
//			DisplayEnum("", 0, "     G:0   = Fullscreen Enabled( emulator only ) ");
//			DisplayEnum("", 0, "     G:1   = Windowed Enabled ( emulator only ) ");
//			nextAddr++;
//		// ... END DEPRECIATED REGISTERS

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

	// add the extended bitmap registers
	m_gfx_extended = new GfxExtended(this);
	nextAddr += Attach(m_gfx_extended, nextAddr);

	// add the standard bitmap registers
	m_gfx_bitmap = new GfxBitmap(this);
	nextAddr += Attach(m_gfx_bitmap, nextAddr);

	// add the text registers
	m_gfx_text = new GfxText(this);
	nextAddr += Attach(m_gfx_text, nextAddr);

	// add the sprite registers
	m_gfx_sprite = new GfxSprite(this);
	nextAddr += Attach(m_gfx_sprite, nextAddr);

	// add the debug registers
	m_debug = new GfxDebug(this);
	nextAddr += Attach(m_debug, nextAddr);

	// add the mouse registers
	m_mouse = new GfxMouse(this);
	nextAddr += Attach(m_mouse, nextAddr);

	DisplayEnum("GFX_END", nextAddr, "End of GFX Device Registers");

	return nextAddr - old_addr;
}

void Gfx::OnQuit()
{
//	// remove the mouse device
//	if (m_mouse)
//	{
//		delete m_mouse;
//		m_mouse = nullptr;
//	}
//	// clear the _gfx_devices vector
//	_gfx_devices.clear();
//	// run OnQuit() for the other graphics devices
//	for (auto& d : _gfx_devices)
//		d->OnQuit();
// 	

	// remove the extended bitmap device
	if (m_gfx_extended)
	{
		m_gfx_extended->OnQuit();
		delete m_gfx_extended;
		m_gfx_extended = nullptr;
	}
	// remove the standard bitmap device
	if (m_gfx_bitmap)
	{
		m_gfx_bitmap->OnQuit();
		delete m_gfx_bitmap;
		m_gfx_bitmap = nullptr;
	}
	// remove the text device
	if (m_gfx_text)
	{
		m_gfx_text->OnQuit();
		delete m_gfx_text;
		m_gfx_text = nullptr;
	}
	// remove the sprite device
	if (m_gfx_sprite)
	{
		m_gfx_sprite->OnQuit();
		delete m_gfx_sprite;
		m_gfx_sprite = nullptr;
	}
	// remove the mouse device
	if (m_debug)
	{
		m_debug->OnQuit();
		delete m_debug;
		m_debug = nullptr;
	}
	// remove the debug device
	if (m_mouse)
	{
		m_mouse->OnQuit();
		delete m_mouse;
		m_mouse = nullptr;
	}

	// clear the _gfx_devices vector
	_gfx_devices.clear();
}


void Gfx::OnInit() 
{
	// printf("Gfx::OnInit() \n");

	// initialize the timing mode reference vector
	_scr_timing_modes.push_back({ 1600,  900, 0.0f });	// $00
	_scr_timing_modes.push_back({ 1440,  900, 0.0f });	// $01
	_scr_timing_modes.push_back({ 1920, 1080, 0.0f });	// $02
	_scr_timing_modes.push_back({ 1280,  800, 0.0f });	// $03
	_scr_timing_modes.push_back({ 1280,  720, 0.0f });	// $04
	_scr_timing_modes.push_back({ 1024,  768, 0.0f });	// $05
	_scr_timing_modes.push_back({  800,  600, 0.0f });	// $06
	_scr_timing_modes.push_back({  640,  480, 0.0f });	// $07
	for (auto &a : _scr_timing_modes)
		a.aspect = a.width / a.height;

	// initialize the display mode reference vector      std    txt
	_scr_display_modes.push_back({ 0,  800,  450, 2, 1, false, false });		// $00
	_scr_display_modes.push_back({ 0,  800,  450, 2, 2, false, false });		// $01 (duplicate of $00)
	_scr_display_modes.push_back({ 0,  800,  300, 2, 3, false,  true });		// $02
	_scr_display_modes.push_back({ 0,  800,  225, 2, 4, false,  true });		// $03
	_scr_display_modes.push_back({ 0,  800,  180, 2, 5, false,  true });		// $04
	_scr_display_modes.push_back({ 0,  800,  150, 2, 6, false,  true });		// $05
	_scr_display_modes.push_back({ 0,  800,  128, 2, 7, false,  true });		// $06
	_scr_display_modes.push_back({ 0,  800,  112, 2, 8, false,  true });		// $07
	_scr_display_modes.push_back({ 0,  400,  900, 4, 1, false, false });		// $08
	_scr_display_modes.push_back({ 0,  400,  450, 4, 2, false,  true });		// $09
	_scr_display_modes.push_back({ 0,  400,  300, 4, 3, false,  true });		// $0A
	_scr_display_modes.push_back({ 0,  400,  225, 4, 4, false,  true });		// $0B
	_scr_display_modes.push_back({ 0,  400,  180, 4, 5, false,  true });		// $0C
	_scr_display_modes.push_back({ 0,  400,  150, 4, 6,  true,  true });		// $0D
	_scr_display_modes.push_back({ 0,  400,  128, 4, 7,  true,  true });		// $0E
	_scr_display_modes.push_back({ 0,  400,  112, 4, 8,  true,  true });		// $0F
	_scr_display_modes.push_back({ 0,  320,  900, 5, 1, false, false });		// $10
	_scr_display_modes.push_back({ 0,  320,  450, 5, 2, false,  true });		// $11
	_scr_display_modes.push_back({ 0,  320,  300, 5, 3, false,  true });		// $12
	_scr_display_modes.push_back({ 0,  320,  225, 5, 4, false,  true });		// $13
	_scr_display_modes.push_back({ 0,  320,  180, 5, 5,  true,  true });		// $14
	_scr_display_modes.push_back({ 0,  320,  150, 5, 6,  true,  true });		// $15
	_scr_display_modes.push_back({ 0,  320,  128, 5, 7,  true,  true });		// $16
	_scr_display_modes.push_back({ 0,  320,  112, 5, 8,  true,  true });		// $17
	_scr_display_modes.push_back({ 0,  200,  900, 8, 1, false,  true });		// $18
	_scr_display_modes.push_back({ 0,  200,  450, 8, 2, false,  true });		// $19
	_scr_display_modes.push_back({ 0,  200,  300, 8, 3, false,  true });		// $1A
	_scr_display_modes.push_back({ 0,  200,  225, 8, 4, false,  true });		// $1B
	_scr_display_modes.push_back({ 0,  200,  180, 8, 5,  true,  true });		// $1C
	_scr_display_modes.push_back({ 0,  200,  150, 8, 6,  true,  true });		// $1D
	_scr_display_modes.push_back({ 0,  200,  128, 8, 7,  true,  true });		// $1E
	_scr_display_modes.push_back({ 0,  200,  112, 8, 8,  true,  true });		// $1F
	//                  timing index width height PW PH   std    txt	
	_scr_display_modes.push_back({ 1,  720,  450, 2, 1, false, false });		// $20
	_scr_display_modes.push_back({ 1,  720,  450, 2, 2, false, false });		// $21 (duplicate of $20)
	_scr_display_modes.push_back({ 1,  720,  300, 2, 3, false,  true });		// $22
	_scr_display_modes.push_back({ 1,  720,  225, 2, 4, false,  true });		// $23
	_scr_display_modes.push_back({ 1,  720,  180, 2, 5, false,  true });		// $24
	_scr_display_modes.push_back({ 1,  720,  150, 2, 6, false,  true });		// $25
	_scr_display_modes.push_back({ 1,  720,  128, 2, 7, false,  true });		// $26
	_scr_display_modes.push_back({ 1,  720,  112, 2, 8, false,  true });		// $27
	_scr_display_modes.push_back({ 1,  480,  900, 3, 1, false, false });		// $28
	_scr_display_modes.push_back({ 1,  480,  450, 3, 2, false,  true });		// $29
	_scr_display_modes.push_back({ 1,  480,  300, 3, 3, false,  true });		// $2A
	_scr_display_modes.push_back({ 1,  480,  225, 3, 4, false,  true });		// $2B
	_scr_display_modes.push_back({ 1,  480,  180, 3, 5, false,  true });		// $2C
	_scr_display_modes.push_back({ 1,  480,  150, 3, 6, false,  true });		// $2D
	_scr_display_modes.push_back({ 1,  480,  128, 3, 7,  true,  true });		// $2E
	_scr_display_modes.push_back({ 1,  480,  112, 3, 8,  true,  true });		// $2F
	_scr_display_modes.push_back({ 1,  360,  900, 4, 1, false, false });		// $30
	_scr_display_modes.push_back({ 1,  360,  450, 4, 2, false,  true });		// $31
	_scr_display_modes.push_back({ 1,  360,  300, 4, 3, false,  true });		// $32
	_scr_display_modes.push_back({ 1,  360,  225, 4, 4, false,  true });		// $33
	_scr_display_modes.push_back({ 1,  360,  180, 4, 5,  true,  true });		// $34
	_scr_display_modes.push_back({ 1,  360,  150, 4, 6,  true,  true });		// $35
	_scr_display_modes.push_back({ 1,  360,  128, 4, 7,  true,  true });		// $36
	_scr_display_modes.push_back({ 1,  360,  112, 4, 8,  true,  true });		// $37
	_scr_display_modes.push_back({ 1,  288,  900, 5, 1, false,  true });		// $38
	_scr_display_modes.push_back({ 1,  288,  450, 5, 2, false,  true });		// $39
	_scr_display_modes.push_back({ 1,  288,  300, 5, 3, false,  true });		// $3A
	_scr_display_modes.push_back({ 1,  288,  225, 5, 4,  true,  true });		// $3B
	_scr_display_modes.push_back({ 1,  288,  180, 5, 5,  true,  true });		// $3C
	_scr_display_modes.push_back({ 1,  288,  150, 5, 6,  true,  true });		// $3D
	_scr_display_modes.push_back({ 1,  288,  128, 5, 7,  true,  true });		// $3E
	_scr_display_modes.push_back({ 1,  288,  112, 5, 8,  true,  true });		// $3F
	//                  timing index width height PW PH   std    txt	
	_scr_display_modes.push_back({ 2,  800,  525, 2, 2, false, false });		// $40
	_scr_display_modes.push_back({ 2,  800,  350, 2, 3, false, false });		// $41
	_scr_display_modes.push_back({ 2,  800,  210, 2, 5, false,  true });		// $42
	_scr_display_modes.push_back({ 2,  800,  175, 2, 6, false,  true });		// $43
	_scr_display_modes.push_back({ 2,  800,  150, 2, 7, false,  true });		// $44
	_scr_display_modes.push_back({ 2,  800,  105, 2,10, false,  true });		// $45
	_scr_display_modes.push_back({ 2,  800,   75, 2,14,  true,  true });		// $46
	_scr_display_modes.push_back({ 2,  800,   70, 2,15,  true,  true });		// $47
	_scr_display_modes.push_back({ 2,  400,  525, 4, 2, false,  true });		// $48
	_scr_display_modes.push_back({ 2,  400,  350, 4, 3, false,  true });		// $49
	_scr_display_modes.push_back({ 2,  400,  210, 4, 5, false,  true });		// $4A
	_scr_display_modes.push_back({ 2,  400,  175, 4, 6, false,  true });		// $4B
	_scr_display_modes.push_back({ 2,  400,  150, 4, 7,  true,  true });		// $4C
	_scr_display_modes.push_back({ 2,  400,  105, 4,10,  true,  true });		// $4D
	_scr_display_modes.push_back({ 2,  400,   75, 4,14,  true,  true });		// $4E
	_scr_display_modes.push_back({ 2,  400,   70, 4,15,  true,  true });		// $4F
	_scr_display_modes.push_back({ 2,  320,  525, 5, 2, false,  true });		// $50
	_scr_display_modes.push_back({ 2,  320,  350, 5, 3, false,  true });		// $51
	_scr_display_modes.push_back({ 2,  320,  210, 5, 5, false,  true });		// $52
	_scr_display_modes.push_back({ 2,  320,  175, 5, 6,  true,  true });		// $53
	_scr_display_modes.push_back({ 2,  320,  150, 5, 7,  true,  true });		// $54
	_scr_display_modes.push_back({ 2,  320,  105, 5,10,  true,  true });		// $55
	_scr_display_modes.push_back({ 2,  320,   75, 5,14,  true,  true });		// $56
	_scr_display_modes.push_back({ 2,  320,   70, 5,15,  true,  true });		// $57
	_scr_display_modes.push_back({ 2,  200,  525, 8, 2, false,  true });		// $58
	_scr_display_modes.push_back({ 2,  200,  350, 8, 3, false,  true });		// $59
	_scr_display_modes.push_back({ 2,  200,  210, 8, 5,  true,  true });		// $5A
	_scr_display_modes.push_back({ 2,  200,  175, 8, 6,  true,  true });		// $5B
	_scr_display_modes.push_back({ 2,  200,  150, 8, 7,  true,  true });		// $5C
	_scr_display_modes.push_back({ 2,  200,  105, 8,10,  true,  true });		// $5D
	_scr_display_modes.push_back({ 2,  200,   75, 8,14,  true,  true });		// $5E
	_scr_display_modes.push_back({ 2,  200,   70, 8,15,  true,  true });		// $5F
	//                  timing index width height PW PH   std    txt	
	_scr_display_modes.push_back({ 3,  640,  800, 2, 1, false, false });		// $60
	_scr_display_modes.push_back({ 3,  640,  400, 2, 2, false,  true });		// $61
	_scr_display_modes.push_back({ 3,  640,  266, 2, 3, false,  true });		// $62
	_scr_display_modes.push_back({ 3,  640,  200, 2, 4, false,  true });		// $63
	_scr_display_modes.push_back({ 3,  640,  160, 2, 5, false,  true });		// $64
	_scr_display_modes.push_back({ 3,  640,  133, 2, 6, false,  true });		// $65
	_scr_display_modes.push_back({ 3,  640,  114, 2, 7, false,  true });		// $66
	_scr_display_modes.push_back({ 3,  640,  100, 2, 8,  true,  true });		// $67
	_scr_display_modes.push_back({ 3,  320,  800, 4, 1, false,  true });		// $68
	_scr_display_modes.push_back({ 3,  320,  400, 4, 2, false,  true });		// $69
	_scr_display_modes.push_back({ 3,  320,  266, 4, 3, false,  true });		// $6A
	_scr_display_modes.push_back({ 3,  320,  200, 4, 4,  true,  true });		// $6B
	_scr_display_modes.push_back({ 3,  320,  160, 4, 5,  true,  true });		// $6C
	_scr_display_modes.push_back({ 3,  320,  133, 4, 6,  true,  true });		// $6D
	_scr_display_modes.push_back({ 3,  320,  114, 4, 7,  true,  true });		// $6E
	_scr_display_modes.push_back({ 3,  320,  100, 4, 8,  true,  true });		// $6F
	_scr_display_modes.push_back({ 3,  256,  800, 5, 1, false,  true });		// $70
	_scr_display_modes.push_back({ 3,  256,  400, 5, 2, false,  true });		// $71
	_scr_display_modes.push_back({ 3,  256,  266, 5, 3, false,  true });		// $72
	_scr_display_modes.push_back({ 3,  256,  200, 5, 4,  true,  true });		// $73
	_scr_display_modes.push_back({ 3,  256,  160, 5, 5,  true,  true });		// $74
	_scr_display_modes.push_back({ 3,  256,  133, 5, 6,  true,  true });		// $75
	_scr_display_modes.push_back({ 3,  256,  114, 5, 7,  true,  true });		// $76
	_scr_display_modes.push_back({ 3,  256,  100, 5, 8,  true,  true });		// $77
	_scr_display_modes.push_back({ 3,  160,  800, 8, 1, false,  true });		// $78
	_scr_display_modes.push_back({ 3,  160,  400, 8, 2, false,  true });		// $79
	_scr_display_modes.push_back({ 3,  160,  266, 8, 3, false,  true });		// $7A
	_scr_display_modes.push_back({ 3,  160,  200, 8, 4,  true,  true });		// $7B
	_scr_display_modes.push_back({ 3,  160,  160, 8, 5,  true,  true });		// $7C
	_scr_display_modes.push_back({ 3,  160,  133, 8, 6,  true,  true });		// $7D
	_scr_display_modes.push_back({ 3,  160,  114, 8, 7,  true,  true });		// $7E
	_scr_display_modes.push_back({ 3,  160,  100, 8, 8,  true,  true });		// $7F
	//                  timing index width height PW PH   std    txt	
	_scr_display_modes.push_back({ 4,  640,  720, 2, 1, false, false });		// $80
	_scr_display_modes.push_back({ 4,  640,  360, 2, 2, false,  true });		// $81
	_scr_display_modes.push_back({ 4,  640,  240, 2, 3, false,  true });		// $82
	_scr_display_modes.push_back({ 4,  640,  180, 2, 4, false,  true });		// $83
	_scr_display_modes.push_back({ 4,  640,  144, 2, 5, false,  true });		// $84
	_scr_display_modes.push_back({ 4,  640,  120, 2, 6, false,  true });		// $85
	_scr_display_modes.push_back({ 4,  640,  102, 2, 7,  true,  true });		// $86
	_scr_display_modes.push_back({ 4,  640,   90, 2, 8,  true,  true });		// $87
	_scr_display_modes.push_back({ 4,  320,  720, 4, 1, false,  true });		// $88
	_scr_display_modes.push_back({ 4,  320,  360, 4, 2, false,  true });		// $89
	_scr_display_modes.push_back({ 4,  320,  240, 4, 3, false,  true });		// $8A
	_scr_display_modes.push_back({ 4,  320,  180, 4, 4,  true,  true });		// $8B
	_scr_display_modes.push_back({ 4,  320,  144, 4, 5,  true,  true });		// $8C
	_scr_display_modes.push_back({ 4,  320,  120, 4, 6,  true,  true });		// $8D
	_scr_display_modes.push_back({ 4,  320,  102, 4, 7,  true,  true });		// $8E
	_scr_display_modes.push_back({ 4,  320,   90, 4, 8,  true,  true });		// $8F
	_scr_display_modes.push_back({ 4,  256,  720, 5, 1, false,  true });		// $90
	_scr_display_modes.push_back({ 4,  256,  360, 5, 2, false,  true });		// $91
	_scr_display_modes.push_back({ 4,  256,  240, 5, 3,  true,  true });		// $92
	_scr_display_modes.push_back({ 4,  256,  180, 5, 4,  true,  true });		// $93
	_scr_display_modes.push_back({ 4,  256,  144, 5, 5,  true,  true });		// $94
	_scr_display_modes.push_back({ 4,  256,  120, 5, 6,  true,  true });		// $95
	_scr_display_modes.push_back({ 4,  256,  102, 5, 7,  true,  true });		// $96
	_scr_display_modes.push_back({ 4,  256,   90, 5, 8,  true,  true });		// $97
	_scr_display_modes.push_back({ 4,  160,  720, 8, 1, false,  true });		// $98
	_scr_display_modes.push_back({ 4,  160,  360, 8, 2, false,  true });		// $99
	_scr_display_modes.push_back({ 4,  160,  240, 8, 3,  true,  true });		// $9A
	_scr_display_modes.push_back({ 4,  160,  180, 8, 4,  true,  true });		// $9B
	_scr_display_modes.push_back({ 4,  160,  144, 8, 5,  true,  true });		// $9C
	_scr_display_modes.push_back({ 4,  160,  120, 8, 6,  true,  true });		// $9D
	_scr_display_modes.push_back({ 4,  160,  102, 8, 7,  true,  true });		// $9E
	_scr_display_modes.push_back({ 4,  160,   90, 8, 8,  true,  true });		// $9F
	//                  timing index width height PW PH   std    txt	
	_scr_display_modes.push_back({ 5,  512,  768, 2, 1, false, false });		// $A0
	_scr_display_modes.push_back({ 5,  512,  384, 2, 2, false,  true });		// $A1
	_scr_display_modes.push_back({ 5,  512,  256, 2, 3, false,  true });		// $A2
	_scr_display_modes.push_back({ 5,  512,  192, 2, 4, false,  true });		// $A3
	_scr_display_modes.push_back({ 5,  512,  153, 2, 5, false,  true });		// $A4
	_scr_display_modes.push_back({ 5,  512,  128, 2, 6,  true,  true });		// $A5
	_scr_display_modes.push_back({ 5,  512,  109, 2, 7,  true,  true });		// $A6
	_scr_display_modes.push_back({ 5,  512,   96, 2, 8,  true,  true });		// $A7
	_scr_display_modes.push_back({ 5,  340,  768, 3, 1, false,  true });		// $A8		// 341 ?
	_scr_display_modes.push_back({ 5,  340,  384, 3, 2, false,  true });		// $A9		// 341 ?
	_scr_display_modes.push_back({ 5,  340,  256, 3, 3, false,  true });		// $AA		// 341 ?
	_scr_display_modes.push_back({ 5,  340,  192, 3, 4,  true,  true });		// $AB		// 341 ?
	_scr_display_modes.push_back({ 5,  340,  153, 3, 5,  true,  true });		// $AC		// 341 ?
	_scr_display_modes.push_back({ 5,  340,  128, 3, 6,  true,  true });		// $AD		// 341 ?
	_scr_display_modes.push_back({ 5,  340,  109, 3, 7,  true,  true });		// $AE		// 341 ?
	_scr_display_modes.push_back({ 5,  340,   96, 3, 8,  true,  true });		// $AF		// 341 ?
	_scr_display_modes.push_back({ 5,  256,  768, 4, 1, false,  true });		// $B0
	_scr_display_modes.push_back({ 5,  256,  384, 4, 2, false,  true });		// $B1
	_scr_display_modes.push_back({ 5,  256,  256, 4, 3,  true,  true });		// $B2
	_scr_display_modes.push_back({ 5,  256,  192, 4, 4,  true,  true });		// $B3
	_scr_display_modes.push_back({ 5,  256,  153, 4, 5,  true,  true });		// $B4
	_scr_display_modes.push_back({ 5,  256,  128, 4, 6,  true,  true });		// $B5
	_scr_display_modes.push_back({ 5,  256,  109, 4, 7,  true,  true });		// $B6
	_scr_display_modes.push_back({ 5,  256,   96, 4, 8,  true,  true });		// $B7
	_scr_display_modes.push_back({ 5,  128,  768, 5, 1, false,  true });		// $B8
	_scr_display_modes.push_back({ 5,  128,  384, 5, 2, false,  true });		// $B9
	_scr_display_modes.push_back({ 5,  128,  256, 5, 3, false,  true });		// $BA
	_scr_display_modes.push_back({ 5,  128,  192, 5, 4,  true,  true });		// $BB
	_scr_display_modes.push_back({ 5,  128,  153, 5, 5,  true,  true });		// $BC
	_scr_display_modes.push_back({ 5,  128,  128, 5, 6,  true,  true });		// $BD
	_scr_display_modes.push_back({ 5,  128,  109, 5, 7,  true,  true });		// $BE
	_scr_display_modes.push_back({ 5,  128,   96, 5, 8,  true,  true });		// $BF
	//                  timing index width height PW PH   std    txt	
	_scr_display_modes.push_back({ 6,  400,  600, 2, 1, false, false });		// $C0
	_scr_display_modes.push_back({ 6,  400,  300, 2, 2, false,  true });		// $C1
	_scr_display_modes.push_back({ 6,  400,  200, 2, 3, false,  true });		// $C2
	_scr_display_modes.push_back({ 6,  400,  150, 2, 4,  true,  true });		// $C3
	_scr_display_modes.push_back({ 6,  400,  120, 2, 5,  true,  true });		// $C4
	_scr_display_modes.push_back({ 6,  400,  100, 2, 6,  true,  true });		// $C5
	_scr_display_modes.push_back({ 6,  400,   85, 2, 7,  true,  true });		// $C6
	_scr_display_modes.push_back({ 6,  400,   75, 2, 8,  true,  true });		// $C7
	_scr_display_modes.push_back({ 6,  200,  600, 4, 1, false,  true });		// $C8
	_scr_display_modes.push_back({ 6,  200,  300, 4, 2,  true,  true });		// $C9
	_scr_display_modes.push_back({ 6,  200,  200, 4, 3,  true,  true });		// $CA
	_scr_display_modes.push_back({ 6,  200,  150, 4, 4,  true,  true });		// $CB
	_scr_display_modes.push_back({ 6,  200,  120, 4, 5,  true,  true });		// $CC
	_scr_display_modes.push_back({ 6,  200,  100, 4, 6,  true,  true });		// $CD
	_scr_display_modes.push_back({ 6,  200,   85, 4, 7,  true,  true });		// $CE
	_scr_display_modes.push_back({ 6,  200,   75, 4, 8,  true,  true });		// $CF
	_scr_display_modes.push_back({ 6,  160,  600, 5, 1, false,  true });		// $D0
	_scr_display_modes.push_back({ 6,  160,  300, 5, 2, false,  true });		// $D1
	_scr_display_modes.push_back({ 6,  160,  200, 5, 3,  true,  true });		// $D2
	_scr_display_modes.push_back({ 6,  160,  150, 5, 4,  true,  true });		// $D3
	_scr_display_modes.push_back({ 6,  160,  120, 5, 5,  true,  true });		// $D4
	_scr_display_modes.push_back({ 6,  160,  100, 5, 6,  true,  true });		// $D5
	_scr_display_modes.push_back({ 6,  160,   85, 5, 7,  true,  true });		// $D6
	_scr_display_modes.push_back({ 6,  160,   75, 5, 8,  true,  true });		// $D7
	_scr_display_modes.push_back({ 6,  100,  600, 8, 1, false,  true });		// $D8
	_scr_display_modes.push_back({ 6,  100,  300, 8, 2,  true,  true });		// $D9
	_scr_display_modes.push_back({ 6,  100,  200, 8, 3,  true,  true });		// $DA
	_scr_display_modes.push_back({ 6,  100,  150, 8, 4,  true,  true });		// $DB
	_scr_display_modes.push_back({ 6,  100,  120, 8, 5,  true,  true });		// $DC
	_scr_display_modes.push_back({ 6,  100,  100, 8, 6,  true,  true });		// $DD
	_scr_display_modes.push_back({ 6,  100,   85, 8, 7,  true,  true });		// $DE
	_scr_display_modes.push_back({ 6,  100,   75, 8, 8,  true,  true });		// $DF
	//                  timing index width height PW PH   std    txt	
	_scr_display_modes.push_back({ 7,  640,  480, 1, 1, false, false });		// $E0
	_scr_display_modes.push_back({ 7,  640,  240, 1, 2, false,  true });		// $E1
	_scr_display_modes.push_back({ 7,  640,  160, 1, 3, false,  true });		// $E2
	_scr_display_modes.push_back({ 7,  640,  120, 1, 4, false,  true });		// $E3
	_scr_display_modes.push_back({ 7,  640,   96, 1, 5,  true,  true });		// $E4
	_scr_display_modes.push_back({ 7,  640,   80, 1, 6,  true,  true });		// $E5
	_scr_display_modes.push_back({ 7,  640,   68, 1, 7,  true,  true });		// $E6
	_scr_display_modes.push_back({ 7,  640,   60, 1, 8,  true,  true });		// $E7
	_scr_display_modes.push_back({ 7,  320,  480, 2, 1, false,  true });		// $E8
	_scr_display_modes.push_back({ 7,  320,  240, 2, 2, false,  true });		// $E9
	_scr_display_modes.push_back({ 7,  320,  160, 2, 3,  true,  true });		// $EA
	_scr_display_modes.push_back({ 7,  320,  120, 2, 4,  true,  true });		// $EB
	_scr_display_modes.push_back({ 7,  320,   96, 2, 5,  true,  true });		// $EC
	_scr_display_modes.push_back({ 7,  320,   80, 2, 6,  true,  true });		// $ED
	_scr_display_modes.push_back({ 7,  320,   68, 2, 7,  true,  true });		// $EE
	_scr_display_modes.push_back({ 7,  320,   60, 2, 8,  true,  true });		// $EF
	_scr_display_modes.push_back({ 7,  160,  480, 4, 1, false,  true });		// $F0
	_scr_display_modes.push_back({ 7,  160,  240, 4, 2,  true,  true });		// $F1
	_scr_display_modes.push_back({ 7,  160,  160, 4, 3,  true,  true });		// $F2
	_scr_display_modes.push_back({ 7,  160,  120, 4, 4,  true,  true });		// $F3
	_scr_display_modes.push_back({ 7,  160,   96, 4, 5,  true,  true });		// $F4
	_scr_display_modes.push_back({ 7,  160,   80, 4, 6,  true,  true });		// $F5
	_scr_display_modes.push_back({ 7,  160,   68, 4, 7,  true,  true });		// $F6
	_scr_display_modes.push_back({ 7,  160,   60, 4, 8,  true,  true });		// $F7
	_scr_display_modes.push_back({ 7,  128,  480, 5, 1,  true,  true });		// $F8
	_scr_display_modes.push_back({ 7,  128,  240, 5, 2,  true,  true });		// $F9
	_scr_display_modes.push_back({ 7,  128,  160, 5, 3,  true,  true });		// $FA
	_scr_display_modes.push_back({ 7,  128,  120, 5, 4,  true,  true });		// $FB
	_scr_display_modes.push_back({ 7,  128,   96, 5, 5,  true,  true });		// $FC
	_scr_display_modes.push_back({ 7,  128,   80, 5, 6,  true,  true });		// $FD
	_scr_display_modes.push_back({ 7,  128,   68, 5, 7,  true,  true });		// $FE
	_scr_display_modes.push_back({ 7,  128,   60, 5, 8,  true,  true });		// $FF

	// DEBUGGING: display the raw display mode data
		if (false) {
			for (int t=0; t<256; t++) 	{
				printf("I:%2X  W:%4d, H:%4d    ", 
					t, 
					_scr_display_modes[t].res_width, 
					_scr_display_modes[t].res_height
				);
				// how many colors are available in this mode?
				int ext_bpp = 8;
				int std_bpp = 8;
				int size = (_scr_display_modes[t].res_width * _scr_display_modes[t].res_height)/8;
				while (size*ext_bpp>65536)
				{
					if (ext_bpp==8)	ext_bpp=4;
					else if (ext_bpp==4)	ext_bpp=2;
					else if (ext_bpp==2)	ext_bpp=1;
					else if (ext_bpp==1)	ext_bpp=0;
					if (ext_bpp==0)
						break;
				}
				while (size*std_bpp>VID_BUFFER_SIZE)
				{
					if (std_bpp==8)	std_bpp=4;
					else if (std_bpp==4)	std_bpp=2;
					else if (std_bpp==2)	std_bpp=1;
					else if (std_bpp==1)	std_bpp=0;
					if (std_bpp==0)
						break;
				}
				int ext_colors = 1<<ext_bpp;	if (ext_colors==1) ext_colors = 0;
				int std_colors = 1<<std_bpp;	if (std_colors==1) std_colors = 0;
				if (ext_colors>0 && std_colors>0)
					printf("   %3d      %3d", ext_colors, std_colors);
				else if (ext_colors>0)
					printf("   %3d         ", ext_colors);
				else if (std_colors>0)
					printf("            %3d", std_colors);
				// text res
				int col = _scr_display_modes[t].res_width / 8;
				int row = _scr_display_modes[t].res_height / 8;
				if ((col*row)*2 > VID_BUFFER_SIZE)
					printf("\n");
				else
					printf("    %3d x%3d  %4d %3dx\n", col, row, (col*row*2), VID_BUFFER_SIZE/(col*row*2));
			}
		}
	// END DEBUGGING


	// initialize the default color palette
	if (_palette.size() == 0)
	{
		std::vector<PALETTE> ref = {
			{ 0xF000 },		// 0: black
			{ 0xF555 },		// 1: dk gray
			{ 0xF007 },		// 2: dk blue
			{ 0xF600 },		// 3: dk red
			{ 0xF140 },		// 4: dk green
			{ 0xF840 },		// 5: brown
			{ 0xF406 },		// 6: purple          
			{ 0xF046 },		// 7: deep sea           	
			{ 0xF888 },		// 8: lt gray
			{ 0xF22F },		// 9: blue
			{ 0xFd00 },		// A: red
			{ 0xF4F6 },		// B: lt green
			{ 0xFED0 },		// C: yellow
			{ 0xF85b },		// D: Lt Purple
			{ 0xF59f },		// E: lt sky
			{ 0xFFFF },		// F: white

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
    for (int t= STD_VID_MIN; t< STD_VID_MIN + VID_BUFFER_SIZE; t+=2)
    {
        write(t, 32);        
        write(t+1, 0xF0);       
    }    
    // initialize the font glyph buffer
    for (int i=0; i<256; i++)
        for (int r=0; r<8; r++)
            _dsp_glyph_data[i][r] = font8x8_system[i][r];

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

void Gfx::OnActivate() 
{
	// printf("void Gfx::OnActivate() \n");


	// decode the gfx registers
//		// DEPRECATED...
//			_decode_dsp_gres();
//			_decode_dsp_ext();
//		// ...DEPRECATED

	_decode_display();

	// create the window
    _window = SDL_CreateWindow("Retro_6809",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               (int)_window_width,
                               (int)_window_height,
                               _window_flags);		

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

				// on mouse up...
				// Bus::IsDirty(true);		
            }
			break;
		}

        case SDL_KEYUP:
        {
            // toggle fullscreen/windowed 
            if (evnt->key.keysym.sym == SDLK_RETURN)
            {
                if (SDL_GetModState() & KMOD_ALT)
                {
                    Byte data = Bus::Read(EMU_FLAGS);
                    if (_windowed)
                        data &= ~0x01;
                    else
                        data |= 0x01;
                    Bus::Write(EMU_FLAGS, data);
                }
//                if (SDL_GetModState() & KMOD_ALT)
//                {
//                    Byte data = Bus::Read(DSP_EXT);
//                    if (_windowed)
//                        data &= ~0x01;
//                    else
//                        data |= 0x01;
//                    Bus::Write(DSP_EXT, data);
//                }
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
















void Gfx::_decode_display()
{
	// printf("Gfx::_decode_display()\n");
	// if (DEBUG_SCANTYPE_OLD==true)
	// 	return;


	// window measurements
	_window_width  = _scr_timing_modes[_scr_display_modes[_dsp_res].timing_index].width;
	_window_height = _scr_timing_modes[_scr_display_modes[_dsp_res].timing_index].height;
	_aspect = _window_width / _window_height;
	_pixel_w = _scr_display_modes[_dsp_res].pixel_width;
	_pixel_h = _scr_display_modes[_dsp_res].pixel_height;
	
	// _vsync = Vertical Sync
	_vsync = Bus::Read(EMU_FLAGS) & 0b00000010;

	// SDL Renderer Flags
	_renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
	if (_vsync)
		_renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC;

	// _windowed = Emulation Mode: 0:FULLSCREEN or 1:WINDOWED 
	bool _old = _windowed;
	_windowed = Bus::Read(EMU_FLAGS) & 0b00000001;
	if (_windowed != _old)
		m_debug->bIsCursorVisible = false;

	// SDL Window	
	if (_windowed) {
		// _window_height = _window_width / _aspect;
		_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	}
	else
	{
		// Screen Size
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;  
		_window_width = DM.w;
		_window_height = DM.h;
	}
	_texture_width  = _scr_display_modes[_dsp_res].res_width;
	_texture_height = _scr_display_modes[_dsp_res].res_height;

	// bits per pixel
	_ext_bpp = 8;
	_std_bpp = 8;
	int size = (_scr_display_modes[_dsp_res].res_width * _scr_display_modes[_dsp_res].res_height)/8;
	while (size*_ext_bpp>65536)
	{
		if (_ext_bpp==8)	_ext_bpp=4;
		else if (_ext_bpp==4)	_ext_bpp=2;
		else if (_ext_bpp==2)	_ext_bpp=1;
		else if (_ext_bpp==1)	_ext_bpp=0;
		if (_ext_bpp==0)
			break;
	}
	while (size*_std_bpp>VID_BUFFER_SIZE)
	{
		if (_std_bpp==8)	_std_bpp=4;
		else if (_std_bpp==4)	_std_bpp=2;
		else if (_std_bpp==2)	_std_bpp=1;
		else if (_std_bpp==1)	_std_bpp=0;
		if (_std_bpp==0)
			break;
	}
	int ext_colors = 1<<_ext_bpp;	if (ext_colors==1) ext_colors = 0;
	int std_colors = 1<<_std_bpp;	if (std_colors==1) std_colors = 0;
	_pixel_w = _scr_display_modes[_dsp_res].pixel_width;
	_pixel_h = _scr_display_modes[_dsp_res].pixel_height;

	Byte data = Bus::Read(DSP_MODE);
	_extended_display_mode = 	(data & 0b10000000);	// 0:ext bitmap,  1:ext tiles
	_standard_display_mode = 	(data & 0b01000000);	// 0:std bitmap,  1:std text
	_extended_graphics_enable = (data & 0b00100000);	// 1:ext enable
	_standard_graphics_enable = (data & 0b00010000);	// 1:std enable

	// [STD_VID_MAX] video buffer end
	int col = _scr_display_modes[_dsp_res].res_width / 8;
	int row = _scr_display_modes[_dsp_res].res_height / 8;
	_std_vid_max = (STD_VID_MIN + (col*row*2))-1;

	// ...
	if (_windowed)
	{
		if (_window_width < 1280 || _window_width > 1280)
		{
			_window_width = 1280;
			_window_height = _window_width / _aspect;
		}
	}


	printf("_window_width: %f\n", _window_width);
	printf("_window_height: %f\n", _window_height);
	printf("_aspect: %f\n", _aspect);
	printf("_vsync: %d\n", _vsync);
	printf("_windowed: %d\n", _windowed);
	printf("_texture_width: %d\n", _texture_width);
	printf("_texture_height: %d\n", _texture_height);
	printf("Extended Bitmap Colors: %d\n",ext_colors);
	printf("Standard Bitmap Colors: %d\n",std_colors);
	printf("_pixel_w: %d\n", _pixel_w);
	printf("_pixel_h: %d\n", _pixel_h);
	printf("Text: %dx%d\n", col, row);
	printf("EXT DISPLAY MODE: %d\n", _extended_display_mode);
	printf("STD DISPLAY MODE: %d\n", _standard_display_mode);
	printf("EXT GFX ENABLE: %d\n",	 _extended_graphics_enable);
	printf("STD GFX ENABLE: %d\n", 	 _standard_graphics_enable);
	printf("DSP_MODE: 0x%02X\n", Bus::Read(DSP_MODE));
	printf("Windowed: %d\n", _windowed);
}














/******************************************
		// DEPRECATED
			void Gfx::_decode_dsp_gres()
			{
				if (DEBUG_SCANTYPE_OLD==false)
					return;

				// shortcut to the Bus instance and DSP_GRES data
				Byte data = Bus::Read(DSP_GRES);

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
				static Byte s_old_h_scan = _pixel_w;
				_pixel_w = (3-((data >> 2) & 0x03))+1;
				if (s_old_h_scan != _pixel_w)
				{
					s_old_h_scan = _pixel_w;
					//Bus::m_cpu->nmi();
				}

				// _v_scan = Vertical Overscan Multiplier
				static Byte s_old_v_scan = _pixel_h;
				_pixel_h = (3-((data >> 0) & 0x03))+1;
				if (s_old_v_scan != _pixel_h)
				{
					s_old_v_scan = _pixel_h;
					//Bus::m_cpu->nmi();
				}

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
				float real_width = _base_texture_width * (5-_pixel_w);
				float real_height = (_base_texture_width / _aspect) * (5-_pixel_h);	
				float div = get_div(_std_bpp);
				float req_buffer_size = (real_width * real_height) / div;


				// text mode
				Byte tm = Bus::Read(DSP_EXT) & 0b00000100;
				if (tm == 0)	
				{
					req_buffer_size = ((real_width/8) * (real_height/8)) * 2;
					_std_bpp = 1;
				}
				else
				{
					// adjust for the bitmap mode
					if (req_buffer_size > VID_BUFFER_SIZE)
						printf ("    ERROR: Buffer Overrun... Making Adjustments\n");
					while (req_buffer_size > VID_BUFFER_SIZE)
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
						Bus::Write(DSP_GRES, d, true);
						// set the error bit
						d = Bus::Read(SYS_STATE) | 0x80;
						Bus::Write(SYS_STATE, d);
					}

					// bpp error buffer too big
					if (_std_bpp == 0)	
					{
						printf ("    ERROR: Buffer Overrun... Making Adjustments\n");
						_std_bpp = 1;	// back to 2-colors
						// try reducing vertical resolution
						while (req_buffer_size > VID_BUFFER_SIZE)
						{
							_pixel_h++;					// _v_scan changed
							real_height = (_base_texture_width / _aspect) * (5-_pixel_h);	
							printf("    -->real_height: %4.2f\n", real_height);
							div = get_div(_std_bpp);
							req_buffer_size = (real_width * real_height) / div;	
							// ENCODE THE CHANGES (_pixel_h: 0000.00XX)
							Byte d = data;
							d &= 0xFC;
							d |= ((_pixel_h-1) & 0x03);
							Bus::Write(DSP_GRES, d, true);			
						}
					}
				}	

				// SDL Texture Size
				_texture_width = (int)real_width;
				_texture_height = (int)real_height;

				// [STD_VID_MAX] video buffer end
				_std_vid_max = (STD_VID_MIN + (int)req_buffer_size)-1;

				// output debugging text
				if (false)
				{
					printf("----====#####################################################====----\n");
					printf("DSP_GRES decoded:\n");
					printf("  Aspect Ratio: %f\n", _aspect);
					printf("  Horizontal Overscan: %dx\n", _pixel_w);
					printf("  Vertical Overscan: %dx\n", _pixel_h);
					printf("  Real Width: %3.2f\n", real_width);
					printf("  Real Height: %3.2f\n", real_height);
					printf("  Standard Graphics BPP: %d\n", _std_bpp);
					printf("  Buffer Size: %3.2fK\n", req_buffer_size / 1024.0f);	
					printf("  Buffer Top: $%04X\n", read_word(STD_VID_MAX));
				}
			}


			void Gfx::_decode_dsp_ext()
			{
				if (DEBUG_SCANTYPE_OLD==false)
					return;

				// shortcut to the Bus instance and DSP_EXT data
				Byte data = Bus::Read(DSP_EXT);

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
				bool _old = _windowed;
				_windowed = data & 0b00000001;
				if (_windowed != _old)
					m_debug->bIsCursorVisible = false;

				// SDL Window Flags	
				// _window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;  
				_window_width = _base_texture_width * 10;
				if (_windowed)
				{
					// _window_width = 1280;
					_window_height = _window_width / _aspect;
					_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
				}
				else
				{
					// Screen Size
					SDL_DisplayMode DM;
					SDL_GetCurrentDisplayMode(0, &DM);
					_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;  
					_window_width = DM.w;
					_window_height = DM.h;
				}

				// SDL Renderer Flags
				_renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
				if (_vsync)
					_renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC;

				// output debugging text
				if (false)
				{
					printf("DSP_EXT decoded:\n");
					printf("  Window Width: %3.2f\n", _window_width);
					printf("  Window Height: %3.2f\n", _window_height);
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
			}
		// DEPRECATED
 ******************************************/



























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
			Word pixel_index = STD_VID_MIN;
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
							Byte index = Bus::Read(pixel_index++);
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
						}
						// 16 color mode
						else if (_std_bpp == 4)
						{
							Byte data = Bus::Read(pixel_index++);
							Byte index = (data >> 4);
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
							index = (data & 0x0f);
							_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
						}
						// 4 color mode
						else if (_std_bpp == 2)
						{
							Byte data = Bus::Read(pixel_index++);
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
							Byte data = Bus::Read(pixel_index++);
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
    void *pixels;
    int pitch;

    if (SDL_LockTexture(_std_texture, NULL, &pixels, &pitch) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
        Bus::Error("");
    }
    else
    {
		Word end = Bus::Read_Word(STD_VID_MAX);
		Word addr = STD_VID_MIN;
		for (; addr <= end; addr += 2)
		{
			Byte ch = Bus::Read(addr, true);
			Byte at = Bus::Read(addr + 1, true);
			Byte fg = at >> 4;
			Byte bg = at & 0x0f;
			Word index = addr - STD_VID_MIN;
			Byte width = _texture_width / 8;
			int x = ((index / 2) % width) * 8;
			int y = ((index / 2) / width) * 8;
			for (int v = 0; v < 8; v++)
			{
				for (int h = 0; h < 8; h++)
				{
					int color = bg;
					if (_dsp_glyph_data[ch][v] & (1 << 7 - h))
						color = fg;
					_setPixel_unlocked(pixels, pitch, x + h, y + v, color);
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

