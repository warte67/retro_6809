/***  IDevice.hpp  ****************************
*  _____ _____             _            _                 
* |_   _|  __ \           (_)          | |                
*   | | | |  | | _____   ___  ___ ___  | |__  _ __  _ __  
*   | | | |  | |/ _ \ \ / / |/ __/ _ \ | '_ \| '_ \| '_ \ 
*  _| |_| |__| |  __/\ V /| | (_|  __/_| | | | |_) | |_) |
* |_____|_____/ \___| \_/ |_|\___\___(_)_| |_| .__/| .__/ 
*                                            | |   | |    
*                                            |_|   |_|    
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
    virtual Word OnAttach(Word nextAddr) = 0;   

public: // PUBLIC ACCESSORS
    // ... 

protected: // PROTECTED ACCESSORS
    Word base();
    void base(Word addr);
    Word size();
    void size(Word size);
    std::string name();
    void name(std::string n);
    Byte memory(Word ofs);
    void memory(Word ofs, Byte data);

    Uint16 _base = 0;						std::mutex _mutex_base;		
    Uint16 _size = 0;						std::mutex _mutex_size;		
    std::string _device_name = "??DEV??";	std::mutex _mutex_device_name;				
    std::vector<Uint8> _memory;				std::mutex _mutex_memory;
};




/////////////////////
//
// Standard Memory Devices
//
////////

class RAM : public IDevice
{
    public:
        RAM() {  name("RAM"); }
        RAM(std::string sName) { name(sName); }
        virtual ~RAM() {}    

		Word OnAttach(Word nextAddr) override 		{ return nextAddr; }        // { return 0xA5A5; }
		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; }           
		bool OnRender() override 					{ return true; } 

        // Word Attach(Word size, std::string sComment)
        // {
        //     // build a memory map node
        //     //  ...
        //     //      Bus:: _next_address
        //     //      sComment
        //     //      size
        //     return size;
        // }
};

class ROM : public IDevice
{
    public:
        ROM() {  name("ROM"); }
        ROM(std::string sName) { name(sName); }
        virtual ~ROM() {}    

		Word OnAttach(Word nextAddr) override 		{ return nextAddr; }        // { return 0xA5A5; }
		bool OnInit() override 						{ return true; }
		bool OnQuit() override 						{ return true; }
		bool OnActivate() override 					{ return true; }
		bool OnDeactivate() override 				{ return true; }
		// bool OnEvent(SDL_Event* evnt) override 		{ return true; }
		bool OnEvent(SDL_Event* evnt) override 		{ return (evnt==nullptr); }         // return true
		bool OnUpdate(float fElapsedTime) override 	{ return (fElapsedTime==0.0f); }    // { return true; } 
		bool OnRender() override 					{ return true; }        

		// helper to set initial ROM values
        void write_to_rom(Word offset, Byte data);

        // Word Attach(Word size, std::string sComment)
        // {
        //     // build a memory map node
        //     //  ...
        //     //      Bus:: _next_address
        //     //      sComment
        //     //      size
        //     return size;
        // } 
};


