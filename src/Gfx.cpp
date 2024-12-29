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


#include "Bus.hpp"
#include "Gfx.hpp"
#include "Memory.hpp"


/****************
 * Read / Write *
 ***************/ 

Byte Gfx::OnRead(Word offset) 
{ 
    Byte data = IDevice::OnRead(offset);
    // std::cout << clr::indent() << clr::CYAN << "Gfx::OnRead($"<< clr::hex(offset,4) << ") = $" << clr::hex(data,2) << "\n" << clr::RESET;

    if (offset == MAP(GFX_MODE))    { data = _gfx_mode; }
    if (offset == MAP(GFX_EMU))     { data = _gfx_emu; }

    return data;
} // END: Gfx::OnRead()

void Gfx::OnWrite(Word offset, Byte data) 
{ 
    // std::cout << clr::indent() << clr::CYAN << "Gfx::OnWrite($" << clr::hex(offset,4) << ", $" << clr::hex(data,2) << ")\n" << clr::RESET;

    if (offset == MAP(GFX_MODE))    
    { 
        if (_change_gfx_mode(data)) 
        { 
            _gfx_mode = data; 
        }
    }
    if (offset == MAP(GFX_EMU))     
    {
        if (_change_emu_mode(data)) 
        { 
            _gfx_emu = data; 
        }
    }

    IDevice::OnWrite( offset, data);
} // END: Gfx::OnWrite()


/***************************
* Constructor / Destructor *
***************************/

Gfx::Gfx() 
{ 
    std::cout << clr::indent_push() << clr::CYAN << "Gfx Created" << clr::RETURN;
    _device_name = "Gfx"; 
} // END: Gfx()

Gfx::~Gfx() 
{ 
    std::cout << clr::indent_pop() << clr::CYAN << "Gfx Destroyed" << clr::RETURN; 
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
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnAttach() Entry" << clr::RETURN;
    if (nextAddr == 0) { ; } // stop the compiler from complaining

    Word old_address=nextAddr;
    this->heading = "Start of Gfx Device Hardware Registers";

    register_node new_node;
    new_node = { "GFX_MODE", nextAddr,  {   "(Byte) Graphics Mode",
                                            "   - bit  7   = reserved" ,
                                            "   - bit  6   = 0:screen is text, ",
                                            "               1:screen is bitmap",
                                            "   - bits 4-5 = horizontal overscan: ",
                                            "               00=1x, 01=2x, 10=4x",
                                            "   - bits 2-3 = vertical overscan: ",
                                            "               00=1x, 01=2x, 10=4x",
                                            "   - bits 0-1 = Color Mode: 00=2-clr, ",
                                            "               01=4-clr, 10=16-clr, ",
                                            "               11=256-clr",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "GFX_EMU", nextAddr,  {    "(Byte) Emulation Flags",
                                            "   - bit  7    = vsync: 0=off, 1=on",
                                            "   - bit  6    = main: 0=windowed,",
                                            "                 1=fullscreen",
                                            "   - bit  5    = debug: 0=off, 1=on",
                                            "   - bit  4    = debug: 0=windowed, ",
                                            "                 1=fullscreen",                                                                                        
                                            "   - bits 2-3  = Debug Monitor 0-3",
                                            "   - bits 0-1  = Active Monitor 0-3",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    _size = nextAddr - old_address;
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnAttach() Exit" << clr::RETURN;
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
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnInit() Entry" << clr::RETURN;

    { // BEGIN OF SDL3 Initialization
        // initialize SDL3
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onInit() Error" << clr::RETURN;
            Bus::Error(SDL_GetError(), __FILE__, __LINE__);
            return false;
        }
        // create the main window
        pWindow = SDL_CreateWindow("SDL3 Retro_6809", 1280, 800, window_flags); 
        SDL_ShowWindow(pWindow);
        // create the renderer
        pRenderer = SDL_CreateRenderer(pWindow, NULL);
        SDL_SetRenderLogicalPresentation(pRenderer, 320, 200, SDL_LOGICAL_PRESENTATION_STRETCH);
    } // END OF SDL3 Initialization

    std::cout << clr::indent() << clr::CYAN << "Gfx::OnInit() Exit" << clr::RETURN;
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
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnQuit() Entry" << clr::RETURN;
    
    { // BEGIN OF SDL3 Shutdown
        if (pRenderer)
        { // destroy the renderer
            SDL_DestroyRenderer(pRenderer);
            pRenderer = nullptr;
        }
        if (pWindow)
        { // destroy the window
            SDL_DestroyWindow(pWindow);
            pWindow = nullptr;
        }
        // shutdown SDL
        SDL_Quit();
    } // END OF SDL3 Shutdown

    std::cout << clr::indent() << clr::CYAN << "Gfx::OnQuit() Exit" << clr::RETURN;
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
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnActivate() Exit" << clr::RETURN;
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
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnDeactivate() Exit" << clr::RETURN;
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
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnEvent() Entry" << clr::RETURN;
    if (evnt) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnEvent() Exit" << clr::RETURN;
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
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnUpdate() Entry" << clr::RETURN;
    if (fElapsedTime==0.0f) { ; } // stop the compiler from complaining

    { // TESTING:  Something to look at while running these tests...
        SDL_SetRenderTarget(pRenderer, NULL);
        static Uint16 r=0;
        static Uint16 g=0;
        static Uint16 b=0;
        Uint16 a=255;
        SDL_SetRenderDrawColor(pRenderer, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a);
        Uint16 t=4;
        b+=t;
        if (b>255) {  b=0; g+=t;  }
        if (g>255) {  g=0; r+=t;  }
        if (r>255) {  r=0; b+=t;  }
        SDL_RenderClear(pRenderer);
    } // END TESTING ...

    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnUpdate() Exit" << clr::RETURN;
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
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnRender() Exit" << clr::RETURN;
    return true;
}

/*************************************************************************
 * bool _change_gfx_mode(Byte mode):
 * 
 * This function processes the given graphics mode and changes the
 * internal state of the device accordingly. It returns true if the
 * mode change was successful, false otherwise.
 ************************************************************************/
bool Gfx::_change_gfx_mode(Byte mode) 
{ 
// GFX_MODE          equ   0xFE00  ; (Byte) Graphics Mode
//                                 ;    - bit  7   = reserved
//                                 ;    - bit  6   = 0:screen is text, 
//                                 ;                1:screen is bitmap
//                                 ;    - bits 4-5 = horizontal overscan: 
//                                 ;                00=1x, 01=2x, 10=4x
//                                 ;    - bits 2-3 = vertical overscan: 
//                                 ;                00=1x, 01=2x, 10=4x
//                                 ;    - bits 0-1 = Color Mode: 00=2-clr, 
//                                 ;                01=4-clr, 10=16-clr, 
//                                 ;                11=256-clr

    // horizontal overscan pattern 11 is also 4x (instead of error)
    if ((mode & 0b00110000) == 0b00110000) 
    { 
        mode &= 0b11001111;
        mode |= 0b00100000;
    }
    // vertical overscan pattern 11 is also 4x (instead of error)
    if ((mode & 0b00001100) == 0b00001100) 
    { 
        mode &= 0b11110011;
        mode |= 0b00001000;
    }

    return true; 
}

/*************************************************************************
 * bool _change_emu_mode(Byte emu):
 * 
 * This function processes the given emulation flags and changes the
 * internal state of the device accordingly. It returns true if the
 * flag change was successful, false otherwise.
 ************************************************************************/
bool Gfx::_change_emu_mode(Byte emu) 
{ 
// GFX_EMU           equ   0xFE01  ; (Byte) Emulation Flags
//                             ;    - bit  7    = vsync: 0=off, 1=on
//                             ;    - bit  6    = main: 0=windowed,
//                             ;                  1=fullscreen
//                             ;    - bit  5    = debug: 0=off, 1=on
//                             ;    - bit  4    = debug: 0=windowed, 
//                             ;                  1=fullscreen
//                             ;    - bits 2-3  = Debug Monitor 0-3
//                             ;    - bits 0-1  = Active Monitor 0-3

    if (emu)  {;}     // stop the compiler from complaining

    return true; 
}    




// END: Gfx.cpp
