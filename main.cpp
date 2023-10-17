//////
//
// retro_6809 main.cpp
//
////////////////////

// #include <iostream>
#include "Bus.h"

int main()
{
	Bus& bus = Bus::GetInst();
	bus.Run();

	//Bus::GetInst().Run();		// also works

	
	return 0;
}
