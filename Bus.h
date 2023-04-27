// Bus.h
//
#ifndef __BUS_H__
#define __BUS_H__

// #include <string>
// #include <SDL2/SDL.h>
#include "types.h"
#include "Memory.h"
#include "Gfx.h"

class Bus
{
    private:
        Bus();

        static Bus* s_bus;
        static bool s_bIsRunning;
        static bool s_bIsDirty;

        static void _quit();
        static void _onetime_init();
        static void _final_quit();
        static void _onActivate();
        static void _onDeactivate();
        static void _onUpdate();
        static void _onEvent();
        static void _onRender();
        
        static Memory* s_memory;

        int _fps = 0;
        static Gfx* _gfx;

    public:    
        ~Bus();
        Bus(Bus &other) = delete;
        void operator=(const Bus &) = delete;

        static void Error(std::string sErr);
        static Bus* GetInstance();
        static void Run();
    
        // static accessors
        int FPS() { return _fps; }
        static Gfx* gfx() { return _gfx;}
        static Memory* memory() {return s_memory;}
        static bool IsRunning() { return s_bIsRunning;}
        static void IsRunning(bool state) { s_bIsRunning = state;}
        static bool IsDirty() { return s_bIsDirty;}
        static void IsDirty(bool state) { s_bIsDirty = state;}
        // static memory access
        static Byte read(Word offset, bool debug = false);
        static void write(Word offset, Byte data, bool debug = false);
        static Word read_word(Word offset, bool debug = false);
        static void write_word(Word offset, Word data, bool debug = false);          
};




#endif // __BUS_H__
