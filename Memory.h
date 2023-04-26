////
// Memory.h
//////

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "types.h"
#include "Device.h"

class Memory
{
    friend class Bus;

    public:
        //
        Memory();
        virtual ~Memory();

        Byte read(Word offset, bool debug = false);
        void write(Word offset, Byte data, bool debug = false);
        Word read_word(Word offset, bool debug = false);
        void write_word(Word offset, Word data, bool debug = false);  

		Word Attach(Device* dev);   // attach a user defined memory node
        void DumpMemoryMap();

    private:
		void _onInit();
		void _onQuit();
		void _onActivate();
		void _onDeactivate();
		void _onEvent(SDL_Event* evnt);
		void _onUpdate(float fElapsedTime);
		void _onRender();

        int _lastAddress = 0;
        std::vector<Device*> _memoryNodes;
};


#endif // __MEMORY_H__