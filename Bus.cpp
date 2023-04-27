// Bus.cpp
//
#include <stdio.h>
#include <sstream>
#include <chrono>

#include "Bus.h"
#include "Gfx.h"

Bus* Bus::s_bus = nullptr;
Memory* Bus::s_memory = nullptr;
bool Bus::s_bIsDirty = true;
bool Bus::s_bIsRunning = true;
Gfx* Bus::_gfx = nullptr;

// temporary statics
// SDL_Window* Bus::s_window = nullptr;
// SDL_Renderer* Bus::s_renderer = nullptr;
// int Bus::s_window_width = 512 * 2;
// int Bus::s_window_height = 384 * 2;


// private constructor
Bus::Bus() {
    //printf("Bus::Bus()\n");
}   
// public destructor
Bus::~Bus() {
   // printf("Bus::~Bus()\n");
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

    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0, "Text Display Buffer (5K bytes)");
    dev = new RAM("SCREEN_BUFFER");
    s_memory->Attach(dev, 5120);    

    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0, "Device Registers (2K Bytes)");
    dev->DisplayEnum("HDW_REGS", 0x1800, "Begin Device Hardware Registers");

    dev = new Gfx("GFX_DEVICE");
    s_memory->Attach(dev); 
    _gfx = dynamic_cast<Gfx*>(dev);   

    // calculate space remaining for registers
    dev->DisplayEnum("",0, "");
    int ap = 0x2000 - s_memory->ap();
    std::string sC = ";";
    if (MEMORY_MAP_OUTPUT_CPP)
        sC = "//";
    if (COMPILE_MEMORY_MAP)
        printf("    %s %d ($%04X) bytes remaining for additional registers.\n",sC.c_str(), ap, ap);
    std::stringstream ss;
    ss << "RESERVED (" << ap << " bytes remaining)";
    dev = new RAM(ss.str().c_str());
    // dev = new RAM("RESERVED");
    s_memory->Attach(dev, ap);

    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0x2000, "User RAM (44K bytes)");
    dev = new RAM("USER_RAM");
    s_memory->Attach(dev, 0xB000);    

    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0xD000, "Paged Memory (8K)");
    dev = new RAM("PAGED_MEMORY");
    s_memory->Attach(dev, 0x2000);   

    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0xF000, "KERNEL ROM (4K bytes)");
    dev = new ROM("KERNEL_ROM");
    s_memory->Attach(dev, 0x1000);      

    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0,"Hardware Interrupt Vectors:");
    dev->DisplayEnum("HARD_RSRVD",0xfff0, "Motorola RESERVED Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_SWI3", 0xfff2, "SWI3 Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_SWI2", 0xfff4, "SWI2 Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_FIRQ", 0xfff6, "FIRQ Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_IRQ",  0xfff8, "IRQ Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_SWI",  0xfffA, "SWI / SYS Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_NMI",  0xfffC, "NMI Hardware Interrupt Vector");
    dev->DisplayEnum("HARD_RESET",0xfffE, "RESET Hardware Interrupt Vector");

    // close the memory map definitions
    if (COMPILE_MEMORY_MAP)
    {
        if (MEMORY_MAP_OUTPUT_CPP)
            printf("};  // END: enum MEMMAP\n\n");
        else
            printf("; END of definitions\n\n");

        printf("\n\nThe flag COMPILE_MEMORY_MAP is set as true, therefore this app\n");
        printf("cannot continue past this point. Set the definition of COMPILE_MEMORY_MAP\n");
        printf("to false in the file types.h and recompile to run this application.\n\n");
        Error("Copy the above enumerated definitions above to memory_map.h");
        exit(0);
    }
    s_memory->DumpMemoryMap();



    // TESTING READ/WRITE
    if (false)
    {
        Byte wr = 0xAA;
        Word addr = 0x0181;
        s_memory->write(addr, 69, true);
        s_memory->write(addr, wr);
        Byte data = s_memory->read(addr);
        printf("  $%04x:$%02x written, $%02X read\n", addr, wr, data);

        s_memory->write(0x1800, 0x7f);
        s_memory->read(0x1800);
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

    // no longer dirty
    s_bIsDirty = false;
}


void Bus::_onDeactivate()
{    
    // OnDeactivate()     // call for each attached device
    s_memory->_onDeactivate();
 
    // no longer dirty
    s_bIsDirty = true;
}

void Bus::_onUpdate()
{
    // Handle Timing
    static std::chrono::time_point<std::chrono::system_clock> tp1 = std::chrono::system_clock::now();
    static std::chrono::time_point<std::chrono::system_clock> tp2 = std::chrono::system_clock::now();

    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedTime = tp2 - tp1;
    tp1 = tp2;
    // Our time per frame coefficient
    float fElapsedTime = elapsedTime.count();
    static float fLastElapsed = fElapsedTime;
    static float fFrameTimer = fElapsedTime;

    // count frames per second
    static int fps = 0;
    static int frame_count = 0;
    static float frame_acc = fElapsedTime;
    frame_count++;
    frame_acc += fElapsedTime;    

    if (frame_acc > 1.0f + fElapsedTime)
    {
        frame_acc -= 1.0f;
		s_bus->_fps = frame_count;
		frame_count = 0;
    }    
    // float elapsedtime = time since last frame
    // OnUpdate(elapsedtime)     // call for each attached device
    s_memory->_onUpdate(fElapsedTime);  // for now zero elapsed time
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

        // swap display buffers / present
        if (_gfx)
            SDL_RenderPresent(_gfx->GetRenderer());           
    }
    _final_quit();
}


Byte Bus::read(Word offset, bool debug) 
{
    return s_memory->read(offset, debug);
}
void Bus::write(Word offset, Byte data, bool debug) 
{
    s_memory->write(offset, data, debug);
}
Word Bus::read_word(Word offset, bool debug) 
{
    return s_memory->read_word(offset, debug);
}
void Bus::write_word(Word offset, Word data, bool debug) 
{
    s_memory->write_word(offset, data, debug);
}
