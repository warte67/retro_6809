// Bus.cpp
//
#include <stdio.h>

#include "Bus.h"

Bus* Bus::s_bus = nullptr;
Memory* Bus::s_memory = nullptr;
bool Bus::s_bIsDirty = true;
bool Bus::s_bIsRunning = true;

// temporary statics
SDL_Window* Bus::s_window = nullptr;
SDL_Renderer* Bus::s_renderer = nullptr;
int Bus::s_window_width = 512 * 2;
int Bus::s_window_height = 384 * 2;


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
    // create the memory map object
    s_memory = new Memory();

    // install the memory devices
    Device* dev = nullptr;

    dev = new RAM("Zero Page");
    s_memory->Attach(dev, 256);

    dev = new Device("User Page");
    s_memory->Attach(dev, 128);    

    dev = new ROM("Simple ROM");
    s_memory->Attach(dev, 128);    

    s_memory->DumpMemoryMap();

    Byte wr = 127;
    Word addr = 0x0001;
    s_memory->write(addr, wr);
    Byte data = s_memory->read(addr);
    printf("  $%04x:$%02x written, $%02X read\n", addr, wr, data);

    // setup/initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Bus::Error("Error initializing SDL!");
    }    

    // OnInit()     // call for each attached device
    s_memory->_onInit();

    // start up external threads
    // ...    
}

void Bus::_final_quit()
{    
    _onDeactivate();

    // OnQuit()     // call for each attached device
    if (s_memory)
    {
        s_memory->_onQuit();
        delete s_memory;
        s_memory = nullptr;
    }
        
    // close SDL
    SDL_Quit();

    // shutdown external threads
    // ...

    _quit();
}

void Bus::_onActivate()
{    
    // OnActivate()     // call for each attached device
    s_memory->_onActivate();

    // move to the Gfx object
        Uint32 window_flags = 0;
        Uint32 render_flags = SDL_RENDERER_ACCELERATED;
        // int pixel_scan = 2;
        // int window_width = 512 * pixel_scan;
        // int window_height = 384 * pixel_scan;
        // create a new environment
        s_window = SDL_CreateWindow("Retro_6809",
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        s_window_width, 
                        s_window_height, 
                        window_flags);
        s_renderer = SDL_CreateRenderer(s_window, -1, render_flags);

        for (int t=0; t<4; t++)
        {
            SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 255);
            SDL_RenderClear(s_renderer);
            SDL_RenderCopy(s_renderer, NULL, NULL, NULL);
            SDL_RenderPresent(s_renderer); 
        }
        // SDL_RenderClear(s_renderer);
        // SDL_RenderCopy(s_renderer, NULL, NULL, NULL);
        // SDL_RenderPresent(s_renderer); 


    // no longer dirty
    s_bIsDirty = false;
}


void Bus::_onDeactivate()
{    
    // OnDeactivate()     // call for each attached device
    s_memory->_onDeactivate();

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
    s_memory->_onUpdate(0.0f);  // for now zero elapsed time
}

void Bus::_onEvent()
{
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt))
    {
        // OnEvent(evnt)
        s_memory->_onEvent(&evnt);

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
    //SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 255);
    //SDL_RenderClear(s_renderer);
    //SDL_RenderCopy(s_renderer, NULL, NULL, NULL); 

    // testing
    for (int t = 0; t < 10000; t++)
    {
        int x = rand() % (s_window_width);
        int y = rand() % (s_window_height);
        Uint8 r = rand() % 256;
        Uint8 g = rand() % 256;
        Uint8 b = rand() % 256;
        SDL_SetRenderDrawColor(s_renderer, r, g, b, 255);
        SDL_RenderDrawPoint(s_renderer, x, y);
        SDL_RenderCopy(s_renderer, NULL, NULL, NULL); 
    }
    // OnRender()     // call for each attached device
    s_memory->_onRender();
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
