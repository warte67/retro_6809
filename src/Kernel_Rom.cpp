/*** Kernel_Rom.cpp *******************************************
 *      _  __                    _        ____                       _                 
 *     | |/ /___ _ __ _ __   ___| |      |  _ \ ___  _ __ ___       | |__  _ __  _ __  
 *     | ' // _ \ '__| '_ \ / _ \ |      | |_) / _ \| '_ ` _ \      | '_ \| '_ \| '_ \ 
 *     | . \  __/ |  | | | |  __/ |      |  _ < (_) | | | | | |  _  | | | | |_) | |_) |
 *     |_|\_\___|_|  |_| |_|\___|_|  ____|_| \_\___/|_| |_| |_| (_) |_| |_| .__/| .__/ 
 *                                  |_____|                               |_|   |_|    
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#include "Bus.hpp"
#include "Memory.hpp"
#include "Kernel_Rom.hpp"




/***************************
* Constructor / Destructor *
***************************/

// KERNEL_ROM::KERNEL_ROM() 
// { 
//     std::cout << clr::indent_push() << clr::LT_BLUE << "KERNEL_ROM Device Created" << clr::RETURN;
//     _device_name = "KERNEL_ROM_DEVICE"; 
// } // END: KERNEL_ROM()

KERNEL_ROM::KERNEL_ROM(const char* hexfile) : hex_filename(hexfile) {
    _device_name = "KERNEL_ROM_DEVICE";
}  

KERNEL_ROM::~KERNEL_ROM() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "KERNEL_ROM Device Destroyed" << clr::RETURN; 
} // END: ~KERNEL_ROM()



/******************
* Virtual Methods *
******************/



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

void KERNEL_ROM::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "KERNEL_ROM Device Initialized" << clr::RETURN;
    
    // Load the ROM
    std::cout << clr::indent() << "Loading ROM from: " << hex_filename << clr::RETURN;

    Bus::load_hex(hex_filename.c_str());
}


void KERNEL_ROM::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "KERNEL_ROM Device Destroyed" << clr::RETURN;
}






// END: KERNEL_ROM.cpp
