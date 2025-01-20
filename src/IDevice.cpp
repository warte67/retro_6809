/***  IDevice.cpp  ****************************
 *      _____   _____                   _                                          
 *     |_   _| |  __ \                 (_)                                         
 *       | |   | |  | |   ___  __   __  _    ___    ___        ___   _ __    _ __  
 *       | |   | |  | |  / _ \ \ \ / / | |  / __|  / _ \      / __| | '_ \  | '_ \ 
 *      _| |_  | |__| | |  __/  \ V /  | | | (__  |  __/  _  | (__  | |_) | | |_) |
 *     |_____| |_____/   \___|   \_/   |_|  \___|  \___| (_)  \___| | .__/  | .__/ 
 *                                                                  | |     | |    
 *                                                                  |_|     |_|    
 * 
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 * 
******************/

#include "UnitTest.hpp"
#include "IDevice.hpp"
#include "Memory.hpp"



bool IDevice::OnTest() 
{
    std::string msg = "No unit tests defined for " + _device_name + ".";
    UnitTest::Log(msg);
    return true; 
}



/////////////////////////////
// Protected Helper Methods
/////////////////////////////

// READ
Byte IDevice::memory(Word address) { 
    return Memory::_raw_cpu_memory[address]; 
}

// WRITE
void IDevice::memory(Word address, Byte data) { 
    Memory::_raw_cpu_memory[address] = data; 
}



/////////////////////////////
// ROM Device Helper Method
/////////////////////////////



void ROM::write_to_rom(Word address, Byte data) 
{
    (void)address;
    (void)data;

    // ToDo:  Implement This Function and
    //        verify that it works while
    //        writing the Kernel ROM.
    // ...
    
    // THIS NO LONGER WORKS!
    // if (offset - base() < size())
    //     memory(offset - base(), data);
}



int KERNEL_ROM::OnAttach(int nextAddr) 
{ 
    int bank_size = 3.5f*1024;
    Word old_address=nextAddr;
    this->heading = "Kernel Rom (3.5K)";

    mapped_register.push_back({ "KERNEL_START", nextAddr, nullptr, nullptr,
            { "Start of Kernel Rom Space"     }}); nextAddr+=(bank_size-1);
    mapped_register.push_back({ "KERNEL_END",   nextAddr, nullptr, nullptr,
            { "End of Kernel Rom Space"       }}); nextAddr+=1;
    mapped_register.push_back({ "KERNEL_TOP",   nextAddr, nullptr, nullptr,
            { "Top of Kernel Rom Space", "---"}});

    // // Register all addresses between KERNEL_START and KERNEL_END
    // for (Word addr = old_address; addr < nextAddr; ++addr) {
    //     Memory::add_ROM_entry_to_device_map(addr);
    // }                

    _size = nextAddr - old_address;
    return _size;
}  