/*** GPU.cpp *******************************************
 *     _____ _____  _    _                        
 *    / ____|  __ \| |  | |                       
 *   | |  __| |__) | |  | |       ___ _ __  _ __  
 *   | | |_ |  ___/| |  | |      / __| '_ \| '_ \ 
 *   | |__| | |    | |__| |  _  | (__| |_) | |_) |
 *    \_____|_|     \____/  (_)  \___| .__/| .__/ 
 *                                   | |   | |    
 *                                   |_|   |_|    
 *
 * This is the base class for the primary graphics devices. It provides the
 * basic methods for reading and writing to the device's memory.
 * 
 ************************************/


#include "Bus.hpp"
#include "GPU.hpp"
#include "Memory.hpp"


/****************
 * Read / Write *
 ***************/ 

/**
 * This is the read callback for the GPU device. It is called whenever the CPU
 * reads from a location in the device's memory space.
 *
 * The read callback is responsible for returning the value stored at the
 * specified memory location.
 *
 * If the offset is out of range, the function returns a default value of 0xCC
 * to indicate an error.
 *
 * @param offset The memory offset from which to read.
 * 
 * @return The value read from the specified memory location, or 0xCC if the
 *         offset is out of range.
 */
Byte GPU::OnRead(Word offset) 
{ 
    Byte data = IDevice::OnRead(offset);    // use this for other devices
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnRead($"<< clr::hex(offset,4) << ") = $" << clr::hex(data,2) << "\n" << clr::RESET;

    if (offset == MAP(GPU_STD_MODE))    { data = _gpu_std_mode; }
    if (offset == MAP(GPU_EXT_MODE))    { data = _gpu_ext_mode; }
    if (offset == MAP(GPU_EMULATION))   { data = _gpu_emu_mode; }

    return data;
} // END: GPU::OnRead()


/**
 * This is the write callback for the GPU device. It is called whenever the CPU
 * writes to a location in the device's memory space.
 *
 * The write callback is responsible for modifying the device state according
 * to the value written to the specified memory location.
 *
 * If the offset is out of range, the function does nothing.
 *
 * @param offset The memory offset to which to write.
 * @param data The value to be written to the specified memory location.
 */
void GPU::OnWrite(Word offset, Byte data) 
{ 
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnWrite($" << clr::hex(offset,4) << ", $" << clr::hex(data,2) << ")\n" << clr::RESET;
    if (offset == MAP(GPU_STD_MODE))    
    { 
        data = _gpu_std_mode = _change_std_mode(data);
    }
    if (offset == MAP(GPU_EXT_MODE))    
    { 
        data = _gpu_ext_mode = _change_ext_mode(data);
    }
    if (offset == MAP(GPU_EMULATION))     
    {
        data = _gpu_emu_mode = _change_emu_mode(data);
    }

    IDevice::OnWrite( offset, data);
} // END: GPU::OnWrite()





/***************************
* Constructor / Destructor *
***************************/

GPU::GPU() 
{ 
    std::cout << clr::indent_push() << clr::CYAN << "GPU Created" << clr::RETURN;
    _device_name = "GPU"; 
} // END: GPU()

GPU::~GPU() 
{ 
    std::cout << clr::indent_pop() << clr::CYAN << "GPU Destroyed" << clr::RETURN; 
} // END: ~GPU()



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
int  GPU::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Entry" << clr::RETURN;
    if (nextAddr == 0) { ; } // stop the compiler from complaining

    Word old_address=nextAddr;
    this->heading = "GPU Device Hardware Registers";

    register_node new_node;

    new_node = { "GPU_ENABLE", nextAddr,  {   "(Byte) Bitflag Enables",
                                            "   - bits 5-7 = reserved",
                                            "   - bit  4   = 0:disable ext display,",
                                            "                1:enable ext display",
                                            "   - bit  3   = 0:disable std display,",
                                            "                1:enable std display",
                                            "   - bit  2   = 0:disable sprites,",
                                            "                1:enable sprites",
                                            "   - bit  1   = 0:disable tilemap,",
                                            "                1:enable tilemap",
                                            "   - bit  0   = 0:disable mouse cursor,",
                                            "                1:enable mouse cursor",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "GPU_STD_MODE", nextAddr,  {   "(Byte) Standard Graphics Mode",
                                            "   - bit  7   = 0:screen is text, ",
                                            "                1:screen is bitmap",
                                            "   - bit  6   = video timing: ",
                                            "                0=512x384, 1=640x400",
                                            "   - bits 4-5 = horizontal overscan: ",
                                            "                00=1x, 01=2x, 10=4x, 11=8x",
                                            "   - bits 2-3 = vertical overscan: ",
                                            "                00=1x, 01=2x, 10=4x",
                                            "   - bits 0-1 = Color Mode: 00=2-clr, ",
                                            "                01=4-clr, 10=16-clr, ",
                                            "                11=256-clr",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "GPU_EXT_MODE", nextAddr,  {   "(Byte) Extended Graphics Mode",
                                            "   - bit  7   = reserved",
                                            "   - bit  6   = video timing: ",
                                            "                0=512x384, 1=640x400",
                                            "   - bits 4-5 = horizontal overscan: ",
                                            "                00=1x, 01=2x, 10=4x, 11=8x",
                                            "   - bits 2-3 = vertical overscan: ",
                                            "                00=1x, 01=2x, 10=4x",
                                            "   - bits 0-1 = Color Mode: 00=2-clr, ",
                                            "                01=4-clr, 10=16-clr, ",
                                            "                11=256-clr",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "GPU_EMULATION", nextAddr,  {    "(Byte) Emulation Flags",
                                            "   - bit  7    = vsync: 0=off, 1=on",
                                            "   - bit  6    = main: 0=windowed,",
                                            "                 1=fullscreen",
                                            "   - bit  5    = debug: 0=off, 1=on",
                                            "   - bit  4    = debug: 0=windowed, ",
                                            "                 1=fullscreen",                                                                                        
                                            "   - bits 2-3  = Active Monitor 0-3",
                                            "   - bits 0-1  = Debug Monitor 0-3",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    _size = nextAddr - old_address;
    std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Exit" << clr::RETURN;
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
bool GPU::OnInit()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnInit() Entry" << clr::RETURN;

    { // BEGIN OF SDL3 Initialization
        // initialize SDL3
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onInit() Error" << clr::RETURN;
            Bus::Error(SDL_GetError(), __FILE__, __LINE__);
            return false;
        }
        // create the main window
        pWindow = SDL_CreateWindow("SDL3 Retro_6809", initial_width, initial_height, window_flags); 
        SDL_ShowWindow(pWindow);
        // create the renderer
        pRenderer = SDL_CreateRenderer(pWindow, NULL);
        // SDL_SetRenderLogicalPresentation(pRenderer, 320, 200, SDL_LOGICAL_PRESENTATION_STRETCH);
        SDL_SetRenderLogicalPresentation(pRenderer, 320, 200, SDL_LOGICAL_PRESENTATION_LETTERBOX);

        
    } // END OF SDL3 Initialization

    std::cout << clr::indent() << clr::CYAN << "GPU::OnInit() Exit" << clr::RETURN;
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
bool GPU::OnQuit()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnQuit() Entry" << clr::RETURN;
    
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

    std::cout << clr::indent() << clr::CYAN << "GPU::OnQuit() Exit" << clr::RETURN;
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
bool GPU::OnActivate()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Exit" << clr::RETURN;
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
bool GPU::OnDeactivate()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::CYAN << "GPU::OnDeactivate() Exit" << clr::RETURN;
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

bool GPU::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::CYAN << "GPU::OnEvent() Entry" << clr::RETURN;
    if (evnt) { ; } // stop the compiler from complaining
    
    switch (evnt->type) 
    {
        case SDL_EVENT_KEY_DOWN:
        {
            // [ESCAPE]  KMOD_SHIFT
            SDL_Keymod mod = SDL_GetModState();

            // TESTING [F] and [F-SHIFT] to toggle fullscreen
            if (evnt->key.key == SDLK_F)
            {
                if (mod & SDL_KMOD_SHIFT) {
                    // Byte data = Memory::Read( MAP(GFX_EMU) );
                    // data &= 0b10111111;
                    // Memory::Write(MAP(GFX_EMU), data);
                    SDL_SetWindowFullscreen(pWindow, false);                    
                } else {
                    // Byte data = Memory::Read( MAP(GFX_EMU) );
                    // data |= 0b01000000  ;
                    // Memory::Write(MAP(GFX_EMU), data);
                    SDL_SetWindowFullscreen(pWindow, true);
                }                    
            } // END: if (evnt->key.key == SDLK_F)
            break;            
        } // END: case SDL_EVENT_KEY_DOWN

        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        {            
            _gpu_emu_mode |= 0b01000000; // Set bit 6 = ENTER FULLSCREEN
            std::cout << "SDL_EVENT_WINDOW_ENTER_FULLSCREEN (0x" << clr::hex(_gpu_emu_mode,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        {
            _gpu_emu_mode &= 0b10111111; // Clear bit 6 = LEAVE FULLSCREEN
            std::cout << "SDL_EVENT_WINDOW_LEAVE_FULLSCREEN (0x" << clr::hex(_gpu_emu_mode,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

    } // END: switch  (evnt->type) 

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnEvent() Exit" << clr::RETURN;
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
bool GPU::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::CYAN << "GPU::OnUpdate() Entry" << clr::RETURN;
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

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnUpdate() Exit" << clr::RETURN;
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
bool GPU::OnRender()
{
    //std::cout << clr::indent() << clr::CYAN << "GPU::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::CYAN << "GPU::OnRender() Exit" << clr::RETURN;
    return true;
}




/**
 * Updates the standard graphics mode of the GPU based on GPU_STD_MODE.
 * The mode byte is structured as follows:
 *   - Bit 7: Screen type (0 = text, 1 = bitmap)
 *   - Bit 6: Video timing (0 = 512x384, 1 = 640x400)
 *   - Bits 4-5: Horizontal overscan (00 = 1x, 01 = 2x, 10 = 4x, 11 = 8x)
 *   - Bits 2-3: Vertical overscan (00 = 1x, 01 = 2x, 10 = 4x)
 *   - Bits 0-1: Color mode (00 = 2-color, 01 = 4-color, 10 = 16-color, 11 = 256-color)
 *
 * @param mode The mode byte representing the desired standard graphics mode.
 * @return The updated mode configuration.
 */
Byte GPU::_change_std_mode(Byte mode) 
{ 
    if (mode)  {;}     // stop the compiler from complaining

    // Byte data = _gpu_std_mode;
    Byte data = mode;

    // bit 7 = 0:screen is text, 1:screen is bitmap
    // ...

    // bit 6 = video timing: 0=512x384, 1=640x400
    // ...

    // bits 4-5 = horizontal overscan: 00=1x, 01=2x, 10=4x, 11=8x
    // ...  

    // bits 2-3 = vertical overscan: 00=1x, 01=2x, 10=4x
    // ...

    // bits 0-1 = Color Mode: 00=2-clr, 01=4-clr, 10=16-clr, 11=256-clr 
    // ...

    return data; 
}


/**
 * Updates the extended graphics mode of the GPU based on GPU_EXT_MODE.
 * The mode byte is structured as follows:
 *   - Bit 7: Reserved
 *   - Bit 6: Video timing (0 = 512x384, 1 = 640x400)
 *   - Bits 4-5: Horizontal overscan (00 = 1x, 01 = 2x, 10 = 4x, 11 = 8x)
 *   - Bits 2-3: Vertical overscan (00 = 1x, 01 = 2x, 10 = 4x)
 *   - Bits 0-1: Color mode (00 = 2-color, 01 = 4-color, 10 = 16-color, 11 = 256-color)
 *
 * @param mode The mode byte representing the desired extended graphics mode.
 * @return The updated mode configuration.
 */
Byte GPU::_change_ext_mode(Byte mode) 
{ 
    if (mode)  {;}     // stop the compiler from complaining

    // Byte data = _gpu_ext_mode;
    Byte data = mode;

    // bit 7 = reserved
    // ...

    // bit 6 = video timing: 0=512x384, 1=640x400   
    // ...

    // bits 4-5 = horizontal overscan: 00=1x, 01=2x, 10=4x, 11=8x
    // ...  

    // bits 2-3 = vertical overscan: 00=1x, 01=2x, 10=4x
    // ...

    // bits 0-1 = Color Mode: 00=2-clr, 01=4-clr, 10=16-clr, 11=256-clr 
    // ...
    
    return data; 
}



/**
 * Updates the emulation mode of the GPU based on GPU_EMULATION.
 * The mode byte is structured as follows:
 *   - Bit 7: VSync (0 = off, 1 = on)
 *   - Bit 6: Main display (0 = windowed, 1 = fullscreen)
 *   - Bit 5: Debug display (0 = off, 1 = on)
 *   - Bit 4: Debug display (0 = windowed, 1 = fullscreen)
 *   - Bits 2-3: Active monitor (0-3)
 *   - Bits 0-1: Debug monitor (0-3)
 *
 * @param emu The mode byte representing the desired emulation mode.
 * @return The updated mode configuration.
 */
Byte GPU::_change_emu_mode(Byte mode) 
{ 
    if (mode)  {;}     // stop the compiler from complaining

    // Byte data = _gpu_emu_mode;
    Byte data = mode;

    // bit 7: vsync: 0=off, 1=on
    // ...

    // bit 6: main: 0=windowed, 1=fullscreen
    // ...

    // bit 5: debug: 0=off, 1=on
    // ...

    // bit 4: debug: 0=windowed, 1=fullscreen
    // ...   

    // bits 2-3: Active Monitor 0-3 
    // ...   

    // bits 0-1: Debug Monitor 0-3
    // ...  

    return data; 
}    




// END: Gfx.cpp
