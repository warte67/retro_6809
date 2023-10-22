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

	std::cout << "Retro_6809\n\n";

	Bus& bus = Bus::Inst();
	bus.Run();

	return 0;
}

/*******************************************************

	To Do Next:

	1) GfxMouse - build a register for mouse_cursor_enable
					- also include a flag for debug_enable
					- should this be a Gfx register or one each for GfxMouse and GfxDebug?
	2) GfxMouse - add an independant 16-color pallete for the mouse cursor
	3) GfxMouse - remove the full sized mouse layer.  just use the cursor texture


 *******************************************************/

