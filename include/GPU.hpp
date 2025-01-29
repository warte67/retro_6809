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

    float Get_Width() { return _gpu_hres; }
    float Get_Height() { return _gpu_vres; }

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

    // GPU_MODE
    Word _gpu_mode = 0b1011'1000'1011'0000; // (Word) Display Mode:
    // GPU_MODE_MSB                     // (Byte) Display Mode Most Significant Byte
                                        //    - bits 15 (7)  = Extended Display Enable
                                        //                 0: Disabled
                                        //                 1: Enabled
                                        //    - bit 14 (6)  = (reserved)
                                        //    - bits 12-13 (4-5) = Extended Color Depth:
                                        //                  00: 2-Colors
                                        //                  01: 4-Colors
                                        //                  10: 16-Colors
                                        //                  11: 256-Colors
                                        //    - bit 11 (3)   = Extended Rendering Mode:
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
                                        //    - bit 6    = (reserved)
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
    // *** Graphics Processor Extension Registers *** //

    // GPU_BGND_SIZE
    Word _gpu_bgnd_size = 0;            // (Word) Extended Graphics Buffer Size (Read Only)
                                        // Note: The primary extended graphics buffer
                                        //       always begins at $0000. This is also highest
                                        //       accessible memory location of the currently
                                        //       active background video mode.       

    // GPU_BLIT_ADDR
    Word _gpu_blit_addr = 0;            // (Word) Graphics Memory Address Port   
                                        // Note: When GPU_BLIT_DATA is accessed, this register
                                        //       is automatically incremented to point to the  
                                        //       next byte to be read or written based on the 
                                        //       values in GPU_BLIT_PITCH and GPU_BLIT_WIDTH.

    // GPU_BLIT_PITCH
    Word _gpu_blit_pitch = 0;           // (Word) Number of Bytes Per Display Line

    // GPU_BLIT_WIDTH
    Word _gpu_blit_width = 0;           // (Word) Width of the Image Block in Pixels
    Word _gpu_blit_width_count = 0;

    // GPU_BLIT_DATA
    // Byte _gpu_blit_data = 0;            // (Byte) GPU Memory Data Port
    Byte _gpu_blit_read_data();
    void _gpu_blit_write_data(Byte data);
    void _gpu_blit_increment_address();


// GPU Dynamic Memory Registers:
// GPU_DYN_HANDLE          (Byte)      // Dynamic Memory HANDLE
// GPU_DYN_CUR_ADDR        (Word)      // Current Dynamic Memory ADDRESS
//                                     // (autoincrements by 1 on read/write)
// GPU_DYN_END_ADDR                    // Last Dynamic Memory ADDRESS in this block
// GPU_DYN_END_DISTANCE    (Word)      // Dynamic Memory Distance to End
// GPU_DYN_DATA            (Byte)      // Dynamic Memory DATA (Read/Write)
//    


    void _render_extended_graphics();
    void _render_standard_graphics();
    void _update_text_buffer();
    void _update_tile_buffer();
    void _display_mode_helper(Byte mode, int &width, int &height);
    // Byte _verify_gpu_mode_change(Byte data, Word map_register);
    void _verify_gpu_mode_change(Word mode_data);
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
    
GPU_BGND_SIZE       (Word)          // (Word) Extended Graphics Buffer Size (Read Only)
                                    // The primary extended graphics buffer
                                    // always begins at $0000. This is also Last
                                    // Address for extended graphics.

//
// GPU_BLIT_ADDR       (Word)       // (Word) Graphics Memory Address Port
// GPU_BLIT_PITCH      (Word)       // (Word) number of bytes per line
// GPU_BLIT_WIDTH      (Word)       // (Word) width before skipping to next line
// GPU_BLIT_DATA       (Byte)       // (Byte) GPU Memory Data Port

// GPU Dynamic Memory Registers:
// GPU_DYN_HANDLE          (Byte)   // Dynamic Memory HANDLE
// GPU_DYN_CUR_ADDR        (Word)   // Current Dynamic Memory ADDRESS
//                                  // (autoincrements by 1 on read/write)
// GPU_DYN_END_ADDR                 // Last Dynamic Memory ADDRESS in this block
// GPU_DYN_END_DISTANCE    (Word)   // Dynamic Memory Distance to End
// GPU_DYN_DATA            (Byte)   // Dynamic Memory DATA (Read/Write)
//
// GPU_ARG_1           (Word)       // Argument 1
// GPU_ARG_2           (Word)       // Argument 2
// GPU_ARG_3           (Word)       // Argument 3
// GPU_ARG_4           (Word)       // Argument 4
// GPU_ARG_5           (Word)       // Argument 5
//
// GPU_COMMAND         (Byte)       // Graphics Processing Unit Command
// GPU_CMD_CLEAR                    // Clear Video Buffer:
//                                  // GPU_ARG_1_MSB = Color Index
// GPU_CMD_COPY                     // Copy GPU Memory to GPU Memory
//                                  // Copy from [GPU_ARG_1] through [GPU_ARG_2]
//                                  // to [GPU_ARG_3] through [GPU_ARG_4]                                
// GPU_CMD_BLIT_GPU                 // BLiT from GPU memory to Display (RAM to Screen) 
// GPU_CMD_GPU_BLIT                 // BLiT from Display to GPU memory (Screen to RAM)
// GPU_CMD_SCROLL                   // Scroll Video Buffer:
//                                  //     GPU_ARG_1_MSB = signed 8-bit horiz. offset
//                                  //     GPU_ARG_1_LSB = signed 8-bit vert. offset
// GPU_CMD_NEW_IMG                  // Allocate a new GPU Image as in a Sprite or Tile
//                                  // returns a valid node address for the new image                                
//                                  // size is based on color depth as 32, 64, 128, or 256
// GPU_CMD_FREE_IMG                 // Free a GPU Image (GPU_ARG_1_MSB = Image Index)
// GPU_CMD_NEW_BUFFER               // Allocate a new GPU Buffer (of arbetrary size)
// GPU_CMD_FREE_BUFFER              // Free a GPU Buffer (GPU_ARG_1_MSB = Buffer Index)
// GPU_CMD_DRAW_LINE
// GPU_CMD_DRAW_CIRCLE
// GPU_CMD_DRAW_RECT
// GPU_CMD_FILL_RECT
//     ... etc
//
//
//
//
// GPU_ERROR           (Byte)       // (Byte) Graphics Processing Unit Error
//                                  // $NN = No Error
//                                  // $NN = Invalid Command
//                                  // $NN = Invalid Width
//                                  // $NN = Invalid Height
//                                  // $NN = Invalid Pitch
//                                  // $NN = Invalid Data
//                                  // $NN = Invalid Mode
//                                  // $NN = Invalid Index
//                                  // $NN = Invalid Address
//                                  // $NN = Invalid Argument
//                                  // $NN = Out of Memory
//                                  // 
//                                  // ... etc.
//  
//  
// // Sprite Registers: 
// GPU_SPR_MAX         (Byte)       // Maximum Sprite Index (Read Only)
// GPU_SPR_IDX         (Byte)       // Sprite Index
// GPU_SPR_XPOS        (SInt16)     // Sprite X Position
// GPU_SPR_YPOS        (SInt16)     // Sprite Y Position
// GPU_SPR_IMG_IDX     (Byte)       // Sprite Image Index
// GPU_SPR_MASK        (Byte)       // Sprite Collision Mask (4x4)
// GPU_SPR_FLAGS       (Byte)       // Sprite Flags:
//                                  // % 0000'0011: 
//                                  //      00 = 2 colors, 
//                                  //      01 = 4 colors, 
//                                  //      10 = 16 colors, 
//                                  //      11 = 256 colors
//                                  // % 0000'0100: Double Width
//                                  // % 0000'1000: Double Height
//                                  // % 0001'0000: Flip Horizontal
//                                  // % 0010'0000: Flip Vertical
//                                  // % 0100'0000: Collision Enable
//                                  // % 1000'0000: Display Enable
//  
// // Image Editing Registers:               
// GPU_BMP_MAX         (Byte)       // Maximum Bitmap Index (Read Only)
// GPU_BMP_IDX         (Byte)       // Bitmap Image Index (0-255)
// GPU_BMP_OFFSET      (Byte)       // Offset Within the Image Buffer(0-255)
// GPU_BMP_DATA        (Byte)       // Bitmap Data (Read Write)
// GPU_BMP_FLAGS       (Byte)       // Bitmap Flags:
//                                  // % 0000'0011: 
//                                  //      00 = 2 colors (size=32), 
//                                  //      01 = 4 colors (size=64), 
//                                  //      10 = 16 colors (size=128), 
//                                  //      11 = 256 colors (size=256)
//                                  // % 1111'1100: Reserved
//  
// // Tilemap Registers:    
// GPU_TMAP_WIDTH      (Word)       // Tilemap Width (in pixels)
// GPU_TMAP_HEIGHT     (Word)       // Tilemap Height (in Pixels)
// GPU_TMAP_XPOS       (SInt16)     // Tilemap X Position (top left corner)
// GPU_TMAP_YPOS       (SInt16)     // Tilemap Y Position (top left corner)
// GPU_TMAP_CLIP_X1    (Word)       // Tilemap Clip Region X1
// GPU_TMAP_CLIP_Y1    (Word)       // Tilemap Clip Region Y1
// GPU_TMAP_CLIP_X2    (Word)       // Tilemap Clip Region X2
// GPU_TMAP_CLIP_Y2    (Word)       // Tilemap Clip Region Y2



******************************************************/

// END: Gfx.hpp
