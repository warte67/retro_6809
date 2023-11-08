//////////
//
//  Bus.h  (thread-safe singleton)
//
/////////////
#pragma once
#ifndef __BUS_H__
#define __BUS_H__

#include <thread>
#include "types.h"
#include "Device.h"
#include "Gfx.h"
// #include "C6809.h"


//#include "Memory.h"		// integrate the memory class into the bus class
							// and use static read and write:
							// 		Bus::Read() and Bus::Write()

// forward declarations
class C6809;
class Keyboard;
class FileIO;


class Bus
{
	friend class C6809;
	friend class Gfx;
	friend class GfxDebug;
	friend class Keyboard;
	friend class Gamepad;
	friend class FileIO;
	friend class Math;

	protected:
		Bus();									// constructor

		Gfx* m_gfx = nullptr;
		inline static Keyboard* m_keyboard = nullptr;

		inline static bool s_bIsRunning = true;
		inline static bool s_bIsDirty = true;
		int _fps = 0;
		float _avg_cpu_cycle_time = 0.0f;
		int _avg_cpu_clock = 0;

		// Bus CPU clock registers (Uses the Gfx Device register block)
		inline static Byte _sys_state = 0x01;			// SYS_STATE	(R/W Byte) register
		inline static Word _sys_cpu_speed = 0;			// SYS_SPEED	(Read Byte) register
		inline static Byte _clock_div = 0;				// SYS_CLOCK_DIV (Byte) 60 hz Clock Divider  (Read Only) 
		inline static Word _clock_timer = 0;			// SYS_TIMER	(R/W Word) increments at 0.46875 hz

		Byte clock_div(Byte& cl_div, int bit);
		void clockDivider();

		// CPU Stuff
		inline static C6809* m_cpu = nullptr;
		inline static void _cpuThread();
		inline static bool bCpuEnabled = false;
		std::thread m_cpuThread;

	public:
		~Bus();									// destructor
		Bus(const Bus&) = delete;				// copy constructor
		Bus(Bus&&) = delete;					// move constructor
			// allowing the copy and move assignment operators for now
			//Bus& operator=(const Bus&) = delete;	// copy assignment operator
			//Bus& operator=(Bus&&) = delete;		// move assignment operator
		static Bus& Inst() 						
		{ 
			static Bus inst; 
			return inst; 
		}

		void Run();
		static void IsDirty(bool _bIsDirty)			{ s_bIsDirty = _bIsDirty; }
		static void IsRunning(bool _bIsRunning)		{ s_bIsRunning = _bIsRunning; }
		static void Error(const std::string& sErr);

		// Public Memory Stuff
		Word Attach(Device* dev, Word size = 0);   // attach a user defined memory node
        void DumpMemoryMap();        
        int ap() { return _lastAddress; }					// move to private as _ap() helper
        void ap(int _addr)  { _lastAddress = _addr; }		// move to private as _ap() helper
		int FPS() { return _fps; }

		// static read and writes
		static Byte Read(Word offset, bool debug = false);
		static void Write(Word offset, Byte data, bool debug = false);
		static Word Read_Word(Word offset, bool debug = false);
		static void Write_Word(Word offset, Word data, bool debug = false);	

		void load_hex(const char* filename);


	private:
		void _onInit();			// one time init after all devices have been created
		void _onQuit();			// quit each device prior to app termination
		void _onDeactivate();	// calls each device on deactivation
		void _onActivate();		// calls each device on activation
		void _onUpdate();		// calls each device on update
		void _onEvent();		// calls each device on SDL event
		void _onRender();		// calls each device for rendering
		void _onPresent();		// calls SDL_RenderPresent() from the Gfx device

		// Private Memory Stuff
        int _lastAddress = 0;
        inline static std::vector<Device*> _memoryNodes;		

		// load hex helpers
		Byte _fread_hex_byte(std::ifstream& ifs);
		Word _fread_hex_word(std::ifstream& ifs);	

};    




/////////////////////
//
// Standard Memory Devices
//
////////

class RAM : public Device
{
    public:
        RAM() {  Name("RAM"); }
        RAM(std::string sName) { Name(sName); }
        virtual ~RAM() {}    
};

class ROM : public Device
{
    public:
        ROM() {  Name("ROM"); }
        ROM(std::string sName) { Name(sName); }
        virtual ~ROM() {}    

        virtual void write(Word offset, Byte data, bool debug = false);
};






#endif //__BUS_H__