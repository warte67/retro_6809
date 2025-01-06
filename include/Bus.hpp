/***  Bus.hpp  (singleton) **************************** 
 *      ____                      _                     
 *     |  _ \                    | |                    
 *     | |_) |  _   _   ___      | |__    _ __    _ __  
 *     |  _ <  | | | | / __|     | '_ \  | '_ \  | '_ \ 
 *     | |_) | | |_| | \__ \  _  | | | | | |_) | | |_) |
 *     |____/   \__,_| |___/ (_) |_| |_| | .__/  | .__/ 
 *                                       | |     | |    
 *                                       |_|     |_|    * 
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
#include "Tests.hpp"
#include "GPU.hpp"
#include "Debug.hpp"

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
    void _onInit(void);
    void _onQuit(void);
    void _onActivate(void);
    void _onDeactivate(void);	
    void _onEvent(SDL_Event* evnt); 
    void _onUpdate(float fElapsedTime);
    void _onRender(void);

public: // PUBLIC METHODS
    bool Run(void);
    static bool IsRunning();
    static void IsRunning(bool b);
    static bool IsDirty();
    static void IsDirty(bool b);

    static void Error(std::string err_msg, std::string file=__FILE__, int line=__LINE__);
    #define ERROR(msg) Bus::Error(#msg, __FILE__, __LINE__)

    static float FPS() { return _fps; }
    static std::string GetTitle() { return _s_title; }

private: // INTERNAL PRIVATES
	bool _bWasInit = false;

    inline static std::mutex _mutex_IsDirty;
    inline static std::mutex _mutex_IsRunning;

    // quick and dirty reference to the Gfx object:
    inline static GPU* _pGPU = nullptr;   // singlular but not necessarily a singleton
    inline static Tests* _pTests = nullptr;
    inline static Debug* _pDebug = nullptr;

    // static Memory Management Device:
    Memory& _memory = Memory::GetInstance();   

    // frames per second
    inline static float _fps;
    inline static std::string _s_title;

};
