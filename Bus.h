//////////
//
//  Bus.h  (thread-safe singleton)
//
/////////////
#pragma once
#ifndef __BUS_H__
#define __BUS_H__

#include "types.h"
#include "Device.h"
#include "Gfx.h"

//#include "Memory.h"		// integrate the memory class into the bus class
							// and use static read and write:
							// 		Bus::Read() and Bus::Write()

// // forward declarations:
// class RAM;
// class ROM;

class Bus
{
	protected:
		Bus();									// constructor
		
		inline static bool s_bIsRunning = true;
		inline static bool s_bIsDirty = true;
		Gfx* m_gfx = nullptr;
		int _fps = 0;

	public:
		~Bus();									// destructor
		Bus(const Bus&) = delete;				// copy constructor
		Bus(Bus&&) = delete;					// move constructor
		Bus& operator=(const Bus&) = delete;	// copy assignment operator
		Bus& operator=(Bus&&) = delete;			// move assignment operator
		static Bus& Inst() 						{ static Bus inst; return inst; }

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