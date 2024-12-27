/***  Memory.hpp  **************************** 
 *      __  __                                                                      
 *     |  \/  |                                                                     
 *     | \  / |   ___   _ __ ___     ___    _ __   _   _        ___   _ __    _ __  
 *     | |\/| |  / _ \ | '_ ` _ \   / _ \  | '__| | | | |      / __| | '_ \  | '_ \ 
 *     | |  | | |  __/ | | | | | | | (_) | | |    | |_| |  _  | (__  | |_) | | |_) |
 *     |_|  |_|  \___| |_| |_| |_|  \___/  |_|     \__, | (_)  \___| | .__/  | .__/ 
 *                                                  __/ |            | |     | |    
 *                                                 |___/             |_|     |_|     
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

/**
 * OnInit() is a virtual method that is called when the bus is initialized.
 * OnInit() will call OnInit() on all of the attached devices.
 * OnInit() will return true if all of the devices OnInit() methods return true,
 * otherwise OnInit() will return false.
 */
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


/**
 * OnQuit() is a virtual method that is called when the system is shutting down.
 * It iterates over all memory nodes and calls their OnQuit() methods.
 * If any node returns false, an error message is logged and the function
 * returns false. After calling OnQuit() on all nodes, it deletes each node
 * to properly release the memory. The function logs the entry and exit of
 * the process and returns true if all nodes successfully quit.
 *
 * @return True if all memory nodes successfully quit, otherwise false.
 */

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


/**
 * OnActivate() is a virtual method that is called to activate all memory nodes.
 * It iterates over each memory node and calls their OnActivate() method.
 * If any node fails to activate, an error message is logged and the function
 * will return false. The function logs the entry and exit of the activation
 * process and returns true if all nodes successfully activate.
 *
 * @return True if all memory nodes are successfully activated, otherwise false.
 */

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


/**
 * OnDeactivate() is a virtual method that is called to deactivate all memory nodes.
 * It iterates over each memory node and calls their OnDeactivate() method.
 * If any node fails to deactivate, an error message is logged and the function
 * will return false. The function logs the entry and exit of the deactivation
 * process and returns true if all nodes successfully deactivate.
 *
 * @return True if all memory nodes are successfully deactivated, otherwise false.
 */
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



/**
 * OnEvent() is a virtual method called to process an SDL_Event for each memory node.
 * It iterates over all memory nodes and calls their OnEvent() method with the provided event.
 * If any node returns false, indicating failure to process the event, the function returns false.
 * The function returns true if all nodes successfully handle the event.
 *
 * @param evnt A pointer to an SDL_Event structure that contains the event data.
 * 
 * @return True if all memory nodes successfully handle the event, otherwise false.
 */

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


/**
 * OnUpdate() is a virtual method called to update all memory nodes.
 * It iterates over all memory nodes and calls their OnUpdate() method with the provided
 * time since the last update.
 * If any node returns false, indicating failure to update, the function returns false.
 * The function returns true if all nodes successfully update.
 *
 * @param fElapsedTime The time, in seconds, that has elapsed since the last update.
 * 
 * @return True if all memory nodes successfully update, otherwise false.
 */
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


/**
 * OnRender() is a virtual method called to render all memory nodes.
 * It iterates over all memory nodes and calls their OnRender() method.
 * If any node returns false, indicating failure to render, the function returns false.
 * The function returns true if all nodes successfully render.
 *
 * @return True if all memory nodes successfully render, otherwise false.
 */
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



/**
 * Reads a byte from memory at the specified offset.
 * 
 * This function iterates over the memory nodes to find the appropriate
 * device node that contains the specified offset. If found, it reads 
 * a byte from the device's memory. If the debug flag is set, it directly 
 * accesses the device's memory array; otherwise, it calls the device's
 * OnRead method to perform the read operation.
 * 
 * If the offset is out of range, the function returns a default value 
 * of 0xCC to indicate an error.
 *
 * @param offset The memory offset from which to read.
 * @param debug A boolean flag indicating whether to perform a debug read.
 * 
 * @return The byte read from the specified memory offset, or 0xCC if the
 *         offset is out of range.
 */

Byte Memory::Read(Word offset, bool debug)
{
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


/**
 * Writes a byte to memory at the specified offset.
 * 
 * This function iterates over the memory nodes to find the appropriate
 * device node that contains the specified offset. If found, it writes 
 * a byte to the device's memory. If the debug flag is set, it directly 
 * accesses the device's memory array; otherwise, it calls the device's
 * OnWrite method to perform the write operation.
 * 
 * If the offset is out of range, the function does nothing.
 * 
 * @param offset The memory offset to which to write.
 * @param data The byte to be written to the specified memory offset.
 * @param debug A boolean flag indicating whether to perform a debug write.
 */
void Memory::Write(Word offset, Byte data, bool debug)
{
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


/**
 * Reads a 16-bit word from memory at the specified offset.
 * 
 * This function reads two consecutive bytes from memory at the specified
 * offset and combines them into a 16-bit word. The upper byte is read
 * from the offset and the lower byte is read from the offset + 1.
 * 
 * If the debug flag is set, it directly accesses the device's memory array;
 * otherwise, it calls the device's OnRead method to perform the read operation.
 * 
 * If the offset is out of range, the function returns a default value of 0x0000
 * to indicate an error.
 *
 * @param offset The memory offset from which to read the word.
 * @param debug A boolean flag indicating whether to perform a debug read.
 * 
 * @return The word read from the specified memory offset, or 0x0000 if the
 *         offset is out of range.
 */
Word Memory::Read_Word(Word offset, bool debug)
{
    if (debug) {;}  // stop the unused argument warning

    Word ret = (Memory::Read(offset) << 8) | Memory::Read(offset + 1);
    return ret;
}


/**
 * Writes a 16-bit word to memory at the specified offset.
 * 
 * This function splits the given word into two bytes, the upper byte and the
 * lower byte, and writes each byte to consecutive memory offsets. The upper
 * byte is written to the specified offset and the lower byte is written to the
 * offset + 1.
 * 
 * If the debug flag is set, it directly accesses the device's memory array;
 * otherwise, it calls the device's OnWrite method to perform the write operation.
 * 
 * If the offset is out of range, the function does nothing.
 * 
 * @param offset The memory offset to which to write the word.
 * @param data The 16-bit word to be written to the specified memory offset.
 * @param debug A boolean flag indicating whether to perform a debug write.
 */
void Memory::Write_Word(Word offset, Word data, bool debug)
{    
    if (debug) {;}  // stop the unused argument warning

    Byte msb = (data >> 8) & 0xFF;
    Byte lsb = data & 0xff;
    Memory::Write(offset, msb);
    Memory::Write(offset + 1, lsb);
}


/**
 * Reads a 32-bit double word from memory at the specified offset.
 * 
 * This function reads four consecutive bytes from memory at the specified
 * offset and combines them into a 32-bit double word. The upper byte is read
 * from the offset, the upper-middle byte is read from the offset + 1, the
 * lower-middle byte is read from the offset + 2, and the lower byte is read
 * from the offset + 3.
 * 
 * If the debug flag is set, it directly accesses the device's memory array;
 * otherwise, it calls the device's OnRead method to perform the read operation.
 * 
 * If the offset is out of range, the function returns a default value of 0x00000000
 * to indicate an error.
 *
 * @param offset The memory offset from which to read the double word.
 * @param debug A boolean flag indicating whether to perform a debug read.
 * 
 * @return The double word read from the specified memory offset, or 0x00000000 if the
 *         offset is out of range.
 */
DWord Memory::Read_DWord(Word offset, bool debug)
{
    if (debug) {;}  // stop the unused argument warning

    DWord ret = (Memory::Read(offset+0)<<24) |
                (Memory::Read(offset+1)<<16) | 
                (Memory::Read(offset+2)<< 8) | 
                (Memory::Read(offset+3)<< 0);
    return ret;
}


/**
 * Writes a 32-bit double word to memory at the specified offset.
 * 
 * This function splits the given double word into four bytes and writes 
 * each byte to consecutive memory offsets. The most significant byte is 
 * written to the specified offset, followed by the next most significant 
 * byte, and so on, until the least significant byte is written to 
 * offset + 3.
 * 
 * If the debug flag is set, it performs no additional actions 
 * but suppresses unused variable warnings.
 * 
 * @param offset The memory offset to which to write the double word.
 * @param data The 32-bit double word to be written to the specified memory offset.
 * @param debug A boolean flag used to suppress unused variable warnings.
 */

void Memory::Write_DWord(Word offset, DWord data, bool debug)
{
    if (debug) {;}  // stop the unused argument warning

    Memory::Write(offset+0, (data>>24) & 0xFF);
    Memory::Write(offset+1, (data>>16) & 0xFF);
    Memory::Write(offset+2, (data>> 8) & 0xFF);
    Memory::Write(offset+3, (data>> 0) & 0xFF);
}


/**
 * Attaches a device to the memory map by allowing the device to build its 
 * own descriptor node and updating the memory map accordingly.
 * 
 * This function calls the device's OnAttach method to determine the size 
 * of the memory allocation for the device, setting the device's base 
 * address and size. If the allocation is successful, the device is added 
 * to the memory nodes list, and the next available address is updated. 
 * In case of failure (e.g., zero size), an error is reported, and the 
 * system is marked as not running.
 * 
 * If the address exceeds the 64k boundary, an error is reported, and the 
 * system is stopped. Additionally, a message is logged if the size is zero.
 * 
 * @param device A pointer to the device to be attached.
 * @return The size of the memory allocated for the device, 
 *         or zero if the attachment failed.
 */

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


/**
 * Displays the memory map as C++ or ASM code to the console.
 *
 * If the MEMORY_MAP_OUTPUT_CPP macro is defined, this function generates
 * C++ code that can be used to define the memory map as an enumeration.
 * If the macro is not defined, this function generates an ASM memory map
 * definition.
 *
 * In both cases, the memory map is displayed in a formatted manner,
 * with each device's name and base address displayed, followed by the
 * register names and addresses for each device.
 */
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