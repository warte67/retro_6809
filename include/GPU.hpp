/*** GPU.hpp *******************************************
 *     _____ _____  _    _       _                 
 *    / ____|  __ \| |  | |     | |                
 *   | |  __| |__) | |  | |     | |__  _ __  _ __  
 *   | | |_ |  ___/| |  | |     | '_ \| '_ \| '_ \ 
 *   | |__| | |    | |__| |  _  | | | | |_) | |_) |
 *    \_____|_|     \____/  (_) |_| |_| .__/| .__/ 
 *                                    | |   | |    
 *                                    |_|   |_|    
 *   
 * This is the base class for the primary graphics devices. It provides the
 * basic methods for reading and writing to the device's memory.
 * 
 ************************************/

#pragma once

#include "IDevice.hpp"
#include "font8x8_system.hpp"

class GPU : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    GPU();
    virtual ~GPU();

public: // VIRTUAL METHODS
    virtual Byte OnRead(Word offset);               // read
    virtual void OnWrite(Word offset, Byte data);   // write
    // =============================================
    virtual int  OnAttach(int nextAddr);            // attach to the memory map
    virtual bool OnInit();                          // initialize
    virtual bool OnQuit();                          // shutdown
    virtual bool OnActivate();                      // activate
    virtual bool OnDeactivate();                    // deactivate
    virtual bool OnEvent(SDL_Event* evnt);          // handle events
    virtual bool OnUpdate(float fElapsedTime);      // update
    virtual bool OnRender();                        // render

public: // PUBLIC ACCESSORS
    void RenderPresent() {
        SDL_RenderPresent(pRenderer);
    }
    SDL_Window* GetWindow() { return pWindow; }         // get the SDL window
    SDL_Renderer* GetRenderer() { return pRenderer; }   // get the SDL renderer

    enum _GPU_ENABLE{
        ENABLE_STD      = 0b00001000,
        ENABLE_EXT      = 0b00000100,
        ENABLE_SPRITES  = 0b00000010,
        ENABLE_CURSOR   = 0b00000001
    };

private: // PRIVATE MEMBERS
    // internal hardware register states:
    Byte _gpu_enable    = ENABLE_STD;
    Byte _gpu_std_mode  = 0x0B;   // 0x0B = 40x25 (320x200)
    Byte _gpu_ext_mode  = 0x8B;   // 0x8B = 320x200
    Byte _gpu_emu_mode  = 0b00000000;
    bool _video_hires = false;          // true: 640x400, false: 512x384 
    float _screen_width = 0.0f;
    float _screen_height = 0.0f;

    // standard display flags
    bool _is_std_text_mode = false;
    bool _is_std_bitmap_mode = false;
    Byte _std_color_depth = 0;
    float _std_overscan_horiz = 1.0f;
    float _std_overscan_vert = 1.0f;
    float _std_width = 0.0f;
    float _std_height = 0.0f;

    // extended display flags
    bool _is_ext_tile_mode = false;
    bool _is_ext_bitmap_mode = false;
    Byte _ext_color_depth = 0;
    float _ext_overscan_horiz = 1.0f;
    float _ext_overscan_vert = 1.0f;
    float _ext_width = 0.0f;
    float _ext_height = 0.0f;

    Byte _change_gpu_enable(Byte data);
    Byte _change_std_mode(Byte data);
    Byte _change_ext_mode(Byte data);
    Byte _change_emu_mode(Byte data);

	// SDL stuff
    int initial_width = 640*2.75;      // 1280;
    int initial_height = 400*2.75;     // 800;
	SDL_Window* pWindow = nullptr;
	SDL_Renderer* pRenderer = nullptr;

    SDL_Texture* pExt_Texture = nullptr;
    SDL_Texture* pStd_Texture = nullptr;
    SDL_Texture* pMain_Texture = nullptr;

	Uint32 window_flags = SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE;
    Uint32 renderer_flags = SDL_RENDERER_VSYNC_ADAPTIVE;    
    // SDL_RENDERER_VSYNC_DISABLED
};

/*** NOTES: ****************************************
 
    A fundamental change in display modes has been incorporated. A more 
    conventional display mode of 640x480 @ 70hz will be implemented. From 
    this mode a 640x400 and 512x384 will be derrived. Single, Double, Quad, 
    and Octa overscan modes will be supported. Including text/bitmap, both 
    base pixel resolutions, and overscan modes there will be a total of 256 
    different display modes as defined in the GFX_MODE register.



    SDL3 Specific Notes:

    SDL3 Pixel Format:  SDL_PIXELFORMAT_ARGB4444

    see: https://wiki.libsdl.org/SDL3/SDL_PixelFormatDetails
    see: https://wiki.libsdl.org/SDL3/SDL_CreateTexture
    see: https://wiki.libsdl.org/SDL3/SDL_SetRenderLogicalPresentation
    see: https://wiki.libsdl.org/SDL3/SDL_ConvertEventToRenderCoordinates


    //Lock a portion of the texture for write-only pixel access.
    SDL_LockTexture(texture, NULL, &pixels, &pitch);
    //Unlock a texture, uploading the changes to video memory
    SDL_UnlockTexture(texture);




EXAMPLE:

#include <SDL3/SDL.h>

#include <stdlib.h>

// Moving Rectangle 
int main(int argc, char *argv[])
{
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Event event;
        SDL_FRect r;

        if (!SDL_Init(SDL_INIT_VIDEO)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
                return 3;
        }

        window = SDL_CreateWindow("SDL_CreateTexture",
                        1024, 768,
                        SDL_WINDOW_RESIZABLE);

        r.w = 100;
        r.h = 50;

        renderer = SDL_CreateRenderer(window, NULL);

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);

        while (1) {
                SDL_PollEvent(&event);
                if(event.type == SDL_EVENT_QUIT)
                        break;
                r.x=rand()%500;
                r.y=rand()%500;

                SDL_SetRenderTarget(renderer, texture);
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
                SDL_RenderClear(renderer);
                SDL_RenderRect(renderer,&r);
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
                SDL_RenderFillRect(renderer, &r);
                SDL_SetRenderTarget(renderer, NULL);
                SDL_RenderTexture(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);
        }
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 0;
}


Also take a look at this example of rendering to a buffer then 
copying it to a texture to be rendered.



#include <SDL.h>

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;

int main( int argc, char *args[] )
{
    int w = 640;
    int h = 480;
    Uint32 format = SDL_PIXELFORMAT_RGB888;
    SDL_CreateWindowAndRenderer(w, h, 0, &sdlWindow, &sdlRenderer);
    SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer, format, SDL_TEXTUREACCESS_STREAMING, w, h);
    extern uint32_t *pixels;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            pixels[x + y * w] = SDL_MapRGB(format, 255, 255, 255);
        }
    }

    SDL_UpdateTexture(sdlTexture, NULL, pixels, 640 * sizeof (Uint32));
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);
    SDL_Delay(5000);

    SDL_Quit();
    return 0;
}




  
 ****************************************/



// END: Gfx.hpp