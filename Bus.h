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

    public:    
        ~Bus();
        Bus(Bus &other) = delete;
        void operator=(const Bus &) = delete;

        static void Error(std::string sErr);
        static Bus* GetInstance();

        static void OnInit();
        static void run();

        // // move to the Gfx object
        // SDL_Window* pWin = nullptr;
        // Uint32 window_flags = 0;
        // SDL_Renderer* rend = nullptr;
        // Uint32 render_flags = SDL_RENDERER_ACCELERATED;
        // int pixel_scan = 2;
        // int window_width = 640 * pixel_scan;
        // int window_height = 400 * pixel_scan;

};




#endif // __BUS_H__


/*
		OnAttach()			called on creation to add to memory map
		OnInit()			calls once after all devices are added
		OnQuit()			called when Gfx is closed
		OnActivate()		called when opening a new graphics mode
		OnDeactivate()		called to close current graphics mode
		OnEvent(event)		called when an event was received
		OnUpdate(elapsedTime)	called every	
		OnRender()			called every frame for rendering images
*/