/*** class Kernel_Rom.hpp *******************************************************
 *      _  __                    _        ____                       _                 
 *     | |/ /___ _ __ _ __   ___| |      |  _ \ ___  _ __ ___       | |__  _ __  _ __  
 *     | ' // _ \ '__| '_ \ / _ \ |      | |_) / _ \| '_ ` _ \      | '_ \| '_ \| '_ \ 
 *     | . \  __/ |  | | | |  __/ |      |  _ < (_) | | | | | |  _  | | | | |_) | |_) |
 *     |_|\_\___|_|  |_| |_|\___|_|  ____|_| \_\___/|_| |_| |_| (_) |_| |_| .__/| .__/ 
 *                                  |_____|                               |_|   |_|  
 * 
 * (This will be moved to its own files)
 * Currently using a RAM model for testing.
 ****************************************************************/
#pragma once

#include "IDevice.hpp"

class KERNEL_ROM : public IDevice
{
public:
    KERNEL_ROM(const char* hexfile);        
    virtual ~KERNEL_ROM();    

    void OnInit() override;
    void OnQuit() override;
    void OnActivate() override 					    {}
    void OnDeactivate() override 				    {}
    void OnEvent(SDL_Event* evnt) override 		    { if (evnt==nullptr) {;} }
    void OnUpdate(float fElapsedTime) override 	    { if (fElapsedTime==0) {;} }         
    void OnRender() override 					    {}
    
    int OnAttach(int nextAddr) override; 

    bool OnTest() 
    { 
        UnitTest::TestInit(this, " Testing ...");
        
        // Check the number of mapped registers
        size_t expectedRegisters = 3; // Number of interrupt vectors
        ASSERT(mapped_register.size() == expectedRegisters, _device_name + ": Incorrect number of mapped registers");
        // Check the mapped registers
        bool result = UnitTest::RangeTest_RO(_device_name, base_address, base_address+_size); 
        return result;
    }     

private:
    std::string hex_filename;
};