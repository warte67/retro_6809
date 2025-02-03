/*** GPU_EXT.cpp *******************************************
 *       ___ ___ _   _         _____  _______                    
 *      / __| _ \ | | |       | __\ \/ /_   _|      __ _ __ _ __ 
 *     | (_ |  _/ |_| |       | _| >  <  | |    _  / _| '_ \ '_ \
 *      \___|_|  \___/   ___  |___/_/\_\ |_|   (_) \__| .__/ .__/
 *                      |___|                         |_|  |_|       
 *   
 * This class extends the GPU class to provide sprite and tile graphics
 * support, as well as a command-based interface for accessing and
 * manipulating extended graphics hardware features.
 * 
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *                                                                                       
 ******************/

#include "Bus.hpp"
#include "GPU.hpp"
#include "GPU_EXT.hpp"

int GPU_EXT::_onAttach(int nextAddr) 
{
    if (_gpu == nullptr)
    {
        Bus::Error("GPU not initialized!", __FILE__, __LINE__);
        return -1;
    }

    ////////////////////////////////////////////////
    // (Word) GPU_BGND_SIZE
    //      (Word) Extended Graphics Buffer Size (Read Only)
    //      Note: The primary extended graphics buffer
    //            always begins at $0000. This is also highest
    //            accessible memory location of the currently
    //            active background video mode.
    /////
    _gpu->mapped_register.push_back( { "GPU_BGND_SIZE", nextAddr,
        [this](Word, bool) { return _gpu_bgnd_size >> 8; }, 
        nullptr, 
        {   
            "(Word) Extended Graphics Buffer Size (Read Only)",
            "Note: The primary extended graphics buffer",
            "      always begins at $0000. This is also highest",
            "      accessible memory location of the currently",
            "      active background video mode.",""
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_bgnd_size & 0xFF; }, 
        nullptr, 
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_BLIT_ADDR
    //      Graphics Memory Address Port
    //      Note: When GPU_BLIT_DATA is accessed, this register
    //            is automatically incremented to point to the
    //            next byte to be read or written based on the
    //            values in GPU_BLIT_PITCH and GPU_BLIT_WIDTH.
    /////
    _gpu->mapped_register.push_back( { "GPU_BLIT_ADDR", nextAddr,
        [this](Word, bool) { return _gpu_blit_addr >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_blit_addr = (_gpu_blit_addr & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Graphics Memory Address Port ",
            "Note: When GPU_BLIT_DATA is accessed, this register",
            "      is automatically incremented to point to the  ",
            "      next byte to be read or written based on the ",
            "      values in GPU_BLIT_PITCH and GPU_BLIT_WIDTH.",""
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_blit_addr & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_blit_addr = (_gpu_blit_addr & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_BLIT_PITCH
    //        (Word) Number of Bytes Per Display Line
    //        Note: This value represents the number of displayed
    //              pixels per line.
    /////
    _gpu->mapped_register.push_back( { "GPU_BLIT_PITCH", nextAddr,
        [this](Word, bool) { return _gpu_blit_pitch >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_blit_pitch = (_gpu_blit_pitch & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Number of Bytes Per Display Line",
            "Note: This value represents the number of displayed",
            "      pixels per line.",""
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_blit_pitch & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_blit_pitch = (_gpu_blit_pitch & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_BLIT_WIDTH
    //      Width of the Image Block in Pixels
    /////
    _gpu->mapped_register.push_back( { "GPU_BLIT_WIDTH", nextAddr,
        // READ
        [this](Word, bool) { return _gpu_blit_width >> 8; }, 
        // WRITE
        [this](Word, Byte data, bool) 
        { 
            _gpu_blit_width = (_gpu_blit_width & 0x00FF) | (data << 8); 
            // this is a counter that is decremented each time a pixel is copied
            _gpu_blit_width_count = _gpu_blit_width; 
        }, 
        {   
            "(Word) Width of the Image Block in Pixels"
        }
    }); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        // READ
        [this](Word, bool) { return _gpu_blit_width & 0xFF; }, 
        // WRITE
        [this](Word, Byte data, bool) 
        { 
            _gpu_blit_width = (_gpu_blit_width & 0xFF00) | (data << 0); 
            // this is a counter that is decremented each time a pixel is copied
            _gpu_blit_width_count = _gpu_blit_width; 
        },  
        {""}
    }); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) GPU_BLIT_DATA
    //      GPU Memory Data Port
    /////
    _gpu->mapped_register.push_back( { "GPU_BLIT_DATA", nextAddr,
        [this](Word, bool) { return _gpu_blit_read_data(); }, 
        [this](Word, Byte data, bool) { _gpu_blit_write_data(data); },
        {   
            "(Byte) GPU Memory Data Port",""
        }}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_CMD_ARG_1
    //      (Word) GPU Command Argument 1
    /////
    _gpu->mapped_register.push_back( { "GPU_CMD_ARG_1", nextAddr,
        [this](Word, bool) { return _gpu_cmd_arg_1 >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_cmd_arg_1 = (_gpu_cmd_arg_1 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) GPU Command Argument 1"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_cmd_arg_1 & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_cmd_arg_1 = (_gpu_cmd_arg_1 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_CMD_ARG_2
    //      (Word) GPU Command Argument 2
    /////
    _gpu->mapped_register.push_back( { "GPU_CMD_ARG_2", nextAddr,
        [this](Word, bool) { return _gpu_cmd_arg_2 >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_cmd_arg_2 = (_gpu_cmd_arg_2 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) GPU Command Argument 2"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_cmd_arg_2 & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_cmd_arg_2 = (_gpu_cmd_arg_2 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_CMD_ARG_3
    //      (Word) GPU Command Argument 3
    /////
    _gpu->mapped_register.push_back( { "GPU_CMD_ARG_3", nextAddr,
        [this](Word, bool) { return _gpu_cmd_arg_3 >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_cmd_arg_3 = (_gpu_cmd_arg_3 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) GPU Command Argument 3"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_cmd_arg_3 & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_cmd_arg_3 = (_gpu_cmd_arg_3 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_CMD_ARG_4
    //      (Word) GPU Command Argument 4
    /////
    _gpu->mapped_register.push_back( { "GPU_CMD_ARG_4", nextAddr,
        [this](Word, bool) { return _gpu_cmd_arg_4 >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_cmd_arg_4 = (_gpu_cmd_arg_4 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) GPU Command Argument 4"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_cmd_arg_4 & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_cmd_arg_4 = (_gpu_cmd_arg_4 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_CMD_ARG_5
    //      (Word) GPU Command Argument 5
    /////
    _gpu->mapped_register.push_back( { "GPU_CMD_ARG_5", nextAddr,
        [this](Word, bool) { return _gpu_cmd_arg_5 >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_cmd_arg_5 = (_gpu_cmd_arg_5 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) GPU Command Argument 5",""
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_cmd_arg_5 & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_cmd_arg_5 = (_gpu_cmd_arg_5 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) GPU_COMMAND
    //      Memory Management Unit Command
    /////
    _gpu->mapped_register.push_back({ "GPU_COMMAND", nextAddr, 
        [this](Word, bool) { return _gpu_command; },  
        [this](Word, Byte data, bool) { 
            // dispatch the GPU Command
            _gpu_command = data; 
            do_command(data);
        },   
        { "(Byte) Memory Management Unit Command:","" }}); nextAddr++;
    // ADD COMMAND ENUMERATION:
    Byte cmd = 0;
    for (const auto& command : _gpu_command_list) 
    {
        const auto& key = command.key;
        const auto& description = command.description;
        std::vector<std::string> comment = { "   $" + clr::hex(cmd, 2) + " = " + description };
        // Check for the special case of "GPU_CMD_SIZE" and add an extra empty string
        if (key == "GPU_CMD_SIZE")
        {
            comment.push_back("");  // Adding an extra empty string to add a blank line
        }
        _gpu->mapped_register.push_back({
            key,                    // Command name (e.g., "GPU_CMD_NOP")
            cmd++,                  // Increment the cmd value for each entry
            nullptr,                // Placeholder for the read function pointer
            nullptr,                // Placeholder for the write function pointer
            comment                 // Formatted description
        });
    }
 

    ////////////////////////////////////////////////
    // (Byte) GPU_BMP_IDX
    //      Bitmap Image Index (0-255)
    /////
    _gpu->mapped_register.push_back({ "GPU_BMP_IDX", nextAddr,  
        // READ 
        [this](Word, bool) { return _gpu_bmp_idx; }, 
        // WRITE
        [this](Word, Byte data, bool) { _gpu_bmp_idx = data; },  
        // COMMENT
        { "(Byte) Bitmap Image Index (0-255)", }
    }); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_BMP_OFFSET
    //      Offset Within the Image Buffer (0-255)
    /////
    _gpu->mapped_register.push_back({ "GPU_BMP_OFFSET", nextAddr, 
        // READ 
        [this](Word, bool) { return _gpu_bmp_offset; }, 
        // WRITE
        [this](Word, Byte data, bool) { _gpu_bmp_offset = data; },  
        // COMMENT
        { "(Byte) Offset Within the Image Buffer    (0-255)", }
    }); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_USR_OFFSET
    //      Offset Within the Image Buffer Where User Data Starts (0-255)
    /////
    _gpu->mapped_register.push_back({ "GPU_USR_OFFSET", nextAddr, 
        // READ 
        [this](Word, bool) { return _gpu_usr_offset; }, 
        // WRITE
        [this](Word, Byte data, bool) { _gpu_usr_offset = data; },    // GPU_ERR_OFFSET error condition if 256-color mode or other invalid offset
        // COMMENT
        { "(Byte) Offset Where User Data Starts (0-255)", }
    }); nextAddr+=1;    


    ////////////////////////////////////////////////
    // (Byte) GPU_BMP_DATA
    //      Bitmap Data (Read Write)
    /////
    _gpu->mapped_register.push_back({ "GPU_BMP_DATA", nextAddr,  
        // READ
        [this](Word, bool debug) 
        { 
            Byte data = _gpu_bmp_data[_gpu_bmp_idx][_gpu_bmp_offset];
            if (!debug)
                _gpu_bmp_offset = (_gpu_bmp_offset == 255) ? 255 : _gpu_bmp_offset + 1;
            return data;
        },
        // WRITE
        [this](Word, Byte data, bool debug) 
        { 
            _gpu_bmp_data[_gpu_bmp_idx][_gpu_bmp_offset] = data;
            if (!debug)
                _gpu_bmp_offset = (_gpu_bmp_offset == 255) ? 255 : _gpu_bmp_offset + 1;
        },
        { "(Byte) Bitmap Data      (Read Write)","" }
    }); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Sint16) GPU_SPR_XPOS
    //      Sprite X Position
    /////
    _gpu->mapped_register.push_back( { "GPU_SPR_XPOS", nextAddr, // MSB
        // READ
        [this](Word, bool) { return (_gpu_img_info[_gpu_bmp_idx].XPos) >> 8; }, 
        // WRITE
        [this](Word, Byte data, bool) { _gpu_img_info[_gpu_bmp_idx].XPos = (_gpu_img_info[_gpu_bmp_idx].XPos & 0x00FF) | (data << 8); }, 
        {   
            "(Sint16) Sprite X Position",
        }
    }); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr, // LSB
        // READ
        [this](Word, bool) { return _gpu_img_info[_gpu_bmp_idx].XPos & 0xFF; }, 
        // WRITE
        [this](Word, Byte data, bool) { _gpu_img_info[_gpu_bmp_idx].XPos = (_gpu_img_info[_gpu_bmp_idx].XPos & 0xFF00) | (data << 0); },  
        {""}
    }); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Sint16) GPU_SPR_YPOS
    //      Sprite Y Position
    /////
    _gpu->mapped_register.push_back( { "GPU_SPR_YPOS", nextAddr, // MSB
        // READ
        [this](Word, bool) { return _gpu_img_info[_gpu_bmp_idx].YPos >> 8; }, 
        // WRITE
        [this](Word, Byte data, bool) { _gpu_img_info[_gpu_bmp_idx].YPos = (_gpu_img_info[_gpu_bmp_idx].YPos & 0x00FF) | (data << 8); }, 
        {   
            "(Sint16) Sprite Y Position",
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr, // LSB
        // READ
        [this](Word, bool) { return _gpu_img_info[_gpu_bmp_idx].YPos & 0xFF; }, 
        // WRITE
        [this](Word, Byte data, bool) { _gpu_img_info[_gpu_bmp_idx].YPos = (_gpu_img_info[_gpu_bmp_idx].YPos & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_SPR_FLAGS
    //      Sprite Flags:
    /////
    _gpu->mapped_register.push_back({ "GPU_SPR_FLAGS", nextAddr,  
        [this](Word, bool) { return _gpu_img_info[_gpu_bmp_idx].spr_flags; }, 
        [this](Word, Byte data, bool) { _gpu_img_info[_gpu_bmp_idx].spr_flags = _validate_spr_flags(data); },
        { "(Byte) Sprite Flags:","" }
    }); nextAddr+=1;  
    Byte flags = 0;
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_DBL_WIDTH"  , (0b0000'0001), nullptr,nullptr, { "   % 0000'0001:  Double Width"}}); 
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_DBL_HEIGHT" , (0b0000'0010), nullptr,nullptr, { "   % 0000'0010:  Double Height"}}); 
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_FLP_HORIZ"  , (0b0000'0100), nullptr,nullptr, { "   % 0000'0100:  Flip Horizontal"}}); 
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_FLP_VERT"   , (0b0000'1000), nullptr,nullptr, { "   % 0000'1000:  Flip Vertical",
                                                                                                  "   % 0011'0000:  Collision Type:"}}); 
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_COL_NONE"   , (flags++),     nullptr,nullptr, { "        00 = none"}}); 
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_COL_BNDS"   , (flags++),     nullptr,nullptr, { "        01 = bounding box"}}); 
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_COL_CNTR"   , (flags++),     nullptr,nullptr, { "        10 = center box"}}); 
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_COL_PXL"    , (flags++),     nullptr,nullptr, { "        11 = pixel mask"}}); 
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_SPR_ENABLE" , (0b0100'0000), nullptr,nullptr, { "   % 0100'0000:  Sprite Enable"}}); 
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_RESERVED"   , (0b1000'0000), nullptr,nullptr, { "   % 1000'0000:  (reserved)",""}}); 


    ////////////////////////////////////////////////
    // (Byte) GPU_IMG_FLAGS
    //      Image Flags:
    /////
    _gpu->mapped_register.push_back({ "GPU_IMG_FLAGS", nextAddr,  
        [this](Word, bool) { return _gpu_img_info[_gpu_bmp_idx].img_flags; }, 
        [this](Word, Byte data, bool) { _gpu_img_info[_gpu_bmp_idx].img_flags = _validate_img_flags(data); },
        { "(Byte) Image Flags:","" }
    }); nextAddr+=1;
    flags = 0;
    _gpu->mapped_register.push_back({ "GPU_IMG_COLOR_MODE"    , (0b0000'0011), nullptr,nullptr, { "   % 0000'0011:  Color Mode:"}}); 
    _gpu->mapped_register.push_back({ "GPU_IMG_FL_2_COLORS"   , (flags++),     nullptr,nullptr, { "        00 = 2-colors"}}); 
    _gpu->mapped_register.push_back({ "GPU_IMG_FL_4_COLORS"   , (flags++),     nullptr,nullptr, { "        01 = 4-colors"}}); 
    _gpu->mapped_register.push_back({ "GPU_IMG_FL_16_COLORS"  , (flags++),     nullptr,nullptr, { "        10 = 16-colors"}}); 
    _gpu->mapped_register.push_back({ "GPU_IMG_FL_256_COLORS" , (flags++),     nullptr,nullptr, { "        11 = 256-colors"}}); 
    _gpu->mapped_register.push_back({ "GPU_IMG_FL_SEC_PAL"    , (0b0000'0100), nullptr,nullptr, { "   % 0000'0100:  Secondary Palette (rules apply)"}}); 
    _gpu->mapped_register.push_back({ "GPU_IMG_FL_32_WIDTH"   , (0b0000'1000), nullptr,nullptr, { "   % 0000'1000:  32 pixel width (rules apply)"}}); 
    _gpu->mapped_register.push_back({ "GPU_IMG_FL_32_HEIGHT"  , (0b0001'0000), nullptr,nullptr, { "   % 0001'0000:  32 pixel height (rules apply)"}}); 
    _gpu->mapped_register.push_back({ "GPU_IMG_FL_DBL_WIDTH"  , (0b1110'0000), nullptr,nullptr, { "   % 1110'0000:  (reserved)",""}}); 


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_WIDTH
    //      Tilemap Width (in pixels)
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_WIDTH", nextAddr,
        [this](Word, bool) { return _gpu_tmap_width >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_width = (_gpu_tmap_width & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Width (in pixels)"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_tmap_width & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_width = (_gpu_tmap_width & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_HEIGHT
    //      Tilemap Height (in pixels)
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_HEIGHT", nextAddr,
        [this](Word, bool) { return _gpu_tmap_height >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_height = (_gpu_tmap_height & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Height (in pixels)"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_tmap_height & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_height = (_gpu_tmap_height & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (SInt16) GPU_TMAP_XPOS
    //      Tilemap X Position (top left corner)
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_XPOS", nextAddr,
        [this](Word, bool) { return _gpu_tmap_xpos >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_xpos = (_gpu_tmap_xpos & 0x00FF) | (data << 8); }, 
        {   
            "(SInt16) Tilemap X Position (top left corner)"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_tmap_xpos & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_xpos = (_gpu_tmap_xpos & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (SInt16) GPU_TMAP_YPOS
    //      Tilemap Y Position (top left corner)
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_YPOS", nextAddr,
        [this](Word, bool) { return _gpu_tmap_ypos >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_ypos = (_gpu_tmap_ypos & 0x00FF) | (data << 8); }, 
        {   
            "(SInt16) Tilemap Y Position (top left corner)"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_tmap_ypos & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_ypos = (_gpu_tmap_ypos & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_CLIP_X1
    //      Tilemap Clip Region X1
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_CLIP_X1", nextAddr,
        [this](Word, bool) { return _gpu_tmap_clip_x1 >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_clip_x1 = (_gpu_tmap_clip_x1 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region X1"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_tmap_clip_x1 & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_clip_x1 = (_gpu_tmap_clip_x1 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_CLIP_Y1
    //      Tilemap Clip Region Y1
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_CLIP_Y1", nextAddr,
        [this](Word, bool) { return _gpu_tmap_clip_y1 >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_clip_y1 = (_gpu_tmap_clip_y1 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region Y1"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_tmap_clip_y1 & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_clip_y1 = (_gpu_tmap_clip_y1 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_CLIP_X2
    //      Tilemap Clip Region X2
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_CLIP_X2", nextAddr,
        [this](Word, bool) { return _gpu_tmap_clip_x2 >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_clip_x2 = (_gpu_tmap_clip_x2 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region X2"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_tmap_clip_x2 & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_clip_x2 = (_gpu_tmap_clip_x2 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_CLIP_Y2
    //      Tilemap Clip Region Y2
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_CLIP_Y2", nextAddr,
        [this](Word, bool) { return _gpu_tmap_clip_y2 >> 8; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_clip_y2 = (_gpu_tmap_clip_y2 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region Y2",""
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word, bool) { return _gpu_tmap_clip_y2 & 0xFF; }, 
        [this](Word, Byte data, bool) { _gpu_tmap_clip_y2 = (_gpu_tmap_clip_y2 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_ERROR             
    //      Memory Management Unit Error Code:    (Read Only)
    /////
    _gpu->mapped_register.push_back({ "GPU_ERROR", nextAddr, 
        [this](Word, bool) { return _gpu_error; },  
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
        _gpu->mapped_register.push_back({
            error.first,            // The constant name (e.g., "GPU_ERR_NONE")
            err++,                  // Increment the error code value for each entry
            nullptr,                // Placeholder for function pointer (not used here)
            nullptr,                // Placeholder for another value (not used here)
            { comments }            // Formatted description (error message)
        });
    }
    // return the next available address
    return nextAddr;
}

void GPU_EXT::_onInit(GPU* pGpu) 
{
    // initialize the GPU command functions:
    for (const auto& command : _gpu_command_list) 
    {
        register_command(Memory::Map(command.key, __FILE__, __LINE__), 
            [this, action = command.action]() -> Byte 
            {
                return action();
            }
        );
    }
    // initial image info data
    _gpu_img_info.resize(256);
}

void GPU_EXT::_onQuit() {
    // TO DO: implement shutdown
}





// **************************** //
// Extended Graphics Functions: //
// **************************** //

void GPU_EXT::_gpu_calc_bgnd_size()
{
    // Extended Display is Enabled
    if (_gpu->_gpu_mode & 0b1000'0000'0000'0000)
    {
        // bitmap mode?
        if (_gpu->_gpu_mode & 0b0000'0000'0000'1000)
        {
            Byte color_depth_shift = (_gpu->_gpu_mode & 0b0011'0000'0000'0000) >> 12;
            _gpu_bgnd_size = (_gpu->_gpu_hres * _gpu->_gpu_vres) / (1 << color_depth_shift);
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



Byte GPU_EXT::_gpu_blit_read_data() 
{ 
    Byte data = _gpu->_ext_video_buffer[_gpu_blit_addr];
    _gpu_blit_increment_address();
    return data; 
}

void GPU_EXT::_gpu_blit_write_data(Byte data) 
{ 
    _gpu->_ext_video_buffer[_gpu_blit_addr] = data; 
    _gpu_blit_increment_address();
}

void GPU_EXT::_gpu_blit_increment_address()
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

void GPU_EXT::error(Byte error_code, bool bLog) 
{
    _gpu_error = error_code;    // Set the error code
    _gpu_command = 0;           // Clear the command (indicates an error)

    // Log the error with a helpful message
    if (bLog)
    {
        if (error_code < _gpu_error_list.size())
        {
            std::string comment = _gpu_error_list[error_code].second;
            UnitTest::Log(_gpu, clr::RED + "Error: " + comment + " $" + clr::hex(error_code, 2) );
        }
        else
        {
            UnitTest::Log(_gpu, clr::RED + "Error: Unknown Error $" + clr::hex(error_code, 2) );
        }
    }
}


Byte GPU_EXT::_validate_spr_flags(Byte data)
{
    Byte result = data;
    // This one is easy, nothing to do. This function is implemented 
    // for the potential future use of the (reserved) bit.
    // ...
    return result;
}


Byte GPU_EXT::_validate_img_flags(Byte data)
{
    Byte result = data;
    Byte color_mode = data & 0b0000'0011;
    // if 256-color mode, nothing else is valid
    if (color_mode == 0b0000'0011) {
        result = data &0b0000'0011;
        _gpu_usr_offset=255;
    }
    else
    {
        // new base offset
        switch (color_mode & 0x03) 
        {
            case 0: // 2-color mode
                _gpu_usr_offset = 32;
                break;
            case 1: // 4-color mode
                _gpu_usr_offset = 64;
                break;
            case 2: // 16-color mode
                _gpu_usr_offset = 128;
                break;
        }

        // Is this a sprite?
        if (Memory::Read(MAP(GPU_SPR_FLAGS)) & MAP(GPU_SPR_FL_SPR_ENABLE))
        {
            // secondary palette
            if (data & MAP(GPU_IMG_FL_SEC_PAL))
            {
                // ...
            }
            // 32 pixel width
            if (data & MAP(GPU_IMG_FL_32_WIDTH)) 
            {
                // ...
            }
            // 32 pixel height
            if (data & MAP(GPU_IMG_FL_32_HEIGHT) && color_mode <= 2) 
            {
                // ...
            }
        }
    }

    return result;
}




    // ********************** //
    // GPU COMMANDS AND TESTS //
    // ********************** //


void GPU_EXT::do_command(Byte command)
{
    _gpu_command = command;
    // find the command and run it
    auto it = _gpu_commands.find(command);
    if (it != _gpu_commands.end()) {
        it->second();  // Call the associated function
    } else {
        error(MAP(GPU_ERR_COMMAND));  // Handle invalid commands
    }    
}


bool GPU_EXT::_onTest() 
{
    bool all_tests_passed = true;

    // Perform Unit Tests for each command
    for (const auto& command : _gpu_command_list) 
    {
        UnitTest::TestInit(_gpu, "Testing " + clr::YELLOW + command.key + clr::RESET);
        if (!command.test()) 
        {
            UnitTest::Log(_gpu, clr::RED + "command " + clr::YELLOW + command.key + " " + clr::RED + "FAILED! ");
            all_tests_passed = false;
        }
        else
        {
            UnitTest::Log(_gpu, clr::GREEN + "command " + clr::YELLOW + command.key + " " + clr::GREEN + "PASSED! ");
        }
    }

    // Display the result of the tests
    if (all_tests_passed)
        UnitTest::Log(_gpu, "Unit Tests PASSED" );
    else
        UnitTest::Log(_gpu, clr::RED + "Unit Tests FAILED" );

    return all_tests_passed;
}

void GPU_EXT::register_command(Byte command, std::function<Byte()> handler)
{
    if (_gpu_commands.find(command) != _gpu_commands.end()) {
        UnitTest::Log(_gpu, "Warning: Command already registered: " + std::to_string(command));
        Bus::Error("Command already registered", __FILE__, __LINE__);
        return;
    }        
    _gpu_commands[command] = handler;
}


Byte GPU_EXT::do_nop()
{
    // This function literally does nothing
    // (as in NO OPERATION)
    return MAP(GPU_CMD_NOP);
}
bool GPU_EXT::_test_nop()
{
    bool test_results = true;
    // This function literally does nothing
    // (as in NO OPERATION)
    return test_results;
}            // if (_gpu_dyn_map.empty()) {
            //     // error(MAP(GPU_ERR_HANDLE));
            //     return (Byte)0xCC;
            // }            


// ...

Byte GPU_EXT::do_size()
{
    return MAP(GPU_CMD_SIZE);
}
bool GPU_EXT::_test_size()
{
    bool test_results = true;
    // ...
    return test_results;
}


