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


int ROM_VECTS::OnAttach(int nextAddr)
{ 
    Word old_address=nextAddr;
    this->heading = "Hardware Interrupt Vectors";
    mapped_register.push_back({ "HARD_EXEC",  nextAddr, nullptr, nullptr, 
        { "EXEC Hardware Interrupt Vector"      }}); nextAddr+=2;
    mapped_register.push_back({ "HARD_SWI3",  nextAddr, nullptr, nullptr, 
        { "SWI3 Hardware Interrupt Vector"      }}); nextAddr+=2;
    mapped_register.push_back({ "HARD_SWI2",  nextAddr, nullptr, nullptr, 
        { "SWI2 Hardware Interrupt Vector"      }}); nextAddr+=2;
    mapped_register.push_back({ "HARD_FIRQ",  nextAddr, nullptr, nullptr, 
        { "FIRQ Hardware Interrupt Vector"      }}); nextAddr+=2;
    mapped_register.push_back({ "HARD_IRQ",   nextAddr, nullptr, nullptr, 
        { "IRQ Hardware Interrupt Vector"       }}); nextAddr+=2;
    mapped_register.push_back({ "HARD_SWI",   nextAddr, nullptr, nullptr, 
        { "SWI / SYS Hardware Interrupt Vector" }}); nextAddr+=2;
    mapped_register.push_back({ "HARD_NMI",   nextAddr, nullptr, nullptr, 
        { "NMI Hardware Interrupt Vector"       }}); nextAddr+=2;
    mapped_register.push_back({ "HARD_RESET", nextAddr, nullptr, nullptr, 
        { "RESET Hardware Interrupt Vector"     }}); nextAddr+=2;

    for (int addr = old_address; addr < nextAddr; addr++)   
    {
        Memory::add_entry_to_device_map(
            addr, 
            nullptr, 
            [this](Word addr, Byte d) { 
                (void)addr; 
                (void)d;             
            }        
        );           
    }

    _size = nextAddr - old_address;
    return _size;
}  

int HDW_RESERVED::OnAttach(int nextAddr)       
{
    Word old_address=nextAddr;
    this->heading = "Reserved Register Space";
    // reserve space for future use
    int bank_size = 0xFFF0-nextAddr;      
    std::string res = std::to_string(bank_size);
    res += " bytes reserved for future use.";
    nextAddr+=bank_size;
    mapped_register.push_back({ "HDW_REG_END", nextAddr, nullptr, nullptr,  
        { res , "---"}}); 

    for (int addr = old_address; addr < nextAddr; addr++)   
    {
        Memory::add_entry_to_device_map(
            addr, 
            [this](Word addr) { return memory(addr); },
            [this](Word addr, Byte d) { memory(addr, d); }        
        );           
    }    

    _size = nextAddr - old_address;
    return  _size;
}