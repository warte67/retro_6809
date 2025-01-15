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
    friend class Memory;    // This is okay! The Memory Device serves as 
                            // the parent container for these objects.

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    IDevice() { _device_name = "IDevice_Device"; }
    IDevice(std::string sName) : _device_name(sName) {}
    virtual ~IDevice() {}

public: // VIRTUAL METHODS
    // virtual Byte OnRead(Word address);
    // virtual void OnWrite(Word address, Byte data);

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
    Word base_address = 0;;
 
    // device memory map description
    std::string heading;                    // string that describes the entire device
    struct REGISTER_NODE {
        std::string name;                   // register label
        int address;                        // register starting address
        std::function<Byte()> read;         // Read handler (lambda or function pointer)
        std::function<void(Byte)> write;    // Write handler (lambda or function pointer)    
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
