/*** NewDevice.cpp *******************************************
 *      _   _                     _____                   _                     _                     
 *     | \ | |                   |  __ \                 (_)                   | |                    
 *     |  \| |   ___  __      __ | |  | |   ___  __   __  _    ___    ___      | |__    _ __    _ __  
 *     | . ` |  / _ \ \ \ /\ / / | |  | |  / _ \ \ \ / / | |  / __|  / _ \     | '_ \  | '_ \  | '_ \ 
 *     | |\  | |  __/  \ V  V /  | |__| | |  __/  \ V /  | | | (__  |  __/  _  | | | | | |_) | | |_) |
 *     |_| \_|  \___|   \_/\_/   |_____/   \___|   \_/   |_|  \___|  \___| (_) |_| |_| | .__/  | .__/ 
 *                                                                                     | |     | |    
 *                                                                                     |_|     |_|    
 * This file serves meerly as a template as a
 * starting point for new devices.
 * 
 ************************************/

#include "NewDevice.hpp"


/****************
 * Read / Write *
 ***************/ 

Byte NewDevice::OnRead(Word offset) 
{ 
    Byte data = IDevice::OnRead(offset);
    // ...

    // std::cout << "MAP(GFX_MODE) = $" << clr::hex(MAP(GFX_MODE),4) << "\n";
    // std::cout << "MAP(GFX_EMU) = $" << clr::hex(MAP(GFX_EMU),4) << "\n";

    // ...
    return data; 
} // END: NewDevice::OnRead()

void NewDevice::OnWrite(Word offset, Byte data) 
{ 
    IDevice::OnWrite( offset, data);
} // END: NewDevice::OnWrite()



/***************************
* Constructor / Destructor *
***************************/

NewDevice::NewDevice() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "NewDevice Created" << clr::RETURN;
    _device_name = "NewDevice"; 
} // END: NewDevice()

NewDevice::~NewDevice() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "NewDevice Destroyed" << clr::RETURN; 
} // END: ~NewDevice()



/******************
* Virtual Methods *
******************/



int  NewDevice::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnAttach() Entry" << clr::RETURN;
    if (nextAddr == 0) { ; } // stop the compiler from complaining
    
    Word old_address=nextAddr;
    this->heading = "Default New Device";

    mapped_register.push_back({ "NEW_DEVICE", nextAddr, nullptr, nullptr,  { "Default New Device"} }); nextAddr+=2;

    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnAttach() Exit" << clr::RETURN;
    
    _size = nextAddr - old_address;
    return _size; 
}



void NewDevice::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnInit() Exit" << clr::RETURN;
}



void NewDevice::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnQuit() Exit" << clr::RETURN;
}



void NewDevice::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnActivate() Exit" << clr::RETURN;
}



void NewDevice::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnDeactivate() Exit" << clr::RETURN;
}


void NewDevice::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnEvent() Entry" << clr::RETURN;
    if (evnt) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnEvent() Exit" << clr::RETURN;
}


void NewDevice::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnUpdate() Entry" << clr::RETURN;
    if (fElapsedTime==0.0f) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnUpdate() Exit" << clr::RETURN;
}


void NewDevice::OnRender()
{
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnRender() Exit" << clr::RETURN;
}



// END: NewDevice.cpp
