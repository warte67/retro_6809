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


/*****************************************************************
 * virtual int OnAttach(int nextAddr):
 * 
 * This is a virtual method that is called by the Memory class.
 * It is called when the device is attached to the memory map.
 * The device is responsible for allocating its own memory space
 * and returning the size of the allocation.
 * 
 * @param nextAddr The current address in the memory map.
 * @return The size of the allocation.
 ********************/
int  NewDevice::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnAttach() Entry" << clr::RETURN;
    if (nextAddr == 0) { ; } // stop the compiler from complaining
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnAttach() Exit" << clr::RETURN;
    return 0;   // return the size of the allocation
}


/*****************************************************************
 * bool OnInit():
 * 
 * This is a virtual method that is called by the Memory class.
 * It is called when the device is initialized.
 * The device is responsible for initializing its internal state
 * and returning true if the initialization was successful.
 * 
 * @return True if the initialization was successful.
 *********************/
bool NewDevice::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnInit() Exit" << clr::RETURN;
    return true;
}


/******************************************************************
 * virtual bool OnQuit(): 
 * 
 * This is a virtual method that is called by the Memory class.
 * It is called when the device is being shut down.
 * The device is responsible for releasing any allocated memory
 * and any other resources.
 *
 * @return True if the shutdown was successful.
 **********************/
bool NewDevice::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnQuit() Exit" << clr::RETURN;
    return true;
}


/******************************************************************
 * virtual bool OnActivate():
 * 
 * This is a virtual method that is called to activate the device.
 * It is responsible for preparing the device to be operational,
 * ensuring that all necessary conditions are met for the device to
 * function.
 * 
 * @return True if the device was successfully activated, false
 *          otherwise.
 ***********************/
bool NewDevice::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnActivate() Exit" << clr::RETURN;
    return true;
}


/******************************************************************
 * virtual bool OnDeactivate():
 *
 * This is a virtual method that is called to deactivate the device.
 * It is responsible for releasing any resources that were allocated
 * when the device was activated, and for ensuring that all necessary
 * conditions are met for the device to be safely deactivated.
 * 
 * @return True if the device was successfully deactivated, false 
 *          otherwise.
 ***********************/
bool NewDevice::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnDeactivate() Exit" << clr::RETURN;
    return true;
}


/********************************************************************
 * virtual bool OnEvent(SDL_Event* evnt):
 * 
 * This is a virtual method that handles events sent to the device.
 * It processes the given SDL_Event and performs the necessary actions
 * based on the event type. 
 *
 * @param evnt A pointer to an SDL_Event structure that contains 
 * the event data to be processed.
 *
 * @return True if the event was handled successfully, false otherwise.
 ********************************************************************/

bool NewDevice::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnEvent() Entry" << clr::RETURN;
    if (evnt) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnEvent() Exit" << clr::RETURN;
    return true;
}


/****************************************************************
 * virtual bool OnUpdate(float fElapsedTime):
 *
 * This is a virtual method that is called periodically to allow the
 * device to update its internal state. The frequency of the calls
 * is determined by the Memory class, and the time since the last
 * call is given in the fElapsedTime parameter.
 *
 * @param fElapsedTime The time, in seconds, that has elapsed since
 *  the last call to OnUpdate.
 *
 * @return True if the update was successful, false otherwise.
 ************************************************************************/
bool NewDevice::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnUpdate() Entry" << clr::RETURN;
    if (fElapsedTime==0.0f) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnUpdate() Exit" << clr::RETURN;
    return true;
}


/*************************************************************************
 * virtual bool OnRender():
 *
 * This is a virtual method that is called to draw the device's user
 * interface. It is responsible for rendering all necessary graphics
 * and for updating the display.
 * 
 * @return True if the render was successful, false otherwise.
 *************************************************************************/
bool NewDevice::OnRender()
{
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnRender() Exit" << clr::RETURN;
    return true;
}



// END: NewDevice.cpp
