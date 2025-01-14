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
#include <fstream>
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


void Memory::OnInit() 
{
    std::cout << clr::indent_push() << clr::CYAN << "Memory::OnInit() Entry" << clr::RETURN;
    for (auto &d : Memory::_memory_nodes) {
        d->OnInit();
    }    
    // _build_map();
    std::cout << clr::indent_pop() << clr::CYAN << "Memory::OnInit() Exit" << clr::RETURN;                
}



void Memory::OnQuit() 
{
    std::cout << clr::indent_push() << clr::CYAN << "Memory::OnQuit() Entry" << clr::RETURN;
	for (auto &d : Memory::_memory_nodes) {
        d->OnQuit();
    }
    for (auto &d : Memory::_memory_nodes) {
        delete d;  // Add this line to properly release memory for each device
    }    
    std::cout << clr::indent_pop() << clr::CYAN << "Memory::OnQuit() Exit" << clr::RETURN;
}


void Memory::OnActivate() 
{
    //std::cout << clr::indent_push() << clr::CYAN << "Memory::OnActivate() Entry" << clr::RETURN;
	for (auto &d : Memory::_memory_nodes) {
		d->OnActivate();
    }
    //std::cout << clr::indent_pop() << clr::CYAN << "Memory::OnActivate() Exit" << clr::RETURN;
}


void Memory::OnDeactivate() 
{
    //std::cout << clr::indent_push() << clr::CYAN << "Memory::OnDeactivate() Entry" << clr::RETURN;
	for (auto &d : Memory::_memory_nodes) {
		d->OnDeactivate();
    }
    //std::cout << clr::indent_pop() << clr::CYAN << "Memory::OnDeactivate() Exit" << clr::RETURN;
}



void Memory::OnEvent(SDL_Event* evnt) 
{
	for (auto &d : Memory::_memory_nodes) {
		d->OnEvent(evnt);
    }
}


void Memory::OnUpdate(float fElapsedTime) 
{
	for (auto &d : Memory::_memory_nodes) {
		d->OnUpdate(fElapsedTime);
    }
}



void Memory::OnRender() 
{
	for (auto &d : Memory::_memory_nodes) {
		d->OnRender();
    }
}


//////////////////////
// PUBLIC ACCESSORS //
//////////////////////



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



void Memory::Write(Word offset, Byte data, bool debug)
{
    for (auto& a : Memory::_memory_nodes)
    {
        if (offset - a->base() < a->size())
        {
            ROM* isROM = dynamic_cast<ROM*>(a);
            if (debug && isROM) {
                isROM->write_to_rom(offset, data);
            } else {
                a->OnWrite(offset, data);
            }
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

            // update the memory map
            for (auto &n : device->mapped_register) {
                _map[n.name] = n.address;
                // std::cout << "_map[\"" << n.name << "\"] = $" << clr::hex(n.address,4) << std::endl;
            }
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


void Memory::Generate_Memory_Map() 
{
    { // Generate C++ Memory_Map.hpp
        constexpr int FIRST_TAB = 4;
        constexpr int VAR_LEN = 22;
        constexpr int COMMENT_START = 34;

        std::ofstream fout(MEMORY_MAP_OUTPUT_FILE_HPP);
        if (fout.is_open())
        {
            fout << "\n\n// memory_map.hpp\n";
            fout << "#ifndef __MEMORY_MAP_HPP__\n";
            fout << "#define __MEMORY_MAP_HPP__\n";
            fout << std::endl;
            fout << "enum MEMMAP\n";
            fout << "{\n";
            fout << clr::pad(" ",FIRST_TAB) << "//  **********************************************\n";
            fout << clr::pad(" ",FIRST_TAB) << "//  * Allocated 64k Memory Mapped System Symbols *\n";
            fout << clr::pad(" ",FIRST_TAB) << "//  **********************************************\n";
            for (auto &node : Memory::_memory_nodes) 
            {
                fout << std::endl;
                fout << clr::pad(clr::pad(" ",FIRST_TAB) + clr::pad(node->name(), VAR_LEN) + "= 0x" + clr::hex(node->base(),4)+",", COMMENT_START+4) << "// START: " << node->heading << std::endl;
                for (auto &r : node->mapped_register)
                {
                    if (r.name == "") { continue; }

                    std::string _out = clr::pad(clr::pad(r.name, VAR_LEN) + "= 0x" + clr::hex(r.address, 4) +",", COMMENT_START);
                    // comments
                    for (auto &c : r.comment)
                    {
                        // horizontal rule
                        if (c == "---") {
                            fout << "// _______________________________________________________________________\n";
                        } else {
                            // comment
                            if (_out.length() > 0) { 
                                fout << clr::pad(" ",FIRST_TAB) << _out << "// " << c << std::endl;   
                                _out = "";                  
                            } else {
                                fout << clr::pad(" ",FIRST_TAB) << clr::pad(" ", COMMENT_START) << "// " << c << std::endl;
                            }
                        }
                    }
                }
            }
            fout << clr::pad(" ",FIRST_TAB) << "MEMMAP_END\n";
            fout << "}; // END: enum MEMMAP\n";
            fout << "\n\n#endif // __MEMORY_MAP_H__\n\n\n";
            fout.close();
        } else {
            Bus::Error("Unable to open mem_test.hpp", __FILE__, __LINE__);
        }
        // output that text file
        std::ifstream fin(MEMORY_MAP_OUTPUT_FILE_HPP);
        if (fin.is_open())
        {
            std::string line;
            while (std::getline(fin, line))
            {
                std::cout << line << std::endl;
            }
            fin.close();
        } else {
            Bus::Error("Unable to open mem_test.hpp", __FILE__, __LINE__);
        }
    } // END: Generate C++ Memory_Map.hpp    
    
    { // Generate Assembly Memory_Map.asm
        constexpr int FIRST_TAB = 0;
        constexpr int VAR_LEN = 22;
        constexpr int COMMENT_START = 38;
        std::ofstream fout(MEMORY_MAP_OUTPUT_FILE_ASM);
        if (fout.is_open())
        {
            fout << "\n\n;    memory_map.asm\n;\n";
            fout << clr::pad("",FIRST_TAB) << ";   **********************************************\n";
            fout << clr::pad("",FIRST_TAB) << ";   * Allocated 64k Memory Mapped System Symbols *\n";
            fout << clr::pad("",FIRST_TAB) << ";   **********************************************\n;\n";
            for (auto &node : Memory::_memory_nodes) 
            {
                fout << std::endl;
                fout << clr::pad(clr::pad("",FIRST_TAB) + clr::pad(node->name(), VAR_LEN) + "equ   0x" + clr::hex(node->base(),4), COMMENT_START) << "; START: " << node->heading << std::endl;
                for (auto &r : node->mapped_register)
                {
                    if (r.name == "") { continue; }
                    
                    std::string _out = clr::pad(clr::pad(r.name, VAR_LEN) + "equ   0x" + clr::hex(r.address, 4), COMMENT_START);
                    // comments
                    for (auto &c : r.comment)
                    {
                        // horizontal rule
                        if (c == "---") {
                            fout << "; _______________________________________________________________________\n";
                        } else {
                            // comment                        
                            if (_out.length() > 0) { 
                                fout << clr::pad("",FIRST_TAB) << _out << "; " << c << std::endl;   
                                _out = "";                  
                            } else {
                                fout << clr::pad("",FIRST_TAB) << clr::pad("", COMMENT_START) << "; " << c << std::endl;
                            }
                        }
                    }
                }
            }
            fout << "\n\n; END of memory_map.asm definitions\n\n\n";
            fout.close();
        }
        // output that text file
        std::ifstream fin(MEMORY_MAP_OUTPUT_FILE_ASM);
        if (fin.is_open())
        {
            std::string line;
            while (std::getline(fin, line))
            {
                std::cout << line << std::endl;
            }
            fin.close();
        } else {
            Bus::Error("Unable to open mem_test.hpp", __FILE__, __LINE__);
        }
    } // END: Generate Assembly Memory_Map.asm
}

// Map a device name to its address.

// Note: This method will not compile properly with switch statements
//       due to MAP() being a macro and it does not always reference
//       the constants from 'Memory_map.hpp' but sometimes references
//       the unordered_map '_map' that is generated by the devices 
//       attached to the memory map during initialization. 
//       Use if/else instead.
//
 Word Memory::Map(std::string name, std::string file = __FILE__, int line = __LINE__)   
{ 
    if (_map.find(name) == _map.end()) 
    {
        Bus::Error("Memory node '" + name + "' not found!", file, line);    
        return 0;
    }
    return _map[name]; 
}

// END: Memory.cpp