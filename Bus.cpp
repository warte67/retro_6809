// Bus.cpp
//
#include <stdio.h>

#include "Bus.h"

Bus* Bus::s_bus = nullptr;
bool Bus::s_bIsDirty = true;
bool Bus::s_bIsRunning = true;

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

void Bus::OnInit()
{
    
}

// main loop: returns false on error
void Bus::run()
{
    // OnInit()

        // move to the Gfx object
        SDL_Window* pWin = nullptr;
        Uint32 window_flags = 0;
        SDL_Renderer* rend = nullptr;
        Uint32 render_flags = SDL_RENDERER_ACCELERATED;
        int pixel_scan = 2;
        int window_width = 640 * pixel_scan;
        int window_height = 400 * pixel_scan;
    
        // setup/initialize SDL
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            Bus::Error("Error initializing SDL!");
        }    

        // start up external threads
        // ...

    while (s_bIsRunning)
    {
        // create a new environment
        if (s_bIsDirty)
        {
            // shutdown the old environment
            //  OnDeactivate()
            //  ...

            // create a new environment
            // OnActivate()
                pWin = SDL_CreateWindow("Retro_6809",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        window_width, 
                                        window_height, 
                                        window_flags);
                rend = SDL_CreateRenderer(pWin, -1, render_flags);
                // no longer dirty
                s_bIsDirty = false;
        }

        // OnUpdate(elapsedTime);
        // ...

        // Poll for SDL events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // OnEvent(event)

            switch (event.type) {
 
            case SDL_QUIT:
                // handling of close button
                s_bIsRunning = false;
                break;
            
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    s_bIsRunning = false;
                    break;
                }
            }
        }

        // OnRender()
                // clears the screen
                SDL_RenderClear(rend);
                SDL_RenderCopy(rend, NULL, NULL, NULL);     

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);           
    }

    // OnQuit()
            // OnDeactivate()
            // ...

            // shutdown SDL based devices
            // destroy texture
            //SDL_DestroyTexture(tex);
        
            // destroy renderer
            if (rend != nullptr) 
                SDL_DestroyRenderer(rend);
        
            // destroy window
            if (pWin != nullptr)
                SDL_DestroyWindow(pWin);
            
            // close SDL
            SDL_Quit();

            // shutdown external threads
            // ...

            _quit();
}
