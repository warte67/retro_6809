//////
//
// retro_6809 main.cpp
//
// Notes:  remember to open an extra list window use
//       chord:  CTRL-K then O
//
//	Build: 	make clean
//			make
//		
//		cd asm	
//		./build.sh kernel_f000
// 
//			or
// 
//		asm kernel_f000 (on Windows with the asm.bat file)
//
////////////////////

#include <iostream>
#include "Bus.h"

int main(int argc, char* argv[])
{	
	// clear screen using shell. (Yeah, I know. Whatever!)
	if (false)	
	{
		#ifdef __linux__		// LINUX
			system("clear");
		#endif
		//	#ifdef _WIN32		// WINDOWS
		//		system("cls");
		//	#endif
		// __unix__		// Unix (Linux, *BSD, but not Mac OS X)
		// __APPLE__	// Mac OS
		// __FreeBSD__
		// __ANDROID__
	} // END IF clear screen

	// std::cout << "Retro_6809\n\n";

	Bus& bus = Bus::Inst();
	bus.Run();

	return 0;
}


/**** To Do: **************************************************

	1) The _updateTextScreen() function in Gfx.cpp is proving 
		to be too slow using the _setPixel_unlocked() method. 
		This means that graphics rendering in the higher resolutions
		will also be very slow. Text modes should be able to be
		sped up using 8x8 textures for the glyphs and render to
		texture for text screen updates. However, the graphics
		modes will still have to be drawn on a per-pixel basis.

		Revamp the _updateTextScreen() function to use glyph 
		textures.

		- remove _updateTextScreen() and _display_standard() from 
			the Gfx device.
		- replace with new GfxDevices with standard text/bitmap
			and extended bitmap/tile modes

		- build the bitmap modes using scanline (or multiple
			scanline) textures to speed up framerates during 
			updates. I.E. texuture size: texture width x 8 pixel 
			high. Update textures only when they change and only
			at a maximum rate.

	2) Remove the EDT_BUFFER from the Keyboard object. Instead 
		of limiting the keyboard line input buffer to 64 bytes
		in hardware register space, allow for a 255 byte line 
		input buffer in the FIO_BUFFER ($100-$1ff) space.

		- Remeber EDIT_BUFFER_SIZE is defined in types.h

	3) Add command line history (remember the limited PI Pico
		memory constraints). This could actually be event
		driven and use the second Pi PICO's memory.

	4) Continue developing the FileIO object command options.
		- Revise the error bits (ABCD) of the SYS_STATE register.

	5) Build the memory management system. Include the paged RAM 
		and ROM systems. Consider a dynamic memory management
		system using the paged RAM and kernel function calls. 
		This should emulate the use of an external UART or SPI
		based static RAM buffer.

			- SRAM 2Mb 3V 45ns 256K x 8 LP SRAM https://www.mouser.com/ProductDetail/Infineon-Technologies/CY62138FV30LL-45ZAXI?qs=7NE86GkllXdEdjoNobHX4Q%3D%3D
				or
			- SRAM 4Mb 55ns 512K x 8 Low Power SRAM https://www.mouser.com/ProductDetail/Infineon-Technologies/CY62148ESL-55ZAXI?qs=DX9BD6oIz0YmniYlJcSIDQ%3D%3D

			with a series of Serial to Parallel converters:
				- Serial to Parallel Logic Converters Auto Cat Ser Par Interface https://www.mouser.com/ProductDetail/Texas-Instruments/SN74LV8153QPWRQ1?qs=UG%2F8xqv%2F6WdLzHWeyc%2FUXQ%3D%3D

		or SPI based Static RAMS
			- 64KB (512k bits): 23LC512-I/ST https://www.mouser.com/ProductDetail/Microchip-Technology/23LC512-I-ST?qs=zOnbk8%2FsRx5lbrJpAKhheg%3D%3D
				8 x 8k pages
			- 128KB (1024k bits): 23LC1024-I/ST ($2.60 each) https://www.mouser.com/ProductDetail/Microchip-Technology/23LC1024-I-ST?qs=q1LPnGnpSmbEwymRcNxgsQ%3D%3D
				16 x 8k pages

	6) Continue development of the Extended Graphics systems. 
		Remember: The performance of the Raspberry PI Pico 
		is very limited. 
		- Sprites
		- Tiles

	7) Update bit C of the SYS_STATE to reflect a warm reset
		state. Clear this bit on cold start. Set this bit
		immediately following the cold startup initialization.
		This could actually be done in the Kernel ROM code.

	8) Begin building more appropriate KERNEL functions and
		wrappers such as:
		- char_out
		- line_out
		- cls
		- strcpy
		- strcmp
		- strtok
		- line_edit
		- open/close file
		- etc...

**** To Do: **************************************************/


/**** KNOWN BUGS **********************************************

	DSP_GRES mode $FF is super slow outputting characters. 
		Is even slower when scrolling. Proportionally slower than
		mode $FE. 


**** KNOWN BUGS **********************************************/