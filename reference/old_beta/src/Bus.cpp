/***  Bus.cpp  ****************************
*     ____                              
*    |  _ \                             
*    | |_) |_   _ ___   ___ _ __  _ __  
*    |  _ <| | | / __| / __| '_ \| '_ \ 
*    | |_) | |_| \__ \| (__| |_) | |_) |
*    |____/ \__,_|___(_)___| .__/| .__/ 
*                          | |   | |    
*                          |_|   |_|    
******************/

#include <iostream>     // temporary for initial text to terminal

#include "Bus.hpp"

Bus::Bus()  
{ 
    // Events::GetInstance();  // instantiate the event sub-subsystem

    std::cout << clr::LT_BLUE << "Bus Singleton Created" << clr::RETURN;  
}

Bus::~Bus()
{ 
    std::cout << clr::LT_BLUE << "Bus Singleton Destroyed" << clr::RETURN; 
}


inline static std::mutex _mutex_IsRunning;
bool Bus::IsRunning()	    
{ 
    std::lock_guard<std::mutex> guard(_mutex_IsRunning); 
    return s_bIsRunning; 
}
void Bus::IsRunning(bool b)	
{ 
    std::lock_guard<std::mutex> guard(_mutex_IsRunning); 
    s_bIsRunning = b; 
}


inline static std::mutex _mutex_IsDirty;
bool Bus::IsDirty()			
{ 
    std::lock_guard<std::mutex> guard(_mutex_IsDirty); 
    return s_bIsDirty; 
}
void Bus::IsDirty(bool b)	
{ 
    std::lock_guard<std::mutex> guard(_mutex_IsDirty); 
    s_bIsDirty = b; 
}

void Bus::Error(std::string err_msg, std::string file, int line)
{
	// std::string err_msg = "Something like a simulated error happened!";
	std::cout << std::endl;
	std::cout << clr::RED << "  ╭──" << clr::YELLOW << "  ERROR:  " << clr::RED << "────────====####" << clr::RETURN;
	std::cout << clr::RED << "  │" << clr::NORMAL << " in file: " << clr::WHITE << file << clr::RETURN;
	std::cout << clr::RED << "  │" << clr::NORMAL << " on line: " << clr::WHITE << line << clr::RETURN;
	std::cout << clr::RED << "  │" << clr::RETURN;
	std::cout << clr::RED << "  │" << clr::NORMAL << clr::WHITE << "   " << err_msg << clr::RETURN;
	std::cout << clr::RED << "  │" << clr::RETURN;
	std::cout << clr::RED << "  ╰────────────────────====####" << clr::RETURN;
	std::cout << std::endl;
	Bus::IsRunning(false);
}  

bool Bus::Run()
{
    std::cout << "Bus::Run()\n";
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
            // Gfx::Present();
            SDL_RenderPresent(pRenderer);

            // (( TESTING ))
            // Bus::IsRunning(false);
        }
    }
    else
        return false;

    // shutdown the environment
    _onDeactivate();    

    // shutdown SDL2 and return with status
    return _onQuit();
}



// this should only ever run exactly once when the app starts
bool Bus::_onInit()
{
    std::cout << "Bus::_onInit()\n";



	// TESTING....

        // initialize SDL2
		if (SDL_Init(SDL_INIT_EVERYTHING))
		{
			// std::cout << SDL_GetError() << std::endl;
			Bus::ERROR(SDL_GetError());
			return false;
		}

        // create the main window
		pWindow = SDL_CreateWindow("temp", 
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1024,640, 
			window_flags);
		SDL_ShowWindow(pWindow);

        // create the renderer
        pRenderer = SDL_CreateRenderer(pWindow, -1, renderer_flags);
	// END TESTING


    
    if (_memory.OnInit() == false)
	{
		Bus::ERROR("Device Initialization Failure!");
        return false;
	}
    _bWasInit = true;
    return true;
}

// this should only ever run exactly once when the app shuts down
bool Bus::_onQuit()
{
    std::cout << "Bus::_onQuit()\n";
    bool bRet = false;
    if (_bWasInit)   
    { 
        // shut down SDL stuff
        if (pRenderer)
        {
            SDL_DestroyRenderer(pRenderer);
            pRenderer = nullptr;
        }
        if (pWindow)
        {
            SDL_DestroyWindow(pWindow);
            pWindow = nullptr;
        }

        _bWasInit = false;         
        SDL_Quit();
        bRet = true;
        if (_memory.OnQuit() == false)
		{
			Bus::ERROR("Device Termination Failure!");
            bRet = false;
		}
    }    
    return bRet;
}


bool Bus::_onActivate(void) 
{
    std::cout << "Bus::_onActivate()\n";
    if (_memory.OnActivate() == false)
    {
		Bus::ERROR("Device Failed to Activate!");
		return false;
	}
    return true;
}

bool Bus::_onDeactivate(void) 
{
    std::cout << "Bus::_onDeactivate()\n";
    if (_memory.OnDeactivate() == false)
	{
		Bus::ERROR("Device Failed to Deactivate!");		
        return false;
	}
    return true;
}

// bool Bus::_onEvent(SDL_Event* __na)
bool Bus::_onEvent(SDL_Event* __na)
{
    // std::cout << "Bus::_onEvent()\n";

    // SDL_Event evnt;
    SDL_Event evnt;
    //while (Events::EV_PollEvent(&evnt))
    while (SDL_PollEvent(&evnt))
    {
        switch (evnt.type) {
            // handle default events SDL_QUIT and ALT-X quits

            // case EV_QUIT:  
            case SDL_QUIT:  
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

            case SDL_KEYDOWN:
            {
                // [ESCAPE]  KMOD_SHIFT
                SDL_Keymod mod = SDL_GetModState();
                if (mod & KMOD_SHIFT)
                {
                    if (evnt.key.keysym.sym == SDLK_ESCAPE)
                        Bus::IsRunning(false);
                }                
                if (mod & KMOD_ALT)
                {
                    if (evnt.key.keysym.sym == SDLK_x)
                        Bus::IsRunning(false);
                }
                break;                
            }
        }
        if (_memory.OnEvent(&evnt) == false)		// this is likely broken
            return false;							// this is likely broken
	
    }     
    return true;
}

bool Bus::_onUpdate(float __na) 
{
    // std::cout << "Bus::_onUpdate()\n";

    // NOTE: This might be a good place to monitor the
    // 		terminal dimensions. Perhaps a new event
    //		can be triggered from here.
    //
    //		Be sure to fetch the initial console 
    //		terminal demensions during OnInit() too.

    static int s_width=0, s_height=0;
    int w, h;
    clr::get_terminal_size(w, h);
    if (s_width != w || s_height != h)
    {        
        s_width = w;
        s_height = h;
        // fire a terminal resize event from here
        // ...
        std::cout << "Terminal Resized: W:" << w << "  H:" << h << std::endl;
    }
    if (_memory.OnUpdate(__na) == false)
	{
		Bus::ERROR("Device Update Failure!");
        return false;
	}
    return true;
}

bool Bus::_onRender(void) 
{
    // std::cout << "Bus::_onRender()\n";
    if (_memory.OnRender() == false)
	{
		Bus::ERROR("Device Render Failure!");
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

