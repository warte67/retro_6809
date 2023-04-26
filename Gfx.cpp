/////
// Gfx.cpp
// 
// Graphics Device
////////

#include "Gfx.h"


Byte Gfx::read(Word offset, bool debug) 
{
	// if (offset - _base < _size)
	// 	return memory[(Word)(offset - _base)];

    printf("READ GFX: $%04X = ???\n", offset);

	return 0xCC;
}

void Gfx::write(Word offset, Byte data, bool debug) 
{
    // if (debug)
    //     if (offset - _base < _size)
    //         memory[(Word)(offset -_base)] = data;

    printf("WRITE GFX: $%04X = $%02X\n", offset, data);
}


Word Gfx::OnAttach(Word nextAddr) 
{
    int size = 0;
    Word old_addr = nextAddr;

    printf("\n");
    DisplayEnum("DSP_GMODE", nextAddr, " (Byte) Display Mode Register");   
    DisplayEnum("",0,"DSP_GMODE: ABCC.DDEE");
    DisplayEnum("",0,"A = VSYNC     0:off       1:on");
    DisplayEnum("",0,"B = timing    0:512x384   1:640x400");
    DisplayEnum("",0,"C = bit depth 00:1bpp    01:2bpp  10:4bpp 11:8bpp (text00:mono)");
    DisplayEnum("",0,"D = h_scan    00:1x      01:2x    10:4x           (text: lsb only)");
    DisplayEnum("",0,"E = v_scan    00:1x      01:2x    10:4x           (text: lsb only)");
    nextAddr++;

    printf("\n");
    DisplayEnum("DSP_PXLOFS", nextAddr, " (Byte) Display Pixel Offset");   
    DisplayEnum("",0,"DSP_PXLOFS: AAAA.BBBB");
	DisplayEnum("",0,"A = horizontal pixel offset (signed 4-bit)");
	DisplayEnum("",0,"B = vertical pixel offset (signed 4-bit)");
	DisplayEnum("",0,"Smooth scrolling affects only text modes.");
    nextAddr++;

    printf("\n");
    DisplayEnum("DSP_TBASE", nextAddr, " (Word) Text Glyph Base Address");   
    DisplayEnum("",0,"DSP_TBASE:  ($0400 default)");
	DisplayEnum("",0,"NOTE: While this can be changed to any valid 16-bit address, ");
	DisplayEnum("",0,"    care should be taken to ensure the screen remains within ");
	DisplayEnum("",0,"    the text buffer ($0400-$1800).");        
    nextAddr+=2;

    printf("\n");
    DisplayEnum("DSP_GBASE", nextAddr, " (Word) Graphics Base Address");   
    DisplayEnum("",0,"DSP_GBASE: Can be used for page swapping ($0000 default)");
	DisplayEnum("",0,"Note: This is the base address to begin displaying pixel");
	DisplayEnum("",0,"    graphics information within the external serial QSPI RAM.");
    nextAddr+=2;

    printf("\n");
    DisplayEnum("DSP_GADDR", nextAddr, " (Word) Pixel Address");   
    DisplayEnum("",0,"DSP_GADDR: $0000-$FFFF (16-bit address)");
	DisplayEnum("",0,"Note: This is the 16-bit address register for the graphics modes.");
	DisplayEnum("",0,"    Write to set or read to retrieve the current register value.");
    nextAddr+=2;

    printf("\n");
    DisplayEnum("DSP_GDATA", nextAddr, " (Byte) Pixel Data");   
    DisplayEnum("",0,"DSP_GDATA: $00-$FF (8-bit data)");
	DisplayEnum("",0,"Note: Current pixel data stored at where the address register points.");
	DisplayEnum("",0,"    Can be used for both read and write operations.");
    nextAddr+=1;

    printf("\n");
    DisplayEnum("DSP_PAL_IDX", nextAddr, " (Byte) Color Palette Index");   
    DisplayEnum("",0,"DSP_PAL_IDX: 0-255");
    DisplayEnum("",0,"Note: Use this register to set the index into theColor Palette. ");
    DisplayEnum("",0,"  Set this value prior referencing color data (DSP_PAL_CLR).");
    nextAddr+=1;

    printf("\n");
    DisplayEnum("DSP_PAL_CLR", nextAddr, " (Word) Indexed Color Palette Data");   
    DisplayEnum("",0,"DSP_PAL_CLR: Color Data A4R4G4B4 format");
	DisplayEnum("",0,"Note: This is the color data for an individual palette entry.");
	DisplayEnum("",0,"    Write to DSP_PAL_IDX with the index within the color palette");
	DisplayEnum("",0,"    prior to reading or writing the color data in the DSP_PAL_CLR register.");
    nextAddr+=2;

    printf("\n");
    DisplayEnum("DSP_GLYPH_IDX", nextAddr, " (Byte) Text Glyph Index");   
    DisplayEnum("",0,"DSP_GLYPH_IDX: 0-256");
    DisplayEnum("",0,"Note: Set this register to index a specific text glyph. Set this value");
    DisplayEnum("",0,"    prior to updating glyph pixel data.");
    nextAddr+=1;

    printf("\n");
    DisplayEnum("DSP_GLYPH_DATA", nextAddr, " (8-Bytes) Text Glyph Pixel Data Array");   
    DisplayEnum("",0,"DSP_GLYPH_DATA: 8 rows of binary encoded glyph pixel data");
    DisplayEnum("",0,"Note: Each 8x8 text glyph is composed of 8 bytes. The first byte in this ");
    DisplayEnum("",0,"    array represents the top line of 8 pixels. Each array entry represents");
    DisplayEnum("",0,"    a row of 8 pixels. ");
    nextAddr+=8;

    printf("\n");
    DisplayEnum("DSP_EMUFLAGS", nextAddr, " (Byte) Auxillary Emulation Flags");   
    DisplayEnum("",0,"DSP_EMUFLAGS: 0000.000A");
    DisplayEnum("",0,"A=0:WINDOWED 1:FULLSCREEN");
    nextAddr+=1;
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
void Gfx::OnActivate() {}
void Gfx::OnDeactivate() {}
void Gfx::OnEvent(SDL_Event* evnt) {}
void Gfx::OnUpdate(float fElapsedTime) {}
void Gfx::OnRender() {}


