//////////
//
//  Bus.h  (thread-safe singleton)
//
/////////////

#include <iostream>
#include "Bus.h"

Bus::Bus()
{
	std::cout << "Bus::Bus()\n";
}

Bus::~Bus()
{
	std::cout << "Bus::~Bus()\n";
}

void Bus::_onInit() 
{
	std::cout << "void Bus::_onInit()\n";
}

void Bus::_onQuit() 
{
	std::cout << "void Bus::_onQuit()\n";
}

void Bus::_onDeactivate() 
{
	std::cout << "void Bus::_onDeactivate()\n";
}

void Bus::_onActivate() 
{
	std::cout << "void Bus::_onActivate()\n";
}

void Bus::_onUpdate() 
{
	// std::cout << "void Bus::_onUpdate()\n";
}

void Bus::_onEvent() 
{
	// std::cout << "void Bus::_onEvent() \n";
}

void Bus::_onRender() 
{
	// std::cout << "void Bus::_onRender()\n";
}

void Bus::_onPresent() 
{
	// std::cout << "void Bus::_onPresent()\n";
}

void Bus::Error(const std::string& sErr)
{
	std::cout << "\n    ERROR: " << sErr << SDL_GetError() << "\n\n";
    s_bIsRunning = false;
}



// main loop
void Bus::Run()
{
	// called after Bus() constructor for each attached device
    _onInit();	

    while (s_bIsRunning)
    {
        // create a new environment
        if (s_bIsDirty)
        {
            // shutdown the old environment
            _onDeactivate();
            // create a new environment
            _onActivate();
			// no longer dirty
			s_bIsDirty = false;
        }
        _onUpdate();
        _onEvent();
        _onRender();  

        // swap display buffers / present
        // if (s_gfx)
        //     SDL_RenderPresent(s_gfx->GetRenderer());      
		_onPresent();


		// TESTING: for now just stop
		//IsRunning(false);
		//IsDirty(true);

		Bus::Error("Automated Stop");

    }
	// shutdown the old environment
	_onDeactivate();	

	// one time shutdown
    _onQuit();
}
