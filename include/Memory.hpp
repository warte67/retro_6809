/***  Memory.hpp  (singleton) ****************************
 *      __  __                                                 _                     
 *     |  \/  |                                               | |                    
 *     | \  / |   ___   _ __ ___     ___    _ __   _   _      | |__    _ __    _ __  
 *     | |\/| |  / _ \ | '_ ` _ \   / _ \  | '__| | | | |     | '_ \  | '_ \  | '_ \ 
 *     | |  | | |  __/ | | | | | | | (_) | | |    | |_| |  _  | | | | | |_) | | |_) |
 *     |_|  |_|  \___| |_| |_| |_|  \___/  |_|     \__, | (_) |_| |_| | .__/  | .__/ 
 *                                                  __/ |             | |     | |    
 *                                                 |___/              |_|     |_|   
 * 
 *  The Memory Object is responsible for maintaining the
 *  CPU addressable memory map, reading, and writing. It
 *  acts as a static singleton  container object for all 
 *  of the attached mememory devices. 
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ******************/
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>


#include "IDevice.hpp"
#include "types.hpp"
#include "clr.hpp"

class Memory : public IDevice
{
    friend class IDevice;   // This is okay! The Memory device is 
                            // the parent container of IDevices.

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
    void OnInit() override;
    void OnQuit() override;
    void OnActivate() override;
    void OnDeactivate() override;
    void OnEvent(SDL_Event* evnt) override;
    void OnUpdate(float fElapsedTime) override;
    void OnRender() override;
    bool OnTest() override;         // returns true if tests pass

    int OnAttach(int nextAddr) override 		{ if (nextAddr) { return 0; } return 0; } // stop the argument not used warning

public:     // PUBLIC ACCESSORS
    static Byte Read(Word address, bool debug = false);
    static void Write(Word address, Byte data, bool debug = false);
    static Word Read_Word(Word address, bool debug = false);
    static void Write_Word(Word address, Word data, bool debug = false);     
    static DWord Read_DWord(Word address, bool debug = false);
    static void Write_DWord(Word address, DWord data, bool debug = false);   

    // Template method for device attachment with flexible parameters
    template<typename T, typename... Args>
    static T* Attach(Args&&... args) {
        //std::cout << clr::indent_push() << clr::CYAN << "Memory::Attach() Entry" << clr::RETURN;        
        T* device = new T(std::forward<Args>(args)...);
        int sz = _attach(device);
        if (sz == 0) {
            delete device;
            //std::cout << clr::indent_pop() << clr::ORANGE << "Memory::Attach() Error" << clr::RETURN;
            return nullptr;
        }        
        //std::cout << clr::indent_pop() << clr::CYAN << "Memory::Attach() Exit" << clr::RETURN;
        return device;
    }    

    static int NextAddress() { return _next_address; }
    static void Generate_Device_Map();
    static void Generate_Memory_Map();
    // static void add_ROM_entry_to_device_map(Word addr);

    
    // Map a constants name to its address
    static Word Map(std::string name, std::string file, int line);

protected:
    inline static std::vector<Byte> _raw_cpu_memory;

private:
    static int _attach(IDevice* device);

    inline static int _next_address = 0;    // next available address    

    inline static std::unordered_map<Word, REGISTER_NODE> _device_map;   // addressable hardware registers

    inline static std::vector<IDevice*> _memory_nodes;  // all of the attached devices	
    inline static std::unordered_map<std::string, Word> _map;   // constants
    bool bWasInit = false;
};

// end: Memory.hpp

