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


class Bus
{
	friend class C6809;
	friend class Gfx;
	friend class GfxDebug;

	protected:
		Bus();									// constructor
		
		inline static bool s_bIsRunning = true;
		inline static bool s_bIsDirty = true;
		Gfx* m_gfx = nullptr;
		int _fps = 0;
		float _avg_cpu_cycle_time = 0.0f;
		int _avg_cpu_clock = 0;

		// Bus CPU clock registers (Uses the Gfx Device register block)
		inline static Byte _sys_state = 0x0D;			// SYS_STATE	(R/W Byte) register
		inline static Word _sys_cpu_speed = 0;			// SYS_SPEED	(Read Byte) register

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
		void IsDirty(bool _bIsDirty)			{ s_bIsDirty = _bIsDirty; }
		void IsRunning(bool _bIsRunning)		{ s_bIsRunning = _bIsRunning; }
		static void Error(const std::string& sErr);

		// Public Memory Stuff
		Word Attach(Device* dev, Word size = 0);   // attach a user defined memory node
        void DumpMemoryMap();        
        int ap() { return _lastAddress; }					// move to private as _ap() helper
        void ap(int _addr)  { _lastAddress = _addr; }		// move to private as _ap() helper
		int FPS() { return _fps; }

        Byte read(Word offset, bool debug = false);
        void write(Word offset, Byte data, bool debug = false);
        Word read_word(Word offset, bool debug = false);
        void write_word(Word offset, Word data, bool debug = false);   				

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
        std::vector<Device*> _memoryNodes;		

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