//////////
//
//  Bus.h  (thread-safe singleton)
//
/////////////

#include <chrono>
#include <iostream>
#include <fstream>
#include "Bus.h"

Bus::Bus()
{
	std::cout << "Bus::Bus()\n";

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
    dev->DisplayEnum("SOFT_RSRVD",0x0000, "Motorola RESERVED Software Interrupt Vector");
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

	// system stack 512 bytes
    dev = new RAM("SYSTEM_STACK");
    addr += Attach(dev, 512);  
    dev->DisplayEnum("SSTACK_TOP",0x0300, "Top of the system stack space");  	

	// user stack 256 bytes
    dev = new RAM("USER_STACK");
    addr += Attach(dev, 256);  
    dev->DisplayEnum("USTACK_TOP",0x0400, "Top of the user stack space");  	

	// display buffer 6k bytes
    dev = new RAM("SCREEN_BUFFER");
    dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0, "Display Buffer (6K bytes)");
    addr += Attach(dev, 6*1024);

	// start hardware registers
	dev->DisplayEnum("",0, "");
    dev->DisplayEnum("",0, "Device Registers:");
    dev->DisplayEnum("HDW_REGS", addr, "Begin Device Hardware Registers");




	// base graphics device
    dev = new Gfx();    
    addr += Attach(dev); 
    m_gfx = dynamic_cast<Gfx*>(dev);   





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

    // OnInit()     // call for each attached device
    _onInit();

	// load up the memory map
	load_hex("asm/test.hex");	
	printf("BUS::read(0xFFFE): 0x%04X\n", read_word(0xFFFE));
}


Bus::~Bus()
{
	std::cout << "Bus::~Bus()\n";

    // close SDL
    SDL_Quit();
}

void Bus::_onInit() 
{
	std::cout << "void Bus::_onInit()\n";

	// for (auto &d : _memoryNodes)
	// 	d->OnInit();
}

void Bus::_onQuit() 
{
	std::cout << "void Bus::_onQuit()\n";

	// for (auto &d : _memoryNodes)
	// 	d->OnQuit();
}

void Bus::_onDeactivate() 
{
	std::cout << "void Bus::_onDeactivate()\n";

	// for (auto &d : _memoryNodes)
	// 	d->OnDeactivate();
}

void Bus::_onActivate() 
{
	std::cout << "void Bus::_onActivate()\n";

	for (auto &d : _memoryNodes)
		d->OnActivate();
}

void Bus::_onUpdate() 
{
	// std::cout << "void Bus::_onUpdate()\n";

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

    if (frame_acc > 1.0f + fElapsedTime)
    {
        frame_acc -= 1.0f;
		_fps = frame_count;
		frame_count = 0;

		// clean this up (get rid of the requirement for th GFX friend class Bus bullshit)
		std::string sTitle = "Retro 6809";
		sTitle += "  FPS: ";
		sTitle += std::to_string(_fps);
		if (m_gfx)
			if (m_gfx->_window)
				 SDL_SetWindowTitle(m_gfx->_window, sTitle.c_str());
    }    

	// update the devices
	for (auto &d : _memoryNodes)
		d->OnUpdate(fElapsedTime);	
}

void Bus::_onEvent() 
{
	// std::cout << "void Bus::_onEvent() \n";

    SDL_Event evnt;
    while (SDL_PollEvent(&evnt))
    {
        // OnEvent(evnt)
		for (auto &d : _memoryNodes)
			d->OnEvent(&evnt);		

        switch (evnt.type) {

		// handle default events SDL_QUIT and [ESC] quits
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
	// std::cout << "void Bus::_onRender()\n";

	for (auto &d : _memoryNodes)
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


Byte Bus::read(Word offset, bool debug) 
{    
    for (auto &a : _memoryNodes)
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

void Bus::write(Word offset, Byte data, bool debug) 
{
    for (auto &a : _memoryNodes)
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
Word Bus::read_word(Word offset, bool debug) 
{
	Word ret = (read(offset) << 8) | read(offset + 1);
	return ret;
}
void Bus::write_word(Word offset, Word data, bool debug) 
{    
	Byte lsb = (data >> 8) & 0xFF;
	Byte msb = data & 0xff;
	write(offset, msb);
	write(offset + 1, lsb);
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
        _memoryNodes.push_back(dev);
    }
    if (size > 65536)
        Bus::Error("Memory allocation beyond 64k boundary!");
    return size;
}

void Bus::DumpMemoryMap()
{
    std::cout << "  Simple Memory Map:\n";
    for(auto &o : _memoryNodes)
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
    _onInit();	

	
	// m_thread = std::thread(&Bus::_cpuThread);


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
				write(addr,b,true);
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




// ROM /////////////////////////////////////

// only write to ROM when the debug flag is true
void ROM::write(Word offset, Byte data, bool debug) 
{
    if (debug)
        if (offset - m_base < m_size)
            m_memory[(Word)(offset - m_base)] = data;
}

