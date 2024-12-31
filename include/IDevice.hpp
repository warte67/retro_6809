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

#include "types.hpp"
// #include "Events.hpp"

class IDevice
{
    friend class Memory;   // This is okay! The Bus serves as the 
                        // parent container for these objects.

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    IDevice() { _device_name = "IDevice"; }
    IDevice(std::string sName) : _device_name(sName) {}
    virtual ~IDevice() {}

public: // VIRTUAL METHODS
    virtual Byte OnRead(Word offset);
    virtual void OnWrite(Word offset, Byte data);

public: // INTERFACE (PURE VIRTUAL METHODS)
    virtual bool OnInit() = 0;
    virtual bool OnQuit() = 0;
    virtual bool OnActivate() = 0;
    virtual bool OnDeactivate() = 0;
    // virtual bool OnEvent(SDL_Event* evnt) = 0;
    virtual bool OnEvent(SDL_Event* evnt) = 0;
    virtual bool OnUpdate(float fElapsedTime) = 0;
    virtual bool OnRender() = 0;

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

    Uint16 _base = 0;					    std::mutex _mutex_base;		
    int _size = 0;						    std::mutex _mutex_size;		
    std::string _device_name = "??DEV??";	std::mutex _mutex_device_name;				
    std::vector<Uint8> _memory;				std::mutex _mutex_memory;


    // device memory map description
    std::string heading;                    // string that describes the entire device
    struct register_node {
        std::string name;                   // register label
        int address;                       // register starting address
        std::vector<std::string> comment;   // register comments (can be multiple lines)
    };
    std::vector<register_node> mapped_register;
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
            name("RAM");
            _size = size;
        }
        virtual ~RAM() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		int OnAttach(int nextAddr) override        { return nextAddr; }  
		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 
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
        ROM() {  name("ROM"); }
        ROM(std::string sName) { name(sName); }
        virtual ~ROM() {}    

		int OnAttach(int nextAddr) override       { if (nextAddr==0) { ; }  return _size; }  
		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
        void OnWrite(Word offset, Byte data) override { if (offset || data) { ; } }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; } 
		bool OnRender() override 					{ return true; }        

		// helper to set initial ROM values
        void write_to_rom(Word offset, Byte data);
};

/*** class RAM_64K *******************************************************
 * 
 * ██████╗  █████╗ ███╗   ███╗         ██████╗ ██╗  ██╗██╗  ██╗
 * ██╔══██╗██╔══██╗████╗ ████║        ██╔════╝ ██║  ██║██║ ██╔╝
 * ██████╔╝███████║██╔████╔██║        ███████╗ ███████║█████╔╝ 
 * ██╔══██╗██╔══██║██║╚██╔╝██║        ██╔═══██╗╚════██║██╔═██╗ 
 * ██║  ██║██║  ██║██║ ╚═╝ ██║███████╗╚██████╔╝     ██║██║  ██╗
 * ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝ ╚═════╝      ╚═╝╚═╝  ╚═╝
 * 
 * (This may be moved to its own files)
 ****************************************************************/
class RAM_64K : public IDevice
{
    public:
        RAM_64K(int size) {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
            _size = size;
            _device_name = "RAM_64K";
        }
        virtual ~RAM_64K() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 

		int OnAttach(int nextAddr) override       { 
            int old_address=nextAddr;
            this->heading = "Full Bank of System RAM";
            register_node new_node;
            new_node = { "SOFT_EXEC", nextAddr,  { "Exec Software Interrupt Vector"} }; nextAddr+=(64*1024);
            mapped_register.push_back(new_node);

            _size = nextAddr - old_address;
            return _size; 
        }  


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
            _device_name = "SOFT_VECTORS";
        }
        virtual ~SOFT_VECTORS() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 

		int OnAttach(int nextAddr) override       { 
            Word old_address=nextAddr;
            this->heading = "Software Interrupt Vectors";
            register_node new_node;
            new_node = { "SOFT_EXEC", nextAddr,  { "Exec Software Interrupt Vector"} }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "SOFT_SWI3", nextAddr,  { "SWI3 Software Interrupt Vector"} }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "SOFT_SWI2", nextAddr,  { "SWI2 Software Interrupt Vector"} }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "SOFT_FIRQ", nextAddr,  { "FIRQ Software Interrupt Vector"} }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "SOFT_IRQ", nextAddr,   { "IRQ Software Interrupt Vector"} }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "SOFT_SWI", nextAddr,   { "SWI / SYS Software Interrupt Vector"} }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "SOFT_NMI", nextAddr,   { "NMI Software Interrupt Vector"} }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "SOFT_RESET", nextAddr, { "RESET Software Interrupt Vector"} }; nextAddr+=2;
            mapped_register.push_back(new_node);

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
            _device_name = "SYSTEM_MEMORY";
        }
        virtual ~SYSTEM_MEMORY() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 

		int OnAttach(int nextAddr) override       { 
            Word old_address=nextAddr;
            this->heading = "System Memory";
            register_node new_node;
            new_node = { "ZERO_PAGE", nextAddr,     { "Zero Page System and User Variables"} }; nextAddr=0x100;
            mapped_register.push_back(new_node);
            new_node = { "FIO_BUFFER", nextAddr,    { "START: File Input/Output Buffer"} }; nextAddr+=0xFF;
            mapped_register.push_back(new_node);
            new_node = { "FIO_BFR_END", nextAddr,   { "END: File Input/Output Buffer"} }; nextAddr+=1;
            mapped_register.push_back(new_node);
            new_node = { "SYSTEM_STACK", nextAddr,  { "Bottom of the system stack spcace"} }; nextAddr=0x400;
            mapped_register.push_back(new_node);
            new_node = { "SSTACK_TOP", nextAddr,    { "Top of the system statck space"} };
            mapped_register.push_back(new_node);

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
            _device_name = "VIDEO_BUFFER";
        }
        virtual ~VIDEO_BUFFER() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 

		int OnAttach(int nextAddr) override       { 
            constexpr int vbfr_size = 8*1024;
            Word old_address=nextAddr;
            this->heading = "Video Buffer (" + std::to_string(vbfr_size/1024) + "K)";
            register_node new_node;
            new_node = { "VIDEO_START", nextAddr,   { "Start of standard video buffer"} }; nextAddr+=vbfr_size-1;
            mapped_register.push_back(new_node);
            new_node = { "VIDEO_END", nextAddr,     { "End of standard video buffer"} }; nextAddr+=1;
            mapped_register.push_back(new_node);
            new_node = { "VIDEO_TOP", nextAddr,     { "Top of standard video buffer"} };             
            mapped_register.push_back(new_node);

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
            _device_name = "USER_MEMORY";
        }
        virtual ~USER_MEMORY() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 

		int OnAttach(int nextAddr) override       { 
            int ram_size = 0xAFFF-nextAddr;
            Word old_address=nextAddr;
            this->heading = "User Memory (" + std::to_string(ram_size/1024) + "K)";
            register_node new_node;
            new_node = { "USER_RAM", nextAddr,      { "User Accessable RAM"} }; nextAddr+=ram_size;
            mapped_register.push_back(new_node);
            new_node = { "USER_RAM_END", nextAddr,  { "End User Accessable RAM"} }; nextAddr+=1;
            mapped_register.push_back(new_node);
            new_node = { "USER_RAM_TOP", nextAddr,  { "Top User Accessable RAM"} };             
            mapped_register.push_back(new_node);

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
            _device_name = "MEMBANK";
        }
        virtual ~MEMBANK() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 

		int OnAttach(int nextAddr) override       { 
            int bank_size = 8*1024;
            Word old_address=nextAddr;
            this->heading = "Banked Memory Region (" + std::to_string(bank_size/512) + "K)";
            register_node new_node;
            new_node = { "MEMBANK_ONE", nextAddr,  { "Banked Memory Page One (8K)"} }; nextAddr+=bank_size;
            mapped_register.push_back(new_node);
            new_node = { "MEMBANK_TWO", nextAddr,  { "Banked Memory Page Two (8K)"} }; nextAddr+=(bank_size-1);
            mapped_register.push_back(new_node);
            new_node = { "MEMBANK_END", nextAddr,  { "End of Banked Memory Region"} }; nextAddr+=1;            
            mapped_register.push_back(new_node);
            new_node = { "MEMBANK_TOP", nextAddr,  { "TOP of Banked Memory Region"} }; 
            mapped_register.push_back(new_node);

            _size = nextAddr - old_address;
            return _size; 
        }  
};


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
            _device_name = "KERNEL_ROM";
        }
        virtual ~KERNEL_ROM() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 
        void OnWrite(Word offset, Byte data) override { if (offset || data) { ; } }

		int OnAttach(int nextAddr) override       { 
            int bank_size = 3.5f*1024;
            Word old_address=nextAddr;
            this->heading = "Kernel Rom (3.5K)";
            register_node new_node;
            new_node = { "KERNEL_START", nextAddr,  { "Start of Kernel Rom Space"} }; nextAddr+=(bank_size-1);
            mapped_register.push_back(new_node);
            new_node = { "KERNEL_END", nextAddr,  { "End of Kernel Rom Space"} }; nextAddr+=1;
            mapped_register.push_back(new_node);
            new_node = { "KERNEL_TOP", nextAddr,  { "Top of Kernel Rom Space"} };
            mapped_register.push_back(new_node);

            _size = nextAddr - old_address;
            return _size; 
        }  
};


/*** class HDW_REGISTERS *******************************************************
 *
 * ██╗  ██╗██████╗ ██╗    ██╗        ██████╗ ███████╗ ██████╗ ██╗███████╗████████╗███████╗██████╗ ███████╗
 * ██║  ██║██╔══██╗██║    ██║        ██╔══██╗██╔════╝██╔════╝ ██║██╔════╝╚══██╔══╝██╔════╝██╔══██╗██╔════╝
 * ███████║██║  ██║██║ █╗ ██║        ██████╔╝█████╗  ██║  ███╗██║███████╗   ██║   █████╗  ██████╔╝███████╗
 * ██╔══██║██║  ██║██║███╗██║        ██╔══██╗██╔══╝  ██║   ██║██║╚════██║   ██║   ██╔══╝  ██╔══██╗╚════██║
 * ██║  ██║██████╔╝╚███╔███╔╝███████╗██║  ██║███████╗╚██████╔╝██║███████║   ██║   ███████╗██║  ██║███████║
 * ╚═╝  ╚═╝╚═════╝  ╚══╝╚══╝ ╚══════╝╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝
 * 
 * (This will be moved to its own files)
 * Currently using a RAM model for testing.
 ****************************************************************/
class HDW_REGISTERS : public IDevice
{
    public:
        HDW_REGISTERS() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
            _size = 16 * 1024;
            _device_name = "HDW_REGISTERS";
        }
        virtual ~HDW_REGISTERS() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 

		int OnAttach(int nextAddr) override       { 
            Word old_address=nextAddr;
            this->heading = "Hardware Register Space";
            register_node new_node;
            new_node = { "HDW_REG_START", nextAddr,  { "Start of Register Space"} }; nextAddr+=2;
            mapped_register.push_back(new_node);

            // // reserve space for future use
            // int bank_size = 0xFFF0-nextAddr;      
            // std::string res = std::to_string(bank_size);
            // res += " bytes reserved for future use.";
            // new_node = { "HDW_RESERVED", nextAddr,  { res } }; nextAddr+=bank_size;
            // mapped_register.push_back(new_node);        

            _size = nextAddr - old_address;
            return _size; 
        }  
};


class HDW_RESERVED : public IDevice
{
    public:
        HDW_RESERVED() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;                    
            _size = 0;
            _device_name = "HDW_RESERVED";
        }
        virtual ~HDW_RESERVED() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

        bool OnInit() override 						{ return true; }
        bool OnQuit() override 						{ return true; }
        bool OnActivate() override 					{ return true; }
        bool OnDeactivate() override 				{ return true; }
        // bool OnEvent(SDL_Event* evnt) override 		{ return true; }
        bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
        bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
        bool OnRender() override 					{ return true; } 

        int OnAttach(int nextAddr) override       {
            Word old_address=nextAddr-1;
            this->heading = "Reserved Register Space";
            register_node new_node;

            // reserve space for future use
            int bank_size = 0xFFEF-nextAddr;      
            std::string res = std::to_string(bank_size);
            res += " bytes reserved for future use.";
            nextAddr+=bank_size;
            new_node = { "HDW_REG_END", nextAddr,  { res } }; // nextAddr+=1;
            mapped_register.push_back(new_node);     

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
            _device_name = "ROM_VECTS";
        }
        virtual ~ROM_VECTS() {
            //std::cout << clr::indent() << clr::LT_BLUE << "RAM Device Created" << clr::RETURN;        
        }    

		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 

		int OnAttach(int nextAddr) override       { 
            Word old_address=nextAddr;
            this->heading = "Hardware Interrupt Vectors";
            register_node new_node;
            new_node = { "HARD_EXEC", nextAddr,     { "EXEC Hardware Interrupt Vector" } }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "HARD_SWI3", nextAddr,     { "SWI3 Hardware Interrupt Vector" } }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "HARD_SWI2", nextAddr,     { "SWI2 Hardware Interrupt Vector" } }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "HARD_FIRQ", nextAddr,     { "FIRQ Hardware Interrupt Vector" } }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "HARD_IRQ", nextAddr,      { "IRQ Hardware Interrupt Vector" } }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "HARD_SWI", nextAddr,      { "SWI / SYS Hardware Interrupt Vector" } }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "HARD_NMI", nextAddr,      { "NMI Hardware Interrupt Vector" } }; nextAddr+=2;
            mapped_register.push_back(new_node);
            new_node = { "HARD_RESET", nextAddr,    { "RESET Hardware Interrupt Vector" } }; nextAddr+=2;
            mapped_register.push_back(new_node);

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
