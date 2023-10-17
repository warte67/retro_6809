//////////
//
//  Bus.h  (thread-safe singleton)
//
/////////////
#pragma once
#ifndef __BUS_H__
#define __BUS_H__

#include "types.h"
//#include "Memory.h"		// integrate the memory class into the bus class
							// and use static read and write:
							// 		Bus::Read() and Bus::Write()

class Bus
{
	protected:
		Bus();									// constructor
		
		inline static bool s_bIsRunning = true;
		inline static bool s_bIsDirty = true;

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

	private:
		void _onInit();			// one time init after all devices have been created
		void _onQuit();			// quit each device prior to app termination
		void _onDeactivate();	// calls each device on deactivation
		void _onActivate();		// calls each device on activation
		void _onUpdate();		// calls each device on update
		void _onEvent();		// calls each device on SDL event
		void _onRender();		// calls each device for rendering
		void _onPresent();		// calls SDL_RenderPresent() from the Gfx device
};





#endif //__BUS_H__