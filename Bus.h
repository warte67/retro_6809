//////////
//
//  Bus.h  (singleton)
//
/////////////
#pragma once
#ifndef __BUS_H__
#define __BUS_H__

class Bus
{
	protected:
		Bus() = default;


	public:
		static Bus& GetInst() 
		{
			static Bus inst;
			return inst;
		}

		void Run();

	private:
		inline static bool s_bIsRunning = false;
		inline static bool s_bIsDirty = false;
		void _onetime_init();
		void _onDeactivate();
		void _onActivate();
		void _onUpdate();
		void _onEvent();
		void _onRender();
		void _onPresent();
		void _onetime_quit();
};





#endif //__BUS_H__