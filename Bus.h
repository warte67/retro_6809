// Bus.h
//
#ifndef __BUS_H__
#define __BUS_H__

#include <string>
#include <SDL2/SDL.h>

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


    public:    
        ~Bus();
        Bus(Bus &other) = delete;
        void operator=(const Bus &) = delete;

        static void Error(std::string sErr);
        static Bus* GetInstance();
        static void Run();

        // temporary statics
        static SDL_Window* s_window;
        static SDL_Renderer* s_renderer;
        static int s_window_width;
        static int s_window_height;

};




#endif // __BUS_H__
