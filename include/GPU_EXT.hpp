/*** GPU_EXT.hpp *******************************************
 *       ___ ___ _   _         _____  _______       _              
 *      / __| _ \ | | |       | __\ \/ /_   _|     | |_  _ __ _ __ 
 *     | (_ |  _/ |_| |       | _| >  <  | |    _  | ' \| '_ \ '_ \
 *      \___|_|  \___/   ___  |___/_/\_\ |_|   (_) |_||_| .__/ .__/
 *                      |___|                           |_|  |_|     
 *   
 * This class extends the GPU class to provide sprite and tile graphics
 * support, as well as a command-based interface for accessing and
 * manipulating extended graphics hardware features.
 * 
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *                                                                                       
 ******************/

#pragma once

#include "IDevice.hpp"

class GPU;

class GPU_EXT
{
    friend class GPU;

private:    // PRIVATE SINGLETON STUFF
    GPU_EXT() {}
    GPU* _gpu = nullptr;    
    ~GPU_EXT() {};       

public:      // PUBLIC SINGLETON STUFF  
    GPU_EXT(const GPU_EXT&) = delete;				// delete the copy constructor
    GPU_EXT(GPU_EXT&&) = delete;					// delete the move constructor
    GPU_EXT& operator=(const GPU_EXT&) = delete;	// delete the copy assignment operator
    GPU_EXT& operator=(GPU_EXT&&) = delete;		// delete the move assignment operator    

    // This object can only be instantiated by the friend GPU class:
    // static GPU_EXT& GetInstance() { static GPU_EXT inst; return inst; }

private: 

    int  _onAttach(int nextAddr);   // attach to the memory map
    void _onInit(GPU* pGpu);        // initialize
    void _onQuit();                 // shutdown

    


    // ****************** //
    // HARDWARE REGISTERS //
    // ****************** //


    // *** Graphics Processor Extension Registers *** //

    // GPU_BGND_SIZE                        // (Word) Extended Graphics Buffer Size (Read Only)
    Word _gpu_bgnd_size = 0;                // Note: The primary extended graphics buffer
                                            //       always begins at $0000. This is also highest
                                            //       accessible memory location of the currently
                                            //       active background video mode.  
    void _gpu_calc_bgnd_size();     

    // GPU_BLIT_ADDR                        // (Word) Graphics Memory Address Port   
    Word _gpu_blit_addr = 0;                // Note: When GPU_BLIT_DATA is accessed, this register
                                            //       is automatically incremented to point to the  
                                            //       next byte to be read or written based on the 
                                            //       values in GPU_BLIT_PITCH and GPU_BLIT_WIDTH.

    // GPU_BLIT_PITCH                       // (Word) Number of Bytes Per Display Line
    Word _gpu_blit_pitch = 0;           

    // GPU_BLIT_WIDTH                       // (Word) Width of the Image Block in Pixels
    Word _gpu_blit_width = 0;           
    Word _gpu_blit_width_count = 0;

    // GPU_BLIT_DATA                        // (Byte) GPU Memory Data Port
    Byte _gpu_blit_read_data();             // GPU_BLIT_DATA Helper Function
    void _gpu_blit_write_data(Byte data);   // GPU_BLIT_DATA Helper Function
    void _gpu_blit_increment_address();     // GPU_BLIT_DATA Helper Function


// ...

    // // GPU_DYN_HANDLE                       // (Word) Dynamic Memory HANDLE
    // Word _gpu_dyn_handle = 0;               // On Write: resets GPU_DYN_CUR_ADDR, 
    //                                         // GPU_DYN_END_ADDR,  and GPU_DYN_END_DIST

    // struct DYNAMIC_MEMORY
    // {
    //     Word address;   // Memory Starting Address
    //     Word cur_addr;  // Current Address
    //     Word end_addr;  // End Address
    // };
    // std::unordered_map<Word, DYNAMIC_MEMORY> _gpu_dyn_map;
    // // _gpu_dyn_map_new();
    // // _gpu_dyn_map_del();

    // // GPU_DYN_CUR_ADDR                     // (Word) Current Dynamic Memory ADDRESS
    // Word _gpu_dyn_cur_addr = 0;
    // // _gpu_dyn_map[_gpu_dyn_handle].cur_addr;  //      (autoincrements by 1 on read/write)
    
    // // GPU_DYN_END_ADDR                     // (Word) Last Useful Dynamic Memory ADDRESS in this block
    // Word _gpu_dyn_end_addr = 0;
    // // _gpu_dyn_map[_gpu_dyn_handle].end_addr;

    // // GPU_DYN_END_DIST                     // (Word) Distance to End of this Dynamic Memory Block
    // Word _gpu_dyn_end_dist = 0;
    // // _gpu_dyn_map[_gpu_dyn_handle].end_dist;

    // // GPU_DYN_DATA                         // (Byte) Dynamic Memory DATA (Read/Write)
    // Byte _gpu_dyn_data = 0;
    // // _gpu_dyn_data = _ext_video_buffer[_gpu_dyn_map[_gpu_dyn_handle].cur_addr]


// GPU Command Registers:
    // GPU_CMD_ARG_1            (Word)      // Argument 1
    Word _gpu_cmd_arg_1 = 0;

    // GPU_CMD_ARG_2            (Word)      // Argument 2
    Word _gpu_cmd_arg_2 = 0;

    // GPU_CMD_ARG_3            (Word)      // Argument 3
    Word _gpu_cmd_arg_3 = 0;

    // GPU_CMD_ARG_4            (Word)      // Argument 4
    Word _gpu_cmd_arg_4 = 0;

    // GPU_CMD_ARG_5            (Word)      // Argument 5
    Word _gpu_cmd_arg_5 = 0;

    // GPU_COMMAND              (Byte)      // Graphics Processing Unit Command
    Byte _gpu_command = 0;
    void do_command(Byte command);

    // GPU Command Functions and their Unit Tests
    Byte do_nop();
    bool _test_nop();
    // ...
    Byte do_size();
    bool _test_size();

    // Command Structures and Maps
    struct CommandInfo {
        std::string key;
        std::string description;
        std::function<Byte()> action;  // Function pointer for the command
        std::function<bool()> test;    // Function pointer for the unit test
    };
    // std::vector<CommandInfo> _gpu_command_list;
    std::vector<CommandInfo> _gpu_command_list = {
        //    CONSTANT            , DESCRIPTION                                 , FUNCTION POINTER FOR THE COMMAND,             , FUNCTION POINTER FOR THE UNIT TEST
            {"GPU_CMD_NOP"        , "No Operation / Error",                     [this]() -> Byte { return do_nop(); },          [this]() -> bool { return _test_nop(); }},
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
            {"GPU_CMD_SIZE"       , "Total Number of MMU Commands",             [this]() -> Byte { return do_size(); },         [this]() -> bool { return _test_size(); }}
        };          
    std::unordered_map<Byte, std::function<Byte()>> _gpu_commands;
    void register_command(Byte command, std::function<Byte()> handler);

    
// Bitmap Image Modification Registers:               

    // GPU_BMP_IDX              (Byte)      // Bitmap Image Index (0-255)
    Byte _gpu_bmp_idx = 0;

    // GPU_BMP_OFFSET           (Byte)      // Offset Within the Image Buffer(0-255)
    Byte _gpu_bmp_offset = 0;

    // GPU_BMP_DATA             (Byte)      // Bitmap Data (Read Write)
    std::array<std::array<Byte, 256>, 256> _gpu_bmp_data = {0};  // this is the secondary 64k block of GPU memory

    struct IMG_INFO 
    {
        Sint16 XPos = 0;    // Sprite X Position
        Sint16 YPos = 0;    // Sprite Y Position
        Byte spr_flags = 0; // Sprite Bitmap Image Index
        Byte img_flags = 0; // Sprite Collision Mask (4x4)
    };
    std::vector<IMG_INFO> _gpu_img_info;

    // GPU_SPR_MAX              (Byte)      // Maximum Sprite Index (Read Only)
    Byte _gpu_spr_max = 0;

    // // GPU_SPR_IDX              (Byte)      // Sprite Index
    // Byte _gpu_spr_idx = 0;

    // GPU_SPR_XPOS             (SInt16)    // Sprite X Position
    Sint16 _gpu_spr_xpos = 0;

    // GPU_SPR_YPOS             (SInt16)    // Sprite Y Position
    Sint16 _gpu_spr_ypos = 0;

    // GPU_SPR_BMP_IDX          (Byte)      // Sprite Bitmap Image Index
    Byte _gpu_spr_bmp_idx = 0;

    // // GPU_SPR_APR_MASK         (DWord)      // Sprite Collision Approx. Mask (4x4)
    // DWord _gpu_spr_apr_mask = 0;

    // GPU_IMG_FLAGS            (Byte)      // Sprite Flags:
    Byte _gpu_img_flags = 0;
                            // % 0000'0001: Double Width    // GPU_SPR_FL_DBL_WIDTH 
                            // % 0000'0010: Double Height   // GPU_SPR_FL_DBL_HEIGHT    
                            // % 0000'0100: Flip Horizontal // GPU_SPR_FL_FLIP_HORIZ    
                            // % 0000'1000: Flip Vertical   // GPU_SPR_FL_FLIP_VERT     
                            // % 0011'0000: Collision Type:
                            //      00 = collision disabled // GPU_SPR_FL_COL_NONE      
                            //      01 = bounding box       // GPU_SPR_FL_COL_BOUNDS    
                            //      10 = center box         // GPU_SPR_FL_COL_CENTER    
                            //      11 = pixel mask         // GPU_SPR_FL_COL_PIXEL     
                            // % 0100'0000: Is Collided     // GPU_SPR_FL_IS_COLLIDED   
                            // % 1000'0000: Display Enable  // GPU_SPR_FL_ENABLE           

    // struct BITMAP16_INFO 
    // {
    //     Word handle = 0;    // handle to the dynamic memory
    //     Byte offset = 0;    // offset within the 16x16 image buffer (0-255)
    //     Byte flags = 0;     // 16x16 bitmap flags
    //                         // % 0000'0011: 
    //                         //      00 = 2 colors (size=32), 
    //                         //      01 = 4 colors (size=64), 
    //                         //      10 = 16 colors (size=128), 
    //                         //      11 = 256 colors (size=256)
    //                         // % 0111'1100: Reserved
    //                         // % 1000'0000: Is Allocated
    // };
    // std::vector<BITMAP16_INFO> _gpu_bitmaps;
        

// Tilemap Registers:     
    // GPU_TMAP_WIDTH           
    Word _gpu_tmap_width = 0;           // (Word) Tilemap Width (in pixels)

    // GPU_TMAP_HEIGHT           
    Word _gpu_tmap_height = 0;          // (Word) Tilemap Height (in pixels)

    // GPU_TMAP_XPOS           
    Sint16 _gpu_tmap_xpos = 0;          // (Sint16) Tilemap X Position (top left corner)

    // GPU_TMAP_YPOS           
    Sint16 _gpu_tmap_ypos = 0;          // (Sint16) Tilemap Y Position (top left corner)

    // GPU_TMAP_CLIP_X1        
    Word _gpu_tmap_clip_x1 = 0;         // (Word) Tilemap Clip Region X1

    // GPU_TMAP_CLIP_Y1        
    Word _gpu_tmap_clip_y1 = 0;         // (Word) Tilemap Clip Region Y1

    // GPU_TMAP_CLIP_X2        
    Word _gpu_tmap_clip_x2 = 255;       // (Word) Tilemap Clip Region X2

    // GPU_TMAP_CLIP_Y2        
    Word _gpu_tmap_clip_y2 = 159;       // (Word) Tilemap Clip Region Y2

// GPU Error Registers:
    // GPU_ERROR                (Byte)      // (Byte) Graphics Processing Unit Error
    Byte _gpu_error = 0;
    std::vector<std::pair<std::string, std::string>> _gpu_error_list = {
        { "GPU_ERR_NONE",       "No Error" },
        { "GPU_ERR_COMMAND",    "Invalid Command" },
        { "GPU_ERR_ADDRESS",    "Invalid Address" },
        { "GPU_ERR_HANDLE",     "Invalid Handle" },
        { "GPU_ERR_WIDTH",      "Invalid Width" },
        { "GPU_ERR_HEIGHT",     "Invalid Height" },
        { "GPU_ERR_PITCH",      "Invalid Pitch" },
        { "GPU_ERR_DATA",       "Invalid Data" },
        { "GPU_ERR_OFFSET",     "Invalid Offset" },
        { "GPU_ERR_ARGUMENT",   "Invalid Argument" },
        { "GPU_ERR_OOM",        "Out of Memory" },
        { "GPU_ERR_SIZE",       "Total Number of GPU Errors" }
    };    
    void error(Byte error_code, bool bLog = true);        // helper that sets an error code


    // ********************** //
    // GPU COMMANDS AND TESTS //
    // ********************** //

    bool _onTest();                 // return true for successful unit tests

};


/*

Image Descriptor (6-Bytes):
Sint16 Xpos
Sint16 Ypos
Byte spr_flags
Byte img_flags




(Byte) Sprite Flags:

 % 0000'0001:  Double Width
 % 0000'0010:  Double Height
 % 0000'0100:  Flip Horizontal
 % 0000'1000:  Flip Vertical
 % 0011'0000:  Collision Type
      00 = none
      01 = bounding box
      10 = center box
      11 = pixel mask
 % 0100'0000:  Sprite Enable (0=disable, 1=enable)
 % 1000'0000:  (reserved)

(Byte) Image Flags:
 % 0000'0011:  Color Mode
      00 = 2-color
      01 = 4-color
      10 = 16-color
      11 = 256-color
    Note: Bits 2-7 are only valid if the Color Mode is not 256-color.
 % 0000'0100:  Secondary Palette (rules apply)
 % 0000'1000:  32 pixel width (rules apply)
 % 0001'0000:  32 pixel height (rules apply)
 % 1110'0000:  (reserved)



*/