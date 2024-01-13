////
// Memory.cpp
//////

#include "Memory.h"
#include "Bus.h"



Byte Memory::read(Word offset, bool debug) 
{    
    for (auto &a : _memoryNodes)
    {
        if (offset - a->Base() < a->Size())
        {
            Byte data = a->read(offset, debug);
            return data;
        }
    }
    return 0xCC;
}

void Memory::write(Word offset, Byte data, bool debug) 
{
    for (auto &a : _memoryNodes)
    {
        if (offset - a->Base() < a->Size())
        {
            a->write(offset, data, debug);
            return;
        }
    }    
}
Word Memory::read_word(Word offset, bool debug) 
{
	Word ret = (read(offset) << 8) | read(offset + 1);
	return ret;
}
void Memory::write_word(Word offset, Word data, bool debug) 
{    
	Byte lsb = (data >> 8) & 0xFF;
	Byte msb = data & 0xff;
	write(offset, msb);
	write(offset + 1, lsb);
}



Word Memory::Attach(Device* dev, Word size) 
{    
    if (dev != nullptr)
    {
        if (size == 0)
            size = dev->OnAttach((Word)_lastAddress);     
        else
            dev->DisplayEnum(dev->Name(), _lastAddress, "");
        dev->Base(_lastAddress);
        dev->Size(size);
        _lastAddress += size;               
        _memoryNodes.push_back(dev);
    }
    if (size > 65536)
        Bus::Error("Memory allocation beyond 64k boundary!");
    return size;
}

void Memory::DumpMemoryMap()
{
    printf("  Simple Memory Map:\n");
    for(auto &o : _memoryNodes)
    {
        printf("    $%04X-$%04X %s\n", 
            o->Base(), 
            o->Base() + o->Size() - 1,
            o->Name().c_str()
        );
    }
}

Memory::Memory()
{
    //printf("Memory::Memory()\n");
    if (COMPILE_MEMORY_MAP)
    {
        if (MEMORY_MAP_OUTPUT_CPP)
        {
            printf("\n");
            printf("enum MEMMAP\n");
            printf("{\n");
            printf("    //  **********************************************\n");
            printf("    //  * Allocated 64k Memory Mapped System Symbols *\n");
            printf("    //  **********************************************\n");
        }
        else
        {
            printf(";  **********************************************\n");
            printf(";  * Allocated 64k Memory Mapped System Symbols *\n");
            printf(";  **********************************************\n");
        }
    }
}
Memory::~Memory()
{
    //printf("Memory::~Memory()\n");
}

void Memory::_onInit() 
{
    //printf("Memory::_onInit()\n");
    for (auto &m : _memoryNodes)
        m->OnInit();
}
void Memory::_onQuit() 
{
    //printf("Memory::_onQuit()\n");
    // close out each memory node
    for (auto& m : _memoryNodes)
        m->OnQuit();
    // release all allocated memory nodes
    for(auto &a : _memoryNodes)
        delete a;
    // clear the memory node list
    _memoryNodes.clear();
}
void Memory::_onActivate() 
{
    //printf("Memory::_onActivate()\n");
    for (auto &m : _memoryNodes)
        m->OnActivate();    
}
void Memory::_onDeactivate() 
{
    //printf("Memory::_onDeactivate()\n");
    for (auto &m : _memoryNodes)
        m->OnDeactivate();      
}
void Memory::_onEvent(SDL_Event* evnt) 
{
    //printf("Memory::_onEvent()\n");
    for (auto &m : _memoryNodes)
        m->OnEvent(evnt);      
}
void Memory::_onUpdate(float fElapsedTime)
{
    //printf("Memory::_onUpdate()\n");
    for (auto &m : _memoryNodes)
        m->OnUpdate(fElapsedTime);      
}
void Memory::_onRender() 
{
    //printf("Memory::_onRender()\n");
    for (auto &m : _memoryNodes)
        m->OnRender();       
}

// ROM /////////////////////////////////////

// only write to ROM when the debug flag is true
void ROM::write(Word offset, Byte data, bool debug) 
{
    if (debug)
        if (offset - m_base < m_size)
            m_memory[(Word)(offset - m_base)] = data;
}