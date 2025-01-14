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
 *************************/
#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <functional>

#include "types.hpp"

class IDevice
{
    friend class Memory;   // This is okay! The Bus serves as the 
                        // parent container for these objects.

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    IDevice() { _device_name = "IDevice Device"; }
    IDevice(std::string sName) : _device_name(sName) {}
    virtual ~IDevice() {}

public: // VIRTUAL METHODS
    virtual Byte OnRead(Word offset);
    virtual void OnWrite(Word offset, Byte data);

public: // INTERFACE (PURE VIRTUAL METHODS)
    virtual void OnInit() = 0;
    virtual void OnQuit() = 0;
    virtual void OnActivate() = 0;
    virtual void OnDeactivate() = 0;
    // virtual bool OnEvent(SDL_Event* evnt) = 0;
    virtual void OnEvent(SDL_Event* evnt) = 0;
    virtual void OnUpdate(float fElapsedTime) = 0;
    virtual void OnRender() = 0;

    // ToDo:
    //      create a basic implementation of this to use
    //      a new Memory device to track these nodes
    virtual int OnAttach(int nextAddr) = 0;   

public: // PUBLIC ACCESSORS
    // ... 

protected: // PROTECTED ACCESSORS
    Word base();
    void base(Word addr);
    int size();
    void size(int size);
    std::string name();
    void name(std::string n);
    Byte memory(Word ofs);
    void memory(Word ofs, Byte data);
    Byte memory_no_lock(Word ofs);
    void memory_no_lock(Word ofs, Byte data);

    Uint16 _base = 0;					    
    // std::mutex _mutex_base;		
    int _size = 0;						    
    // std::mutex _mutex_size;		
    std::string _device_name = "??DEV??";	
    // std::mutex _mutex_device_name;				
    std::vector<Uint8> _memory;				
    // std::mutex _mutex_memory;


    // device memory map description
    std::string heading;                    // string that describes the entire device
    struct register_node {
        std::string name;                   // register label
        int address;                        // register starting address
        std::function<Byte()> read;         // Read handler (lambda or function pointer)
        std::function<void(Byte)> write;    // Write handler (lambda or function pointer)
        std::vector<std::string> comment;   // register comments (can be multiple lines)
    };
    std::vector<register_node> mapped_register;

    // struct MAP_NODE {
    //     std::string label;                  // Register label (e.g., "GPU_OPTIONS")
    //     Word address;                       // Memory-mapped address
    //     std::function<Byte()> read;         // Read handler (lambda or function pointer)
    //     std::function<void(Byte)> write;    // Write handler (lambda or function pointer)
    //     std::vector<std::string> comments;  // List of comments/documentation lines
    // };
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
        RAM(Word size) {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN; 
            name("RAM_DEVICE");
            _size = size;
        }
        virtual ~RAM() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		int OnAttach(int nextAddr) override         { return nextAddr; }  
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

		int OnAttach(int nextAddr) override             { if (nextAddr==0) { ; }  return _size; }  
		void OnInit() override 						    {}
		void OnQuit() override 						    {}
		void OnActivate() override 					    {}
		void OnDeactivate() override 				    {}
        void OnWrite(Word offset, Byte data) override   { if (offset==data) {;} }
		void OnUpdate(float fElapsedTime) override 	    { if (fElapsedTime==0) {;} }
		void OnRender() override 					    {}

		// helper to set initial ROM values
        void write_to_rom(Word offset, Byte data);
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

            mapped_register.push_back({ "SOFT_EXEC", nextAddr, nullptr, nullptr,  { "Exec Software Interrupt Vector"} });   nextAddr+=2;
            mapped_register.push_back({ "SOFT_SWI3", nextAddr, nullptr, nullptr,  { "SWI3 Software Interrupt Vector"} });   nextAddr+=2;
            mapped_register.push_back({ "SOFT_SWI2", nextAddr, nullptr, nullptr,  { "SWI2 Software Interrupt Vector"} });   nextAddr+=2;
            mapped_register.push_back({ "SOFT_FIRQ", nextAddr, nullptr, nullptr,  { "FIRQ Software Interrupt Vector"} });   nextAddr+=2;
            mapped_register.push_back({ "SOFT_IRQ",  nextAddr, nullptr, nullptr,  { "IRQ Software Interrupt Vector"} });    nextAddr+=2;
            mapped_register.push_back({ "SOFT_SWI",  nextAddr, nullptr, nullptr,  { "SWI / SYS Software Interrupt Vector"} }); nextAddr+=2;
            mapped_register.push_back({ "SOFT_NMI",  nextAddr, nullptr, nullptr,  { "NMI Software Interrupt Vector"} });    nextAddr+=2;
            mapped_register.push_back({ "SOFT_RESET", nextAddr, nullptr, nullptr, { "RESET Software Interrupt Vector", "---"} });  nextAddr+=2;

            _size = nextAddr - old_address;
            return _size; 
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
            // _size = size;
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

            mapped_register.push_back({ "ZERO_PAGE",    nextAddr, nullptr, nullptr, { "Zero Page System and User Variables"} }); nextAddr=0x100;
            mapped_register.push_back({ "FIO_BUFFER",   nextAddr, nullptr, nullptr, { "START: File Input/Output Buffer"} });     nextAddr+=0xFF;
            mapped_register.push_back({ "FIO_BFR_END",  nextAddr, nullptr, nullptr, { "END: File Input/Output Buffer"} });       nextAddr+=1;
            mapped_register.push_back({ "SYSTEM_STACK", nextAddr, nullptr, nullptr, { "Bottom of the system stack spcace"} });   nextAddr=0x400;
            mapped_register.push_back({ "SSTACK_TOP",   nextAddr, nullptr, nullptr, { "Top of the system statck space", "---"} });

            _size = nextAddr - old_address;
            return _size; 
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
            // _size = size;
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

            mapped_register.push_back({ "VIDEO_START", nextAddr, nullptr, nullptr,   { "Start of standard video buffer"} });    nextAddr+=vbfr_size-1;
            mapped_register.push_back({ "VIDEO_END",   nextAddr, nullptr, nullptr,   { "End of standard video buffer"} });      nextAddr+=1;
            mapped_register.push_back({ "VIDEO_TOP",   nextAddr, nullptr, nullptr,   { "Top of standard video buffer", "---"} });

            _size = nextAddr - old_address;
            return _size; 
        }  

        // Override OnRead and OnWrite with lock-free logic for GPU
        Byte OnRead(Word offset) override
        {
            return Read_NoLock(offset);
        }

        void OnWrite(Word offset, Byte data) override
        {
            Write_NoLock(offset, data);
        }

        Byte Read_NoLock(Word offset)
        {
            if (offset - base() < size())
                return IDevice::_memory[ offset - base() ];
            return 0xCC;
        }

        void Write_NoLock(Word offset, Byte data)
        {
            if (offset - base() < size())
                IDevice::_memory[ offset - base() ] = data;
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
            // _size = size;
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

            mapped_register.push_back({ "USER_RAM", nextAddr, nullptr, nullptr,      { "User Accessable RAM"} });       nextAddr+=ram_size;
            mapped_register.push_back({ "USER_RAM_END", nextAddr, nullptr, nullptr,  { "End User Accessable RAM"} });   nextAddr+=1;
            mapped_register.push_back({ "USER_RAM_TOP", nextAddr, nullptr, nullptr,  { "Top User Accessable RAM", "---"} });

            _size = nextAddr - old_address;
            return _size; 
        }  
};


/*** class MEMBANK *******************************************************
 * 
 * ███╗   ███╗███████╗███╗   ███╗██████╗  █████╗ ███╗   ██╗██╗  ██╗
 * ████╗ ████║██╔════╝████╗ ████║██╔══██╗██╔══██╗████╗  ██║██║ ██╔╝
 * ██╔████╔██║█████╗  ██╔████╔██║██████╔╝███████║██╔██╗ ██║█████╔╝ 
 * ██║╚██╔╝██║██╔══╝  ██║╚██╔╝██║██╔══██╗██╔══██║██║╚██╗██║██╔═██╗ 
 * ██║ ╚═╝ ██║███████╗██║ ╚═╝ ██║██████╔╝██║  ██║██║ ╚████║██║  ██╗
 * ╚═╝     ╚═╝╚══════╝╚═╝     ╚═╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝
 * 
 * (This may be moved to its own files)
 ****************************************************************/
class MEMBANK : public IDevice
{
    public:
        MEMBANK() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
            _size = 16 * 1024;
            _device_name = "MEMBANK_DEVICE";
        }
        virtual ~MEMBANK() {
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
            int bank_size = 8*1024;
            Word old_address=nextAddr;
            this->heading = "Banked Memory Region (" + std::to_string(bank_size/512) + "K)";

            mapped_register.push_back({ "MEMBANK_ONE", nextAddr, nullptr, nullptr,  { "Banked Memory Page One (8K)"} }); nextAddr+=bank_size;
            mapped_register.push_back({ "MEMBANK_TWO", nextAddr, nullptr, nullptr,  { "Banked Memory Page Two (8K)"} }); nextAddr+=(bank_size-1);
            mapped_register.push_back({ "MEMBANK_END", nextAddr, nullptr, nullptr,  { "End of Banked Memory Region"} }); nextAddr+=1;
            mapped_register.push_back({ "MEMBANK_TOP", nextAddr, nullptr, nullptr,  { "TOP of Banked Memory Region", "---"} });

            _size = nextAddr - old_address;
            return _size; 
        }  
};


//              // device memory map description
//              std::string heading;                    // string that describes the entire device
//              struct register_node {
//                  std::string name;                   // register label
//                  Word address;                       // register starting address
//                  std::vector<std::string> comment;   // register comments (can be multiple lines)
//              };
//              std::vector<register_node> mapped_register;


/*** class KERNEL_ROM *******************************************************
 * 
 * ██╗  ██╗███████╗██████╗ ███╗   ██╗███████╗██╗             ██████╗  ██████╗ ███╗   ███╗
 * ██║ ██╔╝██╔════╝██╔══██╗████╗  ██║██╔════╝██║             ██╔══██╗██╔═══██╗████╗ ████║
 * █████╔╝ █████╗  ██████╔╝██╔██╗ ██║█████╗  ██║             ██████╔╝██║   ██║██╔████╔██║
 * ██╔═██╗ ██╔══╝  ██╔══██╗██║╚██╗██║██╔══╝  ██║             ██╔══██╗██║   ██║██║╚██╔╝██║
 * ██║  ██╗███████╗██║  ██║██║ ╚████║███████╗███████╗███████╗██║  ██║╚██████╔╝██║ ╚═╝ ██║
 * ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝ ╚═════╝ ╚═╝     ╚═╝
 * 
 * (This will be moved to its own files)
 * Currently using a RAM model for testing.
 ****************************************************************/
class KERNEL_ROM : public IDevice
{
    public:
        KERNEL_ROM() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
            _size = 16 * 1024;
            _device_name = "KERNEL_ROM_DEVICE";
        }
        KERNEL_ROM(const char* hexfile) : hex_filename(hexfile) {
            _size = 16 * 1024;
            _device_name = "KERNEL_ROM_DEVICE";
            // todo: load the kernel rom hex file
            // ...
        }        
        virtual ~KERNEL_ROM() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		void OnInit() override 						    { }
		void OnQuit() override 						    {}
		void OnActivate() override 					    {}
		void OnDeactivate() override 				    {}
		void OnEvent(SDL_Event* evnt) override 		    { if (evnt==nullptr) {;} }
		void OnUpdate(float fElapsedTime) override 	    { if (fElapsedTime==0) {;} }         
		void OnRender() override 					    {}
        void OnWrite(Word offset, Byte data) override   { if (offset==data) {;} }

		int OnAttach(int nextAddr) override       { 
            int bank_size = 3.5f*1024;
            Word old_address=nextAddr;
            this->heading = "Kernel Rom (3.5K)";

            mapped_register.push_back({ "KERNEL_START", nextAddr, nullptr, nullptr, { "Start of Kernel Rom Space"} });  nextAddr+=(bank_size-1);
            mapped_register.push_back({ "KERNEL_END", nextAddr, nullptr, nullptr,   { "End of Kernel Rom Space"} });    nextAddr+=1;
            mapped_register.push_back({ "KERNEL_TOP", nextAddr, nullptr, nullptr,   { "Top of Kernel Rom Space", "---"} });

            _size = nextAddr - old_address;
            return _size; 
        }  

    private:
        std::string hex_filename;
};


class HDW_RESERVED : public IDevice
{
    public:
        HDW_RESERVED() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
            _size = 0;
            _device_name = "HDW_RESERVED_DEVICE";
        }
        virtual ~HDW_RESERVED() {
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
            Word old_address=nextAddr-1;
            this->heading = "Reserved Register Space";
            register_node new_node;

            // reserve space for future use
            int bank_size = 0xFFEF-nextAddr;      
            std::string res = std::to_string(bank_size);
            res += " bytes reserved for future use.";
            nextAddr+=bank_size;
            // new_node = { "HDW_REG_END", nextAddr, nullptr, nullptr,  { res , "---"} }; // nextAddr+=1;
            // mapped_register.push_back(new_node);     

            mapped_register.push_back({ "HDW_REG_END", nextAddr, nullptr, nullptr,  { res , "---"} } );

            _size = nextAddr - old_address;          
            return _size;
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
            _size = 16 * 1024;
            _device_name = "ROM_VECTS_DEVICE";
        }
        virtual ~ROM_VECTS() {
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
            this->heading = "Hardware Interrupt Vectors";

            mapped_register.push_back({ "HARD_EXEC",  nextAddr, nullptr, nullptr,   { "EXEC Hardware Interrupt Vector" } });    nextAddr+=2;
            mapped_register.push_back({ "HARD_SWI3",  nextAddr, nullptr, nullptr,   { "SWI3 Hardware Interrupt Vector" } });    nextAddr+=2;
            mapped_register.push_back({ "HARD_SWI2",  nextAddr, nullptr, nullptr,   { "SWI2 Hardware Interrupt Vector" } });    nextAddr+=2;
            mapped_register.push_back({ "HARD_FIRQ",  nextAddr, nullptr, nullptr,   { "FIRQ Hardware Interrupt Vector" } });    nextAddr+=2;
            mapped_register.push_back({ "HARD_IRQ",   nextAddr, nullptr, nullptr,   { "IRQ Hardware Interrupt Vector" } });     nextAddr+=2;
            mapped_register.push_back({ "HARD_SWI",   nextAddr, nullptr, nullptr,   { "SWI / SYS Hardware Interrupt Vector" } }); nextAddr+=2;
            mapped_register.push_back({ "HARD_NMI",   nextAddr, nullptr, nullptr,   { "NMI Hardware Interrupt Vector" } });     nextAddr+=2;
            mapped_register.push_back({ "HARD_RESET", nextAddr, nullptr, nullptr,   { "RESET Hardware Interrupt Vector" } });   nextAddr+=2;

            _size = nextAddr - old_address;
            return _size; 
        }  
};

