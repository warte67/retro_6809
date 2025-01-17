/***  Bus.cpp  ****************************
 *      ____                                           
 *     |  _ \                                          
 *     | |_) |  _   _   ___        ___   _ __    _ __  
 *     |  _ <  | | | | / __|      / __| | '_ \  | '_ \ 
 *     | |_) | | |_| | \__ \  _  | (__  | |_) | | |_) |
 *     |____/   \__,_| |___/ (_)  \___| | .__/  | .__/ 
 *                                      | |     | |    
 *                                      |_|     |_|      
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ******************/

#include <chrono>
#include <deque>
#include <fstream>

#include "Bus.hpp"
#include "clr.hpp"
#include "GPU.hpp"
#include "Debug.hpp"
#include "C6809.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Joystick.hpp"


class BusException : public std::exception
{
public:
    BusException(const std::string& err_msg, const std::string& file, int line)
        : err_msg_(err_msg), file_(file), line_(line) {}

    const char* what() const throw()
    {
        return err_msg_.c_str();
    }

    std::string get_file() const
    {
        return file_;
    }

    int get_line() const
    {
        return line_;
    }

private:
    std::string err_msg_;
    std::string file_;
    int line_;
};



Bus::Bus()  
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Bus Singleton Created" << clr::RETURN;
}


Bus::~Bus()
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Bus Singleton Destroyed" << clr::RETURN;
}


bool Bus::IsRunning()	    
{ 
    // std::lock_guard<std::mutex> guard(_mutex_IsRunning); 
    return s_bIsRunning; 
}

void Bus::IsRunning(bool b)	
{ 
    // std::lock_guard<std::mutex> guard(_mutex_IsRunning); 
    s_bIsRunning = b; 
}



bool Bus::IsDirty()			
{ 
    // std::lock_guard<std::mutex> guard(_mutex_IsDirty); 
    return s_bIsDirty; 
}


void Bus::IsDirty(bool b)	
{ 
    // std::lock_guard<std::mutex> guard(_mutex_IsDirty); 
    s_bIsDirty = b; 
}



void Bus::Error(std::string err_msg, std::string file, int line)
{
    Bus::IsDirty(true);  
    Bus::IsRunning(false);

    throw BusException(err_msg, file, line);
}  


bool Bus::Run()
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::Run() Entry" << clr::RETURN;
    bool bWasActivated = false;

    try
    {   
        // initialize everything
        _onInit();

        // terminate the app when the 'bIsRunning' flag is no longer true
        while (Bus::IsRunning())
        {
            // something changed (is dirty). rebuild the environment
            if (Bus::IsDirty())
            {
                // stop the CPU from running while updating system
                C6809::IsCpuEnabled(false);

                // shutdown the old environment
                if (bWasActivated)
                    _onDeactivate();

                // create a new environment
                _onActivate();
                bWasActivated = true;

                // wait 25 milliseconds to re-enable the CPU 
                SDL_Delay(25);

                // reenable the CPU
                C6809::IsCpuEnabled(true);   // no code to run yet!

                // no longer dirty
                Bus::IsDirty(false);           
            }
            // update all of the attached devices
            _onUpdate();
            
            // dispatch SDL events to the devices
            _onEvent();
            
            // render all of the devices to the screen buffers
            _onRender();      

            // only present for GfxCore            
            if (_pGPU) { _pGPU->RenderPresent(); }

            SDL_Delay(1);
        }
        // shutdown the environment
        _onDeactivate();    

        // shutdown SDL2 and return with status
        std::cout << clr::indent_pop() << clr::CYAN << "Bus::Run() Exit" << clr::RETURN;
        _onQuit();

        return true;
    }
    catch (BusException& e)
    {
        std::string err_msg = e.what();
        std::string file = e.get_file();
        int line = e.get_line();    
        std::cout << std::endl;
        std::cout << clr::RED << "  ╭──" << clr::YELLOW << "  ERROR:  " << clr::RED << "────────====####" << clr::RETURN;
        std::cout << clr::RED << "  │" << clr::NORMAL << " in file: " << clr::WHITE << file << clr::RETURN;
        std::cout << clr::RED << "  │" << clr::NORMAL << " on line: " << clr::WHITE << line << clr::RETURN;
        std::cout << clr::RED << "  │" << clr::RETURN;// printw("\n");
        std::cout << clr::RED << "  │" << clr::NORMAL << clr::WHITE << "   " << err_msg << clr::RETURN;
        std::cout << clr::RED << "  │" << clr::RETURN;
        std::cout << clr::RED << "  ╰────────────────────====####" << clr::RETURN;
        std::cout << std::endl;
        SDL_Window* pWin = _pGPU->GetWindow();
        if (pWin)
        {
            err_msg += "\n\nFile: " + file + "\nLine: " + std::to_string(line);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
                "Error", 
                err_msg.c_str(), 
                pWin);
        }

        _onDeactivate();
        _onQuit();
        std::cout << clr::indent_pop() << clr::CYAN << "Bus::Run() Exit" << clr::RETURN;
        return false;
    }
}


void Bus::_onInit()
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::_onInit() Entry" << clr::RETURN;

    ////////////////////////////////////////
    // Core system devices with parameters
    ////////////////////////////////////////

    // Attach the core system devices
    Memory::Attach<SOFT_VECTORS>();     // 0x0000 - 0x000F
    Memory::Attach<SYSTEM_MEMORY>();    // 0x0010 - 0x03FF
    Memory::Attach<VIDEO_BUFFER>();     // 0x0400 - 0x23FF      (8k video buffer)
    Memory::Attach<USER_MEMORY>();      // 0x2400 - 0xAFFF      (42k user RAM)    
    Memory::Attach<MEMBANK>();          // 0xB000 - 0xEFFF      (16k banked memory)
    Memory::Attach<KERNEL_ROM>(); 

    _pDebug = Memory::Attach<Debug>();
    _pGPU = Memory::Attach<GPU>();
    Memory::Attach<Mouse>();
    Memory::Attach<Keyboard>();
    Memory::Attach<Joystick>();

    Memory::Attach<HDW_RESERVED>();     // reserved space for future use
    Memory::Attach<ROM_VECTS>();        // 0xFFF0 - 0xFFFF      (System ROM Vectors)

    // Error Checking
    if (Memory::NextAddress() > 0x10000) {
        Bus::Error("Memory Limit Exceeded!", __FILE__, __LINE__);        
    }


    // Dump the memory map
    if (GENERATE_MEMORY_MAP)    { Memory::Generate_Memory_Map(); }

    // Generate the Device Map
    Memory::Generate_Device_Map();

    // initialize the devices
    _memory.OnInit();

    // load initial applications (Kernel should be loaded with the KERNEL_ROM device)
    load_hex(INITIAL_ASM_APPLICATION);

    // start the CPU thread
    s_c6809 = new C6809(this);
	try 
	{
		s_cpuThread = std::thread(&C6809::ThreadProc);
        // Wait until the CPU thread is ready
        C6809* cpu = Bus::GetC6809();
        std::unique_lock<std::mutex> lock(cpu->mtx);
        cpu->cv.wait(lock, [cpu]{ return cpu->isCpuThreadReady; });
	} 
	catch (const std::exception& e)
	{
		if (s_cpuThread.joinable())
			s_cpuThread.join();		
		Bus::Error("Unable to start the CPU thread");
		Bus::IsRunning(false);
		std::cout << e.what() << std::endl;
	}

    // cleanup and return
    std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onInit() Exit" << clr::RETURN;

    // Set the initialized flag
    _bWasInit = true;
}


void Bus::_onQuit()
{
    std::cout << clr::indent_push() << clr::CYAN << "Bus::_onQuit() Entry" << clr::RETURN;
    
    // check if the bus is initialized
    if (_bWasInit)   
    { 
        // shutdown the CPU thread
        if (s_cpuThread.joinable())
            s_cpuThread.join();
        // Remove the CPU device
        if (s_c6809)
        {
            delete s_c6809;
            s_c6809 = nullptr;
        }
        // shutdown the devices
        _memory.OnQuit();
        // reset the initialized flag
        _bWasInit = false;         
        // shutdown SDL
        SDL_Quit();
    }    
    std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onQuit() Exit" << clr::RETURN;
}



void Bus::_onActivate(void) 
{
    //std::cout << clr::indent_push() << clr::CYAN << "Bus::_onActivate() Entry" << clr::RETURN;    
    _memory.OnActivate();
    //std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onActivate() Exit" << clr::RETURN;
}


void Bus::_onDeactivate(void) 
{
    //std::cout << clr::indent_push() << clr::CYAN << "Bus::_onDeactivate() Entry" << clr::RETURN;
    _memory.OnDeactivate();
    //std::cout << clr::indent_pop() << clr::CYAN << "Bus::_onDeactivate() Exit" << clr::RETURN;
}



void Bus::_onEvent()
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
            case SDL_EVENT_QUIT:  
                Bus::IsRunning(false);
                break;

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
                    // Debugger Toggle ALT-D
                    if (evnt.key.key == SDLK_D)
                    {
                        Byte flags = Memory::Read(MAP(SYS_DBG_FLAGS));
                        if (flags & Debug::_DBG_FLAGS::DBGF_DEBUG_ENABLE)
                        {
                            flags &= ~Debug::_DBG_FLAGS::DBGF_DEBUG_ENABLE;
                            Memory::Write(MAP(SYS_DBG_FLAGS), flags);
                        }
                        else
                        {
                            flags |= Debug::_DBG_FLAGS::DBGF_DEBUG_ENABLE;
                            Memory::Write(MAP(SYS_DBG_FLAGS), flags);
                        }
                    }
                }
                break;                
            }
        }
        // forward the event to all of the memory devices
        _memory.OnEvent(&evnt);

        // flush events? This does seem to help the input lag, 
        // but more still needs to be done.
        // SDL_FlushEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
    }     
}

Byte Bus::clock_div(Byte& cl_div, int bit)
{
    if (bit > 7)    bit = 7;
    if (bit < 0)    bit = 0;
    double count[] =
    { 
        // cycle time       frequency
        //   (ms):          (hz):
          14.28571,         // 70 hz
          28.57143,         // 35 hz
          57.14286,         // 17.5 hz
         114.28571,         // 8.75 hz
         228.57143,         // 4.375 hz
         471.42857,         // 2.1875 hz
         942.85714,         // 1.09375 hz
        1888.57143,         // 0.546875 hz
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
            case 0: before0 = clock::now();  break; //_clock_timer++;  break;
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

void Bus::_onUpdate() 
{
    // std::cout << "Bus::_onUpdate()\n";

    // update the clock divider
    clockDivider();    

    // handle timing
    static std::chrono::time_point<std::chrono::system_clock> tp1 = std::chrono::system_clock::now();
    static std::chrono::time_point<std::chrono::system_clock> tp2 = std::chrono::system_clock::now();    
    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedTime = tp2 - tp1;
    tp1 = tp2;
    // Our time per frame coefficient
    float fElapsedTime = elapsedTime.count();
    // count frames per second
    static int frame_count = 0;
    static float frame_acc = fElapsedTime;
    frame_count++;
    frame_acc += fElapsedTime;    
    if (frame_acc > 0.25f + fElapsedTime)
    {
        static std::deque<float> fps_queue;
        frame_acc -= 0.25f;
        // get an FPS average over the last several iterations
        float f = frame_count * 4;
        fps_queue.push_back(f);
        if (fps_queue.size()>10)
            fps_queue.pop_front();
        float total = 0.0f;
        for (auto &e : fps_queue)
            total += e;
        _fps = total / (float)fps_queue.size();
        // reset the frame count
		frame_count = 0;
        // generate a string for reporting
        _s_title = "R6809"; // "Retro 6809";
		_s_title += " FPS:";		
		_s_title += std::to_string((int)_fps);
        _s_title += " ___ CPU: ~ ";		
        _s_title += std::to_string((int)GetCpuSpeed());
		_s_title += " kHz.";		
    }  


    // NOTE: This might be a good place to monitor the
    // 		terminal dimensions. Perhaps a new event
    //		can be triggered from here.
    //
    //		Be sure to fetch the initial console 
    //		terminal demensions during OnInit() too.
    //
    static int s_width=0, s_height=0;
    int w, h;
    clr::get_terminal_size(w, h);       // getmaxyx(stdscr,h,w);
    

    if (s_width != w || s_height != h)
    {        
        s_width = w;
        s_height = h;
        // fire a terminal resize event from here
        // ...
        // std::cout << "Terminal Resized: W:" << std::to_string(w) << " H:" << std::to_string(h) << "\n";
    }

    _sys_update_event++; // increment the clock each update cycle
    _memory.OnUpdate(fElapsedTime);
}


void Bus::_onRender(void) 
{
    _memory.OnRender();
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
        std::stringstream ss;
        ss << "Unable to open file: " << filename;
        Bus::Error(ss.str());        	

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
				Memory::Write(addr, b, true);
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

Word Bus::GetCpuSpeed()
{ 
    return C6809::_cpu_speed; 
}


