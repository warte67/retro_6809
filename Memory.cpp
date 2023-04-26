////
// Memory.cpp
//////

#include "Memory.h"
#include "Bus.h"




void Memory::DumpMemoryMap()
{
    printf("  Dumping the memory map allocation info:\n");
    for(auto &o : _memoryNodes)
    {
        printf("    $%04x-$%04x %s\n", 
            o->Base(), 
            o->Base() + o->Size() - 1,
            o->Name().c_str()
        );
    }
}

Word Memory::Attach(Device* dev) 
{
    Word size = 0;
    if (dev != nullptr)
    {
        size = dev->OnAttach();
        dev->Base(_lastAddress);
        dev->Size(size);
        _lastAddress += size;
        _memoryNodes.push_back(dev);
    }
    if (size > 65536)
        Bus::Error("Memory allocation beyond 64k boundary!");
    return size;
}

Memory::Memory()
{
    printf("Memory::Memory()\n");
}
Memory::~Memory()
{
    printf("Memory::~Memory()\n");
}

void Memory::_onInit() 
{
    printf("Memory::_onInit()\n");
    for (auto &m : _memoryNodes)
        m->OnInit();
}
void Memory::_onQuit() 
{
    printf("Memory::_onQuit()\n");
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
    printf("Memory::_onActivate()\n");
    for (auto &m : _memoryNodes)
        m->OnActivate();    
}
void Memory::_onDeactivate() 
{
    printf("Memory::_onDeactivate()\n");
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