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





 **** KNOWN BUGS **********************************************/