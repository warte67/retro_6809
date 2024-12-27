/*** Gfx.cpp *******************************************
 *       _____    __                                   
 *      / ____|  / _|                                  
 *     | |  __  | |_  __  __       ___   _ __    _ __  
 *     | | |_ | |  _| \ \/ /      / __| | '_ \  | '_ \ 
 *     | |__| | | |    >  <   _  | (__  | |_) | | |_) |
 *      \_____| |_|   /_/\_\ (_)  \___| | .__/  | .__/ 
 *                                      | |     | |    
 *                                      |_|     |_|    
 *
 * This is the base class for all graphics devices. It provides the
 * basic methods for reading and writing to the device's memory.
 * 
 ************************************/



#include "Gfx.hpp"


/****************
 * Read / Write *
 ***************/ 

Byte Gfx::OnRead(Word offset) 
{ 
    return IDevice::OnRead(offset); 
} // END: Gfx::OnRead()

void Gfx::OnWrite(Word offset, Byte data) 
{ 
    IDevice::OnWrite( offset, data); 
} // END: Gfx::OnWrite()



/***************************
* Constructor / Destructor *
***************************/

Gfx::Gfx() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Gfx Created" << clr::RETURN;
    _device_name = "Gfx"; 
} // END: Gfx()

Gfx::~Gfx() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Gfx Destroyed" << clr::RETURN; 
} // END: ~Gfx()



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
int  Gfx::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnAttach() Entry" << clr::RETURN;
    if (nextAddr == 0) { ; } // stop the compiler from complaining

    Word old_address=nextAddr;
    this->heading = "Start of Gfx Device Hardware Registers";

    register_node new_node;
    new_node = { "GFX_MODE", nextAddr,  {   "(Byte) Graphics Mode",
                                            "   - bits 0-4 = Resoltion Modes (0-15)",
                                            "   - bits 5-6 = Color Depth (0-7)",
                                            "   - bits 7   = 0:text, 1:bitmap" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    _size = nextAddr - old_address;
    std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnAttach() Exit" << clr::RETURN;
    return _size;   // return the size of the allocation
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
bool Gfx::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnInit() Exit" << clr::RETURN;
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
bool Gfx::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnQuit() Exit" << clr::RETURN;
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
bool Gfx::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnActivate() Exit" << clr::RETURN;
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
bool Gfx::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnDeactivate() Exit" << clr::RETURN;
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

bool Gfx::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnEvent() Entry" << clr::RETURN;
    if (evnt) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnEvent() Exit" << clr::RETURN;
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
bool Gfx::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnUpdate() Entry" << clr::RETURN;
    if (fElapsedTime==0.0f) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnUpdate() Exit" << clr::RETURN;
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
bool Gfx::OnRender()
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "Gfx::OnRender() Exit" << clr::RETURN;
    return true;
}



// END: Gfx.cpp
