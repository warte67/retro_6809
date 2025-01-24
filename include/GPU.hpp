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
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 * 
 ************************************/

#pragma once

#include <SDL3/SDL.h>
#include "IDevice.hpp"
#include "font8x8_system.hpp"

class GPU : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    GPU();
    virtual ~GPU();

public: // VIRTUAL METHODS
    virtual int  OnAttach(int nextAddr);            // attach to the memory map
    virtual void OnInit();                          // initialize
    virtual void OnQuit();                          // shutdown
    virtual void OnActivate();                      // activate
    virtual void OnDeactivate();                    // deactivate
    virtual void OnEvent(SDL_Event* evnt);          // handle events
    virtual void OnUpdate(float fElapsedTime);      // update
    virtual void OnRender();                        // render

public: // PUBLIC ACCESSORS
    void RenderPresent() {
        SDL_RenderPresent(pRenderer);
    }
    SDL_Window* GetWindow() { return pWindow; }         // get the SDL window
    SDL_Renderer* GetRenderer() { return pRenderer; }   // get the SDL renderer
    inline Uint32 GetWindowID() { return SDL_GetWindowID(pWindow); }
    inline SDL_Window* GetSDLWindow() { return pWindow; }   


    SDL_Texture* GetTexture() { return pForeground_Texture; }  // fetch an SDL texture to render foreground

    static Byte GetGlyphData(Byte index, Byte row) { return _gpu_glyph_data[index][row]; }


    // // testing ...
    //         float Get_Texture_Width() 
    //         { 
    //             float w, h;
    //             SDL_GetTextureSize(pExt_Texture, &w, &h);
    //             return w; 
    //         }
    //         float Get_Texture_Height() 
    //         { 
    //             float w, h;
    //             SDL_GetTextureSize(pExt_Texture, &w, &h);
    //             return h; 
    //         }

            float Get_Width() { return _gpu_hres; }
            float Get_Height() { return _gpu_vres; }
    // ... testing


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
     // GPU Registers

    // GPU_OPTIONS
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
                                        //    - bits 2   = VSync Enable
                                        //                 0: Disabled
                                        //                 1: Enabled
                                        //    - bit 1    = Presentation
                                        //                 0: Overscan (Stretch)
                                        //                 1: Letterbox
                                        //    - bit  0   = Standard Display Enable
                                        //                 0: Disabled
                                        //                 1: Enabled
                                        // 
    // GPU_MODE
    Byte _gpu_mode = 0b0110'1001;       // (Byte) Bitflag Enables
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
    // GPU_VIDEO_MAX
    Word _gpu_video_max = 0x23FF;       // (Word) Video Buffer Maximum (Read Only)
                                        //  Note: This will change to reflect
                                        //        the size of the last cpu 
                                        //        accessible memory location
                                        //        of the currently active 
                                        //        standard video mode.
                                        //                                        
    // GPU_HRES             
    Word _gpu_hres = 0;                 // (Word) Horizontal Resolution (Read Only)
                                        //   Note: This will reflect the number of
                                        //        pixel columns for bitmap modes.
                                        // 
    // GPU_VRES              
    Word _gpu_vres = 0;                 // (Word) Vertical Resolution (Read Only)
                                        //   Note: This will reflect the number of
                                        //        pixel rows for bitmap modes.
                                        // 
    // GPU_TCOLS 
    Byte _gpu_tcols = 0;                // (Byte) Text Horizontal Columns (Read Only)
                                        //   Note: This will reflect the number of
                                        //        glyph columns for text modes.
                                        // 
    // GPU_TROWS
    Byte _gpu_trows = 0;                // (Byte) Text Vertical Rows (Read Only)
                                        //   Note: This will reflect the number of
                                        //        glyph rows for text modes.
                                        // 
    // GPU_PAL_INDEX
    Byte _gpu_pal_index = 0;            // (Byte) Palette Index (Read Only)
                                        //   Note: This will reflect the currently
                                        //        selected palette index.
                                        // 
    // GPU_PAL_COLOR
    std::vector<PALETTE> _palette;      // (Word) Color Palette Data (A4R4G4B4 format)
                                        //   Note: This is the color data for an
                                        //        individual palette entry. Write to 
                                        //        DSP_PAL_IDX with the index within the
                                        //        color palette prior to reading or
                                        //        writing the color data in the
                                        //        GFX_PAL_CLR register.
                                        // 
    // GFX_GLYPH_IDX 
    Word _gpu_glyph_idx = 0;            // (Word) Glyph Index (Read Only)
                                        //   Note: This will reflect the currently
                                        //        selected glyph index.
                                        //
    // GFX_GLYPH_DATA
    inline static Byte _gpu_glyph_data[256][8]{0};    // (8-Bytes) 8 rows of binary encoded glyph pixel data
                                        //   Note: This is the pixel data for a
                                        //        specific text glyph. Each 8x8
                                        //        text glyph is composed of 8 bytes.
                                        //        The first byte in this array
                                        //        represents the top line of 8 pixels.
                                        //        Each array entry represents a row of 8 pixels.
                                        // 

    void _render_extended_graphics();
    void _render_standard_graphics();
    void _update_text_buffer();
    void _update_tile_buffer();
    void _display_mode_helper(Byte mode, int &width, int &height);
    Byte _verify_gpu_mode_change(Byte data, Word map_register);
    void _setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte color_index, bool bIgnoreAlpha);
    void _build_palette();
    void _clear_texture(SDL_Texture* texture, Byte alpha, Byte red, Byte grn, Byte blu);

    // internal hardware register states:

    float _screen_width = 640.0f;       // display mode timing 640x400 @ 70hz
    float _screen_height = 400.0f;      // display mode timing 640x400 @ 70hz
    float _ext_width = 0.0f;            // ToDo: remove this (redundant) 
    float _ext_height = 0.0f;           // ToDo: remove this (redundant)
    float _std_width = 0.0f;            // ToDo: remove this (redundant)
    float _std_height = 0.0f;           // ToDo: remove this (redundant)


	// SDL stuff
    int initial_width = 640*2.125;
    int initial_height = 400*2.125;
	SDL_Window* pWindow = nullptr;
	SDL_Renderer* pRenderer = nullptr;
    SDL_Texture* pExt_Texture = nullptr;
    SDL_Texture* pStd_Texture = nullptr;
    SDL_Texture* pForeground_Texture = nullptr;
	Uint32 window_flags = SDL_WINDOW_RESIZABLE;
    Uint32 renderer_flags = SDL_RENDERER_VSYNC_DISABLED;

    // Extended Video Buffer (Sprites and Tiles Too?)
    std::vector<Byte> _ext_video_buffer;    // 64k extended video buffer
};

/*** NOTES: ****************************************
 
    //
    // Extended Graphics Registers
    //
        
    MEM_DSPLY_SIZE   = 0xFF83, // (Word) Extended Graphics Buffer Size
        
    MEM_EXT_ADDR     = 0xFF85, // (Word) Extended Memory Address Port
    MEM_EXT_PITCH    = 0xFF87, // (Word) number of bytes per line
    MEM_EXT_WIDTH    = 0xFF89, // (Word) width before skipping to next line
    MEM_EXT_DATA     = 0xFF8B, // (Byte) External Memory Data Port
        
    MEM_DYN_SIZE     = 0xFF8C, // (Word) dynamic memory block size
        //      Notes: Memory allocation occurs when the 
        //             least-significant byte is written.
        //             Reads as total number of bytes allocated
        //             or freed. When $0000 is written to this 
        //             port, memory node at MEM_DYN_ADDR is freed.
    MEM_DYN_ADDR     = 0xFF8E, // (Word) address of a dynamic memory node
    MEM_DYN_AVAIL    = 0xFF90, // (Word) number of non-allocated bytes

    //
    // Sprite Registers
    //

    //
    // Tilemap Registers
    // 




    GPU_OPTIONS & GPU_MODE changes:

    // GPU_OPTIONS
    Byte _gpu_options = 0b1111'0001;    // (Byte) Bitflag Enables





    // GPU_MODE
    Word _gpu_mode;                     // (Word) Display Mode:
    // GPU_MODE_MSB                     // (Byte) Display Mode Most Significant Byte

                                        //    - bits 15 (7)  = Extended Display Enable
                                        //                 0: Disabled
                                        //                 1: Enabled

                                        //    - bit 14 (6)  = Extended Display Slow Update

                                        //    - bits 12-13 (4-5) = Extended Color Depth:
                                        //                  00: 2-Colors
                                        //                  01: 4-Colors
                                        //                  10: 16-Colors
                                        //                  11: 256-Colors

                                        //    - bit 11 (3)   = Extended Bitmap:
                                        //                  0: Tilemap Display
                                        //                  1: Bitmap Display

                                        //    - bits 10 (2)  = Emulation Screen Mode
                                        //                 0: Windowed
                                        //                 1: Fullscreen

                                        //    - bit 9 (1)   = VSync Enable
                                        //                 0: Disabled
                                        //                 1: Enabled

                                        //    - bit 8 (0)   = Presentation
                                        //                 0: Overscan (Stretch)
                                        //                 1: Letterbox

                                        // -------------------------------------------------
    // GPU_MODE_LSB                     // (Byte) Display Mode Least Significant Byte

                                        //    - bit 7   = Standard Display Enable
                                        //                 0: Disabled
                                        //                 1: Enabled

                                        //    - bit 6    = Standard Display Slow Update

                                        //    - bits 4-5 = Standard Bitmap Color Depth:
                                        //                  00: 2-Colors
                                        //                  01: 4-Colors
                                        //                  10: 16-Colors
                                        //                  11: 256-Colors

                                        //    - bit 3    = Standard Bitmap:
                                        //                  0: Text Display
                                        //                  1: Bitmap Display

                                        //    - bit 2    = 0: 320/256 width,  1: 160/128 width
                                        //    - bit 1    = 0: 200/160 height, 1: 160/80 height
                                        //    - bit 0    = Base Resolution: 0:320x200, 1:256x160

******************************************************/

// END: Gfx.hpp