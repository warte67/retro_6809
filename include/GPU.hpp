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

    Byte GetGlyphData(Byte index, Byte row) { return _gfx_glyph_data[index][row]; }

    // palette stuff
    union PALETTE {
        Word color;
        struct {
            Uint8 b : 4;
            Uint8 g : 4;
            Uint8 r : 4;
            Uint8 a : 4;
        };
    };
    Uint8 red(Uint8 index) { Uint8 c = _palette[index].r;  return c; }
    Uint8 grn(Uint8 index) { Uint8 c = _palette[index].g;  return c; }
    Uint8 blu(Uint8 index) { Uint8 c = _palette[index].b;  return c; }
    Uint8 alf(Uint8 index) { Uint8 c = _palette[index].a;  return c; }      

private: // PRIVATE MEMBERS
    // internal hardware register states:

    float _screen_width = 640.0f;
    float _screen_height = 400.0f;
    float _ext_width = 0.0f;
    float _ext_height = 0.0f;
    float _std_width = 0.0f;
    float _std_height = 0.0f;

    Byte _gpu_options = 0b1111'0001;    // (Byte) Bitflag Enables
                                        //    - bit 7    = Extended Bitmap:
                                        //                  0: Tilemap Display
                                        //                  1: Bitmap Display
                                        //    - bits 5-6 = Extended Color Mode:
                                        //                  00: 2-Colors
                                        //                  01: 4-Colors
                                        //                  10: 16-Colors
                                        //                  11: 256-Colors
                                        //    - bits 4   = Extended Display Enable
                                        //                 0: Disabled
                                        //                 1: Enabled
                                        //    - bits 3   = Application Screen Mode
                                        //                 0: Windowed
                                        //                 1: Fullscreen
                                        //    - bits 2   = Debug Enable
                                        //                 0: Disabled
                                        //                 1: Enabled
                                        //    - bits 1   = Sprite Enable
                                        //                 0: Disabled
                                        //                 1: Enabled
                                        //    - bit  0   = Standard Display Enable
                                        //                 0: Disabled
                                        //                 1: Enabled
                                        // 
    Byte _gpu_mode = 0b0110'0001;       // (Byte) Bitflag Enables
                                        //    - bit 7    = Standard Bitmap:
                                        //                  0: Text Display
                                        //                  1: Bitmap Display
                                        //    - bits 5-6 = Standard Color Mode:
                                        //                  00: 2-Colors
                                        //                  01: 4-Colors
                                        //                  10: 16-Colors
                                        //                  11: 256-Colors
                                        //    - bits 0-4 = Display Mode (0-31)
                                        // 

    void _render_extended_graphics();
    void _render_standard_graphics();
    void _update_text_buffer();
    void _update_tile_buffer();

    void _display_mode_helper(Byte mode, int &width, int &height);

    Byte _verify_gpu_mode_change(Byte data, Word map_register);


    void _setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte color_index, bool bIgnoreAlpha);
    void _build_palette();
    void _clear_texture(SDL_Texture* texture, Byte red, Byte grn, Byte blu, Byte alpha);

	// SDL stuff
    int initial_width = 640*2.125;
    int initial_height = 400*2.125;
	SDL_Window* pWindow = nullptr;
	SDL_Renderer* pRenderer = nullptr;

    SDL_Texture* pExt_Texture = nullptr;
    SDL_Texture* pStd_Texture = nullptr;
    SDL_Texture* pMain_Texture = nullptr;

	Uint32 window_flags = SDL_WINDOW_RESIZABLE;
    Uint32 renderer_flags = SDL_RENDERER_VSYNC_DISABLED;

    // Glyph and Palette Data
    std::vector<Byte> _ext_video_buffer;    // 64k extended video buffer
    std::vector<PALETTE> _palette;          // color palette
    Byte _gfx_pal_idx = 0x00;               // GFX_PAL_IDX
    Byte _gfx_glyph_idx = 0x00;             // GFX_GLYPH_IDX
    Byte _gfx_glyph_data[256][8]{0};        // GFX_GLYPH_DATA (Customizeable)

    int _num_displays;
    SDL_DisplayID* _displays = nullptr;
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

******************************************************/


// END: Gfx.hpp