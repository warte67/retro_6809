//////////
//
//  Bus.h  (singleton)
//
/////////////

#include <iostream>
#include "Bus.h"

void Bus::_onetime_init() 
{
	std::cout << "void Bus::_onetime_init()\n";
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
	std::cout << "void Bus::_onUpdate()\n";
}

void Bus::_onEvent() 
{
	std::cout << "void Bus::_onEvent() \n";
}

void Bus::_onRender() 
{
	std::cout << "void Bus::_onRender()\n";
}

void Bus::_onPresent() 
{
	std::cout << "void Bus::_onPresent()\n";
}

void Bus::_onetime_quit() 
{
	std::cout << "void Bus::_onetime_quit()\n";
}


// main loop
void Bus::Run()
{
	std::cout << "Retro_6809\n";

    _onetime_init();
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
    }
    _onetime_quit();
}
