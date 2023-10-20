//////
//
// retro_6809 main.cpp
//
// Notes:  remember to open an extra list window use
//       chord:  CTRL-K then O
//
////////////////////

#include <iostream>
#include "Bus.h"

int main(int argc, char* argv[])
{	
	// clear screen using shell. (Yeah, I know. Whatever!)
	if (true)	
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

	// // testing load_hex()
	// bus.load_hex("asm/test.hex");
	// for (Word addr = 0xf000; addr < 0xF007; addr++)
	// {
	// 	Byte b = bus.read(addr);
	// 	printf("0x%04X: 0x%02X\n", addr, b);
	// }
	// return 0;

	bus.Run();

	return 0;
}
