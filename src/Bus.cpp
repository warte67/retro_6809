/***  Bus.cpp  ****************************
 *      ____                                           
 *     |  _ \                                          
 *     | |_) |  _   _   ___        ___   _ __    _ __  
 *     |  _ <  | | | | / __|      / __| | '_ \  | '_ \ 
 *     | |_) | | |_| | \__ \  _  | (__  | |_) | | |_) |
 *     |____/   \__,_| |___/ (_)  \___| | .__/  | .__/ 
 *                                      | |     | |    
 *                                      |_|     |_|      
 ******************/

#include "Bus.hpp"
#include "clr.hpp"


/**
 * Constructor for the Bus singleton.
 *
 * This constructor is responsible for initializing the Bus's internal state.
 * It is thread-safe and idempotent.
 */
Bus::Bus()  
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Bus Singleton Created" << clr::RETURN;
}


/**
 * Destructor for the Bus singleton.
 *
 * This destructor is responsible for releasing any resources acquired by the Bus
 * and is thread-safe.
 */
Bus::~Bus()
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Bus Singleton Destroyed" << clr::RETURN;
}


/**
 * Checks if the Bus is currently running.
 *
 * This method returns the running state of the Bus. It is thread-safe
 * by utilizing a mutex to protect the access to the shared state.
 *
 * @return True if the Bus is running, false otherwise.
 */
bool Bus::IsRunning()	    
{ 
    std::lock_guard<std::mutex> guard(_mutex_IsRunning); 
    return s_bIsRunning; 
}

/**
 * Sets the running state of the Bus.
 *
 * This method is used to signal that the Bus should
 * stop running, and is thread-safe.
 *
 * @param b If true, the Bus will continue running.
 *          If false, the Bus will stop running.
 */
void Bus::IsRunning(bool b)	
{ 
    std::lock_guard<std::mutex> guard(_mutex_IsRunning); 
    s_bIsRunning = b; 
}


/**
 * Gets the dirty state of the Bus.
 *
 * The dirty state is a flag that signals to the main loop of the Bus
 * that something has changed and that the environment should be
 * rebuilt. This might be the result of a change in the memory map,
 * the connection or disconnection of a device, or some other event
 * that requires the Bus to re-evaluate its internal state.
 *
 * @return True if the Bus is dirty, false otherwise.
 */
bool Bus::IsDirty()			
{ 
    std::lock_guard<std::mutex> guard(_mutex_IsDirty); 
    return s_bIsDirty; 
}

/**
 * Sets the dirty state of the Bus.
 *
 * This function updates the internal static flag that indicates whether the 
 * Bus is in a 'dirty' state, meaning that some changes have occurred that 
 * require the environment to be rebuilt. It uses a mutex to ensure thread 
 * safety while updating the flag.
 *
 * @param b The new state to set for the dirty flag. 'true' indicates the 
 * Bus is dirty, and 'false' indicates it is not.
 */
void Bus::IsDirty(bool b)	
{ 
    std::lock_guard<std::mutex> guard(_mutex_IsDirty); 
    s_bIsDirty = b; 
}


/**
 * Logs an error message with file and line information in a formatted manner,
 * and stops the Bus execution.
 *
 * This function outputs an error message to the console with a specific format,
 * including the file name and line number where the error occurred. The Bus
 * execution is subsequently stopped by setting the running status to false.
 *
 * @param err_msg The error message to display.
 * @param file The name of the file where the error occurred.
 * @param line The line number in the file where the error occurred.
 */

void Bus::Error(std::string err_msg, std::string file, int line)
{
	std::cout << std::endl;
	std::cout << clr::RED << "  ╭──" << clr::YELLOW << "  ERROR:  " << clr::RED << "────────====####" << clr::RETURN;
	std::cout << clr::RED << "  │" << clr::NORMAL << " in file: " << clr::WHITE << file << clr::RETURN;
	std::cout << clr::RED << "  │" << clr::NORMAL << " on line: " << clr::WHITE << line << clr::RETURN;
	std::cout << clr::RED << "  │" << clr::RETURN;// printw("\n");
	std::cout << clr::RED << "  │" << clr::NORMAL << clr::WHITE << "   " << err_msg << clr::RETURN;
	std::cout << clr::RED << "  │" << clr::RETURN;
	std::cout << clr::RED << "  ╰────────────────────====####" << clr::RETURN;
	std::cout << std::endl;

	Bus::IsRunning(false);
}  


/**
 * This is the main loop of the application. It will continue to run until the 'bIsRunning' flag is set to false.
 * It will first call the _onInit() method to initialize the application. If the initialization is successful, then it
 * will enter the main loop. In the main loop it will first check if the 'bIsDirty' flag is set to true. If it is, then
 * it will call the _onDeactivate() method to shutdown the current environment, then it will call the _onActivate() method
 * to create a new environment. After that it will set the 'bWasActivated' flag to true.
 * Then it will call the _onUpdate() method to update all of the devices, and _onEvent() to dispatch SDL events to the
 * devices, and _onRender() to render all of the devices to the screen buffers. After that it will call the
 * SDL_RenderPresent() method to present the rendered frame to the screen. Finally, it will check if the 'bIsRunning'
 * flag is set to false, and if so, it will break out of the loop.
 * If the initialization is not successful, it will print an error message and return false.
 * After the main loop is finished, it will call the _onDeactivate() method to shutdown the environment, and then it will
 * call the _onQuit() method to shutdown SDL2 and return with status.
 * @return True if the application was successfully initialized and the main loop was successfully terminated, otherwise
 * false.
 */
bool Bus::Run()
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::Run() Entry" << clr::RETURN;
    bool bWasActivated = false;

    if (_onInit())
    {
        // terminate the app when the 'bIsRunning' flag is no longer true
        while (Bus::IsRunning())
        {
            // something changed (is dirty). rebuild the environment
            if (Bus::IsDirty())
            {
                // stop the CPU from running while updating system
                //C6809::IsCpuEnabled(false);

                // shutdown the old environment
                if (bWasActivated)
                    _onDeactivate();

                // create a new environment
                _onActivate();
                bWasActivated = true;

                // wait 25 milliseconds to re-enable the CPU 
                SDL_Delay(25);

                // reenable the CPU
                // C6809::IsCpuEnabled(true);

                // no longer dirty
                Bus::IsDirty(false);           
            }
            // update all of the attached devices
            _onUpdate(0);
            
            // dispatch SDL events to the devices
            _onEvent(0);
            
            // render all of the devices to the screen buffers
            _onRender();      

            // only present for GfxCore            
            SDL_RenderPresent(pRenderer);

            // (( TESTING ))
            // Bus::IsRunning(false);
        }
        // Bus::Error("Something like a simulated error happened!", __FILE__, __LINE__);

    }
    else {
        std::cout << clr::indent_pop() << clr::ORANGE << "Bus::Run() Error" << clr::RETURN;
        return false;
    }

    // shutdown the environment
    _onDeactivate();    

    // shutdown SDL2 and return with status
    std::cout << clr::indent_pop() << clr::CYAN << "Bus::Run() Exit" << clr::RETURN;
    return _onQuit();
}


/**
 * Initializes the Bus by setting up the SDL3 environment, attaching core system devices,
 * and performing memory tests. This function should only be called once when the application starts.
 * It initializes SDL3, creates the main window and renderer, and sets the logical presentation.
 * It then attaches core system devices to the memory and performs basic memory tests to ensure
 * everything is functioning correctly. Finally, it sets the initialized flag and dumps the memory map
 * if required.
 *
 * @return True if initialization is successful, false if there's an error during SDL3 initialization,
 *         device initialization, or memory test failure.
 */
bool Bus::_onInit()
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::_onInit() Entry" << clr::RETURN;

    // ToDo: Move this to Gfx::OnInit()
    { // BEGIN OF SDL3 Initialization
        // initialize SDL3
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            // std::cout << SDL_GetError() << std::endl;
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
        
        if (_memory.OnInit() == false)
        {
            std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onInit() Error" << clr::RETURN;
            Bus::Error("Device Initialization Failure!", __FILE__, __LINE__);
            return false;
        }
    } // END OF SDL3 Initialization

    ////////////////////////////////////////
    // Core system devices with parameters
    ////////////////////////////////////////

    // Attach the core system devices
    Memory::Attach<SOFT_VECTORS>();     // 0x0000 - 0x000F
    Memory::Attach<SYSTEM_MEMORY>();    // 0x0010 - 0x03FF
    Memory::Attach<VIDEO_BUFFER>();     // 0x0400 - 0x23FF      (8k video buffer)
    Memory::Attach<USER_MEMORY>();      // 0x2400 - 0xAFFF      (42k user RAM)    
    Memory::Attach<MEMBANK>();          // 0xB000 - 0xEFFF      (16k banked memory)
    Memory::Attach<KERNEL_ROM>();       // 0xF000 - 0xFFFF      (3.5k kernel ROM)
    Memory::Attach<HDW_REGISTERS>();    // 0xFE00 - 0xFFeF      (Hardware Registers)
    Memory::Attach<ROM_VECTS>();        // 0xFFF0 - 0xFFFF      (System ROM Vectors)

    // Error Checking
    if (Memory::NextAddress() > 0x10000) {
        Bus::Error("Memory Limit Exceeded!", __FILE__, __LINE__);        
    }

    // Engange Basic Memory Tests
    int upper_bounds = Memory::NextAddress();  
    std::cout << clr::indent() << clr::GREEN << "Testing Addresses $0000-$" << clr::hex(upper_bounds,4) << " ... ";              
    Byte b = 0;
    for (int a = 0; a<upper_bounds; a++) 
    {
        Memory::Write((Word)a,b);
        if (Memory::Read((Word)a) != b) {
            Bus::Error("Memory Test Failure!", __FILE__, __LINE__);                        
        }
        b++;
    }
    std::cout << clr::YELLOW << "Memory Tests Passed!\n" << clr::RESET;

    // Dump the memory map
    if (DUMP_MEMORY_MAP)    { Memory::Display_Nodes(); }

    // Set the initialized flag
    _bWasInit = true;

    // cleanup and return
    std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onInit() Exit" << clr::RETURN;
    return true;
}


/**
 * Shuts down the Bus by calling the OnQuit() method on all memory devices and then
 * shutting down the SDL3 environment. This function should only be called once when
 * the application is exiting.
 *
 * @return True if shutdown is successful, false if there's an error during device
 *         termination or SDL3 shutdown.
 */
bool Bus::_onQuit()
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::_onQuit() Entry" << clr::RETURN;
    if (_bWasInit)   
    { 
        if (_memory.OnQuit() == false)
		{
            std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onQuit() Error" << clr::RETURN;
			Bus::Error("Device Termination Failure!", __FILE__, __LINE__);
            return false;
		}
        // Move this to Gfx::OnQuit()
        { // BEGIN OF SDL3 Shutdown
            // shut down SDL stuff
            if (pRenderer)
            {
                // SDL_DestroyRenderer(pRenderer);
                pRenderer = nullptr;
            }
            if (pWindow)
            {
                SDL_DestroyWindow(pWindow);
                pWindow = nullptr;
            }
        } // END OF SDL3 Shutdown

        _bWasInit = false;         
        SDL_Quit();
    }    
    std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onQuit() Exit" << clr::RETURN;
    return true;
}


/**
 * Calls the OnActivate() method on all memory devices to prepare them for use.
 * If any device fails to activate, an error message is logged and the function
 * will return false. The function logs the entry and exit of the activation
 * process and returns true if all devices successfully activate.
 *
 * @return True if all devices are successfully activated, otherwise false.
 */
bool Bus::_onActivate(void) 
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::_onActivate() Entry" << clr::RETURN;    
    if (_memory.OnActivate() == false)
    {
        std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onActivate() Error" << clr::RETURN;
		Bus::Error("Device Failed to Activate!", __FILE__, __LINE__);
		return false;
	}
    std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onActivate() Exit" << clr::RETURN;
    return true;
}

/**
 * _onDeactivate() is responsible for deactivating the memory components
 * associated with the bus. It logs the entry and exit of the deactivation
 * process. If any memory component fails to deactivate properly, an error
 * message is logged and the function returns false.
 *
 * @return True if all memory components are successfully deactivated,
 *         otherwise false.
 */

bool Bus::_onDeactivate(void) 
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::_onDeactivate() Entry" << clr::RETURN;
    if (_memory.OnDeactivate() == false)
	{
        std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onDeactivate() Error" << clr::RETURN;
		Bus::Error("Device Failed to Deactivate!", __FILE__, __LINE__);		        
        return false;
	}
    std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onDeactivate() Exit" << clr::RETURN;
    return true;
}


/**
 * _onEvent() is a protected virtual method called by the Bus's event handling loop.
 * It is responsible for processing SDL events and forwarding them to the memory
 * components associated with the bus. If any memory component fails to handle
 * the event properly, an error message is logged and the function returns false.
 * The function logs the entry and exit of the event handling process. If the
 * event is not handled by any memory component, the function returns true.
 *
 * @param __na A pointer to an SDL_Event structure that contains the event data.
 *             This parameter is not currently used, but is included to prevent
 *             warnings about unused function parameters.
 *
 * @return True if all memory components successfully handle the event,
 *         otherwise false.
 */
bool Bus::_onEvent(SDL_Event* __na)
{
    // std::cout << "Bus::_onEvent()\n";

    // stop the unused argument warning
    if (__na == nullptr) { ; }

    // SDL_Event evnt;
    SDL_Event evnt;
    //while (Events::EV_PollEvent(&evnt))
    while (SDL_PollEvent(&evnt))
    {
        switch (evnt.type) {
            // handle default events SDL_QUIT and ALT-X quits

            // case EV_QUIT:  
            case SDL_EVENT_QUIT:  
                Bus::IsRunning(false);
                break;
 
            // case EV_KEYDOWN:
			// // case EV_KEYUP:
            //     // Bus::IsRunning(false);
			// 	if (evnt.key.keysym.sym == SDLK_ESCAPE)		// <--- NOTE:  SDL Defined
			// 		Bus::IsRunning(false);			
            //     // [ESCAPE] KMOD_SHIFT
            //     // ... Need EV_GetModState();
            //     break;

            case SDL_EVENT_KEY_DOWN:
            {
                // [ESCAPE]  KMOD_SHIFT
                SDL_Keymod mod = SDL_GetModState();
                if (mod & SDL_KMOD_SHIFT)
                {
                    if (evnt.key.key == SDLK_ESCAPE)
                        Bus::IsRunning(false);
                }                
                if (mod & SDL_KMOD_ALT)
                {
                    if (evnt.key.key == SDLK_X)
                        Bus::IsRunning(false);
                }
                break;                
            }
        }
        // forward the event to all of the memory devices
        if (_memory.OnEvent(&evnt) == false)
            return false;
	
    }     
    return true;
}


/**
 * This is a virtual method that is called periodically to allow the
 * device to update its internal state. The frequency of the calls
 * is determined by the Memory class, and the time since the last
 * call is given in the fElapsedTime parameter.
 *
 * @param fElapsedTime The time, in seconds, that has elapsed since
 *  the last call to OnUpdate.
 *
 * @return True if the update was successful, false otherwise.
 */
bool Bus::_onUpdate(float __na) 
{
    // stop the unused argument warning
    if (__na == 0.0f) { ; }
    // std::cout << "Bus::_onUpdate()\n";

    // NOTE: This might be a good place to monitor the
    // 		terminal dimensions. Perhaps a new event
    //		can be triggered from here.
    //1
    //		Be sure to fetch the initial console 
    //		terminal demensions during OnInit() too.

    static int s_width=0, s_height=0;
    int w, h;
    clr::get_terminal_size(w, h);       // getmaxyx(stdscr,h,w);
    

    if (s_width != w || s_height != h)
    {        
        s_width = w;
        s_height = h;
        // fire a terminal resize event from here
        // ...
        std::cout << "Terminal Resized: W:" << std::to_string(w) << " H:" << std::to_string(h) << "\n";
    }
    if (_memory.OnUpdate(__na) == false)
	{
		Bus::Error("Device Update Failure!", __FILE__, __LINE__);
        return false;
	}
    return true;
}


/**
 * _onRender() is responsible for rendering the Bus's visual output.
 * It first attempts to render all memory nodes by calling their OnRender() method.
 * If any memory node fails to render, it logs an error and returns false.
 * If successful, it proceeds to perform a test render operation by cycling
 * through colors and clearing the render target.
 *
 * @return True if all operations were successful, false otherwise.
 */
bool Bus::_onRender(void) 
{
    // std::cout << "Bus::_onRender()\n";
    if (_memory.OnRender() == false)
	{
		Bus::Error("Device Render Failure!", __FILE__, __LINE__);
        return false;    
	}

    // TESTING:  Something to look at while running these tests...
        // 
            //
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
            //
        //
    // END TESTING ...

    return true;
}
