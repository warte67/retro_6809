// Bus.cpp
//
#include <stdio.h>

#include "Bus.h"

Bus* Bus::s_bus = nullptr;
Bus::Bus() {
    printf("Bus::Bus()\n");
}   
Bus::~Bus() {
    printf("Bus::~Bus()\n");
}
void Bus::_quit() {
    if (s_bus != nullptr)
        delete s_bus;
}

// main loop: returns false on error
void Bus::run()
{


    _quit();
}

// fetch a pointer to the Bus singleton
Bus* Bus::GetInstance()
{
    if (s_bus == nullptr) {
        s_bus = new Bus();        
    }
    return s_bus;
}
