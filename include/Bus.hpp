/***  Bus.hpp  (singleton) **************************** 
  ____              _                 
 |  _ \            | |                
 | |_) |_   _ ___  | |__  _ __  _ __  
 |  _ <| | | / __| | '_ \| '_ \| '_ \ 
 | |_) | |_| \__ \_| | | | |_) | |_) |
 |____/ \__,_|___(_)_| |_| .__/| .__/ 
                         | |   | |    
                         |_|   |_|    
 * 
 * Notes:
 * 		Must remain thread safe. Consider using jthreads to ensure all threads are joined when
 * 		the main loop is terminated. Use try/catch for error trapping. 		
 * 
 * 		Likely Threads:
 * 			*) Main Loop Thread
 * 				*) Video Display Thread
 * 				*) Event Dispatch Thread
 * 				*) CPU Emulation Thread
 * 				*) Audio Subsystem Thread
 * 
 *      While the Bus object is fundamentally the first created IDevice object, it houses the 
 *      Memory Management object which in turn controls all of the other attached memory based
 *      IDevice objects. 
 * 
 ******************/
#pragma once

// #include <SDL2/SDL.h>
// #include <string>

#include "types.hpp"
#include "Memory.hpp"



class Bus
{
private: // PRIVATE SINGLETON STUFF
    Bus();
    ~Bus();

public: // PUBLIC SINGLETON STUFF
    Bus(const Bus&) = delete;				// delete the copy constructor
    Bus(Bus&&) = delete;					// delete the move constructor
    Bus& operator=(const Bus&) = delete;	// delete the copy assignment operator
    Bus& operator=(Bus&&) = delete;			// delete the move assignment operator
	static Bus& GetInstance() { static Bus inst; return inst; }

private: // PRIVATE STATICS
    inline static bool s_bIsRunning = true;
    inline static bool s_bIsDirty = true;

private: // PRIVATE DISPATCHER METHODS
    bool _onInit(void);
    bool _onQuit(void);
    bool _onActivate(void);
    bool _onDeactivate(void);	
    // bool _onEvent(SDL_Event* evnt); 
    bool _onEvent(SDL_Event* evnt); 
    bool _onUpdate(float fElapsedTime);
    bool _onRender(void);

public: // PUBLIC METHODS
    bool Run(void);
    static bool IsRunning();
    static void IsRunning(bool b);
    static bool IsDirty();
    static void IsDirty(bool b);

    static void Error(std::string err_msg, std::string file=__FILE__, int line=__LINE__);
    //#define ERROR(msg) Bus::Error(#msg, __FILE__, __LINE__)


private: // INTERNAL PRIVATES
	bool _bWasInit = false;

	// SDL stuff
	SDL_Window* pWindow = nullptr;
	SDL_Renderer* pRenderer = nullptr;
	// Uint32 window_flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	Uint32 window_flags = SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE;
	Uint32 renderer_flags = SDL_RENDERER_ACCELERATED;

    // static Memory Management Device:
    Memory& _memory = Memory::GetInstance();   

};
