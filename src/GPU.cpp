/*** GPU.cpp *******************************************
 *     _____ _____  _    _                        
 *    / ____|  __ \| |  | |                       
 *   | |  __| |__) | |  | |       ___ _ __  _ __  
 *   | | |_ |  ___/| |  | |      / __| '_ \| '_ \ 
 *   | |__| | |    | |__| |  _  | (__| |_) | |_) |
 *    \_____|_|     \____/  (_)  \___| .__/| .__/ 
 *                                   | |   | |    
 *                                   |_|   |_|    
 *
 * This singleton device extends the GPU device to include dynamic 
 * memory within the GPU and adds Sprite and Tilemap support.
 * 
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 * 
 ************************************/

#include "Bus.hpp"
#include "GPU.hpp"
#include "Memory.hpp"


/***************************
* Constructor / Destructor *
***************************/

GPU::GPU() 
{ 
    std::cout << clr::indent_push() << clr::CYAN << "GPU Created" << clr::RETURN;
    _device_name = "GPU_DEVICE"; 

    // Enforce Pseudo-Singleton Pattern
    static GPU* pGPU = this;
    if (pGPU != this)
    {
        Bus::Error("GPU already initialized!", __FILE__, __LINE__);
    }
} // END: GPU()

GPU::~GPU() 
{ 
    std::cout << clr::indent_pop() << clr::CYAN << "GPU Destroyed" << clr::RETURN; 
} // END: ~GPU()



/******************
* Virtual Methods *
******************/


int  GPU::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Entry" << clr::RETURN;
 
    SetBaseAddress(nextAddr);
    Word old_address=nextAddr;
    this->heading = "GPU Device Hardware Registers";


    ////////////////////////////////////////////////
    // (Byte) GPU_MODE
    //       Standard Display Mode
    /////
    mapped_register.push_back( { "GPU_MODE", nextAddr, nullptr, nullptr, {""}});
    mapped_register.push_back( { "GPU_MODE_MSB", nextAddr, 
        [this](Word) { return (_gpu_mode>>8) & 0xFF; }, 
        [this](Word, Byte data) { _verify_gpu_mode_change( (_gpu_mode & 0x00FF) | (data << 8) ); }, 
        {   
            "(Byte) Graphics Display Mode",
            "- bit  7   = Extended Display Enable:",
            "              0: Disabled",
            "              1: Enabled",
            "- bit  6   = (reserved)",
            "- bits 4-5 = Extended Color Depth:",
            "              00: 2-Colors",
            "              01: 4-Colors",
            "              10: 16-Colors",
            "              11: 256-Colors",
            "- bit  3   = Extended Rendering Mode", 
            "              0: Tilemap Display",
            "              1: Bitmap Display",
            "- bit  2   = Emulation Screen Mode",
            "              0: Windowed",
            "              1: Fullscreen",
            "- bit  1   = VSync Enable",
            "              0: Disabled",
            "              1: Enabled",
            "- bit  0   = Presentation",
            "              0: Letterbox",
            "              1: Overscan / Stretch","" 
        }
    }); nextAddr+=1;
    // (Byte) GPU_MODE_LSB
    mapped_register.push_back( { "GPU_MODE_LSB", nextAddr,
        [this](Word) { return _gpu_mode & 0xFF; },
        [this](Word, Byte data) { _verify_gpu_mode_change( (_gpu_mode & 0xFF00) | (data & 0xFF) ); }, 
        {
            "- bit  7   = Standard Display Enable",
            "             0: Disabled",
            "             1: Enabled",
            "- bit  6    = (reserved)",
            "- bits 4-5 = Standard Bitmap Color Depth:",
            "              00: 2-Colors",
            "              01: 4-Colors",
            "              10: 16-Colors",
            "              11: 256-Colors",
            "- bit  3    = Standard Bitmap:",
            "              0: Text Display",
            "              1: Bitmap Display",
            "MODE (bits 0-2):",
            "- bit  2    = 0: 320/256 width,  1: 160/128 width",
            "- bit  1    = 0: 200/160 height, 1: 160/80 height",
            "- bit  0    = Base Resolution: 0:320x200, 1:256x160",
            "",
            "              Text Mode Table:",
            "   | MODE | COLUMNS |  ROWS  | BUFFER |",
            "   |:----:|:-------:|:------:|:------:|",
            "   |  $00 |    40   |   25   |  2000  |",
            "   |  $01 |    32   |   20   |  1280  |",
            "   |  $02 |    40   |   12   |   960  |",
            "   |  $03 |    32   |   10   |   640  |",
            "   |  $04 |    20   |   25   |  1000  |",
            "   |  $05 |    16   |   20   |   640  |",
            "   |  $06 |    20   |   12   |   480  |",
            "   |  $07 |    16   |   10   |   320  |",
            "",
            "        Bitmap Display Mode Table:",
            "| MODE | WIDTH | HEIGHT | COLORS | BUFFER |",
            "|------|-------|--------|--------|--------|",
            "| $00  |  320  |   200  |    2   |  8000  |",
            "| $01  |  256  |   160  |    2   |  5120  |",
            "| $02  |  320  |   100  |    2   |  4000  |",
            "| $03  |  256  |    80  |    2   |  2560  |",
            "| $04  |  160  |   200  |    2   |  4000  |",
            "| $05  |  128  |   160  |    2   |  2560  |",
            "| $06  |  160  |   100  |    2   |  2000  |",
            "| $07  |  128  |    80  |    2   |  1280  |",
            "| $00  |  320  |   200  |    4   | 16000  | (bgnd only)",
            "| $01  |  256  |   160  |    4   | 10240  | (bgnd only)",
            "| $02  |  320  |   100  |    4   |  8000  |",
            "| $03  |  256  |    80  |    4   |  5120  |",
            "| $04  |  160  |   200  |    4   |  8000  |",
            "| $05  |  128  |   160  |    4   |  5120  |",
            "| $06  |  160  |   100  |    4   |  4000  |",
            "| $07  |  128  |    80  |    4   |  2560  |",
            "| $00  |  320  |   200  |   16   | 32000  | (bgnd only)",
            "| $01  |  256  |   160  |   16   | 20480  | (bgnd only)",
            "| $02  |  320  |   100  |   16   | 16000  | (bgnd only)",
            "| $03  |  256  |    80  |   16   | 10240  | (bgnd only)",
            "| $04  |  160  |   200  |   16   | 16000  | (bgnd only)",
            "| $05  |  128  |   160  |   16   | 10240  | (bgnd only)",
            "| $06  |  160  |   100  |   16   |  8000  |",
            "| $07  |  128  |    80  |   16   |  5120  |",
            "| $00  |  320  |   200  |  256   | 64000  | (bgnd only)",
            "| $01  |  256  |   160  |  256   | 40960  | (bgnd only)",
            "| $02  |  320  |   100  |  256   | 32000  | (bgnd only)",
            "| $03  |  256  |    80  |  256   | 20480  | (bgnd only)",
            "| $04  |  160  |   200  |  256   | 32000  | (bgnd only)",
            "| $05  |  128  |   160  |  256   | 20480  | (bgnd only)",
            "| $06  |  160  |   100  |  256   | 16000  | (bgnd only)",
            "| $07  |  128  |    80  |  256   | 10240  | (bgnd only)",
            ""
        }
    }); nextAddr+=1;    


    ////////////////////////////////////////////////
    // (Word) GPU_VIDEO_MAX
    //       Video Buffer Maximum (Read Only)
    /////
    mapped_register.push_back( { "GPU_VIDEO_MAX", nextAddr,
        [this](Word) { return _gpu_video_max >> 8; }, 
        nullptr, {   
            "(Word) Video Buffer Maximum (Read Only)",
            "Note: This will change to reflect",
            "      the size of the last cpu",
            "      accessible memory location",
            "      of the currently active",
            "      standard video mode.",""
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_video_max & 0xFF; }, 
        nullptr, {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_HRES
    //      Horizontal Pixel Resolution (Read Only)
    /////
    mapped_register.push_back({ "GPU_HRES", nextAddr, 
        [this](Word) { return _gpu_hres >> 8; }, 
        nullptr,  {
            "(Word) Horizontal Pixel Resolution (Read Only)",
            "Note: This will reflect the number of",
            "      pixel columns for bitmap modes.",""
        }}); nextAddr+=1;        
     mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_hres & 0xFF; }, 
        nullptr, {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_VRES
    //      Vertical Pixel Resolution (Read Only)
    /////
    mapped_register.push_back({ "GPU_VRES", nextAddr, 
        [this](Word) { return _gpu_vres >> 8; }, 
        nullptr,  {
            "(Word) Vertical Pixel Resolution (Read Only)",
            "Note: This will reflect the number of",
            "      pixel rows for bitmap modes.",""
        }}); nextAddr+=1;        
     mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_vres & 0xFF; }, 
        nullptr, {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) GPU_TCOLS
    //      Text Horizontal Columns (Read Only)
    /////
    mapped_register.push_back({ "GPU_TCOLS", nextAddr,  
        [this](Word) { return _gpu_tcols & 0xFF; }, 
        nullptr,  {
            "(Byte) Text Horizontal Columns (Read Only)",
            "Note: This will reflect the number of",
            "      glyph columns for text modes.",""
            ""}}); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_TROWS
    //      Text Vertical Rows (Read Only)
    /////
    mapped_register.push_back({ "GPU_TROWS", nextAddr,  
        [this](Word) { return _gpu_trows & 0xFF; }, 
        nullptr,  {
            "(Byte) Text Vertical Rows (Read Only)",
            "Note: This will reflect the number of",
            "      glyph rows for text modes.",""
            ""}}); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_PAL_INDEX
    //      Color Palette Index
    /////
    mapped_register.push_back({ "GPU_PAL_INDEX", nextAddr,  
        [this](Word) { return _gpu_pal_index; }, 
        [this](Word, Byte data) { _gpu_pal_index = data; }, 
        {
            "(Byte) Color Palette Index",
            "Note: Use this register to set the",
            "      index into the Color Palette.",
            "      Set this value prior referencing",
            "      the color data (GPU_PAL_COLOR).",""
            ""
        }
    }); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Word) GPU_PAL_COLOR
    //      Indexed Color Palette Data (A4R4G4B4 format)
    /////
    mapped_register.push_back({ "GPU_PAL_COLOR", nextAddr,  
        [this](Word) { return (_palette[_gpu_pal_index].color >> 8) & 0xFF; }, 
        [this](Word, Byte data) { 
            Word c = _palette[_gpu_pal_index].color & 0x00ff;
			_palette[_gpu_pal_index].color = c | ((Word)data << 8);
        }, {
            "(Word) Color Palette Data (A4R4G4B4 format)",
            "Note: This is the color data for an",
            "      individual palette entry. Write to ",
            "      DSP_PAL_IDX with the index within the",
            "      color palette prior to reading or",
            "      writing the color data in the",
            "      GFX_PAL_CLR register.",
            ""
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _palette[_gpu_pal_index].color & 0xFF; }, 
        [this](Word nextAddr, Byte data) { 
            (void)nextAddr; 
			Word c = _palette[_gpu_pal_index].color & 0xff00;
			_palette[_gpu_pal_index].color = c | data;
        }, {""}}); nextAddr+=1;      


    ////////////////////////////////////////////////
    // (Byte) GPU_GLYPH_IDX
    //     Text Glyph Index (0-255)
    /////
    mapped_register.push_back({ "GPU_GLYPH_IDX", nextAddr,  
        [this](Word) { return _gpu_glyph_idx; }, 
        [this](Word, Byte data) { _gpu_glyph_idx = data; }, 
        {
            "(Byte) Text Glyph Index",
            "Note: Use this register to set the",
            "      index of a specific text glyph.",
            "      Set this value prior to updating",
            "      the glyph data (GPU_GLYPH_DATA).",
            ""
        }
    }); nextAddr+=1;


    ////////////////////////////////////////////////
    // (8-Bytes) GPU_GLYPH_DATA 
    //      Text Glyph Pixel Data Array
    /////
    mapped_register.push_back({ "GPU_GLYPH_DATA", nextAddr,  
        [this](Word) { return _gpu_glyph_data[_gpu_glyph_idx][0];  }, 
        [this](Word, Byte data) { _gpu_glyph_data[_gpu_glyph_idx][0] = data; }, 
        {
            "(8-Bytes) 8 rows of binary encoded glyph pixel data",
            "Note: This is the pixel data for a", 
            "      specific text glyph. Each 8x8",
            "      text glyph is composed of 8 bytes.",            
            "      The first byte in this array",
            "      represents the top line of 8 pixels.",            
            "      Each array entry represents a row of 8 pixels.",
            ""
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_glyph_data[_gpu_glyph_idx][1]; }, 
        [this](Word, Byte data) { _gpu_glyph_data[_gpu_glyph_idx][1] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_glyph_data[_gpu_glyph_idx][2]; }, 
        [this](Word, Byte data) { _gpu_glyph_data[_gpu_glyph_idx][2] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_glyph_data[_gpu_glyph_idx][3]; }, 
        [this](Word, Byte data) { _gpu_glyph_data[_gpu_glyph_idx][3] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_glyph_data[_gpu_glyph_idx][4]; }, 
        [this](Word, Byte data) { _gpu_glyph_data[_gpu_glyph_idx][4] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_glyph_data[_gpu_glyph_idx][5]; }, 
        [this](Word, Byte data) { _gpu_glyph_data[_gpu_glyph_idx][5] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_glyph_data[_gpu_glyph_idx][6]; }, 
        [this](Word, Byte data) { _gpu_glyph_data[_gpu_glyph_idx][6] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_glyph_data[_gpu_glyph_idx][7]; }, 
        [this](Word, Byte data) { _gpu_glyph_data[_gpu_glyph_idx][7] = data; }, 
        {""}}); nextAddr+=1;      

    // ************************************** //
    // Graphics Processor Extension Registers //
    // ************************************** //


    ////////////////////////////////////////////////
    // (Word) GPU_BGND_SIZE
    //      (Word) Extended Graphics Buffer Size (Read Only)
    //      Note: The primary extended graphics buffer
    //            always begins at $0000. This is also highest
    //            accessible memory location of the currently
    //            active background video mode.
    /////
    mapped_register.push_back( { "GPU_BGND_SIZE", nextAddr,
        [this](Word) { return _gpu_bgnd_size >> 8; }, 
        nullptr, 
        {   
            "(Word) Extended Graphics Buffer Size (Read Only)",
            "Note: The primary extended graphics buffer",
            "      always begins at $0000. This is also highest",
            "      accessible memory location of the currently",
            "      active background video mode.",""
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_bgnd_size & 0xFF; }, 
        nullptr, 
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_BLIT_ADDR
    //      Graphics Memory Address Port
    //      Autoincrements on Read/Write
    /////
    mapped_register.push_back( { "GPU_BLIT_ADDR", nextAddr,
        [this](Word) { return _gpu_blit_addr >> 8; }, 
        [this](Word, Byte data) { _gpu_blit_addr = (_gpu_blit_addr & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Graphics Memory Address Port ",
            "Note: When GPU_BLIT_DATA is accessed, this register",
            "      is automatically incremented to point to the  ",
            "      next byte to be read or written based on the ",
            "      values in GPU_BLIT_PITCH and GPU_BLIT_WIDTH.",""
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_blit_addr & 0xFF; }, 
        [this](Word, Byte data) { _gpu_blit_addr = (_gpu_blit_addr & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_BLIT_PITCH
    //      Graphics Memory Address Port
    //      Autoincrements on Read/Write
    /////
    mapped_register.push_back( { "GPU_BLIT_PITCH", nextAddr,
        [this](Word) { return _gpu_blit_pitch >> 8; }, 
        [this](Word, Byte data) { _gpu_blit_pitch = (_gpu_blit_pitch & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Number of Bytes Per Display Line",
            "Note: This value represents the number of displayed",
            "      pixels per line.",""
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_blit_pitch & 0xFF; }, 
        [this](Word, Byte data) { _gpu_blit_pitch = (_gpu_blit_pitch & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_BLIT_WIDTH
    //      Width of the Image Block in Pixels
    /////
    mapped_register.push_back( { "GPU_BLIT_WIDTH", nextAddr,
        [this](Word) { return _gpu_blit_width >> 8; }, 
        [this](Word, Byte data) { _gpu_blit_width = (_gpu_blit_width & 0x00FF) | (data << 8); _gpu_blit_width_count = _gpu_blit_width; }, 
        {   
            "(Word) Width of the Image Block in Pixels"
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_blit_width & 0xFF; }, 
        [this](Word, Byte data) { _gpu_blit_width = (_gpu_blit_width & 0xFF00) | (data << 0); _gpu_blit_width_count = _gpu_blit_width; },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) GPU_BLIT_DATA
    //      GPU Memory Data Port
    /////
    mapped_register.push_back( { "GPU_BLIT_DATA", nextAddr,
        [this](Word) { return _gpu_blit_read_data(); }, 
        [this](Word, Byte data) { _gpu_blit_write_data(data); },
        {   
            "(Byte) GPU Memory Data Port",""
        }}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_DYN_HANDLE
    //      (Word) Dynamic Memory HANDLE
    /////
    mapped_register.push_back( { "GPU_DYN_HANDLE", nextAddr,
        // READ
        [this](Word) { return _gpu_dyn_handle >> 8; }, 
        // WRITE
        [this](Word, Byte data) 
        {
             _gpu_dyn_handle = (_gpu_dyn_handle & 0x00FF) | (data << 8); 
            auto itr = _gpu_dyn_map.find(_gpu_dyn_handle);
            if (itr != _gpu_dyn_map.end())
            {
                _gpu_dyn_cur_addr = itr->second.address;
                _gpu_dyn_end_addr = itr->second.end_addr;
                _gpu_dyn_end_dist = _gpu_dyn_end_addr - _gpu_dyn_cur_addr;
            }
            else
            {
                // Error Condition: Invalid Handle
                error(MAP(GPU_ERR_HANDLE));
            }
        }, 
        // COMMENTS
        {   
            "(Word) Dynamic Memory HANDLE",
            "On Write: resets GPU_DYN_CUR_ADDR,",
            "    GPU_DYN_END_ADDR,  and GPU_DYN_END_DIST",""
        }
    }); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        // READ
        [this](Word) { return _gpu_dyn_handle & 0xFF; }, 
        // WRITE
        [this](Word, Byte data) 
        { 
            _gpu_dyn_handle = (_gpu_dyn_handle & 0xFF00) | (data << 0); 
            auto itr = _gpu_dyn_map.find(_gpu_dyn_handle);
            if (itr != _gpu_dyn_map.end())
            {
                _gpu_dyn_cur_addr = itr->second.address;
                _gpu_dyn_end_addr = itr->second.end_addr;
                _gpu_dyn_end_dist = _gpu_dyn_end_addr - _gpu_dyn_cur_addr;
            }
            else
            {
                // Error Condition: Invalid Handle
                error(MAP(GPU_ERR_HANDLE));
            }
        },  
        // COMMENTS
        {""}
    }); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_DYN_CUR_ADDR
    //      Current Dynamic Memory ADDRESS
    /////
    mapped_register.push_back( { "GPU_DYN_CUR_ADDR", nextAddr,
        [this](Word) { return _gpu_dyn_cur_addr >> 8; }, 
        [this](Word, Byte data) { _gpu_dyn_cur_addr = (_gpu_dyn_cur_addr & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Current Dynamic Memory ADDRESS",
            "(autoincrements by 1 on read/write)",
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_dyn_cur_addr & 0xFF; }, 
        [this](Word, Byte data) { _gpu_dyn_cur_addr = (_gpu_dyn_cur_addr & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_DYN_END_ADDR
    //      Last Dynamic Useful Memory ADDRESS in this block
    /////
    mapped_register.push_back( { "GPU_DYN_CUR_ADDR", nextAddr,
        [this](Word) { return _gpu_dyn_end_addr >> 8; }, 
        [this](Word, Byte data) { _gpu_dyn_end_addr = (_gpu_dyn_end_addr & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Last Useful Dynamic Memory ADDRESS in this block"
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_dyn_end_addr & 0xFF; }, 
        [this](Word, Byte data) { _gpu_dyn_end_addr = (_gpu_dyn_end_addr & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_DYN_END_DIST
    //      (Word) Distance to End of this Dynamic Memory Block
    /////
    mapped_register.push_back( { "GPU_DYN_END_DIST", nextAddr,
        [this](Word) { return _gpu_dyn_end_dist >> 8; }, 
        [this](Word, Byte data) { _gpu_dyn_end_dist = (_gpu_dyn_end_dist & 0x00FF) | (data << 8); }, 
        {   
            "(Word) (Word) Distance to End of this Dynamic Memory Block"
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_dyn_end_dist & 0xFF; }, 
        [this](Word, Byte data) { _gpu_dyn_end_dist = (_gpu_dyn_end_dist & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) GPU_DYN_DATA
    //      (Byte) Dynamic Memory DATA (Read/Write)
    /////
    mapped_register.push_back( { "GPU_BLIT_DATA", nextAddr,
        // READ
        [this](Word) 
        { 
            auto itr = _gpu_dyn_map.find(_gpu_dyn_handle);
            if (itr != _gpu_dyn_map.end())
            {
                if (_gpu_dyn_cur_addr < _ext_video_buffer.size()) 
                {
                    Byte data = _ext_video_buffer[_gpu_dyn_cur_addr]; 
                    if (_gpu_dyn_cur_addr < _gpu_dyn_end_addr) 
                    {
                        _gpu_dyn_cur_addr++;
                    }
                    return data;
                }
                else
                {
                    // Error Condition: Invalid Address
                    error(MAP(GPU_ERR_ADDRESS));
                }
            }
            else
            {
                // Error Condition: Invalid Handle
                error(MAP(GPU_ERR_HANDLE));
            }
            // Error Condition: Invalid Data
            error(MAP(GPU_ERR_DATA));
            return (Byte)0xCC;  // Mostly Arbitrary but in common with other devices
        }, 
        // WRITE
        [this](Word, Byte data) 
        { 
            auto itr = _gpu_dyn_map.find(_gpu_dyn_handle);
            if (itr != _gpu_dyn_map.end())
            {
                if (_gpu_dyn_cur_addr < _ext_video_buffer.size()) 
                {
                    _ext_video_buffer[_gpu_dyn_cur_addr] = data;
                    if (_gpu_dyn_cur_addr < _gpu_dyn_end_addr) 
                    {
                        _gpu_dyn_cur_addr++;
                    }
                }
                else
                {
                    // Error Condition: Invalid Address
                    error(MAP(GPU_ERR_ADDRESS));
                }
            }
            else
            {
                // Error Condition: Invalid Handle
                error(MAP(GPU_ERR_HANDLE));
            }
        },
        // COMMENTS
        {   
            "(Byte) (Byte) Dynamic Memory DATA (Read/Write)",""
        }}); nextAddr+=1;

//
// GPU_ARG_1                (Word)      // Argument 1
// GPU_ARG_2                (Word)      // Argument 2
// GPU_ARG_3                (Word)      // Argument 3
// GPU_ARG_4                (Word)      // Argument 4
// GPU_ARG_5                (Word)      // Argument 5
//      
// GPU_COMMAND              (Byte)      // Graphics Processing Unit Command
// GPU_CMD_NEW_BUFFER                   // Allocate a new GPU Buffer (of arbetrary size)
// GPU_CMD_FREE_BUFFER                  // Free a GPU Buffer (GPU_ARG_1_MSB = Buffer Index)
// GPU_CMD_NEW_IMG                      // Allocate a new GPU Image as in a Sprite or Tile
//                                      // returns a valid node address for the new image                                
//                                      // size is based on color depth as 32, 64, 128, or 256
// GPU_CMD_FREE_IMG                     // Free a GPU Image (GPU_ARG_1_MSB = Image Index)
// GPU_CMD_CLEAR                        // Clear Video Buffer:
//                                      // GPU_ARG_1_MSB = Color Index
// GPU_CMD_COPY                         // Copy GPU Memory to GPU Memory
//                                      // Copy from [GPU_ARG_1] through [GPU_ARG_2]
//                                      // to [GPU_ARG_3] through [GPU_ARG_4]                                
// GPU_CMD_BLIT_GPU                     // BLiT from GPU memory to Display (RAM to Screen) 
// GPU_CMD_GPU_BLIT                     // BLiT from Display to GPU memory (Screen to RAM)
// GPU_CMD_SCROLL                       // Scroll Video Buffer:
//                                      //     GPU_ARG_1_MSB = signed 8-bit horiz. offset
//                                      //     GPU_ARG_1_LSB = signed 8-bit vert. offset
// GPU_CMD_DRAW_LINE
// GPU_CMD_DRAW_CIRCLE
// GPU_CMD_DRAW_RECT
// GPU_CMD_FILL_RECT
//     ... etc
//
//
//


//  
// // Sprite Registers: 
// GPU_SPR_MAX              (Byte)      // Maximum Sprite Index (Read Only)
// GPU_SPR_IDX              (Byte)      // Sprite Index
// GPU_SPR_XPOS             (SInt16)    // Sprite X Position
// GPU_SPR_YPOS             (SInt16)    // Sprite Y Position
// GPU_SPR_IMG_IDX          (Byte)      // Sprite Image Index
// GPU_SPR_MASK             (Byte)      // Sprite Collision Mask (4x4)
// GPU_SPR_FLAGS            (Byte)      // Sprite Flags:
//                                      // % 0000'0011: 
//                                      //      00 = 2 colors, 
//                                      //      01 = 4 colors, 
//                                      //      10 = 16 colors, 
//                                      //      11 = 256 colors
//                                      // % 0000'0100: Double Width
//                                      // % 0000'1000: Double Height
//                                      // % 0001'0000: Flip Horizontal
//                                      // % 0010'0000: Flip Vertical
//                                      // % 0100'0000: Collision Enable
//                                      // % 1000'0000: Display Enable
//  
// // Image Editing Registers:               
// GPU_BMP_MAX              (Byte)      // Maximum Bitmap Index (Read Only)
// GPU_BMP_IDX              (Byte)      // Bitmap Image Index (0-255)
// GPU_BMP_OFFSET           (Byte)      // Offset Within the Image Buffer(0-255)
// GPU_BMP_DATA             (Byte)      // Bitmap Data (Read Write)
// GPU_BMP_FLAGS            (Byte)      // Bitmap Flags:
//                                      // % 0000'0011: 
//                                      //      00 = 2 colors (size=32), 
//                                      //      01 = 4 colors (size=64), 
//                                      //      10 = 16 colors (size=128), 
//                                      //      11 = 256 colors (size=256)
//                                      // % 1111'1100: Reserved
//  



    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_WIDTH
    //      Tilemap Width (in pixels)
    /////
    mapped_register.push_back( { "GPU_TMAP_WIDTH", nextAddr,
        [this](Word) { return _gpu_tmap_width >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_width = (_gpu_tmap_width & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Width (in pixels)"
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_width & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_width = (_gpu_tmap_width & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_HEIGHT
    //      Tilemap Height (in pixels)
    /////
    mapped_register.push_back( { "GPU_TMAP_HEIGHT", nextAddr,
        [this](Word) { return _gpu_tmap_height >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_height = (_gpu_tmap_height & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Height (in pixels)"
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_height & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_height = (_gpu_tmap_height & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (SInt16) GPU_TMAP_XPOS
    //      Tilemap X Position (top left corner)
    /////
    mapped_register.push_back( { "GPU_TMAP_XPOS", nextAddr,
        [this](Word) { return _gpu_tmap_xpos >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_xpos = (_gpu_tmap_xpos & 0x00FF) | (data << 8); }, 
        {   
            "(SInt16) Tilemap X Position (top left corner)"
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_xpos & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_xpos = (_gpu_tmap_xpos & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (SInt16) GPU_TMAP_YPOS
    //      Tilemap Height (in pixels)
    /////
    mapped_register.push_back( { "GPU_TMAP_YPOS", nextAddr,
        [this](Word) { return _gpu_tmap_ypos >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_ypos = (_gpu_tmap_ypos & 0x00FF) | (data << 8); }, 
        {   
            "(SInt16) Tilemap Y Position (top left corner)"
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_ypos & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_ypos = (_gpu_tmap_ypos & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (SInt16) GPU_TMAP_CLIP_X1
    //      Tilemap X Position (top left corner)
    /////
    mapped_register.push_back( { "GPU_TMAP_CLIP_X1", nextAddr,
        [this](Word) { return _gpu_tmap_clip_x1 >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_x1 = (_gpu_tmap_clip_x1 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region X1"
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_clip_x1 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_x1 = (_gpu_tmap_clip_x1 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (SInt16) GPU_TMAP_CLIP_Y1
    //      Tilemap Height (in pixels)
    /////
    mapped_register.push_back( { "GPU_TMAP_CLIP_Y1", nextAddr,
        [this](Word) { return _gpu_tmap_clip_y1 >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_y1 = (_gpu_tmap_clip_y1 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region Y1"
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_clip_y1 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_y1 = (_gpu_tmap_clip_y1 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (SInt16) GPU_TMAP_CLIP_X2
    //      Tilemap X Position (top left corner)
    /////
    mapped_register.push_back( { "GPU_TMAP_CLIP_X2", nextAddr,
        [this](Word) { return _gpu_tmap_clip_x2 >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_x2 = (_gpu_tmap_clip_x2 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region X2"
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_clip_x2 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_x2 = (_gpu_tmap_clip_x2 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (SInt16) GPU_TMAP_CLIP_Y1
    //      Tilemap Height (in pixels)
    /////
    mapped_register.push_back( { "GPU_TMAP_CLIP_Y2", nextAddr,
        [this](Word) { return _gpu_tmap_clip_y2 >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_y2 = (_gpu_tmap_clip_y2 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region Y2",""
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_clip_y2 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_y2 = (_gpu_tmap_clip_y2 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) GPU_ERROR             
    //      Memory Management Unit Error Code:    (Read Only)
    /////
    mapped_register.push_back({ "GPU_ERROR", nextAddr, 
        [this](Word) { return _gpu_error; },  
        nullptr,  
        { "(Byte) Graphics Processing Unit Error Code:     (Read Only)" }}); nextAddr++;
    // ADD ERROR CODE ENUMERATION:
    Byte err = 0;
    for (const auto& error : _gpu_error_list)
    {
        std::vector<std::string> comments = { "   $" + clr::hex(err, 2) + " = " + error.second };
        if (error.first == "GPU_ERR_SIZE")
        {
            comments.push_back("");  // Adding an extra empty string to add a blank line
        }
        mapped_register.push_back({
            error.first,            // The constant name (e.g., "MMU_ERR_NONE")
            err++,                  // Increment the error code value for each entry
            nullptr,                // Placeholder for function pointer (not used here)
            nullptr,                // Placeholder for another value (not used here)
            { comments }            // Formatted description (error message)
        });
    }


    ////////////////////////////////////////////////
    // (Constant) GPU_END
    //      End of GPU Register Space
    /////
    nextAddr--;
    mapped_register.push_back({ "GPU_END", nextAddr, 
        nullptr, nullptr,  { "End of GPU Register Space"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Constant) GPU_TOP
    //      Top of GPU Register Space
    //      (start of the next device)
    /////
    mapped_register.push_back({ "GPU_TOP", nextAddr, 
    nullptr, nullptr,  { "Top of GPU Register Space", "---"}});
    

    std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Exit" << clr::RETURN;
    _size = nextAddr - old_address;
    return _size;
} // END: GPU::OnAttach()


void GPU::OnInit()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnInit() Entry" << clr::RETURN;

    { // BEGIN OF SDL3 Initialization
        // initialize SDL3
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO))// | SDL_INIT_EVENTS))
        {
            std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onInit() Error" << clr::RETURN;
            Bus::Error(SDL_GetError(), __FILE__, __LINE__);
        }

        // create the main window
        pWindow = SDL_CreateWindow("SDL3 Retro_6809", initial_width, initial_height, window_flags); 
        SDL_ShowWindow(pWindow);


        // create the renderer
        pRenderer = SDL_CreateRenderer(pWindow, NULL);

        SDL_SetRenderLogicalPresentation(pRenderer, (int)_screen_width, (int)_screen_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

        // build the textures
        pExt_Texture = SDL_CreateTexture(pRenderer, 
                SDL_PIXELFORMAT_ARGB4444, 
                SDL_TEXTUREACCESS_STREAMING, 
                (int)_screen_width/2, (int)_screen_height/2);
        SDL_SetTextureScaleMode(pExt_Texture, SDL_SCALEMODE_NEAREST);            

        pStd_Texture = SDL_CreateTexture(pRenderer, 
                SDL_PIXELFORMAT_ARGB4444, 
                SDL_TEXTUREACCESS_STREAMING, 
                (int)_screen_width/2, (int)_screen_height/2);
        SDL_SetTextureScaleMode(pStd_Texture, SDL_SCALEMODE_NEAREST);            

        pForeground_Texture = SDL_CreateTexture(pRenderer, 
                SDL_PIXELFORMAT_ARGB4444, 
                SDL_TEXTUREACCESS_STREAMING, 
                (int)_screen_width/2, (int)_screen_height/2);
        SDL_SetTextureScaleMode(pForeground_Texture, SDL_SCALEMODE_NEAREST); 

    } // END OF SDL3 Initialization

    // Build The Color Palette
    _build_palette();

    // Reserve 64k for the extended video buffer
    int bfr_size = 64*1024;
    _ext_video_buffer.reserve(bfr_size);
  
    // initialize the font glyph buffer
    for (int i=0; i<256; i++)
        for (int r=0; r<8; r++)
            _gpu_glyph_data[i][r] = font8x8_system[i][r];  

    // initialize the initial default display mode   
    Memory::Write_Word(MAP(GPU_MODE), _gpu_mode);

    std::cout << clr::indent() << clr::CYAN << "GPU::OnInit() Exit" << clr::RETURN;
} // END: GPU::OnInit()


void GPU::OnQuit()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnQuit() Entry" << clr::RETURN;
    
    { // BEGIN OF SDL3 Shutdown

        // destroy the textures
        if (pExt_Texture) { 
            SDL_DestroyTexture(pExt_Texture);
            pExt_Texture = nullptr;
        }
        if (pStd_Texture) { 
            SDL_DestroyTexture(pStd_Texture);
            pStd_Texture = nullptr;
        }
        if (pForeground_Texture) { 
            SDL_DestroyTexture(pForeground_Texture);
            pForeground_Texture = nullptr;
        }   
     
        if (pRenderer)
        { // destroy the renderer
            SDL_DestroyRenderer(pRenderer);
            pRenderer = nullptr;
        }
        if (pWindow)
        { // destroy the window
            SDL_DestroyWindow(pWindow);
            pWindow = nullptr;
        }
        // shutdown SDL
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    } // END OF SDL3 Shutdown

    std::cout << clr::indent() << clr::CYAN << "GPU::OnQuit() Exit" << clr::RETURN;
} // END: GPU::OnQuit()



void GPU::OnActivate()
{
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Entry" << clr::RETURN;

    // clear out the extended video buffer
    Word d=0;
    for (int i=0; i<(64*1024); i++) { _ext_video_buffer[i] = d++; }

    // std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Exit" << clr::RETURN;
} // END: GPU::OnActivate()



void GPU::OnDeactivate()
{
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnDeactivate() Entry" << clr::RETURN;       
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnDeactivate() Exit" << clr::RETURN;
} // END: GPU::OnDeactivate()


void GPU::OnEvent(SDL_Event* evnt)
{
    // if not a main window event, just return now
    if (SDL_GetWindowFromEvent(evnt) != pWindow) { return; }

    SDL_Keymod km = SDL_GetModState();

    switch (evnt->type) 
    {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:  
            Bus::IsRunning(false);
            break;

        case SDL_EVENT_KEY_DOWN:
        {
            if (evnt->key.key == SDLK_V)
            {   // [V] Toggle VSYNC
                Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                if (data &  0b0000'0010) {
                    data &= 0b1111'1101;
                } else {
                    data |= 0b0000'0010;
                }
                Memory::Write(MAP(GPU_MODE_MSB), data);
            }

            if (km & SDL_KMOD_CTRL)
            {            
                if (evnt->key.key == SDLK_F)
                {   // [F] Toggle Fullscreen
                    Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                    if (data &  0b0000'0100) {
                        data &= 0b1111'1011;
                    } else {
                        data |= 0b0000'0100;
                    }
                    Memory::Write(MAP(GPU_MODE_MSB), data);
                } // END: if (evnt->key.key == SDLK_F)

                // [E] Extended Display Enable Toggle
                if (evnt->key.key == SDLK_E)
                {
                    SDL_Keymod mod = SDL_GetModState();
                    if (mod & SDL_KMOD_SHIFT)
                    { // [SHIFT] + [E]  // toggle between extended tilemap and bitmap mode
                        Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                        if (data &  0b0000'1000) {
                            data &= 0b1111'0111;
                        } else {
                            data |= 0b0000'1000;
                        }
                        Memory::Write(MAP(GPU_MODE_MSB), data);
                    } else { // [E] Extended Display Enable
                        Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                        if (data &  0b1000'0000) {
                            data &= 0b0111'1111;
                        } else {
                            data |= 0b1000'0000;
                        }
                        Memory::Write(MAP(GPU_MODE_MSB), data);
                    }
                }
                // [S] Standard Display Enable Toggle
                if (evnt->key.key == SDLK_S)
                {
                    SDL_Keymod mod = SDL_GetModState();
                    if (mod & SDL_KMOD_SHIFT)
                    { // [SHIFT] + [S]  // toggle between text and bitmap mode
                        Byte data = Memory::Read( MAP(GPU_MODE_LSB) );
                        if (data &  0b0000'1000) {
                            data &= 0b1111'0111;
                        } else {
                            data |= 0b0000'1000;
                        }
                        Memory::Write(MAP(GPU_MODE_LSB), data);
                    } else { // [S] Standard Display Enable
                        Byte data = Memory::Read( MAP(GPU_MODE_LSB) );
                        if (data &  0b1000'0000) {
                            data &= 0b0111'1111;
                        } else {
                            data |= 0b1000'0000;
                        }
                        Memory::Write(MAP(GPU_MODE_LSB), data);
                    }
                }

                if (evnt->key.key == SDLK_RIGHT)
                {
                    Byte data = Memory::Read( MAP(GPU_MODE_LSB) );
                    Byte mode = data+1;
                    data = (data & 0b1111'1000) | (mode & 0b0000'0111);
                    Memory::Write(MAP(GPU_MODE_LSB), data);
                    // clear the screen
                    for (int t=MAP(VIDEO_START); t<_gpu_video_max; t+=2)     
                    {
                        Word atch = (Memory::Read(0x005C)<<8) | ' ';  // _ATTRIB
                        Memory::Write_Word(t, atch);
                    }    
                }
                if (evnt->key.key == SDLK_LEFT)
                {
                    Byte data = Memory::Read( MAP(GPU_MODE_LSB) );
                    Byte mode = data-1;
                    data = (data & 0b1111'1000) | (mode & 0b0000'0111);
                    Memory::Write(MAP(GPU_MODE_LSB), data);
                    // clear the screen
                    for (int t=MAP(VIDEO_START); t<_gpu_video_max; t+=2)     
                    {
                        Word atch = (Memory::Read(0x005C)<<8) | ' ';  // _ATTRIB
                        Memory::Write_Word(t, atch);
                    }    
				}
                if (evnt->key.key == SDLK_UP)
                {
                    // toggle VSYNC
                    Byte bits = 0b0000'0010;
                    Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                    data ^= bits;
                    Memory::Write(MAP(GPU_MODE_MSB), data);
                }
                if (evnt->key.key == SDLK_DOWN)
                {
                    // toggle between letterbox and overscan/stretch modes
                    Byte bits = 0b0000'0001;
                    Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                    data ^= bits;
                    Memory::Write(MAP(GPU_MODE_MSB), data);
                }
            }
            break;       
        } // END: case SDL_EVENT_KEY_DOWN

        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        {            
            // _gpu_options |= 0b0000'1000; // Set bit 6 = ENTER FULLSCREEN

            _gpu_mode |= 0b0000'0100'0000'0000;
            
            // std::cout << "SDL_EVENT_WINDOW_ENTER_FULLSCREEN (0x" << clr::hex(_gpu_options,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        {
            // _gpu_options &= 0b1111'0111; // Clear bit 6 = LEAVE FULLSCREEN

            _gpu_mode &= ~0b0000'0100'0000'0000;

            // std::cout << "SDL_EVENT_WINDOW_LEAVE_FULLSCREEN (0x" << clr::hex(_gpu_options,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

    } // END: switch  (evnt->type) 

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnEvent() Exit" << clr::RETURN;
} // END: GPU::OnEvent()


void GPU::OnUpdate(float fElapsedTime)
{
    static float deltaTime = fElapsedTime;
    static float runningTime = fElapsedTime;
    if (runningTime > deltaTime + 0.01f) {
        deltaTime = fElapsedTime;
        runningTime = fElapsedTime;
        SDL_SetWindowTitle(pWindow, Bus::GetTitle().c_str());
        // std::cout << "FPS: " << Bus::FPS() << std::endl;
    }
    runningTime += fElapsedTime;   

    // GPU device runs before all other devices. 
    if (true) // Mouse Cursor and/or Foreground Sprites
    {
        // Clear the foreground texture 
        _clear_texture(pForeground_Texture, 0x0, 0x0, 0x0, 0x0);
    }

    // is extended graphics enabled?
    // if (_gpu_options & 0b0001'0000)
    if (_gpu_mode & 0b10000000'0000'0000)
    {
        _render_extended_graphics(); 
    }
    else
    {
        _clear_texture(pExt_Texture, 15, red(0), grn(0), blu(0));
    }


    // is standard graphics enabled?
    // if (_gpu_options & 0b0000'0001)
    if (_gpu_mode & 0b0000'0000'1000'0000)
    {
        _render_standard_graphics();
    }
    else
    {
        _clear_texture(pStd_Texture, 0, 0, 0, 0);
    }    
} // END: GPU::OnUpdate()


void GPU::OnRender()
{
    //std::cout << clr::indent() << clr::CYAN << "GPU::OnRender() Entry" << clr::RETURN;
    SDL_FRect r{0.0f, 0.0f, _ext_width, _ext_height};

    // render Extended Graphics
    SDL_RenderTexture(pRenderer, pExt_Texture, &r, NULL);

    // render Standard Graphics 
    SDL_RenderTexture(pRenderer, pStd_Texture, &r, NULL);

    // render Foreground Graphics 
    SDL_RenderTexture(pRenderer, pForeground_Texture, &r, NULL);

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnRender() Exit" << clr::RETURN;
} // END: GPU::OnRender()



void GPU::_render_extended_graphics()
{    
    //    GPU_EXT_MODE          = 0xFE02, // (Byte) Extended Graphics Mode
    int _width = _ext_width;
    int _height = _ext_height;

    // clear the extended texture
    _clear_texture(pExt_Texture, 0, red(0), grn(0), blu(0));

    // is the extended display enabled
    if ((_gpu_mode & 0b1000'0000'0000'0000)==0)
        return; // nope, just return

    // IS Extended Display In Tiled Mode?
    if ( (_gpu_mode & 0b0000'1000'0000'0000) == 0)
    { 
        // TODO: render tiled graphics
        //std::cout << "GPU::_render_extended_graphics() ---> Displaying extended tilemap buffer" << std::endl;
        _update_tile_buffer();
    } else {
        // extended bitmap mode
        int bpp = 0;
        // switch((_gpu_options & 0b0110'0000)>>5)
        switch( ((_gpu_mode & 0b0011'0000'0000'0000)>>12)& 0x03 )    // extended color mode bits
        {
            case 0x00: bpp = 1; break;
            case 0x01: bpp = 2; break;
            case 0x02: bpp = 4; break;
            case 0x03: bpp = 8; break;
        }
        // display the extended bitmap buffer
        Word pixel_index = 0x0000;
        void *pixels;
        int pitch;
        if (!SDL_LockTexture(pExt_Texture, NULL, (void **)&pixels, &pitch))
            Bus::Error(SDL_GetError());	
        else
        {
            for (int y = 0; y < _height; y++)
            {
                for (int x = 0; x < _width; )
                {
                    // 256 color mode
                    if (bpp == 8)
                    {
                        Byte index = _ext_video_buffer[pixel_index++];
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 16 color mode
                    else if (bpp == 4)
                    {
                        Byte data = _ext_video_buffer[pixel_index++];
                        Byte index = (data >> 4);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data & 0x0f);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 4 color mode
                    else if (bpp == 2)
                    {
                        Byte data = _ext_video_buffer[pixel_index++];
                        Byte index = (data >> 6) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 4) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 2) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 0) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 2 color mode
                    else if (bpp == 1)
                    {
                        Byte data = _ext_video_buffer[pixel_index++];
                        Byte index = (data >> 7) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true); 
                        index = (data >> 6) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 5) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 4) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 3) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 2) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 1) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 0) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    if (pixel_index >  _ext_video_buffer.capacity()) {
                        // std::string err = "Pixel index out of range: " + std::to_string(pixel_index);
                        // Bus::Error(err.c_str());
                        SDL_UnlockTexture(pExt_Texture);
                        return;
                    }
                }
            }
            SDL_UnlockTexture(pExt_Texture); 
        }        
    }
} // END: GPU::_render_extended_graphics()

void GPU::_render_standard_graphics()
{
    // clear the standard texture
    _clear_texture(pStd_Texture, 0, 0, 0, 0);

    // is the standard display enabled
    // if ((_gpu_options & 0b0000'0001)==0)
    if ((_gpu_mode & 0b0000'0000'1000'0000)==0)
        return;

    // Render the Standard Display Buffer
    // if ( (_gpu_mode & 0b1000'0000) == 0) 

    // IS Standard Display Rendering Text?
    if ( (_gpu_mode & 0b0000'0000'0000'1000) == 0) 
    { 
        _update_text_buffer();        
    } 
    else
    { // Standard Display Rendering Graphics
        int bpp = 0;
        int div = 0;
        // int std_color_mode = (_gpu_mode & 0b0110'0000) >> 5;
        int std_color_mode = ((_gpu_mode & 0b0000'0000'0011'0000) >> 4) & 0x03;
        int buffer_size;

        // Reduce the standard color mode if necessary. This
        // should already fit, but this is a safety check.
        do
        {
            switch(std_color_mode)
            {
                case 0x00: div = 8; bpp = 1; break;
                case 0x01: div = 4; bpp = 2; break;
                case 0x02: div = 2; bpp = 4; break;
                case 0x03: div = 1; bpp = 8; break;
            }            
            buffer_size = (_std_width * _std_height)/div;
            std_color_mode--;
        } while (buffer_size > 8000);

        // display the standard bitmap buffer
        Word pixel_index = MAP(VIDEO_START);
        void *pixels;
        int pitch;
        if (!SDL_LockTexture(pStd_Texture, NULL, (void **)&pixels, &pitch))
            Bus::Error(SDL_GetError());	
        else
        {
            for (int y = 0; y < _std_height; y++)
            {
                for (int x = 0; x < _std_width; )
                {
                    // 256 color mode
                    if (bpp == 8)
                    {
                        Byte index = Memory::Read(pixel_index++);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 16 color mode
                    else if (bpp == 4)
                    {
                        Byte data = Memory::Read(pixel_index++);
                        Byte index = (data >> 4);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data & 0x0f);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 4 color mode
                    else if (bpp == 2)
                    {
                        Byte data = Memory::Read(pixel_index++);
                        Byte index = (data >> 6) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 4) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 2) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 0) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 2 color mode
                    else if (bpp == 1)
                    {
                        Byte data = Memory::Read(pixel_index++);
                        Byte index = (data >> 7) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true); 
                        index = (data >> 6) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 5) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 4) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 3) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 2) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 1) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 0) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }

                    if (pixel_index > MAP(VIDEO_END)) {
                        std::string err = "Pixel index out of range: $" + clr::hex(pixel_index, 4);
                        Bus::Error(err.c_str(), __FILE__, __LINE__);
                        SDL_UnlockTexture(pStd_Texture); 
                        return;
                    }
                }
            }
            SDL_UnlockTexture(pStd_Texture); 
        }        
    }
} // END: GPU::_render_standard_graphics()

void GPU::_update_text_buffer() {

    bool ignore_alpha = false;
    void *pixels;
    int pitch;

    // lock the standard texture
    if (!SDL_LockTexture(pStd_Texture, NULL, &pixels, &pitch)) {
        Bus::Error(SDL_GetError());
    }
    else
    { // Render the text
        Byte col = _std_width / 8;
        Byte row = _std_height / 8;
        Word end = ((col*row)*2) + MAP(VIDEO_START);
        // Word end  = VIDEO_START+128;
		Word addr = MAP(VIDEO_START);
		for (; addr < end; addr += 2)
		{
			// Byte at = Memory::Read(addr+0, true);
			// Byte ch = Memory::Read(addr+1, true);
			Byte at = Memory::Read(addr+0, true);
			Byte ch = Memory::Read(addr+1, true);
			Byte bg = at >> 4;
			Byte fg = at & 0x0f;
			Word index = addr - MAP(VIDEO_START);
			Byte width = _std_width / 8;
			int x = ((index / 2) % width) * 8;
			int y = ((index / 2) / width) * 8;
			for (int v = 0; v < 8; v++)
			{
				for (int h = 0; h < 8; h++)
				{
					int color = bg;
                    Byte gd = GetGlyphData(ch, v);
					if (gd & (1 << (7 - h)))
						color = fg;
					_setPixel_unlocked(pixels, pitch, x + h, y + v, color, ignore_alpha);
				}
			}
		}
        SDL_UnlockTexture(pStd_Texture); 
    }
} 


void GPU::_update_tile_buffer()
{
    // for now just color cycle the extended video buffer
    static Byte clr = 1;
    static int delta = 1;
    static Byte r = 0;
    static Byte g = 0;
    static Byte b = 0;
    Byte a = 15;

    static Uint64 _last_time = SDL_GetTicks();
    if (SDL_GetTicks() - _last_time > 25) {
        _last_time = SDL_GetTicks();
    
        if (clr & 0b0000'0100)  r+=delta;
        if (clr & 0b0000'0010)  g+=delta;
        if (clr & 0b0000'0001)  b+=delta;

        // reverse delta
        if (r == 8 || g == 8 || b == 8) { delta *= -1; }
        if (r == 0 || g == 0 || b == 0) {
            delta *= -1;
            clr ++;
            if (clr > 15)
                clr = 1;
        }
    }
    _clear_texture(pExt_Texture, a, r, g, b);
}


/**
 * Sets a pixel in the given surface without locking the surface.
 *
 * This function combines the pixel color with the color index, using the alpha channel
 * of the color index as the alpha value for the combination. If bIgnoreAlpha is true,
 * the alpha value is ignored and the pixel color is set to the color index if the color
 * index is not transparent.
 *
 * @param pixels The surface to draw on.
 * @param pitch The pitch of the surface.
 * @param x The x coordinate of the pixel.
 * @param y The y coordinate of the pixel.
 * @param color_index The color index to use.
 * @param bIgnoreAlpha If true, the alpha value is ignored.
 */
void GPU::_setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte color_index, bool bIgnoreAlpha)
{
    Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));		// because data size is two bytes 

    bool cls = false;
    if (color_index ==0 && bIgnoreAlpha)
        cls = true;

    if (!bIgnoreAlpha)
    {       
        // int ret = ((p1 * (256-a))) + (p2 * (a+1)) >> 8;
        Uint16 pixel = *dst;	// 0xARGB
		Byte r1 = (pixel & 0x0f00) >> 8;
		Byte g1 = (pixel & 0x00f0) >> 4;
		Byte b1 = (pixel & 0x000f) >> 0;
		//
        Byte a2 = alf(color_index);
        Byte r2 = red(color_index);
        Byte g2 = grn(color_index);
        Byte b2 = blu(color_index);
        // if (bIgnoreAlpha)
        //     a2 = 15;
		//
        Byte r = (((r1 * (16-a2))) + (r2 * (a2+1))) >> 4;
        Byte g = (((g1 * (16-a2))) + (g2 * (a2+1))) >> 4;
        Byte b = (((b1 * (16-a2))) + (b2 * (a2+1))) >> 4;

        if (alf(color_index) != 0) // || bIgnoreAlpha)
        {
            *dst = (
                0xF000 | 
                (r<<8) | 
                (g<<4) | 
                (b)
            );                
		}	
    }
    else
    {        
        if (cls)
        {
            *dst = 0x0000;
        }
        else
        {
            *dst = 
            (
                0xF000 |
                (red(color_index)<<8) |
                (grn(color_index)<<4) |
                 blu(color_index)
            );    
        }
    }
} // END: GPU::_setPixel_unlocked()

void GPU::_clear_texture(SDL_Texture* texture, Byte a, Byte r, Byte g, Byte b)
{
    a&=0x0f; r&=0x0f; g&=0x0f; b&=0x0f;
    void *pixels;
    int pitch;
    if (!SDL_LockTexture(texture, NULL, &pixels, &pitch)) {
        Bus::Error(SDL_GetError());
    } else {
        for (int y = 0; y < _std_height; y++)
        {
            for (int x = 0; x < _std_width; x++)
            {
                Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));
                *dst = (
                    (a<<12) | 
                    (r<<8)  | 
                    (g<<4)  | 
                    (b<<0) );          

            }
        }
        SDL_UnlockTexture(texture); 
    }    
}

/**
 * Initializes the palette data by filling it with the following:
 *   - CUSTOM DEFAULT COLORS (0-15)
 *   - BGR 6x6x6 CUBE COLORS (16-232)
 *   - GRAYSCALE RAMP (233-255)
 *   - 100% TRANSPARENT WHITE (256)
 *
 * The palette is only initialized once, and subsequent calls to this
 * method do not modify the palette.
 */
void GPU::_build_palette()
{
    // INITIALIZE PALETTE DATA
    if (_palette.size() == 0)
    {
        // BASIC COLORS (0-15) CUSTOM DEFAULT COLORS
        std::vector<PALETTE> ref = {    
			{ 0xF000 },		// 0: black             // 0xF000
			{ 0xF555 },		// 1: dk gray
			{ 0xF007 },		// 2: dk blue
			{ 0xF600 },		// 3: dk red
			{ 0xF140 },		// 4: dk green
			{ 0xF840 },		// 5: brown
			{ 0xF406 },		// 6: purple          
			{ 0xF046 },		// 7: deep sea          // 0xF046  	
			{ 0xF888 },		// 8: lt gray
			{ 0xF22F },		// 9: blue
			{ 0xFd00 },		// A: red
			{ 0xF4F6 },		// B: lt green
			{ 0xFED0 },		// C: yellow
			{ 0xF85b },		// D: Lt Purple
			{ 0xF59f },		// E: lt sky
			{ 0xFFFF },		// F: white

        // // BASIC COLORS (0-15) STANDARD CGA COLORS
        // std::vector<PALETTE> ref = {    
		// 	{ 0x0000 },	// 0: DK_BLACK
		// 	{ 0xF005 },	// 1: DK_BLUE
		// 	{ 0xF050 },	// 2: DK_GREEN
		// 	{ 0xF055 },	// 3: DK_CYAN
		// 	{ 0xF500 },	// 4: DK_RED
		// 	{ 0xF505 },	// 5: DK_MAGENTA
		// 	{ 0xF631 },	// 6: BROWN
		// 	{ 0xFAAA },	// 7: GRAY
		// 	{ 0xF555 },	// 8: DK_GRAY
		// 	{ 0xF00F },	// 9: BLUE
		// 	{ 0xF0F0 },	// a: GREEN
		// 	{ 0xF0FF },	// b: CYAN
		// 	{ 0xFF00 },	// c: RED
		// 	{ 0xFA0A },	// d: MAGENTA
		// 	{ 0xFFF0 },	// e: YELLOW
		// 	{ 0xFFFF },	// f: WHITE
        };
        for (auto &p : ref)
        {
			_palette.push_back(p);     
            // printf("%3d:$%04X\n", (int)_palette.size(), p.color);
        }
        // BGR 6x6x6 CUBE COLORS (16-232)          
        Byte b=0, g=0, r=0;
        for (float fb=0.0f; fb<=256.0f; fb+=(256.0f / 5.0f))
        {
            b = fb;
            if (fb == 256.0f) b=255;
            for (float fg=0.0f; fg<=256.0f; fg+=(256.0f / 5.0f))
            {
                g = fg;
                if (fg == 256.0f) g=255;
                for (float fr=0.0f; fr<=256.0f; fr+=(256.0f / 5.0f))
                {
                    r = fr;
                    if (fr == 256.0f) r=255;
                    PALETTE ent = { 0x0000 };
                    ent.a = 255>>4;
                    ent.b = b>>4;
                    ent.g = g>>4;
                    ent.r = r>>4;
                    _palette.push_back(ent);
                    // printf("%3d:$%04X\n", (int)_palette.size(), ent.color);
                }
            }
        }
        // GRAYSCALE RAMP (233-255) 
        for (float f=0.0f; f<=256.0f; f+=(256.0f / 24.0f))
        {
            if (f != 0.0f)
            {
                PALETTE ent = { 0x0000 };
                ent.a = 255>>4;
                ent.b = (int)f>>4;
                ent.g = (int)f>>4;
                ent.r = (int)f>>4;
                _palette.push_back(ent);
            }
        }  
        // Add one 100% transparent white entry to the end              
        _palette.push_back({0x0FFF});   // (255 = 100% transparent)
    }
} // END::_buildPalette();

void GPU::_display_mode_helper(Byte mode, int &width, int &height)
{
    //    - bit 2    = 0: normal width,  1: half width
    //    - bit 1    = 0: normal height, 1: half height
    //    - bit 0    = Base Resolution: 0:320x200, 1:256x160    
    switch(mode & 0b0000'0111)
    {
        case 0b0000'0000: width = 320; height = 200; break;
        case 0b0000'0001: width = 256; height = 160; break;
        case 0b0000'0010: width = 320; height = 100; break;
        case 0b0000'0011: width = 256; height = 80 ; break;
        case 0b0000'0100: width = 160; height = 200; break;
        case 0b0000'0101: width = 128; height = 160; break;
        case 0b0000'0110: width = 160; height = 100; break;
        case 0b0000'0111: width = 128; height = 80 ; break;
    }    
}


void GPU::_verify_gpu_mode_change(Word mode_data)
{

    int buffer_size = 99999;

    // toggle fullscreen / windowed

    // if nothing else but the application fullscreen state has changed
    if ( (mode_data & 0b0000'0100'0000'0000) != (_gpu_mode & 0b0000'0100'0000'0000) )
    {
        if (mode_data & 0b0000'0100'0000'0000) {
            SDL_SetWindowFullscreen(pWindow, true);
        } else {
            SDL_SetWindowFullscreen(pWindow, false);
        }             
    }     
    // VSync changed
    if ( (mode_data & 0b0000'0010'0000'0000) != (_gpu_mode & 0b0000'0010'0000'0000) )
    {
        if (mode_data & 0b0000'0010'0000'0000) {
            SDL_SetRenderVSync(pRenderer, 1);
            //std::cout << clr::ORANGE << "GPU::_verify_gpu_mode_change()  --  VSYNC ON" << clr::RETURN;
        } else {
            SDL_SetRenderVSync(pRenderer, 0);
            //std::cout << clr::ORANGE << "GPU::_verify_gpu_mode_change()  --  VSYNC OFF" << clr::RETURN;
        } 
    }
    _gpu_mode = mode_data;

    int width, height;
    _display_mode_helper( (_gpu_mode & 0x0007) , width, height);

    // TODO:  Remove _ext_width and _ext_height
    //    ... Remove _std_width and _std_height
    //
    _ext_width  = (float)width;
    _ext_height = (float)height;
    _std_width  = (float)width;
    _std_height = (float)height;

    // set GPU_HRES and GPU_VRES as appropriate
    _gpu_hres = width;
    _gpu_vres = height;
    _gpu_tcols = width / 8;
    _gpu_trows = height / 8;

    // adjust color depth for the standard bitmap modes
    do
    {
        int div = 0;
        int std_color_mode = ((mode_data & 0b0000'0000'0011'0000) >> 4) & 0x03;
        switch(std_color_mode & 0x03)
        {
            case 0x00: div = 8; break;
            case 0x01: div = 4; break;
            case 0x02: div = 2; break;
            case 0x03: div = 1; break;
        }            
        // buffer_size = (_std_width * _std_height)/div;
        buffer_size = (_gpu_hres * _gpu_vres)/div;
        if (buffer_size > 8000) 
        { 
            // reduce the standard bitmap color depth
            mode_data = (mode_data & 0b1111'1111'1100'1111) | ((std_color_mode-1)<<4);

            //std::cout << "GPU: Reducing Standard Bitmap Color Depth: " << buffer_size << "\n";
            _gpu_mode = mode_data;
        }
    } while (buffer_size > 8000);

    // set the GPU_VIDEO_MAX -- Standard Buffer
    if (_gpu_mode & 0b0000'0000'0000'1000)
    {   // bitmap mode
        _gpu_video_max = ( ( MAP(VIDEO_START) + buffer_size ) / 4) - 1;
    }
    else
    {   // text mode
        // buffer_size = int(((_std_width/8.0f) * (_std_height/8.0f)) * 2.0f);
        buffer_size = (_gpu_tcols * _gpu_trows) * 2;
        _gpu_video_max = ( MAP(VIDEO_START) + buffer_size ) - 1;
    }


    // bounds checking Standard Buffer is between 0x0400 and 0x23FF
    if (_gpu_video_max < MAP(VIDEO_START))  { _gpu_video_max = MAP(VIDEO_START); }
    if (_gpu_video_max > MAP(VIDEO_END))    { _gpu_video_max = MAP(VIDEO_END); }

    // adjust the renderer logical presentation to either stretch or letterbox
    if (_gpu_mode & 0b0000'0001'0000'0000) {
        SDL_SetRenderLogicalPresentation(pRenderer, (int)_ext_width, (int)_ext_height, SDL_LOGICAL_PRESENTATION_STRETCH);
		// std::cout << clr::ORANGE << "GPU::_verify_gpu_mode_change()  --  STRETCH" << clr::RETURN;
    } else {
        SDL_SetRenderLogicalPresentation(pRenderer, (int)_ext_width, (int)_ext_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
		// std::cout << clr::ORANGE << "GPU::_verify_gpu_mode_change()  --  LETTERBOX" << clr::RETURN;
    }
}


// **************************** //
// Extended Graphics Functions: //
// **************************** //

void GPU::_gpu_calc_bgnd_size()
{
    // Extended Display is Enabled
    if (_gpu_mode & 0b1000'0000'0000'0000)
    {
        // bitmap mode?
        if (_gpu_mode & 0b0000'0000'0000'1000)
        {
            Byte color_depth_shift = (_gpu_mode & 0b0011'0000'0000'0000) >> 12;
            _gpu_bgnd_size = (_gpu_hres * _gpu_vres) / (1 << color_depth_shift);
        }
        // tile Mode?
        else
        {
            _gpu_bgnd_size = _gpu_tmap_width * _gpu_tmap_height;
        }
    }
    else
    {   
        // Background Display is Disabled, size is irrelevant
        _gpu_bgnd_size = 0;
    }
}



Byte GPU::_gpu_blit_read_data() 
{ 
    Byte data = _ext_video_buffer[_gpu_blit_addr];
    _gpu_blit_increment_address();
    return data; 
}

void GPU::_gpu_blit_write_data(Byte data) 
{ 
    _ext_video_buffer[_gpu_blit_addr] = data; 
    _gpu_blit_increment_address();
}

void GPU::_gpu_blit_increment_address()
{
    // Ensure width and pitch are at least 1 for safe addressing
    if (_gpu_blit_width == 0) _gpu_blit_width = 1;
    if (_gpu_blit_pitch == 0) _gpu_blit_pitch = 1;

    if (_gpu_blit_width_count > 0)
    {
        // Process a pixel/byte in the current row and decrement the width count
        _gpu_blit_width_count--;
        _gpu_blit_addr++;
    }
    else
    {
        // At the end of the row, move to the next row, considering pitch and width
        _gpu_blit_addr += _gpu_blit_pitch - _gpu_blit_width;
        
        // Reset width count for the next row
        _gpu_blit_width_count = _gpu_blit_width;
    }
}

void GPU::error(Byte error_code) 
{
    _gpu_error = error_code;    // Set the error code
    _gpu_command = 0;           // Clear the command (indicates an error)
    // Log the error with a helpful message
    if (error_code < _gpu_error_list.size())
    {
        std::string comment = _gpu_error_list[error_code].second;
        UnitTest::Log(this, clr::RED + "MMU Error: " + comment + " $" + clr::hex(error_code, 2) );
    }
    else
    {
        UnitTest::Log(this, clr::RED + "MMU Error: Unknown Error $" + clr::hex(error_code, 2) );
    }
}

// END: GPU.cpp



