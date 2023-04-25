/////
// Device.h
// 
// Base device to be attached to the memory map.
////////

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <string>
#include <vector>
#include "types.h"

class Device
{
    public:
        Device() {  _deviceName = "Device"; }
        Device(std::string sName) : _deviceName(sName) {}
        virtual ~Device() {}

        // pure virtuals
		virtual Word OnAttach() = 0;        // map memory and return the size
		virtual void OnInit() = 0;
		virtual void OnQuit() = 0;
		virtual void OnActivate() = 0;
		virtual void OnDeactivate() = 0;
		virtual void OnEvent(SDL_Event* evnt) = 0;
		virtual void OnUpdate(float fElapsedTime) = 0;
		virtual void OnRender() = 0;

        // virtuals
        virtual Byte read(Word offset, bool debug = false);
        virtual void write(Word offset, Byte data, bool debug = false);
        virtual Word read_word(Word offset, bool debug = false);
        virtual void write_word(Word offset, Word data, bool debug = false);  

    private:

        std::string _deviceName = "??DEV??";
        Uint16 base = 0;
        Uint16 size = 0;
        std::vector<Uint8> memory;	// memory mapped to this device    
};


#endif //__DEVICE_H__