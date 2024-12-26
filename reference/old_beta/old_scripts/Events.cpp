/***  Events.cpp  (singleton) *************************
 * 
 *  Wraps the SDL2 Event sub-system to implement
 *  a greatly reduced set of events for the target
 *  hardware.
 * 
 *************************/

// #include <SDL2/SDL.h>
#include "Bus.hpp"
#include "Events.hpp"
#include "clr.hpp"

Events::Events()  
{ 
    std::cout << clr::LT_BLUE << "Events Singleton Created" << clr::RETURN;  

    if (SDL_Init(SDL_INIT_EVENTS) != 0)
    {
        Bus::ERROR("Failed to initialize the Event subsystem");
        Bus::IsRunning(false);
    }
}

Events::~Events() 
{ 
    std::cout << clr::LT_BLUE << "Events Singleton Destroyed" << clr::RETURN; 

    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}


////////////////////////////
// PUBLIC WRAPPER METHODS //
////////////////////////////

// (bool) Returns true if this got an event or false if there are none available.
int Events::EV_PollEvent(EV_Event *evnt)
{
    SDL_Event sdl_event;
    if ( SDL_PollEvent(&sdl_event) )
    {
        // convert SDL_Events to EV_Events and return true if valid
        switch(sdl_event.type)
        {
        case SDL_QUIT:
            evnt->type = EV_QUIT;   
            evnt->quit.timestamp = sdl_event.quit.timestamp;
            return 1;
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (sdl_event.type == SDL_KEYDOWN)
                evnt->type = EV_KEYDOWN;
            if (sdl_event.type == SDL_KEYUP)
                evnt->type = EV_KEYUP;
            evnt->key.timestamp = sdl_event.key.timestamp;
            evnt->key.windowID = sdl_event.key.windowID;         
            evnt->key.state = sdl_event.key.state;          // EV_PRESSED or EV_RELEASED 
            evnt->key.repeat = sdl_event.key.repeat;
            evnt->key.padding2 = sdl_event.key.padding2;
            evnt->key.padding3 = sdl_event.key.padding3;
            evnt->key.keysym = sdl_event.key.keysym;
            return 1;
            break;
        default:
            break;
        }
        // flush and clean up remaining (unused) SDL_Events
        SDL_FlushEvent(sdl_event.type);
    }
    return 0;
}


std::string Events::EV_GetError(void)
{
    return SDL_GetError();
}

void Events::EV_Delay(Uint32 ms)
{
    SDL_Delay(ms);
}


// END: Events.cpp