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
        [this](Word) { return _gpu_bgnd_size >> 8; }, 
        nullptr, 
        {   
            "(Word) Extended Graphics Buffer Size (Read Only)",
            "Note: The primary extended graphics buffer",
            "      always begins at $0000. This is also highest",
            "      accessible memory location of the currently",
            "      active background video mode.",""
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_bgnd_size & 0xFF; }, 
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
        [this](Word) { return _gpu_blit_addr >> 8; }, 
        [this](Word, Byte data) { _gpu_blit_addr = (_gpu_blit_addr & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Graphics Memory Address Port ",
            "Note: When GPU_BLIT_DATA is accessed, this register",
            "      is automatically incremented to point to the  ",
            "      next byte to be read or written based on the ",
            "      values in GPU_BLIT_PITCH and GPU_BLIT_WIDTH.",""
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_blit_addr & 0xFF; }, 
        [this](Word, Byte data) { _gpu_blit_addr = (_gpu_blit_addr & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_BLIT_PITCH
    //        (Word) Number of Bytes Per Display Line
    //        Note: This value represents the number of displayed
    //              pixels per line.
    /////
    _gpu->mapped_register.push_back( { "GPU_BLIT_PITCH", nextAddr,
        [this](Word) { return _gpu_blit_pitch >> 8; }, 
        [this](Word, Byte data) { _gpu_blit_pitch = (_gpu_blit_pitch & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Number of Bytes Per Display Line",
            "Note: This value represents the number of displayed",
            "      pixels per line.",""
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_blit_pitch & 0xFF; }, 
        [this](Word, Byte data) { _gpu_blit_pitch = (_gpu_blit_pitch & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_BLIT_WIDTH
    //      Width of the Image Block in Pixels
    /////
    _gpu->mapped_register.push_back( { "GPU_BLIT_WIDTH", nextAddr,
        // READ
        [this](Word) { return _gpu_blit_width >> 8; }, 
        // WRITE
        [this](Word, Byte data) 
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
        [this](Word) { return _gpu_blit_width & 0xFF; }, 
        // WRITE
        [this](Word, Byte data) 
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
        [this](Word) { return _gpu_blit_read_data(); }, 
        [this](Word, Byte data) { _gpu_blit_write_data(data); },
        {   
            "(Byte) GPU Memory Data Port",""
        }}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_DYN_HANDLE
    //      (Word) Dynamic Memory HANDLE
    /////
    _gpu->mapped_register.push_back( { "GPU_DYN_HANDLE", nextAddr,
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
    _gpu->mapped_register.push_back( { "", nextAddr,
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
    _gpu->mapped_register.push_back( { "GPU_DYN_CUR_ADDR", nextAddr,
        [this](Word) { return _gpu_dyn_cur_addr >> 8; }, 
        [this](Word, Byte data) { _gpu_dyn_cur_addr = (_gpu_dyn_cur_addr & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Current Dynamic Memory ADDRESS",
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_dyn_cur_addr & 0xFF; }, 
        [this](Word, Byte data) { _gpu_dyn_cur_addr = (_gpu_dyn_cur_addr & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_DYN_END_ADDR
    //      Last Dynamic Useful Memory ADDRESS in this block
    /////
    _gpu->mapped_register.push_back( { "GPU_DYN_END_ADDR", nextAddr,
        [this](Word) { return _gpu_dyn_end_addr >> 8; }, 
        [this](Word, Byte data) { _gpu_dyn_end_addr = (_gpu_dyn_end_addr & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Last Useful Dynamic Memory ADDRESS in this block"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_dyn_end_addr & 0xFF; }, 
        [this](Word, Byte data) { _gpu_dyn_end_addr = (_gpu_dyn_end_addr & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_DYN_END_DIST
    //      Distance to End of this Dynamic Memory Block
    /////
    _gpu->mapped_register.push_back( { "GPU_DYN_END_DIST", nextAddr,
        // READ
        [this](Word) { return _gpu_dyn_end_dist >> 8; }, 
        // write (Read Only)
        nullptr, 
        {   
            "(Word) Distance to End of this Dynamic Memory Block"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        // READ
        [this](Word) { return _gpu_dyn_end_dist & 0xFF; }, 
        // write (Read Only)
        nullptr,  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) GPU_DYN_DATA
    //      (Byte) Dynamic Memory DATA (Read/Write)
    /////
    _gpu->mapped_register.push_back( { "GPU_DYN_DATA", nextAddr,
        // READ
        [this](Word) 
        { 
            auto itr = _gpu_dyn_map.find(_gpu_dyn_handle);
            if (itr != _gpu_dyn_map.end())
            {
                if (_gpu_dyn_cur_addr < _gpu->_ext_video_buffer.size()) 
                {
                    Byte data = _gpu->_ext_video_buffer[_gpu_dyn_cur_addr]; 
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
                    return (Byte)0xCC;  // Mostly Arbitrary but in common with other devices
                }
            }
            // Error Condition: Invalid Handle
            error(MAP(GPU_ERR_HANDLE), false);
            return (Byte)0xCC;  // Mostly Arbitrary but in common with other devices
        }, 
        // WRITE
        [this](Word, Byte data) 
        { 
            auto itr = _gpu_dyn_map.find(_gpu_dyn_handle);
            if (itr != _gpu_dyn_map.end())
            {
                if (_gpu_dyn_cur_addr < _gpu->_ext_video_buffer.size()) 
                {
                    _gpu->_ext_video_buffer[_gpu_dyn_cur_addr] = data;
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
            "(Byte) Dynamic Memory DATA (Read/Write)",
            "       (GPU_DYN_CUR_ADDR is auto-incremented by 1 on read/write)"
        }
    }); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_CMD_ARG_1
    //      (Word) GPU Command Argument 1
    /////
    _gpu->mapped_register.push_back( { "GPU_CMD_ARG_1", nextAddr,
        [this](Word) { return _gpu_cmd_arg_1 >> 8; }, 
        [this](Word, Byte data) { _gpu_cmd_arg_1 = (_gpu_cmd_arg_1 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) GPU Command Argument 1"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_cmd_arg_1 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_cmd_arg_1 = (_gpu_cmd_arg_1 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_CMD_ARG_2
    //      (Word) GPU Command Argument 2
    /////
    _gpu->mapped_register.push_back( { "GPU_CMD_ARG_2", nextAddr,
        [this](Word) { return _gpu_cmd_arg_2 >> 8; }, 
        [this](Word, Byte data) { _gpu_cmd_arg_2 = (_gpu_cmd_arg_2 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) GPU Command Argument 2"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_cmd_arg_2 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_cmd_arg_2 = (_gpu_cmd_arg_2 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_CMD_ARG_3
    //      (Word) GPU Command Argument 3
    /////
    _gpu->mapped_register.push_back( { "GPU_CMD_ARG_3", nextAddr,
        [this](Word) { return _gpu_cmd_arg_3 >> 8; }, 
        [this](Word, Byte data) { _gpu_cmd_arg_3 = (_gpu_cmd_arg_3 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) GPU Command Argument 3"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_cmd_arg_3 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_cmd_arg_3 = (_gpu_cmd_arg_3 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_CMD_ARG_4
    //      (Word) GPU Command Argument 4
    /////
    _gpu->mapped_register.push_back( { "GPU_CMD_ARG_4", nextAddr,
        [this](Word) { return _gpu_cmd_arg_4 >> 8; }, 
        [this](Word, Byte data) { _gpu_cmd_arg_4 = (_gpu_cmd_arg_4 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) GPU Command Argument 4"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_cmd_arg_4 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_cmd_arg_4 = (_gpu_cmd_arg_4 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_CMD_ARG_5
    //      (Word) GPU Command Argument 5
    /////
    _gpu->mapped_register.push_back( { "GPU_CMD_ARG_5", nextAddr,
        [this](Word) { return _gpu_cmd_arg_5 >> 8; }, 
        [this](Word, Byte data) { _gpu_cmd_arg_5 = (_gpu_cmd_arg_5 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) GPU Command Argument 5",""
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_cmd_arg_5 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_cmd_arg_5 = (_gpu_cmd_arg_5 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) GPU_COMMAND
    //      Memory Management Unit Command
    /////
    _gpu->mapped_register.push_back({ "GPU_COMMAND", nextAddr, 
        [this](Word) { return _gpu_command; },  
        [this](Word, Byte data) { 
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
    // (Byte) GPU_SPR_MAX
    //      Maximum Sprite Index (Read Only)
    /////
    _gpu->mapped_register.push_back({ "GPU_SPR_MAX", nextAddr,  
        [this](Word) 
        { 
            _gpu_spr_max = _gpu_sprites.size() - 1;
            return _gpu_spr_max; 
        }, 
        nullptr,  {
            "(Byte) Maximum Sprite Index (Read Only)",
    }}); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_SPR_IDX
    //      Sprite Index
    /////
    _gpu->mapped_register.push_back({ "GPU_SPR_IDX", nextAddr,  
        [this](Word) { return _gpu_spr_idx; }, 
        [this](Word, Byte data) 
        { 
            if (data > _gpu_sprites.size()) 
            {
                _gpu_spr_idx = _gpu_sprites.size();
                _gpu_spr_idx = data; 
            }
        },  
        { "(Byte) Sprite Index (loads sprite data for this index)", }
    }); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Sint16) GPU_SPR_XPOS
    //      Sprite X Position
    /////
    _gpu->mapped_register.push_back( { "GPU_SPR_XPOS", nextAddr,
        [this](Word) { return _gpu_cmd_arg_5 >> 8; }, 
        [this](Word, Byte data) { _gpu_spr_xpos = (_gpu_spr_xpos & 0x00FF) | (data << 8); }, 
        {   
            "(Sint16) Sprite X Position",
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_spr_xpos & 0xFF; }, 
        [this](Word, Byte data) { _gpu_spr_xpos = (_gpu_spr_xpos & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Sint16) GPU_SPR_YPOS
    //      Sprite Y Position
    /////
    _gpu->mapped_register.push_back( { "GPU_SPR_YPOS", nextAddr,
        [this](Word) { return _gpu_spr_ypos >> 8; }, 
        [this](Word, Byte data) { _gpu_spr_ypos = (_gpu_spr_ypos & 0x00FF) | (data << 8); }, 
        {   
            "(Sint16) Sprite X Position",
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_spr_ypos & 0xFF; }, 
        [this](Word, Byte data) { _gpu_spr_ypos = (_gpu_spr_ypos & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_SPR_BMP_IDX
    //      Sprite Bitmap Image Index
    /////
    _gpu->mapped_register.push_back({ "GPU_SPR_BMP_IDX", nextAddr,  
        [this](Word) { return _gpu_spr_bmp_idx; }, 
        [this](Word, Byte data) 
        { 
            _gpu_spr_bmp_idx = std::min(data, (Byte)(_gpu_bitmaps.size() - 1));
        },  
        { "(Byte) Sprite Bitmap Image Index", }
    }); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_SPR_APR_MASK
    //      prite Collision Approx. Mask (4x4)
    /////
    _gpu->mapped_register.push_back( { "GPU_SPR_APR_MASK", nextAddr,
        [this](Word) { return _gpu_spr_apr_mask >> 8; }, 
        [this](Word, Byte data) { _gpu_spr_apr_mask = (_gpu_spr_apr_mask & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Sprite Collision Approx. Mask (4x4)",
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_spr_apr_mask & 0xFF; }, 
        [this](Word, Byte data) { _gpu_spr_apr_mask = (_gpu_spr_apr_mask & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;
  

    ////////////////////////////////////////////////
    // (Byte) GPU_SPR_FLAGS
    //      Sprite Flags:
    /////
    _gpu->mapped_register.push_back({ "GPU_SPR_FLAGS", nextAddr,  
        [this](Word) { return _gpu_spr_flags; }, 
        [this](Word, Byte data) { _gpu_spr_flags = data; },
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
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_IS_HIT"     , (0b0100'0000), nullptr,nullptr, { "   % 0100'0000:  Is Collided"}}); 
    _gpu->mapped_register.push_back({ "GPU_SPR_FL_ENABLE"     , (0b1000'0000), nullptr,nullptr, { "   % 1000'0000:  Sprite Enable",""}}); 


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_WIDTH
    //      Tilemap Width (in pixels)
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_WIDTH", nextAddr,
        [this](Word) { return _gpu_tmap_width >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_width = (_gpu_tmap_width & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Width (in pixels)"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_width & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_width = (_gpu_tmap_width & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_HEIGHT
    //      Tilemap Height (in pixels)
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_HEIGHT", nextAddr,
        [this](Word) { return _gpu_tmap_height >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_height = (_gpu_tmap_height & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Height (in pixels)"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_height & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_height = (_gpu_tmap_height & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (SInt16) GPU_TMAP_XPOS
    //      Tilemap X Position (top left corner)
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_XPOS", nextAddr,
        [this](Word) { return _gpu_tmap_xpos >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_xpos = (_gpu_tmap_xpos & 0x00FF) | (data << 8); }, 
        {   
            "(SInt16) Tilemap X Position (top left corner)"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_xpos & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_xpos = (_gpu_tmap_xpos & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (SInt16) GPU_TMAP_YPOS
    //      Tilemap Y Position (top left corner)
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_YPOS", nextAddr,
        [this](Word) { return _gpu_tmap_ypos >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_ypos = (_gpu_tmap_ypos & 0x00FF) | (data << 8); }, 
        {   
            "(SInt16) Tilemap Y Position (top left corner)"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_ypos & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_ypos = (_gpu_tmap_ypos & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_CLIP_X1
    //      Tilemap Clip Region X1
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_CLIP_X1", nextAddr,
        [this](Word) { return _gpu_tmap_clip_x1 >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_x1 = (_gpu_tmap_clip_x1 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region X1"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_clip_x1 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_x1 = (_gpu_tmap_clip_x1 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_CLIP_Y1
    //      Tilemap Clip Region Y1
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_CLIP_Y1", nextAddr,
        [this](Word) { return _gpu_tmap_clip_y1 >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_y1 = (_gpu_tmap_clip_y1 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region Y1"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_clip_y1 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_y1 = (_gpu_tmap_clip_y1 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_CLIP_X2
    //      Tilemap Clip Region X2
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_CLIP_X2", nextAddr,
        [this](Word) { return _gpu_tmap_clip_x2 >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_x2 = (_gpu_tmap_clip_x2 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region X2"
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_clip_x2 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_x2 = (_gpu_tmap_clip_x2 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_TMAP_CLIP_Y2
    //      Tilemap Clip Region Y2
    /////
    _gpu->mapped_register.push_back( { "GPU_TMAP_CLIP_Y2", nextAddr,
        [this](Word) { return _gpu_tmap_clip_y2 >> 8; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_y2 = (_gpu_tmap_clip_y2 & 0x00FF) | (data << 8); }, 
        {   
            "(Word) Tilemap Clip Region Y2",""
        }}); nextAddr+=1;
    _gpu->mapped_register.push_back( { "", nextAddr,
        [this](Word) { return _gpu_tmap_clip_y2 & 0xFF; }, 
        [this](Word, Byte data) { _gpu_tmap_clip_y2 = (_gpu_tmap_clip_y2 & 0xFF00) | (data << 0); },  
        {""}}); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_BMP_MAX
    //      Maximum Bitmap Index (Read Only)
    /////
    _gpu->mapped_register.push_back({ "GPU_BMP_MAX", nextAddr,  
        // READ 
        [this](Word) { return _gpu_bitmaps.size(); }, 
        // write (Read Only)
        nullptr,  
        { "(Byte) Maximum Bitmap Index (Read Only)", }
    }); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_BMP_IDX
    //      Bitmap Image Index (0-255)
    /////
    _gpu->mapped_register.push_back({ "GPU_BMP_IDX", nextAddr,  
        // READ 
        [this](Word) { return _gpu_bmp_idx; }, 
        // WRITE
        [this](Word, Byte data) 
        { 
            _gpu_bmp_idx = std::min(data, static_cast<Byte>(_gpu_bitmaps.size()));
        },  
        // COMMENT
        { "(Byte) Bitmap Image Index (0-255)", }
    }); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_BMP_OFFSET
    //      Offset Within the Image Buffer (0-255)
    /////
    _gpu->mapped_register.push_back({ "GPU_BMP_OFFSET", nextAddr, 
        // READ 
        [this](Word) { return _gpu_bmp_offset; }, 
        // WRITE
        [this](Word, Byte data) 
        { 
            Word handle = _gpu_bitmaps[_gpu_bmp_idx].handle;
            auto itr = _gpu_dyn_map.find(handle);
            if (itr != _gpu_dyn_map.end())
            {
                _gpu_bmp_offset = std::min(data, static_cast<Byte>((_gpu_dyn_map[handle].end_addr - _gpu_dyn_map[handle].address)));
                _gpu_dyn_map[handle].cur_addr = _gpu_dyn_map[handle].address + _gpu_bmp_offset;
            }
        },  
        // COMMENT
        { "(Byte) Offset Within the Image Buffer    (0-255)", }
    }); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) GPU_BMP_DATA
    //      Bitmap Data (Read Write)
    /////
    _gpu->mapped_register.push_back({ "GPU_BMP_DATA", nextAddr,  
        // READ
        [this](Word) 
        { 
            if (_gpu_bmp_offset < _gpu->_ext_video_buffer.size()) {
                return _gpu->_ext_video_buffer[_gpu_bmp_offset];
            } else {
                error(MAP(GPU_ERR_OFFSET));
                return static_cast<Byte>(0xCC); // Arbitrary, but 0xCC is used as convention elsewhere
            }
        },
        // WRITE
        [this](Word, Byte data) 
        { 
            if (_gpu_bmp_offset < _gpu->_ext_video_buffer.size()) {
                _gpu->_ext_video_buffer[_gpu_bmp_offset] = data;
            }
        },
        { "(Byte) Bitmap Data      (Read Write)","" }
    }); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_BMP_FLAGS
    //      Bitmap Flags:
    /////
    _gpu->mapped_register.push_back({ "GPU_BMP_FLAGS", nextAddr,  
        // READ 
        [this](Word) { return _gpu->_ext_video_buffer[_gpu_bmp_offset]; }, 
        // WRITE
        [this](Word, Byte data) { _gpu->_ext_video_buffer[_gpu_bmp_offset] = data; },
        { "(Byte)  Bitmap Flags:", " % 0000'0011 - Colors:"}
    }); nextAddr+=1;
    _gpu->mapped_register.push_back({ "GPU_BMP_2_COLORS"   , (0b0000'0000), nullptr,nullptr, { "     00 = 2 colors (size=32)"}}); 
    _gpu->mapped_register.push_back({ "GPU_BMP_4_COLORS"   , (0b0000'0001), nullptr,nullptr, { "     01 = 4 colors (size=64)"}}); 
    _gpu->mapped_register.push_back({ "GPU_BMP_16_COLORS"  , (0b0000'0010), nullptr,nullptr, { "     10 = 16 colors (size=128)"}}); 
    _gpu->mapped_register.push_back({ "GPU_BMP_256_COLORS" , (0b0000'0011), nullptr,nullptr, { "     11 = 256 colors (size=256)",
                                                                                         " % 1111'1100 - Reserved",""}});    


    ////////////////////////////////////////////////
    // (Byte) GPU_ERROR             
    //      Memory Management Unit Error Code:    (Read Only)
    /////
    _gpu->mapped_register.push_back({ "GPU_ERROR", nextAddr, 
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

