/******************************************************
main.cpp (updated test #2)

build with debug symbols:
    g++ -g *.cpp -o bin/main -std=c++20 -lSDL2 

build without debug symbols:
    g++ *.cpp -o bin/main -std=c++20 -lSDL2 

GIT Stuff:
    git status
    git add .
    git commit -m "Commit Comment"
    git push origin master

******************************************************/

#include <stdio.h>

// #include <SDL2/SDL.h>
// #include "types.h"

#include "Bus.h"

int main(int argc, char* argv[])
{
    //printf("Initializing...\n");

#ifdef _WIN32 
    system("cls");          // this should be "cls" for windows
#else
    system("clear");        // this would be "clear" under linux
#endif

    Bus* bus = Bus::GetInstance();
    bus->Run();

    printf("%s\n", SDL_GetError());

    return 0;
}


/**************************************
 
ToDo NOTES:

	1) To exponentially simplify the graphics  processing overhead, 
		remove the 640x400 timing resolutions.

	2) Debug device object shall run in its own (5k text buffer) in 1x timing

	3) the mouse cursor itself shall run in 1x timing mode, however it will
		by software scaleable

	4) in the hardware version (2xPICO) all graphcis shall render to its own
		50k render buffer.  Simulate this by rendering all graphics to a 
		single render target texture for final presentation.

	5) create a MakeFile to use incremental building with "Make".





***************************************/

