/***  NewDevice.hpp  ****************************
*     _   _               _____             _            _                 
*    | \ | |             |  __ \           (_)          | |                
*    |  \| | _____      _| |  | | _____   ___  ___ ___  | |__  _ __  _ __  
*    | . ` |/ _ \ \ /\ / / |  | |/ _ \ \ / / |/ __/ _ \ | '_ \| '_ \| '_ \ 
*    | |\  |  __/\ V  V /| |__| |  __/\ V /| | (_|  __/_| | | | |_) | |_) |
*    |_| \_|\___| \_/\_/ |_____/ \___| \_/ |_|\___\___(_)_| |_| .__/| .__/ 
*                                                             | |   | |    
*                                                             |_|   |_|    
* 
*      This file serves meerly as a template as a
*      starting point for new devices.
* 
*************************/
#pragma once

#include "IDevice.hpp"

class NewDevice : public IDevice
{
    public:
        NewDevice() { _device_name = "NewDevice"; }
        NewDevice(std::string sName) : IDevice(sName) {}
        virtual ~NewDevice() {}

        // virtual interface methods (not yet implemented)

		Word OnAttach(Word nextAddr) override {}
		bool OnInit() override {}
		bool OnQuit() override {}
		bool OnActivate() override {}
		bool OnDeactivate() override {}
		bool OnEvent(SDL_Event* evnt) override {}
		bool OnUpdate(float fElapsedTime) override {}
		bool OnRender() override {}

        // internal device memory access
        
        Byte OnRead(Word offset) { return IDevice::OnRead(offset); };
        void OnWrite(Word offset, Byte data) { IDevice::OnWrite( offset, data); }
};


