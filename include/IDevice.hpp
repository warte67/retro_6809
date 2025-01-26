/***  IDevice.hpp  ****************************
 *      _____   _____                   _                     _                     
 *     |_   _| |  __ \                 (_)                   | |                    
 *       | |   | |  | |   ___  __   __  _    ___    ___      | |__    _ __    _ __  
 *       | |   | |  | |  / _ \ \ \ / / | |  / __|  / _ \     | '_ \  | '_ \  | '_ \ 
 *      _| |_  | |__| | |  __/  \ V /  | | | (__  |  __/  _  | | | | | |_) | | |_) |
 *     |_____| |_____/   \___|   \_/   |_|  \___|  \___| (_) |_| |_| | .__/  | .__/ 
 *                                                                   | |     | |    
 *                                                                   |_|     |_|    
 *
 *      Interface for the emulated hardware devices 
 *      as they are mapped into cpu readable memory.
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 *************************/
#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <functional>

#include "types.hpp"
#include "UnitTest.hpp"

class IDevice
{
    friend class Memory;    // This is okay! The Memory Device serves as 
                            // the parent container for these objects.

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    IDevice() { _device_name = "IDevice_Device"; }
    IDevice(std::string sName) : _device_name(sName) {}
    virtual ~IDevice() {}

public: // VIRTUAL METHODS

    virtual bool OnTest();  // return true for successful unit tests

public: // INTERFACE (PURE VIRTUAL METHODS)
    virtual void OnInit() = 0;
    virtual void OnQuit() = 0;
    virtual void OnActivate() = 0;
    virtual void OnDeactivate() = 0;
    // virtual bool OnEvent(SDL_Event* evnt) = 0;
    virtual void OnEvent(SDL_Event* evnt) = 0;
    virtual void OnUpdate(float fElapsedTime) = 0;
    virtual void OnRender() = 0;
    virtual int OnAttach(int nextAddr) = 0;   

public: // PUBLIC ACCESSORS
    
    static Byte memory(Word address);               // READ    
    static void memory(Word address, Byte data);    // WRITE

    Word GetBaseAddress() { return base_address; }
    void SetBaseAddress(Word address) { base_address = address; }

protected: // PROTECTED ACCESSORS
    std::string name()          { return _device_name; }
    void name(std::string n)    { _device_name = n; }
    std::string _device_name = "??DEV??";	
    Word base_address = 0;
    Word _size = 0;
 
    // device memory map description
    std::string heading;                    // string that describes the entire device
    struct REGISTER_NODE {
        std::string name;                   // register label
        int address;                        // register starting address
        std::function<Byte(Word)> read;         // Read handler (lambda or function pointer)
        std::function<void(Word, Byte)> write;    // Write handler (lambda or function pointer)    
        std::vector<std::string> comment;   // register comments (can be multiple lines)
    };
    std::vector<REGISTER_NODE> mapped_register;
};




/////////////////////
//
// Standard Memory Devices
//
////////

/*** class RAM *******************************************************
 * 
 * ██████╗  █████╗ ███╗   ███╗
 * ██╔══██╗██╔══██╗████╗ ████║
 * ██████╔╝███████║██╔████╔██║
 * ██╔══██╗██╔══██║██║╚██╔╝██║
 * ██║  ██║██║  ██║██║ ╚═╝ ██║
 * ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝
 * 
 * (This may be moved to its own files)
 ****************************************************************/
class RAM : public IDevice
{
    public:
        // RAM() {  name("RAM"); }
        // RAM(std::string sName) { name(sName); }
        RAM() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN; 
            name("RAM_DEVICE");
        }
        virtual ~RAM() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		int OnAttach(int nextAddr) override     { SetBaseAddress(nextAddr); return 0; }  
		void OnInit() override 					    {}
		void OnQuit() override 					    {}
		void OnActivate() override 				    {}
		void OnDeactivate() override 				{}
		void OnRender() override 					{}
};

/*** class ROM *******************************************************
 * 
 * ██████╗  ██████╗ ███╗   ███╗
 * ██╔══██╗██╔═══██╗████╗ ████║
 * ██████╔╝██║   ██║██╔████╔██║
 * ██╔══██╗██║   ██║██║╚██╔╝██║
 * ██║  ██║╚██████╔╝██║ ╚═╝ ██║
 * ╚═╝  ╚═╝ ╚═════╝ ╚═╝     ╚═╝                         
 * 
 * (This may be moved to its own files)
 ****************************************************************/
class ROM : public IDevice
{
    public:
        ROM() {  name("ROM_DEVICE"); }
        ROM(std::string sName) { name(sName); }
        virtual ~ROM() {}    

		int OnAttach(int nextAddr) override     { SetBaseAddress(nextAddr); return 0; } 
		void OnInit() override 						    {}
		void OnQuit() override 						    {}
		void OnActivate() override 					    {}
		void OnDeactivate() override 				    {}
        // void OnWrite(Word offset, Byte data) override   { (void)offset; (void)data; }
		void OnUpdate(float fElapsedTime) override 	    { (void)fElapsedTime; }
		void OnRender() override 					    {}

        // WRITE
        inline static void memory(Word address, Byte data) { 
            (void)address; (void)data;  
        }

		// helper to set initial ROM values
        void write_to_rom(Word address, Byte data);
};



/*** class SOFT_VECTORS *******************************************************
 * 
 * ███████╗ ██████╗ ███████╗████████╗    ██╗   ██╗███████╗ ██████╗████████╗ ██████╗ ██████╗ ███████╗
 * ██╔════╝██╔═══██╗██╔════╝╚══██╔══╝    ██║   ██║██╔════╝██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗██╔════╝
 * ███████╗██║   ██║█████╗     ██║       ██║   ██║█████╗  ██║        ██║   ██║   ██║██████╔╝███████╗
 * ╚════██║██║   ██║██╔══╝     ██║       ╚██╗ ██╔╝██╔══╝  ██║        ██║   ██║   ██║██╔══██╗╚════██║
 * ███████║╚██████╔╝██║        ██║███████╗╚████╔╝ ███████╗╚██████╗   ██║   ╚██████╔╝██║  ██║███████║
 * ╚══════╝ ╚═════╝ ╚═╝        ╚═╝╚══════╝ ╚═══╝  ╚══════╝ ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝
 * 
 * (This may be moved to its own files)
 ****************************************************************/
class SOFT_VECTORS : public IDevice
{
public:
    SOFT_VECTORS() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
        // _size = size;
        _device_name = "SOFT_VECTORS_DEVICE";
    }
    virtual ~SOFT_VECTORS() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
    }    

    void OnInit() override 						{}
    void OnQuit() override 						{}
    void OnActivate() override 					{}
    void OnDeactivate() override 				{}		
    void OnEvent(SDL_Event* evnt) override 		{ if (evnt==nullptr) {;} }
    void OnUpdate(float fElapsedTime) override 	{ if (fElapsedTime==0) {;} }
    void OnRender() override 					{}

    int OnAttach(int nextAddr) override       { 
        Word old_address=nextAddr;
        this->heading = "Software Interrupt Vectors";

        mapped_register.push_back({ "SOFT_EXEC", nextAddr, nullptr, nullptr,
            { "Exec Software Interrupt Vector"}}); nextAddr+=2;           
        mapped_register.push_back({ "SOFT_SWI3", nextAddr, nullptr, nullptr,  
            { "SWI3 Software Interrupt Vector"}}); nextAddr+=2;
        mapped_register.push_back({ "SOFT_SWI2", nextAddr,  nullptr, nullptr, 
            { "SWI2 Software Interrupt Vector"}}); nextAddr+=2;
        mapped_register.push_back({ "SOFT_FIRQ", nextAddr, nullptr, nullptr,  
            { "FIRQ Software Interrupt Vector"}}); nextAddr+=2;
        mapped_register.push_back({ "SOFT_IRQ", nextAddr,  nullptr, nullptr,  
            { "IRQ Software Interrupt Vector"}}); nextAddr+=2;
        mapped_register.push_back({ "SOFT_SWI", nextAddr, nullptr, nullptr,   
            { "SWI / SYS Software Interrupt Vector"}}); nextAddr+=2;
        mapped_register.push_back({ "SOFT_NMI", nextAddr, nullptr, nullptr,   
            { "NMI Software Interrupt Vector"}}); nextAddr+=2;
        mapped_register.push_back({ "SOFT_RESET", nextAddr, nullptr, nullptr, 
            { "RESET Software Interrupt Vector", "---"}}); nextAddr+=2;

        _size = nextAddr - old_address;
        return _size;
    }  
    bool OnTest() 
    { 
        // Check the number of mapped registers
        size_t expectedRegisters = 8; // Number of interrupt vectors
        ASSERT(mapped_register.size() == expectedRegisters, _device_name + ": Incorrect number of mapped registers");
        // Check the mapped registers
        return UnitTest::RangeTest_RW(_device_name, base_address, base_address+_size);
    }
};






/*** class SYSTEM_MEMORY *******************************************************
 * 
 * ███████╗██╗   ██╗███████╗████████╗███████╗███╗   ███╗        ███╗   ███╗███████╗███╗   ███╗ ██████╗ ██████╗ ██╗   ██╗
 * ██╔════╝╚██╗ ██╔╝██╔════╝╚══██╔══╝██╔════╝████╗ ████║        ████╗ ████║██╔════╝████╗ ████║██╔═══██╗██╔══██╗╚██╗ ██╔╝
 * ███████╗ ╚████╔╝ ███████╗   ██║   █████╗  ██╔████╔██║        ██╔████╔██║█████╗  ██╔████╔██║██║   ██║██████╔╝ ╚████╔╝ 
 * ╚════██║  ╚██╔╝  ╚════██║   ██║   ██╔══╝  ██║╚██╔╝██║        ██║╚██╔╝██║██╔══╝  ██║╚██╔╝██║██║   ██║██╔══██╗  ╚██╔╝  
 * ███████║   ██║   ███████║   ██║   ███████╗██║ ╚═╝ ██║███████╗██║ ╚═╝ ██║███████╗██║ ╚═╝ ██║╚██████╔╝██║  ██║   ██║   
 * ╚══════╝   ╚═╝   ╚══════╝   ╚═╝   ╚══════╝╚═╝     ╚═╝╚══════╝╚═╝     ╚═╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   
 *                                                                                                                      
 * (This may be moved to its own files)
 ****************************************************************/
class SYSTEM_MEMORY : public IDevice
{
public:
    SYSTEM_MEMORY() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
        _device_name = "SYSTEM_MEMORY_DEVICE";
    }
    virtual ~SYSTEM_MEMORY() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
    }    

    void OnInit() override 						{}
    void OnQuit() override 						{}
    void OnActivate() override 					{}
    void OnDeactivate() override 				{}		
    void OnEvent(SDL_Event* evnt) override 		{ if (evnt==nullptr) {;} }
    void OnUpdate(float fElapsedTime) override 	{ if (fElapsedTime==0) {;} }
    void OnRender() override 					{}

    int OnAttach(int nextAddr) override       { 
        Word old_address=nextAddr;
        this->heading = "System Memory";
        
        mapped_register.push_back({ "ZERO_PAGE", nextAddr, nullptr, nullptr,   
            { "Zero Page System and User Variables"}}); nextAddr=0x100;   
        nextAddr--;
        mapped_register.push_back({ "ZERO_PAGE_END", nextAddr, nullptr, nullptr,   
            { "Zero Page System and User Variables"}}); nextAddr=0x100;   

        // mapped_register.push_back({ "FIO_FILE_BUFFER", nextAddr, nullptr, nullptr,    
        //     { "START: File Input/Output Buffer"}}); nextAddr+=0x007F;  
        // mapped_register.push_back({ "FIO_FILE_BFR_END", nextAddr,  nullptr, nullptr,  
        //     { "END: File Input/Output Buffers"}}); nextAddr+=1; 

        mapped_register.push_back({ "EDT_BUFFER", nextAddr, nullptr, nullptr,    
            { "START: Line Edit Character Buffer"}}); nextAddr+=0x007F;  
        mapped_register.push_back({ "KEY_END", nextAddr,  nullptr, nullptr,  
            { "END: Line Edit Character Buffer"}}); nextAddr+=1; 
        mapped_register.push_back({ "FIO_BUFFER", nextAddr, nullptr, nullptr,    
            { "START: Input/Output Buffer"}}); nextAddr+=0x007F;  
        mapped_register.push_back({ "FIO_BFR_END", nextAddr,  nullptr, nullptr,  
            { "END: Input/Output Buffer"}}); nextAddr+=1; 

        mapped_register.push_back({ "SYSTEM_STACK", nextAddr, nullptr, nullptr,  
            { "Bottom of System Stack Spcace"}}); nextAddr=0x400;  
        nextAddr--;   
        mapped_register.push_back({ "SSTACK_END", nextAddr, nullptr, nullptr,   
            { "END: System Stack Space"}});  nextAddr+=1; 
        mapped_register.push_back({ "SSTACK_TOP", nextAddr, nullptr, nullptr,   
            { "TOP: System Stack Space", "---"}});
        
        _size = nextAddr - old_address;
        return _size;
    }  
    bool OnTest() 
    { 
        // Check the number of mapped registers
        size_t expectedRegisters = 9; // Number of interrupt vectors
        ASSERT(mapped_register.size() == expectedRegisters, _device_name + ": Incorrect number of mapped registers");
        // Check the mapped registers
        return UnitTest::RangeTest_RW(_device_name, base_address, base_address+_size);
    }    
};



/*** class VIDEO_BUFFER *******************************************************
 * 
 * ██╗   ██╗██╗██████╗ ███████╗ ██████╗         ██████╗ ██╗   ██╗███████╗███████╗███████╗██████╗ 
 * ██║   ██║██║██╔══██╗██╔════╝██╔═══██╗        ██╔══██╗██║   ██║██╔════╝██╔════╝██╔════╝██╔══██╗
 * ██║   ██║██║██║  ██║█████╗  ██║   ██║        ██████╔╝██║   ██║█████╗  █████╗  █████╗  ██████╔╝
 * ╚██╗ ██╔╝██║██║  ██║██╔══╝  ██║   ██║        ██╔══██╗██║   ██║██╔══╝  ██╔══╝  ██╔══╝  ██╔══██╗
 *  ╚████╔╝ ██║██████╔╝███████╗╚██████╔╝███████╗██████╔╝╚██████╔╝██║     ██║     ███████╗██║  ██║
 *   ╚═══╝  ╚═╝╚═════╝ ╚══════╝ ╚═════╝ ╚══════╝╚═════╝  ╚═════╝ ╚═╝     ╚═╝     ╚══════╝╚═╝  ╚═╝
 * 
 * (This may be moved to its own files)
 ****************************************************************/
class VIDEO_BUFFER : public IDevice
{
public:
    VIDEO_BUFFER() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
        _device_name = "VIDEO_BUFFER_DEVICE";
    }
    virtual ~VIDEO_BUFFER() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
    }    

    void OnInit() override 						{}
    void OnQuit() override 						{}
    void OnActivate() override 					{}
    void OnDeactivate() override 				{}		
    void OnEvent(SDL_Event* evnt) override 		{ if (evnt==nullptr) {;} }
    void OnUpdate(float fElapsedTime) override 	{ if (fElapsedTime==0) {;} }
    void OnRender() override 					{}

    int OnAttach(int nextAddr) override       { 
        constexpr int vbfr_size = 8*1024;
        Word old_address=nextAddr;
        this->heading = "Video Buffer (" + std::to_string(vbfr_size/1024) + "K)";
        
        
        mapped_register.push_back({ "VIDEO_START", nextAddr, nullptr, nullptr, 
            { "Start of standard video buffer"}}); nextAddr+=vbfr_size-1;            
        mapped_register.push_back({ "VIDEO_END", nextAddr, nullptr, nullptr,     
            { "End of standard video buffer"}}); nextAddr+=1;
        mapped_register.push_back({ "VIDEO_TOP", nextAddr, nullptr, nullptr,     
            { "Top of standard video buffer", "---"}});  

        _size = nextAddr - old_address;
        return _size;
    }  
    bool OnTest() 
    { 
        // Check the number of mapped registers
        size_t expectedRegisters = 3; // Number of interrupt vectors
        ASSERT(mapped_register.size() == expectedRegisters, _device_name + ": Incorrect number of mapped registers");
        // Check the mapped registers
        return UnitTest::RangeTest_RW(_device_name, base_address, base_address+_size);
    }   
};



/*** class USER_MEMORY *******************************************************
 * 
* ██╗   ██╗███████╗███████╗██████╗         ███╗   ███╗███████╗███╗   ███╗ ██████╗ ██████╗ ██╗   ██╗
* ██║   ██║██╔════╝██╔════╝██╔══██╗        ████╗ ████║██╔════╝████╗ ████║██╔═══██╗██╔══██╗╚██╗ ██╔╝
* ██║   ██║███████╗█████╗  ██████╔╝        ██╔████╔██║█████╗  ██╔████╔██║██║   ██║██████╔╝ ╚████╔╝ 
* ██║   ██║╚════██║██╔══╝  ██╔══██╗        ██║╚██╔╝██║██╔══╝  ██║╚██╔╝██║██║   ██║██╔══██╗  ╚██╔╝  
* ╚██████╔╝███████║███████╗██║  ██║███████╗██║ ╚═╝ ██║███████╗██║ ╚═╝ ██║╚██████╔╝██║  ██║   ██║   
*  ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝╚═╝     ╚═╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   
 * 
 * (This may be moved to its own files)
 ****************************************************************/
class USER_MEMORY : public IDevice
{
public:
    USER_MEMORY() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
        _device_name = "USER_MEMORY_DEVICE";
    }
    virtual ~USER_MEMORY() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
    }    
    void OnInit() override 						{}
    void OnQuit() override 						{}
    void OnActivate() override 					{}
    void OnDeactivate() override 				{}
    void OnEvent(SDL_Event* evnt) override 		{ if (evnt==nullptr) {;} }
    void OnUpdate(float fElapsedTime) override 	{ if (fElapsedTime==0) {;} }       
    void OnRender() override 					{}
    int OnAttach(int nextAddr) override       { 
        int ram_size = 0xAFFF-nextAddr;
        Word old_address=nextAddr;
        this->heading = "User Memory (" + std::to_string(ram_size/1024) + "K)";          
        mapped_register.push_back({ "USER_RAM", nextAddr,      nullptr, nullptr,
            { "User Accessable RAM"}}); nextAddr+=ram_size;
        mapped_register.push_back({ "USER_RAM_END", nextAddr,  nullptr, nullptr,
            { "End User Accessable RAM"}}); nextAddr+=1;
        mapped_register.push_back({ "USER_RAM_TOP", nextAddr,  nullptr, nullptr,
            { "Top User Accessable RAM", "---"}});             
        _size = nextAddr - old_address;
        return _size;
    }  
    bool OnTest() 
    { 
        // Check the number of mapped registers
        size_t expectedRegisters = 3; // Number of interrupt vectors
        ASSERT(mapped_register.size() == expectedRegisters, _device_name + ": Incorrect number of mapped registers");
        // Check the mapped registers
        return UnitTest::RangeTest_RW(_device_name, base_address, base_address+_size);
    }     
};





class HDW_RESERVED : public IDevice
{
public:
    HDW_RESERVED() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
        _device_name = "HDW_RESERVED_DEVICE";
    }
    virtual ~HDW_RESERVED() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
    }    

    void OnInit() override 						{}
    void OnQuit() override 						{}
    void OnActivate() override 					{}
    void OnDeactivate() override 				{}
    void OnEvent(SDL_Event* evnt) override 		{ (void) evnt;  }
    void OnUpdate(float fElapsedTime) override 	{ (void) fElapsedTime; }
    void OnRender() override 					{}

    int OnAttach(int nextAddr) override;

    bool OnTest() 
    { 
        // Check the number of mapped registers
        size_t expectedRegisters = 1; // Number of interrupt vectors
        ASSERT(mapped_register.size() == expectedRegisters, _device_name + ": Incorrect number of mapped registers");
        // Check the mapped registers
        bool result = UnitTest::RangeTest_RW(_device_name, base_address, base_address+_size); 
        return result;
    }   
};


/*** class ROM_VECTS *******************************************************
 * 
 * ██████╗  ██████╗ ███╗   ███╗       ██╗   ██╗███████╗ ██████╗████████╗███████╗
 * ██╔══██╗██╔═══██╗████╗ ████║       ██║   ██║██╔════╝██╔════╝╚══██╔══╝██╔════╝
 * ██████╔╝██║   ██║██╔████╔██║       ██║   ██║█████╗  ██║        ██║   ███████╗
 * ██╔══██╗██║   ██║██║╚██╔╝██║       ╚██╗ ██╔╝██╔══╝  ██║        ██║   ╚════██║
 * ██║  ██║╚██████╔╝██║ ╚═╝ ██║███████╗╚████╔╝ ███████╗╚██████╗   ██║   ███████║
 * ╚═╝  ╚═╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝ ╚═══╝  ╚══════╝ ╚═════╝   ╚═╝   ╚══════╝
 * 
 ****************************************************************/
class ROM_VECTS : public IDevice
{
public:
    ROM_VECTS() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
        _device_name = "ROM_VECTS_DEVICE";
    }
    virtual ~ROM_VECTS() {
        //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
    }    

    void OnInit() override 						{}
    void OnQuit() override 						{}
    void OnActivate() override 					{}
    void OnDeactivate() override 				{}
    void OnEvent(SDL_Event* evnt) override 		{ (void)evnt; }
    void OnUpdate(float fElapsedTime) override 	{ (void)fElapsedTime; }
    void OnRender() override 					{}

    int OnAttach(int nextAddr) override; 
    bool OnTest() 
    { 
        // Check the number of mapped registers
        size_t expectedRegisters = 8; // Number of interrupt vectors
        ASSERT(mapped_register.size() == expectedRegisters, _device_name + ": Incorrect number of mapped registers");
        // Check the mapped registers
        bool result = UnitTest::RangeTest_RO(_device_name, base_address, base_address+_size); 
        return result;
    }       
};

