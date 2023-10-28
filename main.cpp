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

/**** KNOWN BUGS **********************************************

	1) When starting with DEBUG_SINGLE_STEP = false
		GfxDebug Mouse Coordinates are wrong

		SOLVED! Added calls to:
			_decode_dsp_gres() in Write to DSP_GRES
			_decode_dsp_ext() in Write to DSP_EXT

	2) Typing in Debug mode also sends keys to the main program

		PARTIALLY SOLVED! Added to Keyboard::OnEvent()
			if (GfxDebug::_bIsDebugActive)
				return;

	3) ALT-D is still being sent to the command line when returning
		from the debugger using the alt-d keystroke.


 **** KNOWN BUGS **********************************************/