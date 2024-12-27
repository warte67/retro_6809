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

#include "Bus.hpp"
#include "clr.hpp"


Bus::Bus()  
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Bus Singleton Created" << clr::RETURN;
}

Bus::~Bus()
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Bus Singleton Destroyed" << clr::RETURN;
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
        // Bus::Error("Something like a simulated error happened!");

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



// this should only ever run exactly once when the app starts
bool Bus::_onInit()
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::_onInit() Entry" << clr::RETURN;

	// TESTING....

        // initialize SDL2
		if (SDL_Init(SDL_INIT_EVERYTHING))
		{
			// std::cout << SDL_GetError() << std::endl;
            std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onInit() Error" << clr::RETURN;
			Bus::Error(SDL_GetError());
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
        std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onInit() Error" << clr::RETURN;
		Bus::Error("Device Initialization Failure!");
        return false;
	}

    ////////////////////////////////////////
    // Core system devices with parameters
    ////////////////////////////////////////
    
    // Memory::Attach<SOFT_VECTORS>();     // 0x0000 - 0x000F
    //Memory::Attach<SYSTEM_MEMORY>();    // 0x0010 - 0x03FF
    //Memory::Attach<VIDEO_BUFFER>();     // 0x0400 - 0x23FF      (8k video buffer)
    //Memory::Attach<USER_RAM>();         // 0x0400 - 0x23FF      (42k user RAM)


    Memory::Attach<RAM_64K>(0x10000);       // allocate 64k for testing
    

        // BEGIN TEST...

            if (MEM_TESTS) {
                int upper_bounds = Memory::NextAddress(); 
                Byte b = 0;
                for (int a = 0; a<upper_bounds; a++) {
                    Memory::Write((Word)a,b);
                    if (Memory::Read((Word)a) != b) {
                        Bus::Error("Memory Test Failure!");                        
                    }
                    //std::cout << "Write(0x" << std::hex << (int)a << ", 0x" << (int)b << ") Read=" << (int)Memory::Read(a) << "\n";
                    b++;
                }
                //std::cout << "Memory::_next_address = 0x" << std::hex << Memory::NextAddress() << std::endl;
            } // END MEM_TESTS
            std::cout << clr::indent() << clr::YELLOW << "Memory Tests Passed!\n" << clr::RESET;

            if (DUMP_MEMORY_MAP)    { Memory::Display_Nodes(); }

        // ...END TEST

    // ROM* rom = Memory::Attach<ROM>("./roms/system.hex", 0x8000);
    // RAM* ram = Memory::Attach<RAM>(0x8000);
    // Video* video = Memory::Attach<Video>(320, 200, 8);
    // Sound* sound = Memory::Attach<Sound>(44100, 2);

    _bWasInit = true;

    std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onInit() Exit" << clr::RETURN;
    return true;
}

// this should only ever run exactly once when the app shuts down
bool Bus::_onQuit()
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::_onQuit() Entry" << clr::RETURN;
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
        if (_memory.OnQuit() == false)
		{
            std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onQuit() Error" << clr::RETURN;
			Bus::Error("Device Termination Failure!");
            return false;
		}
    }    
    std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onQuit() Exit" << clr::RETURN;
    return true;
}


bool Bus::_onActivate(void) 
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::_onActivate() Entry" << clr::RETURN;    
    if (_memory.OnActivate() == false)
    {
        std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onActivate() Error" << clr::RETURN;
		Bus::Error("Device Failed to Activate!");
		return false;
	}
    std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onActivate() Exit" << clr::RETURN;
    return true;
}

bool Bus::_onDeactivate(void) 
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::_onDeactivate() Entry" << clr::RETURN;
    if (_memory.OnDeactivate() == false)
	{
        std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onDeactivate() Error" << clr::RETURN;
		Bus::Error("Device Failed to Deactivate!");		        
        return false;
	}
    std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onDeactivate() Exit" << clr::RETURN;
    return true;
}

// bool Bus::_onEvent(SDL_Event* __na)
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
        // printw("Terminal Resized: W:%d H:%d\n",  w, h); refresh();
    }
    if (_memory.OnUpdate(__na) == false)
	{
		Bus::Error("Device Update Failure!");
        return false;
	}
    return true;
}

bool Bus::_onRender(void) 
{
    // std::cout << "Bus::_onRender()\n";
    if (_memory.OnRender() == false)
	{
		Bus::Error("Device Render Failure!");
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
