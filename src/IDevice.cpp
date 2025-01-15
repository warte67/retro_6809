/***  IDevice.cpp  ****************************
 *      _____   _____                   _                                          
 *     |_   _| |  __ \                 (_)                                         
 *       | |   | |  | |   ___  __   __  _    ___    ___        ___   _ __    _ __  
 *       | |   | |  | |  / _ \ \ \ / / | |  / __|  / _ \      / __| | '_ \  | '_ \ 
 *      _| |_  | |__| | |  __/  \ V /  | | | (__  |  __/  _  | (__  | |_) | | |_) |
 *     |_____| |_____/   \___|   \_/   |_|  \___|  \___| (_)  \___| | .__/  | .__/ 
 *                                                                  | |     | |    
 *                                                                  |_|     |_|    
 ******************/

#include "IDevice.hpp"
#include "Memory.hpp"




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

    
    // if (offset - base() < size())
    //     memory(offset - base(), data);
}