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
/******************/

#include "Bus.hpp"
#include "Memory.hpp"


/////////////////////////////
// PRIVATE SINGLETON STUFF //
/////////////////////////////

Memory::Memory()  
{ 
    std::cout << clr::LT_BLUE << "Memory Singleton Created" << clr::RETURN;  
}

Memory::~Memory() 
{ 
    std::cout << clr::LT_BLUE << "Memory Singleton Destroyed" << clr::RETURN; 
}


////////////////////////////////
// PUBLIC VIRTUAL DISPATCHERS //
////////////////////////////////

bool Memory::OnInit() 
{
	for (auto &d : Memory::_memory_nodes)
		if ( d->OnInit() == false ) 
            return false;        
    return true;
}

bool Memory::OnQuit() 
{
	for (auto &d : Memory::_memory_nodes)
        if ( d->OnQuit() == false)
            return false;
    return true;
}
bool Memory::OnActivate() 
{
	for (auto &d : Memory::_memory_nodes)
		if ( d->OnActivate() == false )
			return false;
	return true;
}
bool Memory::OnDeactivate() 
{
	for (auto &d : Memory::_memory_nodes)
		if ( d->OnDeactivate() == false )
			return false;
	return true;
}
// bool Memory::OnEvent(SDL_Event* evnt) 
bool Memory::OnEvent(SDL_Event* evnt) 
{
	for (auto &d : Memory::_memory_nodes)
		if ( d->OnEvent(evnt) == false )
			return false;
	return true;
}
bool Memory::OnUpdate(float fElapsedTime) 
{
	for (auto &d : Memory::_memory_nodes)
		if ( d->OnUpdate(fElapsedTime) == false )
			return false;
	return true;
}
bool Memory::OnRender() 
{
	for (auto &d : Memory::_memory_nodes)
		if ( d->OnRender() == false )
			return false;
	return true;
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
    Word ret = (Memory::Read(offset) << 8) | Memory::Read(offset + 1);
    return ret;
}

void Memory::Write_Word(Word offset, Word data, bool debug)
{
    Byte msb = (data >> 8) & 0xFF;
    Byte lsb = data & 0xff;
    Memory::Write(offset, msb);
    Memory::Write(offset + 1, lsb);
}

DWord Memory::Read_DWord(Word offset, bool debug)
{
    DWord ret = (Memory::Read(offset+0)<<24) |
                (Memory::Read(offset+1)<<16) | 
                (Memory::Read(offset+2)<< 8) | 
                (Memory::Read(offset+3)<< 0);
    return ret;
}

void Memory::Write_DWord(Word offset, DWord data, bool debug)
{
    Memory::Write(offset+0, (data>>24) & 0xFF);
    Memory::Write(offset+1, (data>>16) & 0xFF);
    Memory::Write(offset+2, (data>> 8) & 0xFF);
    Memory::Write(offset+3, (data>> 0) & 0xFF);
}


Word Memory::Attach(IDevice* device)
{    
    Word size = 0;
    if (device != nullptr)
    {
        // attach to the memory map and allow the new device to build its own
        // device descrptor node.
        size = device->OnAttach((Word)_next_address);     
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
            Bus::ERROR(err);
            //std::cout << "Memory node was size zero!\n";
            Bus::IsRunning(false);
        }
    }
    if (_next_address >= 65536)
    {
        // std::cout << "Memory allocation beyond 64k boundary!\n";
        Bus::ERROR("Memory allocation beyond 64k boundary!");
        // Bus::Error("Memory allocation beyond 64k boundary!");
        Bus::IsRunning(false);
    }
    return size;
}
