//////////
//
//  Bus.h  (thread-safe singleton)
//
/////////////

#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>
#include "C6809.h"
#include "Bus.h"
#include "Keyboard.h"
#include "Gamepad.h"
#include "FileIO.h"
#include "Math.h"

Bus::Bus()
{
	// std::cout << "Bus::Bus()\n";

    if (COMPILE_MEMORY_MAP)
    {
        if (MEMORY_MAP_OUTPUT_CPP)		// C++ version
        {
			printf("\n\n");
			printf("// memory_map.h\n");
			printf("#ifndef __MEMORY_MAP_H__\n");
			printf("#define __MEMORY_MAP_H__\n");
            printf("\n");
            printf("enum MEMMAP\n");
            printf("{\n");
            printf("    //  **********************************************\n");
            printf("    //  * Allocated 64k Memory Mapped System Symbols *\n");
            printf("    //  **********************************************\n");
        }
		else							// 6809 ASM version
		{
            printf(";  **********************************************\n");
            printf(";  * Allocated 64k Memory Mapped System Symbols *\n");
            printf(";  **********************************************\n");
        }
    }

    // BUILD THE MEMORY MAP /////////////////////


	// SOFT VECTORS
	Device* dev = nullptr;
	dev = new RAM("SOFT_VECTORS");
    // dev->DisplayEnum("",0, "");

	int addr = Attach(dev, 16);
    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0,"Software Interrupt Vectors:");
    dev->DisplayEnum("SOFT_EXEC", 0x0000, "Exec Software Interrupt Vector");
    dev->DisplayEnum("SOFT_SWI3", 0x0002, "SWI3 Software Interrupt Vector");
    dev->DisplayEnum("SOFT_SWI2", 0x0004, "SWI2 Software Interrupt Vector");
    dev->DisplayEnum("SOFT_FIRQ", 0x0006, "FIRQ Software Interrupt Vector");
    dev->DisplayEnum("SOFT_IRQ",  0x0008, "IRQ Software Interrupt Vector");
    dev->DisplayEnum("SOFT_SWI",  0x000A, "SWI / SYS Software Interrupt Vector");
    dev->DisplayEnum("SOFT_NMI",  0x000C, "NMI Software Interrupt Vector");
    dev->DisplayEnum("SOFT_RESET",0x000E, "RESET Software Interrupt Vector");	
	dev->DisplayEnum("",0, "");

	// zero page 256 bytes	
    dev = new RAM("ZERO_PAGE");
    addr += Attach(dev, 256 - 16);	

    // file input buffer
    // dev = new RAM("FIO_BUFFER");
    dev = new RAM("FIO_BUFFER");
    addr += Attach(dev, 256);
    dev->DisplayEnum("FIO_BFR_TOP", 0x01FF, "Top of the File Input/Output Buffer");

    //// user stack 256 bytes
    //dev = new RAM("USER_STACK");
    //addr += Attach(dev, 256);
    //dev->DisplayEnum("USTACK_TOP", 0x0200, "Top of the user stack space");

	// system stack 512 bytes
    dev = new RAM("SYSTEM_STACK");
    addr += Attach(dev, 512);  
    dev->DisplayEnum("SSTACK_TOP",0x0400, "Top of the system stack space");  	

	// display buffer 6k bytes
    dev = new RAM("SCREEN_BUFFER");
    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0, "Display Buffer");
    addr += Attach(dev, 6*1024);

	// start hardware registers
	dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0, "Device Registers:");
    dev->DisplayEnum("HDW_REGS", addr, "Begin Device Hardware Registers");

	// base graphics device
    m_gfx = new Gfx();
    addr += Attach(m_gfx);

    // Keyboard Device
    m_keyboard = new Keyboard();
    addr += Attach(m_keyboard);

    // Gamepad Controller Device
    dev = new Gamepad();
    addr += Attach(dev);

    // FileIO Device
    dev = new FileIO();
    addr += Attach(dev);

    // Math Device
    dev = new Math();
    addr += Attach(dev);

    // More Devices?
    // ...





    // calculate space remaining for registers
    dev->DisplayEnum("",0, "");
    int ap = 0x3000 - this->ap();
    std::string sC = ";";
    if (MEMORY_MAP_OUTPUT_CPP)
        sC = "//";
    if (COMPILE_MEMORY_MAP)
        printf("    %s %d ($%04X) bytes remaining for additional registers.\n",sC.c_str(), ap, ap);
    dev = new RAM("RESERVED");
    addr += Attach(dev, ap);

	// user ram
    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0x3000, "User RAM (32K)");
    dev = new RAM("USER_RAM");
    addr += Attach(dev, 32*1024);    

	// paged RAM
    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0xD000, "Paged RAM (8K)");
    dev = new RAM("PAGED_RAM");
    addr += Attach(dev, 8*1024);   

	// paged ROM
    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0xF000, "PAGED ROM (8K bytes)");
    dev = new ROM("PAGED_ROM");
    addr += Attach(dev, 8*1024);      

	// KERNEL ROM
    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0xF000, "KERNEL ROM (4K bytes)");
    dev = new ROM("KERNEL_ROM");
    addr += Attach(dev, 4*1024);   

	//printf("ADDR: 0x%04X \n", addr);

	// ROM VECTORS
    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0,"Hardware Interrupt Vectors:");
    dev->DisplayEnum("HARD_EXEC", 0xfff0, "EXEC Hardware Interrupt Vector");
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
		{
            printf("};  // END: enum MEMMAP\n\n");

			printf("\n");
			printf("#endif // __MEMORY_MAP_H__\n");
			printf("\n\n");
		}
        else
            printf("; END of definitions\n\n");

        printf("\n\nThe flag COMPILE_MEMORY_MAP is set as true, therefore this app\n");
        printf("cannot continue past this point. Set the definition of COMPILE_MEMORY_MAP\n");
        printf("to false in the file types.h and recompile to run this application.\n\n");
        Error("Copy the above enumerated definitions above to memory_map.h");
        //exit(0);
    }
    DumpMemoryMap();

    // setup/initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Bus::Error("Error initializing SDL!");
    }    

	// load up the memory map
	load_hex("asm/kernel_f000.hex");	

    // called before the CPU thread starts running
    _onInit();

	// Install the CPU
	m_cpu = new C6809(this);
	m_cpuThread = std::thread(&Bus::_cpuThread);
	bCpuEnabled = true;
}

void Bus::_cpuThread()
{
    while (s_bIsRunning)
    {
        // main CPU clock
        using clock = std::chrono::system_clock;
        using sec = std::chrono::duration<double, std::nano>;
        static auto before_CPU = clock::now();
        const sec duration = clock::now() - before_CPU;

        // 1000.f = 1mhz, 500.0f = 2mhz, etc...
        float cycle_time = 8000.0f;     // in nanohertz
        int cpu_speed = Bus::Inst()._sys_state;
        switch (cpu_speed)
        {
            case 0x00: cycle_time = 39900.0f;   break;      // 25 khz
            case 0x01: cycle_time = 19900.0f;   break;      // 50 khz
            case 0x02: cycle_time = 9900.0f;    break;      // 100 khz
            case 0x03: cycle_time = 4900.0f;    break;      // 200 khz
            case 0x04: cycle_time = 2900.0f;    break;      // 333 khz
            case 0x05: cycle_time = 2300.0f;    break;      // 416 khz
            case 0x06: cycle_time = 1900.0f;    break;      // 500 khz
            case 0x07: cycle_time = 1500.0f;    break;      // 625 khz
            case 0x08: cycle_time = 1200.0f;    break;      // 769 khz
            case 0x09: cycle_time = 1100.0f;    break;      // 833 khz
            case 0x0A: cycle_time = 900.0f;     break;      // 1.0 mhz
            case 0x0B: cycle_time = 600.0f;     break;      // 1.4 mhz
            case 0x0C: cycle_time = 400.0f;     break;      // 2.0 mhz
            case 0x0D: cycle_time = 200.0f;     break;      // 3.3 mhz
            case 0x0E: cycle_time = 100.0f;     break;      // 5 mhz
            case 0x0F: cycle_time = 0.0f;       break;      // Unmetered (10 mhz)
        }
        //if (cpu_speed)
        {
            if (duration.count() > cycle_time)
            {
                before_CPU = clock::now();
                //if (!C6809::s_bHalted)
                if (bCpuEnabled)
                {
                    m_cpu->clock();
                    Bus::Inst()._avg_cpu_cycle_time = duration.count();
                }
            }
        }
        //else
        //{
        //    if (bCpuEnabled)
        //    {
        //        m_cpu->clock();
        //        Bus::Inst()._avg_cpu_cycle_time = duration.count();
        //    }
        //}
    }
}


Bus::~Bus()
{
	// std::cout << "Bus::~Bus()\n";

	// shutdown the CPU thread
	m_cpuThread.join();

	// Remove the CPU
	if (m_cpu)
	{
		delete m_cpu;
		m_cpu = nullptr;
	}

    // delete the attached devices
    for (auto& d : Bus::_memoryNodes)
        delete d;

    // close SDL
    SDL_Quit();
}

void Bus::_onInit() 
{
	// std::cout << "void Bus::_onInit()\n";

	for (auto &d : Bus::_memoryNodes)
		d->OnInit();
}

void Bus::_onQuit() 
{
	// std::cout << "void Bus::_onQuit()\n";

	for (auto &d : Bus::_memoryNodes)
		d->OnQuit();
}

void Bus::_onDeactivate() 
{
	// std::cout << "void Bus::_onDeactivate()\n";

	for (auto &d : Bus::_memoryNodes)
		d->OnDeactivate();
}

void Bus::_onActivate() 
{
	//std::cout << "void Bus::_onActivate()\n";

	for (auto &d : Bus::_memoryNodes)
		d->OnActivate();
}

void Bus::_onUpdate() 
{
	// std::cout << "void Bus::_onUpdate()\n";

    // update the clock divider
    clockDivider();

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
    // static int fps = 0;
    static int frame_count = 0;
    static float frame_acc = fElapsedTime;
    frame_count++;
    frame_acc += fElapsedTime;    

    if (frame_acc > 0.25f + fElapsedTime)
    {
        frame_acc -= 0.25f;
		_fps = frame_count * 4;
		frame_count = 0;

		// clean this up (get rid of the requirement for th GFX friend class Bus bullshit)
		std::string sTitle = "Retro 6809";
		sTitle += "  FPS: ";
		sTitle += std::to_string(_fps);

        _sys_cpu_speed = (int)(1.0f / (_avg_cpu_cycle_time / 1000000.0f));

        sTitle += "   CPU_SPEED: " + std::to_string(_sys_cpu_speed) + " khz.";
		if (m_gfx)
			if (m_gfx->_window)
				 SDL_SetWindowTitle(m_gfx->_window, sTitle.c_str());
    }    

	// update the devices
	for (auto &d : Bus::_memoryNodes)
		d->OnUpdate(fElapsedTime);	
}

void Bus::_onEvent() 
{
	// std::cout << "void Bus::_onEvent() \n";

    SDL_Event evnt;
    while (SDL_PollEvent(&evnt))
    {
        // OnEvent(evnt)
		for (auto &d : Bus::_memoryNodes)
			d->OnEvent(&evnt);		

        switch (evnt.type) {

		// handle default events SDL_QUIT and [ESC] quits
        case SDL_QUIT:
            // handling of close button
            s_bIsRunning = false;
            break;
        
        case SDL_KEYDOWN:
            //if (evnt.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            if (evnt.key.keysym.sym == SDLK_x)
            {
                if (SDL_GetModState() & KMOD_ALT)
                    s_bIsRunning = false;
            }
            break;
        }
    }    
}

void Bus::_onRender() 
{
	// std::cout << "void Bus::_onRender()\n";

	for (auto &d : Bus::_memoryNodes)
		d->OnRender();
}

void Bus::_onPresent() 
{
	// std::cout << "void Bus::_onPresent()\n";

	// only a present for Gfx
	m_gfx->OnPresent();
}

void Bus::Error(const std::string& sErr)
{
	std::cout << "\n    ERROR: " << sErr << SDL_GetError() << "\n\n";
    s_bIsRunning = false;
}

// MEMORY RELATED /////////////////////////////////////////////////



Byte Bus::Read(Word offset, bool debug)
{
    for (auto& a : Bus::_memoryNodes)
    {
        if (offset - a->Base() < a->Size())
        {
            if (debug)
                if (offset - a->Base() < a->Size())
                    return a->_memory((Word)(offset - a->Base()));

            return a->read(offset, debug);
        }
    }
    return 0xCC;
}

void Bus::Write(Word offset, Byte data, bool debug)
{
    for (auto& a : Bus::_memoryNodes)
    {
        if (offset - a->Base() < a->Size())
        {
            if (debug)
            {
                if (offset - a->Base() < a->Size())
                    a->_memory((Word)(offset - a->Base()), data);
                return;
            }
            a->write(offset, data, debug);
            return;
        }
    }
}
Word Bus::Read_Word(Word offset, bool debug)
{
    Word ret = (Bus::Read(offset) << 8) | Bus::Read(offset + 1);
    return ret;
}
void Bus::Write_Word(Word offset, Word data, bool debug)
{
    Byte msb = (data >> 8) & 0xFF;
    Byte lsb = data & 0xff;
    Bus::Write(offset, msb);
    Bus::Write(offset + 1, lsb);
}



Word Bus::Attach(Device* dev, Word size) 
{    
    if (dev != nullptr)
    {
        if (size == 0)
            size = dev->OnAttach((Word)_lastAddress);     
        else
            dev->DisplayEnum(dev->Name(), _lastAddress, "");
        dev->Base(_lastAddress);
        dev->Size(size);
        _lastAddress += size;               
        Bus::_memoryNodes.push_back(dev);
    }
    if (size > 65536)
        Bus::Error("Memory allocation beyond 64k boundary!");
    return size;
}

void Bus::DumpMemoryMap()
{
    std::cout << "  Simple Memory Map:\n";
    for(auto &o : Bus::_memoryNodes)
    {
        printf("    $%04X-$%04X %s\n", 		
            o->Base(), 
            o->Base() + o->Size() - 1,
            o->Name().c_str()
        );
    }
}



// main loop //////////////////////////////////////////////////////
void Bus::Run()
{
	// called after Bus() constructor for each attached device
    // _onInit();	

    while (s_bIsRunning)
    {
        // create a new environment
        if (s_bIsDirty)
        {
            // shutdown the old environment
            _onDeactivate();
            // create a new environment
            _onActivate();
			// no longer dirty
			s_bIsDirty = false;
        }
        _onUpdate();
        _onEvent();
        _onRender();      
		_onPresent();
		//Bus::Error("Automatic Exit");
    }
	// shutdown the old environment
	_onDeactivate();	

	// m_thread.join();

	// one time shutdown	
    _onQuit();
}

// load_hex helpers
Byte Bus::_fread_hex_byte(std::ifstream& ifs)
{
	char str[3];
	long l;
	ifs.get(str[0]);
	ifs.get(str[1]);
	str[2] = '\0';	
	l = strtol(str, NULL, 16);
	return (Byte)(l & 0xff);	
}
Word Bus::_fread_hex_word(std::ifstream& ifs)
{
	Word ret;
	ret = _fread_hex_byte(ifs);
	ret <<= 8;
	ret |= _fread_hex_byte(ifs);
	return ret;				
}
void Bus::load_hex(const char* filename)
{
	// // lambda to convert integer to hex string
	// auto hex = [](uint32_t n, uint8_t digits)
	// {
	// 	std::string s(digits, '0');
	// 	for (int i = digits - 1; i >= 0; i--, n >>= 4)
	// 		s[i] = "0123456789ABCDEF"[n & 0xF];
	// 	return s;
	// };

	std::ifstream ifs(filename);
	if (!ifs.is_open())
	{
		printf("UNABLE TO OPEN FILE '%s'\n", filename);		
		return;
	}
	bool done = false;
	char c;	
	while (!done)
	{
		Byte n, t;
		Word addr;	
		Byte b;
		ifs.get(c);	// skip the leading ":"
		n = _fread_hex_byte(ifs);		// byte count for this line
		addr = _fread_hex_word(ifs);	// fetch the begin address		
		t = _fread_hex_byte(ifs);		// record type
		if (t == 0x00) 
		{
			while (n--) 
			{
				b = _fread_hex_byte(ifs);
				// std::cout << "0x" << hex(addr,4) << ":";
				// std::cout << "0x" << hex(b, 2) << std::endl;
				Bus::Write(addr, b, true);
				++addr;
			}
			// Read and discard checksum byte
			(void)_fread_hex_byte(ifs);	
			// skip the junk at the end of the line	
			if (ifs.peek() == '\r')	ifs.get(c);
			if (ifs.peek() == '\n')	ifs.get(c);
		}
		else if (t == 0x01) 
			done = true;
	}
	// close and return
	ifs.close();
	
	return;
}

Byte Bus::clock_div(Byte& cl_div, int bit)
{
    if (bit > 7)    bit = 7;
    double count[] =
    { // pulse width:   // frequency:
        8.33333,        // 60 hz
        16.66667,       // 30 hz
        33.33333,       // 15 hz
        66.66667,       // 7.5 hz
        133.33333,      // 3.75 hz
        266.66667,      // 1.875 hz
        533.33333,      // 0.9375 hz
        1066.66667,     // 0.46875
    };

    using clock = std::chrono::system_clock;
    using sec = std::chrono::duration<double, std::milli>;
    static auto before0 = clock::now();
    static auto before1 = clock::now();
    static auto before2 = clock::now();
    static auto before3 = clock::now();
    static auto before4 = clock::now();
    static auto before5 = clock::now();
    static auto before6 = clock::now();
    static auto before7 = clock::now();
    static auto before = clock::now();
    switch (bit)
    {
    case 0: before = before0; break;
    case 1: before = before1; break;
    case 2: before = before2; break;
    case 3: before = before3; break;
    case 4: before = before4; break;
    case 5: before = before5; break;
    case 6: before = before6; break;
    case 7: before = before7; break;
    }
    const sec duration = clock::now() - before;
    if (duration.count() > count[bit])
    {
        before = clock::now();
        switch (bit)
        {
        case 0: before0 = clock::now();   _clock_timer++;  break;
        case 1: before1 = clock::now();  break;
        case 2: before2 = clock::now();  break;
        case 3: before3 = clock::now();  break;
        case 4: before4 = clock::now();  break;
        case 5: before5 = clock::now();  break;
        case 6: before6 = clock::now();  break;
        case 7: before7 = clock::now();  break;
        }
        cl_div = (cl_div & (0x01 << bit)) ? cl_div & ~(0x01 << bit) : cl_div | (0x01 << bit);
    }
    return cl_div;
}

void Bus::clockDivider()
{
    // static Byte cl_div = 0;
    for (int bit = 0; bit < 8; bit++)
        clock_div(_clock_div, bit);
}





// ROM /////////////////////////////////////

// only write to ROM when the debug flag is true
void ROM::write(Word offset, Byte data, bool debug) 
{
    if (debug)
        if (offset - m_base < m_size)
            m_memory[(Word)(offset - m_base)] = data;
}

