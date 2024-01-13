/////
// Device.h
// 
// Base device to be attached to the memory map.
////////

#ifndef __DEVICE_H__
#define __DEVICE_H__

// #include <string>
// #include <vector>
#include "types.h"

class Device
{
    public:
        Device() {  _deviceName = "Device"; }
        Device(std::string sName) : _deviceName(sName) {}
        virtual ~Device() {}

        // virtuals
		virtual Word OnAttach(Word nextAddr);        // map memory and return the size
		virtual void OnInit();
		virtual void OnQuit();
		virtual void OnActivate();
		virtual void OnDeactivate();
		virtual void OnEvent(SDL_Event* evnt);
		virtual void OnUpdate(float fElapsedTime);
		virtual void OnRender();

        virtual Byte read(Word offset, bool debug = false);
        virtual void write(Word offset, Byte data, bool debug = false);
        virtual Word read_word(Word offset, bool debug = false);
        virtual void write_word(Word offset, Word data, bool debug = false);  

        // public accessors
        std::string name() { return _deviceName; }
        Word Base() { return m_base; }
        void Base(Word addr) { m_base = addr; }
        Word Size() { return m_size = m_memory.size(); }
        void Size(Word size) { m_size = size; m_memory.resize(size); }
        std::string Name()  { return _deviceName; }
        void Name(std::string n) { _deviceName = n; }
        Byte _memory(Word ofs) { return m_memory[ofs]; }
        void _memory(Word ofs, Byte data) { m_memory[ofs] = data; }

        // helpers
        void DisplayEnum(std::string sToken, Word ofs, std::string sComment);

    private:
        std::string _deviceName = "??DEV??";

    protected:
        Uint16 m_base = 0;
        Uint16 m_size = 0;
        std::vector<Uint8> m_memory;	// memory mapped to this device    
};


#endif //__DEVICE_H__