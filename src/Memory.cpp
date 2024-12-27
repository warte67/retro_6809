/***  Memory.hpp  **************************** 
 *     __  __                                                   
 *    |  \/  |                                                  
 *    | \  / | ___ _ __ ___   ___  _ __ _   _   ___ _ __  _ __  
 *    | |\/| |/ _ \ '_ ` _ \ / _ \| '__| | | | / __| '_ \| '_ \ 
 *    | |  | |  __/ | | | | | (_) | |  | |_| || (__| |_) | |_) |
 *    |_|  |_|\___|_| |_| |_|\___/|_|   \__, (_)___| .__/| .__/ 
 *                                       __/ |     | |   | |    
 *                                      |___/      |_|   |_|    
 * 
 *  The Memory Object is responsible for maintaining the
 *  CPU addressable memory map, reading, and writing. It
 *  acts as a static singleton  container object for all 
 *  of the attached mememory devices. 
 * 
 ******************/

#include <algorithm>
#include "Bus.hpp"
#include "clr.hpp"
#include "Memory.hpp"


/////////////////////////////
// PRIVATE SINGLETON STUFF //
/////////////////////////////

Memory::Memory()  
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Memory Singleton Created" << clr::RETURN;
}

Memory::~Memory() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Memory Singleton Destroyed" << clr::RETURN;
}


////////////////////////////////
// PUBLIC VIRTUAL DISPATCHERS //
////////////////////////////////

bool Memory::OnInit() 
{
    std::cout << clr::indent_push() << clr::CYAN << "Memory::OnInit() Entry" << clr::RETURN;
    bool ret = true;
    for (auto &d : Memory::_memory_nodes) {
        if (d->OnInit() == false) {
            std::cout << clr::indent_pop() << clr::ORANGE << "Memory::OnInit() Error" << clr::RETURN;
            ret = false;
        }
    }
    std::cout << clr::indent_pop() << clr::CYAN << "Memory::OnInit() Exit" << clr::RETURN;                
    return ret;
}

bool Memory::OnQuit() 
{
    std::cout << clr::indent_push() << clr::CYAN << "Memory::OnQuit() Entry" << clr::RETURN;
    bool ret = true;
	for (auto &d : Memory::_memory_nodes) {
        if ( d->OnQuit() == false) {
            std::cout << clr::indent_pop() << clr::ORANGE << "Memory::OnQuit() Error" << clr::RETURN;
            ret = false;
        }
    }
    for (auto &d : Memory::_memory_nodes) {
        delete d;  // Add this line to properly release memory for each device
    }    
    std::cout << clr::indent_pop() << clr::CYAN << "Memory::OnQuit() Exit" << clr::RETURN;
    return ret;
}
bool Memory::OnActivate() 
{
    std::cout << clr::indent_push() << clr::CYAN << "Memory::OnActivate() Entry" << clr::RETURN;
    bool ret = true;
	for (auto &d : Memory::_memory_nodes) {
		if ( d->OnActivate() == false ) {
            std::cout << clr::indent_pop() << clr::ORANGE << "Memory::OnActivate() Error" << clr::RETURN;
			ret = false;
        }
    }
    std::cout << clr::indent_pop() << clr::CYAN << "Memory::OnActivate() Exit" << clr::RETURN;
	return ret;
}
bool Memory::OnDeactivate() 
{
    std::cout << clr::indent_push() << clr::CYAN << "Memory::OnDeactivate() Entry" << clr::RETURN;
    bool ret = true;
	for (auto &d : Memory::_memory_nodes) {
		if ( d->OnDeactivate() == false ) {
            std::cout << clr::indent_pop() << clr::ORANGE << "Memory::OnDeactivate() Error" << clr::RETURN;
			ret = false;
        }
    }
    std::cout << clr::indent_pop() << clr::CYAN << "Memory::OnDeactivate() Exit" << clr::RETURN;
	return ret;
}
// bool Memory::OnEvent(SDL_Event* evnt) 
bool Memory::OnEvent(SDL_Event* evnt) 
{
    bool ret = true;
	for (auto &d : Memory::_memory_nodes) {
		if ( d->OnEvent(evnt) == false ) {
			ret = false;
        }
    }
	return ret;
}
bool Memory::OnUpdate(float fElapsedTime) 
{
    bool ret = true;
	for (auto &d : Memory::_memory_nodes) {
		if ( d->OnUpdate(fElapsedTime) == false ) {
			ret = false;
        }
    }
	return ret;
}
bool Memory::OnRender() 
{
    bool ret = true;
	for (auto &d : Memory::_memory_nodes) {
		if ( d->OnRender() == false ) {
			ret = false;
        }
    }
	return ret;
}


//////////////////////
// PUBLIC ACCESSORS //
//////////////////////

Byte Memory::Read(Word offset, bool debug)
{
    // int index = _binary_search(offset);
    // if (index != -1) {
    //     IDevice* pDevice = _memory_nodes[index];
    //     if (debug) {
    //         return pDevice->_memory[offset - pDevice->base()];
    //     } else {
    //         return pDevice->OnRead(offset - pDevice->base());
    //     }
    // } else {
    //     // Handle error or default value for out-of-range addresses.
    //     Bus::Error("Memory Address Out Of Range");
    //     return 0xCC;
    // }
    for (auto& a : Memory::_memory_nodes)
    {
        if (offset - a->base() < a->size())
        {
            if (debug)
                if (offset - a->base() < a->size())
                    return a->memory(offset - a->base() );
            return a->OnRead(offset);
        }
    }
    return 0xCC;    
}

void Memory::Write(Word offset, Byte data, bool debug)
{
    // int index = _binary_search(offset);
    // if (index != -1) {
    //     IDevice* pDevice = _memory_nodes[index];
    //     ROM* isROM = dynamic_cast<ROM*>(pDevice);
    //     if (debug && isROM) {
    //         isROM->write_to_rom(offset, data);
    //     } else {
    //         pDevice->OnWrite(offset - pDevice->base(), data);
    //     }
    // } else {
    //     // Handle error for out-of-range addresses.
    //     Bus::Error("Memory Address Out Of Range");
    // }
    
    for (auto& a : Memory::_memory_nodes)
    {
        if (offset - a->base() < a->size())
        {
            ROM* isROM = dynamic_cast<ROM*>(a);
            if (debug && isROM)
                isROM->write_to_rom(offset, data);
            else
                a->OnWrite(offset, data);
            return;
        }
    }
}

Word Memory::Read_Word(Word offset, bool debug)
{
    if (debug) {;}  // stop the unused argument warning

    Word ret = (Memory::Read(offset) << 8) | Memory::Read(offset + 1);
    return ret;
}

void Memory::Write_Word(Word offset, Word data, bool debug)
{    
    if (debug) {;}  // stop the unused argument warning

    Byte msb = (data >> 8) & 0xFF;
    Byte lsb = data & 0xff;
    Memory::Write(offset, msb);
    Memory::Write(offset + 1, lsb);
}

DWord Memory::Read_DWord(Word offset, bool debug)
{
    if (debug) {;}  // stop the unused argument warning

    DWord ret = (Memory::Read(offset+0)<<24) |
                (Memory::Read(offset+1)<<16) | 
                (Memory::Read(offset+2)<< 8) | 
                (Memory::Read(offset+3)<< 0);
    return ret;
}

void Memory::Write_DWord(Word offset, DWord data, bool debug)
{
    if (debug) {;}  // stop the unused argument warning

    Memory::Write(offset+0, (data>>24) & 0xFF);
    Memory::Write(offset+1, (data>>16) & 0xFF);
    Memory::Write(offset+2, (data>> 8) & 0xFF);
    Memory::Write(offset+3, (data>> 0) & 0xFF);
}


int Memory::_attach(IDevice* device)
{    
    int size = 0;
    if (device != nullptr)
    {
        // attach to the memory map and allow the new device to build its own
        // device descrptor node.
        size = device->OnAttach(_next_address);     
        if (size > 0)
        {
            device->base(_next_address);
            device->size(size);
            _next_address += size;               
            Memory::_memory_nodes.push_back(device);
        }
        else
        {
            std::string err = "Memory node '";
            err += device->name();
            err += "' was zero size!";
            Bus::Error(err.c_str(), __FILE__, __LINE__);
            Bus::IsRunning(false);
        }
    }
    if (_next_address > 65536)
    {
        Bus::ERROR("Memory allocation beyond 64k boundary!");
        Bus::IsRunning(false);
    }
    if (size==0)  Bus::ERROR("_attach size was zero");
    return size;
}


void Memory::Display_Nodes() 
{
    // 
    // Display C++ memory_map.hpp
    //
    if (MEMORY_MAP_OUTPUT_CPP == true)
    {
        constexpr int FIRST_TAB = 4;
        constexpr int VAR_LEN = 22;
        constexpr int COMMENT_START = 32;

        std::cout << "\n\n// memory_map.hpp\n";
        std::cout << "#ifndef __MEMORY_MAP_HPP__\n";
        std::cout << "#define __MEMORY_MAP_HPP__\n";
        std::cout << std::endl;
        std::cout << "enum MEMMAP\n";
        std::cout << "{\n";
        std::cout << clr::pad(" ",FIRST_TAB) << "//  **********************************************\n";
        std::cout << clr::pad(" ",FIRST_TAB) << "//  * Allocated 64k Memory Mapped System Symbols *\n";
        std::cout << clr::pad(" ",FIRST_TAB) << "//  **********************************************\n";

        for (auto &node : Memory::_memory_nodes) 
        {
            // std::cout << pad(" ",FIRST_TAB) << "\t// " << node->heading << ":\n";
            std::cout << std::endl;
            std::cout << clr::pad(clr::pad(" ",FIRST_TAB) + clr::pad(node->name(), VAR_LEN) + "= 0x" + clr::hex(node->base(),4), COMMENT_START+4) << "// START: " << node->heading << std::endl;

            for (auto &r : node->mapped_register)
            {
                std::string _out = clr::pad(clr::pad(r.name, VAR_LEN) + "= 0x" + clr::hex(r.address, 4), COMMENT_START);
                for (auto &c : r.comment)
                {
                    if (_out.length() > 0) { 
                        std::cout << clr::pad(" ",FIRST_TAB) << _out << "// " << c << std::endl;   
                        _out = "";                  
                    } else {
                        std::cout << clr::pad(" ",FIRST_TAB) << clr::pad(" ", COMMENT_START) << "// " << c << std::endl;
                    }
                }
            }
        }
        std::cout << "}; // END: enum MEMMAP\n";
        std::cout << "\n\n#endif // __MEMORY_MAP_H__\n\n\n";
    } // END MEMORY_MAP_OUTPUT_CPP
    else
    { // Display an ASM memory_map.asm
        constexpr int FIRST_TAB = 0;
        constexpr int VAR_LEN = 18;
        constexpr int COMMENT_START = 32;
        std::cout << "\n\n;    memory_map.hpp\n;\n";
        std::cout << clr::pad("",FIRST_TAB) << ";   **********************************************\n";
        std::cout << clr::pad("",FIRST_TAB) << ";   * Allocated 64k Memory Mapped System Symbols *\n";
        std::cout << clr::pad("",FIRST_TAB) << ";   **********************************************\n;\n";

        for (auto &node : Memory::_memory_nodes) 
        {
            // std::cout << pad("",FIRST_TAB) << "\t// " << node->heading << ":\n";
            std::cout << std::endl;
            std::cout << clr::pad(clr::pad("",FIRST_TAB) + clr::pad(node->name(), VAR_LEN) + "equ   0x" + clr::hex(node->base(),4), COMMENT_START) << "; START: " << node->heading << std::endl;

            for (auto &r : node->mapped_register)
            {
                std::string _out = clr::pad(clr::pad(r.name, VAR_LEN) + "equ   0x" + clr::hex(r.address, 4), COMMENT_START);
                for (auto &c : r.comment)
                {
                    if (_out.length() > 0) { 
                        std::cout << clr::pad("",FIRST_TAB) << _out << "; " << c << std::endl;   
                        _out = "";                  
                    } else {
                        std::cout << clr::pad("",FIRST_TAB) << clr::pad("", COMMENT_START) << "; " << c << std::endl;
                    }
                }
            }
        }
        std::cout << "\n\n; END of memory_map.asm definitions\n\n\n";
    }
}