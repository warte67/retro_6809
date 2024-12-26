/***  Memory.hpp  (singleton) ****************************
*   __  __                                   _                 
*  |  \/  |                                 | |                
*  | \  / | ___ _ __ ___   ___  _ __ _   _  | |__  _ __  _ __  
*  | |\/| |/ _ \ '_ ` _ \ / _ \| '__| | | | | '_ \| '_ \| '_ \ 
*  | |  | |  __/ | | | | | (_) | |  | |_| |_| | | | |_) | |_) |
*  |_|  |_|\___|_| |_| |_|\___/|_|   \__, (_)_| |_| .__/| .__/ 
*                                     __/ |       | |   | |    
*                                    |___/        |_|   |_|    
* 
*  The Memory Object is responsible for maintaining the
*  CPU addressable memory map, reading, and writing. It
*  acts as a static singleton  container object for all 
*  of the attached mememory devices. 
* 
******************/
#pragma once

#include <string>
#include <vector>
#include <mutex>

#include "IDevice.hpp"
#include "types.hpp"
#include "clr.hpp"

class Memory : public IDevice
{
private:    // PRIVATE SINGLETON STUFF
    Memory();
    ~Memory();       

public:      // PUBLIC SINGLETON STUFF  
    Memory(const Memory&) = delete;				// delete the copy constructor
    Memory(Memory&&) = delete;					// delete the move constructor
    Memory& operator=(const Memory&) = delete;	// delete the copy assignment operator
    Memory& operator=(Memory&&) = delete;		// delete the move assignment operator    
    static Memory& GetInstance() { static Memory inst; return inst; }


public:		// PUBLIC VIRTUAL METHODS
    bool OnInit() override;
    bool OnQuit() override;
    bool OnActivate() override;
    bool OnDeactivate() override;
    //bool OnEvent(SDL_Event* evnt) override;
    bool OnEvent(SDL_Event* evnt) override;
    bool OnUpdate(float fElapsedTime) override;
    bool OnRender() override;

    Word OnAttach(Word nextAddr) override 		{ if (nextAddr) { return 0; } return 0; } // stop the argument not used warning

public:     // PUBLIC ACCESSORS
    static Byte Read(Word offset, bool debug = false);
    static void Write(Word offset, Byte data, bool debug = false);
    static Word Read_Word(Word offset, bool debug = false);
    static void Write_Word(Word offset, Word data, bool debug = false);     
    static DWord Read_DWord(Word offset, bool debug = false);
    static void Write_DWord(Word offset, DWord data, bool debug = false);   

    // Template method for device attachment with flexible parameters
    template<typename T, typename... Args>
    static T* Attach(Args&&... args) {
        //std::cout << clr::indent_push() << clr::CYAN << "Memory::Attach() Entry" << clr::RETURN;        
        T* device = new T(std::forward<Args>(args)...);
        Word size = _attach(device);
        if (size == 0) {
            delete device;
            //std::cout << clr::indent_pop() << clr::ORANGE << "Memory::Attach() Error" << clr::RETURN;
            return nullptr;
        }        
        //std::cout << clr::indent_pop() << clr::CYAN << "Memory::Attach() Exit" << clr::RETURN;
        return device;
    }    


protected:

    static Word _attach(IDevice* device);

    // Move to the Memory Management Device
    inline static int _next_address = 0;    // next assignable address
    inline static std::vector<IDevice*> _memory_nodes;		

private: 
    bool bWasInit = false;
};

// end: Memory.hpp