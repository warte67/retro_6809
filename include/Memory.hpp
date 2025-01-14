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

    int OnAttach(int nextAddr) override 		{ if (nextAddr) { return 0; } return 0; } // stop the argument not used warning

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
    static void Generate_Memory_Map();
    
    // Map a device name to its address
    static Word Map(std::string name, std::string file, int line);



private:
    static int _attach(IDevice* device);

    inline static int _next_address = 0;    // next assignable address
    inline static std::vector<IDevice*> _memory_nodes;		
    inline static std::unordered_map<std::string, Word> _map;
    bool bWasInit = false;
};

// end: Memory.hpp



/***********************************************************

#include <unordered_map>
#include <functional>
#include <string>
#include <vector>

// Register node for devices
struct MAP_NODE {
    std::string label;                  // Register label (e.g., "VIDEO_BUFFER_DEVICE")
    Word address;                       // Memory-mapped address
    std::function<Byte()> read;         // Read handler
    std::function<void(Byte)> write;    // Write handler
    std::function<Byte()> debug_read;   // Debug read handler
    std::function<void(Byte)> debug_write; // Debug write handler
    std::vector<std::string> comments;  // Documentation/comments
};

// Constant node (for static values)
struct ConstantNode {
    std::string label;   // Label for the constant
    Word address;        // Memory-mapped address
    Byte value;          // Static value of the constant
    std::vector<std::string> comments; // Comments for the constant
};

class Memory {
public:
    // Store device registers in an unordered_map (O(1) lookup time)
    std::unordered_map<Word, MAP_NODE> registerMap;

    // Store constants in an unordered_map (O(1) lookup time)
    std::unordered_map<Word, ConstantNode> constantMap;

    // Method to attach a register device
    void OnAttachDevice(const MAP_NODE& node) {
        registerMap[node.address] = node;
    }

    // Method to attach a constant value
    void OnAttachConstant(const ConstantNode& node) {
        constantMap[node.address] = node;
    }

    // Macro for adding a constant to the memory
    #define DEFINE_CONSTANT(label, address, value, ...) \
        OnAttachConstant(ConstantNode{#label, address, value, {__VA_ARGS__}})

    // Macro for adding a device register to the memory
    #define DEFINE_DEVICE(label, address, readHandler, writeHandler, debugReadHandler, debugWriteHandler, ...) \
        OnAttachDevice(MAP_NODE{#label, address, readHandler, writeHandler, debugReadHandler, debugWriteHandler, {__VA_ARGS__}})

    // Memory read function (checks for both registers and constants)
    Byte Read(Word address, bool debug) {
        // Check if it's a register device
        auto regIt = registerMap.find(address);
        if (regIt != registerMap.end()) {
            const MAP_NODE& node = regIt->second;

            if (debug && node.debug_read) {
                return node.debug_read();
            }
            if (node.read) {
                return node.read();
            }
            return 0x00;  // Default value if no read function is defined
        }

        // Check if it's a constant value
        auto constIt = constantMap.find(address);
        if (constIt != constantMap.end()) {
            return constIt->second.value;
        }

        return 0xCC;  // Invalid value if address not found
    }

    // Memory write function (for registers only)
    void Write(Word address, Byte data, bool debug) {
        // Check if it's a register device
        auto regIt = registerMap.find(address);
        if (regIt != registerMap.end()) {
            MAP_NODE& node = regIt->second;

            if (debug && node.debug_write) {
                node.debug_write(data);
                return;
            }
            if (node.write) {
                node.write(data);
                return;
            }
        }
    }
};

// Example device and constant definitions:

void OnAttach() {
    // Define constants with comments
    DEFINE_CONSTANT(SYS_STATE, 0x000C, 0x01, "System state register", "Controls system status");
    DEFINE_CONSTANT(SYS_SPEED, 0x000E, 0x05, "CPU speed in high byte", "Speed value for system");
    
    // Define a device with comments
    DEFINE_DEVICE(
        VIDEO_BUFFER_DEVICE, 
        0x0400, 
        []() -> Byte { return 0xFF; },  // Normal read handler
        nullptr,                         // No write handler
        []() -> Byte { return 0xAA; },  // Debug read handler
        nullptr,                         // No debug write handler
        "Video buffer device register", "Contains the video frame data"
    );
}








 ***********************************************************/