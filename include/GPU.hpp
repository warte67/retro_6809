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

#include <SDL3/SDL.h>
#include "IDevice.hpp"
#include "font8x8_system.hpp"

class GPU : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    GPU();
    virtual ~GPU();

public: // VIRTUAL METHODS
    // virtual Byte OnRead(Word offset);               // read
    // virtual void OnWrite(Word offset, Byte data);   // write
    // =============================================
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

    static Byte GetGlyphData(Byte index, Byte row) { return _gfx_glyph_data[index][row]; }

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

    float _screen_width = 640.0f;       // display mode timing 640x400 @ 70hz
    float _screen_height = 400.0f;      // display mode timing 640x400 @ 70hz
    float _ext_width = 0.0f;
    float _ext_height = 0.0f;
    float _std_width = 0.0f;
    float _std_height = 0.0f;

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
                                        //    - bits 1   = Sprite Enable
                                        //                 0: Disabled
                                        //                 1: Enabled
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
    
    Byte _gpu_pal_index = 0x00;             // GPU_PAL_INDEX

    Byte _gfx_glyph_idx = 0x00;             // GFX_GLYPH_IDX
    inline static Byte _gfx_glyph_data[256][8]{0};        // GFX_GLYPH_DATA (Customizeable)
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


    //
    // Color Palette Registers
    //
        
    GFX_PAL_IDX      = 0xFE08, //  (Byte) Color Palette Index
        // GFX_PAL_IDX: 0-255
        // Note: Use this register to set the index into theColor Palette. 
        //       Set this value prior referencing color data (GFX_PAL_CLR).
        
    GFX_PAL_CLR      = 0xFE09, //  (Word) Indexed Color Palette Data
        // GFX_PAL_CLR: Color Data A4R4G4B4 format
        // Note: This is the color data for an individual palette entry.
        //     Write to DSP_PAL_IDX with the index within the color palette
        //     prior to reading or writing the color data in the GFX_PAL_CLR register.
        
    //
    // Text Glyph Registers
    //

    GFX_GLYPH_IDX    = 0xFE0B, //  (Byte) Text Glyph Index
        // GFX_GLYPH_IDX: 0-256
        // Note: Set this register to index a specific text glyph. Set this value
        //     prior to updating glyph pixel data.
        
    GFX_GLYPH_DATA   = 0xFE0C, //  (8-Bytes) Text Glyph Pixel Data Array
        // GFX_GLYPH_DATA: 8 rows of binary encoded glyph pixel data
        // Note: Each 8x8 text glyph is composed of 8 bytes. The first byte in this 
        //     array represents the top line of 8 pixels. Each array entry represents
        //     a row of 8 pixels. 

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




    TODO:

            struct MAP_NODE {
                std::string label;                  // Register label (e.g., "GPU_OPTIONS")
                Word address;                       // Memory-mapped address
                std::function<Byte()> read;         // Read handler (lambda or function pointer)
                std::function<void(Byte)> write;    // Write handler (lambda or function pointer)
                std::vector<std::string> comments;  // List of comments/documentation lines
            };


            std::vector<RegisterRange> registerRanges;

            Byte GPU::OnRead(Word offset) {
                if (readHandlers.count(offset)) {
                    return readHandlers[offset]();
                }
                return IDevice::OnRead(offset); // Fallback for unknown offsets
            }

            void GPU::OnWrite(Word offset, Byte data) {
                if (writeHandlers.count(offset)) {
                    writeHandlers[offset](data);
                } else {
                    IDevice::OnWrite(offset, data); // Fallback for unknown offsets
                }
            }

... 

            void GPU::OnAttach(Word baseAddr) {
                baseAddress = baseAddr;

                registerMap.push_back({
                    "GPU_OPTIONS",
                    baseAddr + 0x00,
                    [this]() { return _gpu_options; },
                    [this](Byte value) { _gpu_options = value; },
                    {
                        "(Byte) Bitflag Enables",
                        "- bit 7    = Extended Bitmap:",
                        "              0: Tilemap Display",
                        "              1: Bitmap Display",
                        "- bits 5-6 = Extended Color Mode:",
                        "              00: 2-Colors",
                        "              01: 4-Colors",
                        "              10: 16-Colors",
                        "              11: 256-Colors",
                        "- bits 4   = Extended Display Enable",
                        "              0: Disabled",
                        "              1: Enabled",
                        "- bits 3   = Application Screen Mode",
                        "              0: Windowed",
                        "              1: Fullscreen",
                        "- bits 2   = VSync Enable",
                        "              0: Disabled",
                        "              1: Enabled",
                        "- bits 1   = Sprite Enable",
                        "              0: Disabled",
                        "              1: Enabled",
                        "- bit  0   = Standard Display Enable",
                        "              0: Disabled",
                        "              1: Enabled"
                    }
                });

                registerMap.push_back({
                    "GPU_MODE",
                    baseAddr + 0x01,
                    [this]() { return _gpu_mode; },
                    [this](Byte value) { _gpu_mode = value; },
                    {
                        "(Byte) Bitflag Enables",
                        "- bit 7    = Standard Bitmap:",
                        "              0: Text Display",
                        "              1: Bitmap Display",
                        "- bits 5-6 = Standard Color Mode:",
                        "              00: 2-Colors",
                        "              01: 4-Colors",
                        "              10: 16-Colors",
                        "              11: 256-Colors",
                        "- bits 0-4 = Display Mode (0-31)"
                    }
                });

                // Continue adding registers...
            }

    ... and / or ...

            std::vector<MAP_NODE> registerMap;
            Word baseAddress = 0; // Base offset for the device

            void GPU::OnAttach(Word baseAddr) {
                baseAddress = baseAddr;

                registerMap.push_back({
                    "GPU_OPTIONS",
                    baseAddr + 0x00,
                    [this]() { return _gpu_options; },
                    [this](Byte value) { _gpu_options = value; }
                });

                registerMap.push_back({
                    "GPU_MODE",
                    baseAddr + 0x01,
                    [this]() { return _gpu_mode; },
                    [this](Byte value) { _gpu_mode = value; }
                });

                registerMap.push_back({
                    "GPU_VIDEO_MAX_HIGH",
                    baseAddr + 0x02,
                    [this]() { return _gpu_video_max >> 8; },
                    nullptr // Read-only
                });

                registerMap.push_back({
                    "GPU_VIDEO_MAX_LOW",
                    baseAddr + 0x03,
                    [this]() { return _gpu_video_max & 0xFF; },
                    nullptr // Read-only
                });

                registerMap.push_back({
                    "GPU_HRES_HIGH",
                    baseAddr + 0x04,
                    [this]() { return _gpu_hres >> 8; },
                    nullptr // Read-only
                });

                registerMap.push_back({
                    "GPU_HRES_LOW",
                    baseAddr + 0x05,
                    [this]() { return _gpu_hres & 0xFF; },
                    nullptr // Read-only
                });

                // Continue for other registers...
            }


            // Lambda Hember Macros: Reduce boilerplate during register 
            // initialization for easier debugging
            #define READ_ONLY(name, var) { #name, [this]() { return var; }, nullptr }
            #define READ_WRITE(name, var) { #name, [this]() { return var; }, [this](Byte value) { var = value; } }

            void GPU::OnAttach(Word nextAddr) {
                baseAddress = nextAddr;

                registerMap.push_back(READ_WRITE(GPU_OPTIONS, _gpu_options));
                registerMap.push_back(READ_WRITE(GPU_MODE, _gpu_mode));
                registerMap.push_back(READ_ONLY(GPU_VIDEO_MAX_HIGH, _gpu_video_max >> 8));
                registerMap.push_back(READ_ONLY(GPU_VIDEO_MAX_LOW, _gpu_video_max & 0xFF));
            }

    ... and ...


            void GPU::ExportASM(const std::string& fileName) {
                std::ofstream asmFile(fileName);
                if (!asmFile.is_open()) {
                    std::cerr << "Error: Unable to open file " << fileName << " for writing.\n";
                    return;
                }

                asmFile << "; Auto-generated memory map for GPU registers\n\n";
                asmFile << "GPU_DEVICE            equ   " << std::hex << baseAddress << "\n\n";

                for (const auto& reg : registerMap) {
                    // Write the register definition
                    asmFile << reg.label << "   equ   0x" << std::hex << reg.address << "\n";

                    // Write associated comments
                    for (const auto& comment : reg.comments) {
                        asmFile << "                                      ; " << comment << "\n";
                    }

                    asmFile << "\n"; // Add a blank line between definitions
                }

                asmFile.close();
                std::cout << "Memory map successfully exported to " << fileName << ".\n";
            }

    ... in the Memory device itself ...ADJ_STATUS

            Byte Memory::Read(Word offset, bool debug) {
                for (auto& node : _memory_nodes) {
                    if (offset >= node->baseAddress && offset < node->baseAddress + node->size) {
                        Word relativeOffset = offset - node->baseAddress;

                        // Debug mode handling, if applicable
                        if (debug) {
                            // You can add any debug-specific behavior here
                        }

                        // Call the node's read handler
                        if (node->read) {
                            return node->read(relativeOffset);
                        }
                        break; // No valid read handler
                    }
                }
                return 0xCC; // Return a default invalid value
            }

            void Memory::Write(Word offset, Byte data, bool debug) {
                for (auto& node : _memory_nodes) {
                    if (offset >= node->baseAddress && offset < node->baseAddress + node->size) {
                        Word relativeOffset = offset - node->baseAddress;

                        // Special handling for ROM writes in debug mode
                        ROM* isROM = dynamic_cast<ROM*>(node);
                        if (debug && isROM) {
                            isROM->write_to_rom(relativeOffset, data);
                        } else if (node->write) {
                            node->write(relativeOffset, data);



        ... more in depth read and write functions

                    
            struct MAP_NODE {
                std::string label;                  // Register label (e.g., "GPU_OPTIONS")
                Word address;                       // Memory-mapped address
                std::function<Byte()> read;         // Normal read handler
                std::function<void(Byte)> write;    // Normal write handler
                std::function<Byte()> debug_read;   // Debug read handler
                std::function<void(Byte)> debug_write; // Debug write handler
                std::vector<std::string> comments; // Documentation/comments for the register
            };



            Byte Memory::Read(Word offset, bool debug) {
                for (const auto& device : _memory_nodes) {
                    if (offset >= device->baseAddress && offset < device->baseAddress + device->size) {
                        Word relativeOffset = offset - device->baseAddress;

                        for (const auto& reg : device->registerMap) {
                            if (relativeOffset == reg.address - device->baseAddress) {
                                if (debug) {
                                    // Use debug_read if available, otherwise fallback to read
                                    if (reg.debug_read) {
                                        return reg.debug_read();
                                    }
                                }

                                // Use normal read if not in debug mode or debug_read is unavailable
                                if (reg.read) {
                                    return reg.read();
                                }

                                return 0x00; // Default value for unmapped reads
                            }
                        }
                    }
                }
                return 0xCC; // Default invalid value
            }


            void Memory::Write(Word offset, Byte data, bool debug) {
                for (const auto& device : _memory_nodes) {
                    if (offset >= device->baseAddress && offset < device->baseAddress + device->size) {
                        Word relativeOffset = offset - device->baseAddress;

                        for (const auto& reg : device->registerMap) {
                            if (relativeOffset == reg.address - device->baseAddress) {
                                if (debug) {
                                    // Use debug_write if available, otherwise fallback to write
                                    if (reg.debug_write) {
                                        reg.debug_write(data);
                                        return;
                                    }
                                }

                                // Use normal write if not in debug mode or debug_write is unavailable
                                if (reg.write) {
                                    reg.write(data);
                                    return;
                                }
                            }
                        }
                    }
                }
            }

        ... potential example:

            void KeyboardDevice::OnAttach(Word baseAddr) {
                baseAddress = baseAddr;
                size = 0x10; // Example size for Keyboard device

                registerMap.push_back({
                    "KEYBOARD_BUFFER",
                    baseAddr + 0x00,
                    [this]() { 
                        // Normal read: Pops a value from the keyboard buffer
                        Byte value = _buffer.front();
                        _buffer.pop();
                        return value;
                    },
                    nullptr, // No normal write
                    [this]() { 
                        // Debug read: Peek at the front of the queue without removing
                        return _buffer.empty() ? 0x00 : _buffer.front();
                    },
                    nullptr, // No debug write
                    {
                        "Keyboard buffer register"
                    }
                });

                registerMap.push_back({
                    "KEYBOARD_STATUS",
                    baseAddr + 0x01,
                    [this]() { return _status; }, // Normal read
                    [this](Byte value) { _status = value; }, // Normal write
                    [this]() { return _status; }, // Debug read is same as normal read
                    nullptr, // No debug write
                    {
                        "Keyboard status register"
                    }
                });
            }




******************************************************/

// END: Gfx.hpp