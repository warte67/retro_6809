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
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ******************/
#pragma once

// #include <SDL2/SDL.h>
// #include <string>

#include <thread>

#include "types.hpp"
#include "Memory.hpp"
#include "GPU.hpp"


class C6809;
class Debug;

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

private: // PRIVATE DISPATCHER METHODS
    void _onInit(void);
    void _onQuit(void);
    void _onActivate(void);
    void _onDeactivate(void);	
    void _onEvent(); 
    void _onUpdate();
    void _onRender();
    bool _onTest();

    Byte clock_div(Byte& cl_div, int bit);
    void clockDivider();

public: // PUBLIC METHODS
    bool Run(void);
    static bool IsRunning();
    static void IsRunning(bool b);
    static bool IsDirty();
    static void IsDirty(bool b);
    // inline static float GetAvgCpuCycleTime() { return s_avg_cpu_cycle_time; }
    // inline static void SetAvgCpuCycleTime(float f) { s_avg_cpu_cycle_time = f; }
    
    static void load_hex(const char* filename);

    static void Error(std::string err_msg, std::string file=__FILE__, int line=__LINE__);
    #define ERROR(msg) Bus::Error(#msg, __FILE__, __LINE__)

    static Byte Read(Word offset)               { return Memory::Read(offset); }
    static void Write(Word offset, Byte data)   { Memory::Write(offset, data); } 
    static void Write_Word(Word offset, Word data) { Memory::Write_Word(offset, data); }
    static Word Read_Word(Word offset)          { return Memory::Read_Word(offset); }
    
    static float FPS() { return _fps; }
    static std::string GetTitle() { return _s_title; }

    static Word GetCpuSpeed();
    static Byte GetClockDiv() { return _clock_div; }
    static DWord GetUpdateCount() { return _sys_update_event; }
    static DWord SetUpdateCount(DWord count) { return _sys_update_event = count; }

    static Debug* GetDebug() { return _pDebug; }
    static GPU* GetGPU() { return _pGPU; }
    static C6809* GetC6809() { return s_c6809; }

private: // INTERNAL PRIVATES
	bool _bWasInit = false;
    inline static bool s_bIsRunning = true;
    inline static bool s_bIsDirty = true;
    inline static float s_avg_cpu_cycle_time = 0;
    inline static Byte _clock_div = 0;				// SYS_CLOCK_DIV (Byte) 60 hz Clock Divider  (Read Only) 
    inline static DWord _sys_update_event = 0;			// SYS_TIMER	(R/W Word) increments with update event
    inline static std::thread s_cpuThread;

    inline static std::mutex _mutex_IsDirty;
    inline static std::mutex _mutex_IsRunning;


    // quick and dirty reference to the Gfx object:
    inline static GPU*   _pGPU   = nullptr;   // singlular but not necessarily a singleton
    inline static Debug* _pDebug = nullptr;
    inline static C6809* s_c6809 = nullptr;

    // static Memory Management Device:
    Memory& _memory = Memory::GetInstance();   

    // frames per second
    inline static float _fps;
    inline static std::string _s_title;


    static Byte _fread_hex_byte(std::ifstream& ifs);
    static Word _fread_hex_word(std::ifstream& ifs);

};
