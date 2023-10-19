//////
//
// retro_6809 main.cpp
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
	bus.Run();
	//Bus::Inst().Run();		// also works	

	return 0;
}
