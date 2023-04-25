// Bus.cpp
//
#include <stdio.h>

#include "Bus.h"

Bus* Bus::s_bus = nullptr;
bool Bus::s_bIsDirty = true;
bool Bus::s_bIsRunning = true;

// temporary statics
SDL_Window* Bus::s_window = nullptr;
SDL_Renderer* Bus::s_renderer = nullptr;


// private constructor
Bus::Bus() {
    printf("Bus::Bus()\n");
}   
// public destructor
Bus::~Bus() {
    printf("Bus::~Bus()\n");
}
// internal shutdown
void Bus::_quit() {
    if (s_bus != nullptr)
        delete s_bus;
}

void Bus::Error(std::string sErr)
{
    printf("%s\n%s\n", sErr.c_str(), SDL_GetError());
    s_bIsRunning = false;
}

// fetch a pointer to the Bus singleton
Bus* Bus::GetInstance()
{
    if (s_bus == nullptr) {
        s_bus = new Bus();        
    }
    return s_bus;
}

void Bus::_onetime_init()
{    
    // setup/initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Bus::Error("Error initializing SDL!");
    }    

    // OnInit()     // call for each attached device

    // start up external threads
    // ...    
}

void Bus::_final_quit()
{    
    _onDeactivate();

    // OnQuit()     // call for each attached device
    
    // close SDL
    SDL_Quit();

    // shutdown external threads
    // ...

    _quit();
}

void Bus::_onActivate()
{    
    // OnActivate()     // call for each attached device

    // move to the Gfx object
        Uint32 window_flags = 0;
        Uint32 render_flags = SDL_RENDERER_ACCELERATED;
        int pixel_scan = 2;
        int window_width = 640 * pixel_scan;
        int window_height = 400 * pixel_scan;
        // create a new environment
        s_window = SDL_CreateWindow("Retro_6809",
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        window_width, 
                        window_height, 
                        window_flags);
        s_renderer = SDL_CreateRenderer(s_window, -1, render_flags);
    // no longer dirty
    s_bIsDirty = false;
}


void Bus::_onDeactivate()
{    
    // OnDeactivate()     // call for each attached device

    // move all of this to the Gfx device object
        // shutdown SDL based devices
        // destroy texture
        //SDL_DestroyTexture(tex);

        // destroy renderer
        if (s_renderer != nullptr) 
        {
            SDL_DestroyRenderer(s_renderer);
            s_renderer=nullptr;
        }
        // destroy window
        if (s_window != nullptr)
        {
            SDL_DestroyWindow(s_window);
        }    
    // no longer dirty
    s_bIsDirty = true;
}

void Bus::_onUpdate()
{
    // float elapsedtime = time since last frame
    // OnUpdate(elapsedtime)     // call for each attached device
}

void Bus::_onEvent()
{
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt))
    {
        // OnEvent(evnt)

        switch (evnt.type) {

        case SDL_QUIT:
            // handling of close button
            s_bIsRunning = false;
            break;
        
        case SDL_KEYDOWN:
            if (evnt.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                s_bIsRunning = false;
                break;
            }
        }
    }    
}

void Bus::_onRender()
{
    // simply clears the screen
    SDL_RenderClear(s_renderer);
    SDL_RenderCopy(s_renderer, NULL, NULL, NULL); 

    // OnRender()     // call for each attached device
}



// main loop
void Bus::Run()
{
    _onetime_init();
    while (s_bIsRunning)
    {
        // create a new environment
        if (s_bIsDirty)
        {
            // shutdown the old environment
            _onDeactivate();
            // create a new environment
            _onActivate();
        }
        _onUpdate();
        _onEvent();
        _onRender();  
        // present the frame
        SDL_RenderPresent(s_renderer);           
    }
    _final_quit();
}
