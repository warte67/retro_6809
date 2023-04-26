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

    // build the memoyr map
    dev = new RAM("ZERO_PAGE");
    s_memory->Attach(dev, 256);

    dev = new RAM("USER_PAGE");
    s_memory->Attach(dev, 256);    
    dev->DisplayEnum("USER_TOP",0x0200, "Top of user variable space");  

    dev = new RAM("SYSTEM_STACK");
    s_memory->Attach(dev, 512);  
    dev->DisplayEnum("STACK_TOP",0x0400, "Top of the system stack space");  


    printf("\n");
    dev->DisplayEnum("",0, "Text Display Buffer (5K bytes)");
    dev = new RAM("SCREEN_BUFFER");
    s_memory->Attach(dev, 5120);    

    printf("\n");
    dev->DisplayEnum("",0, "Device Registers (2K Bytes)");
    dev->DisplayEnum("HDW_REGS", 0x1800, "Begin Device Hardware Registers");


    // TODO:  GFX DEVICE
        dev = new RAM("GFX_DEVICE");
        s_memory->Attach(dev, 0x800);    
    // TODO:  GFX DEVICE

    printf("\n");
    dev->DisplayEnum("",0x2000, "User RAM (44K bytes)");
    dev = new RAM("USER_RAM");
    s_memory->Attach(dev, 0xB000);    

    printf("\n");
    dev->DisplayEnum("",0xD000, "Paged Memory (8K)");
    dev = new RAM("PAGED_MEM");
    s_memory->Attach(dev, 0x2000);   

    printf("\n");
    dev->DisplayEnum("",0xF000, "KERNEL ROM (4K bytes)");
    dev = new ROM("KERNEL_ROM");
    s_memory->Attach(dev, 0x1000);      

    printf("\n");
    dev->DisplayEnum("",0,"Hardware Interrupt Vectors:");
    dev->DisplayEnum("HARD_RSRVD",0xfff0, "Motorola RESERVED Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_SWI3", 0xfff2, "SWI3 Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_SWI2", 0xfff4, "SWI2 Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_FIRQ", 0xfff6, "FIRQ Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_IRQ",  0xfff8, "IRQ Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_SWI",  0xfffA, "SWI / SYS Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_NMI",  0xfffC, "NMI Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_RESET",0xfffE, "RESET Hardware Interrupt Vector");


    // RUN SOME TESTS
    // {
    //     dev = new RAM("ZERO_PAGE");
    //     s_memory->Attach(dev, 256);
    //     dev->DisplayEnum("",0,"This is a single line comment?");
    //     dev->DisplayEnum("FIRST", 0x0000,"This is a comment");
    //     dev->DisplayEnum("SECOND",0x0001,"Another Comment");
    //     dev->DisplayEnum("THIRD", 0x0002,"Something Special");
    //     dev->DisplayEnum("FOURTH",0x0003,"Third wasn't enough");
    //     printf("\n");
    //     dev = new Device("USER_PAGE");
    //     s_memory->Attach(dev, 128);    
    //     dev = new ROM("SIMPLE_ROM");
    //     s_memory->Attach(dev, 128);    
    // }
    // END TESTS




    // close the memory map definitions
    if (COMPILE_MEMORY_MAP)
    {
        if (MEMORY_MAP_OUTPUT_CPP)
            printf("};  // END: enum MEMMAP\n\n");
        else
            printf("; END of definitions\n\n");
    }
    s_memory->DumpMemoryMap();





    // TESTING READ/WRITE
    {
        Byte wr = 127;
        Word addr = 0x0181;
        s_memory->write(addr, 69, true);
        s_memory->write(addr, wr);
        Byte data = s_memory->read(addr);
        printf("  $%04x:$%02x written, $%02X read\n", addr, wr, data);
    }
    // END TESTS




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
