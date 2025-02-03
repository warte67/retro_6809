/*** Memory_Map.hpp *************************************************
*      __  __                                     __  __              _                 
*     |  \/  |                                   |  \/  |            | |                
*     | \  / | ___ _ __ ___   ___  _ __ _   _    | \  / | __ _ _ __  | |__  _ __  _ __  
*     | |\/| |/ _ \ '_ ` _ \ / _ \| '__| | | |   | |\/| |/ _` | '_ \ | '_ \| '_ \| '_ \ 
*     | |  | |  __/ | | | | | (_) | |  | |_| |   | |  | | (_| | |_) || | | | |_) | |_) |
*     |_|  |_|\___|_| |_| |_|\___/|_|   \__, |   |_|  |_|\__,_| .__(_)_| |_| .__/| .__/ 
*                                        __/ |_____           | |          | |   | |    
*                                       |___/______|          |_|          |_|   |_|    
*
* C++ Memory Map Definition File
* 
* Released under the GPL v3.0 License.
* Original Author: Jay Faries (warte67)
* 
************************************/

#ifndef __MEMORY_MAP_HPP__
#define __MEMORY_MAP_HPP__

enum MEMMAP
{
    //  **********************************************
    //  * Allocated 64k Memory Mapped System Symbols *
    //  **********************************************

    SOFT_VECTORS_DEVICE   = 0x0000,   // START: Software Interrupt Vectors
    SOFT_EXEC             = 0x0000,   // Exec Software Interrupt Vector
    SOFT_SWI3             = 0x0002,   // SWI3 Software Interrupt Vector
    SOFT_SWI2             = 0x0004,   // SWI2 Software Interrupt Vector
    SOFT_FIRQ             = 0x0006,   // FIRQ Software Interrupt Vector
    SOFT_IRQ              = 0x0008,   // IRQ Software Interrupt Vector
    SOFT_SWI              = 0x000A,   // SWI / SYS Software Interrupt Vector
    SOFT_NMI              = 0x000C,   // NMI Software Interrupt Vector
    SOFT_RESET            = 0x000E,   // RESET Software Interrupt Vector
// _______________________________________________________________________

    SYSTEM_MEMORY_DEVICE  = 0x0010,   // START: System Memory
    ZERO_PAGE             = 0x0010,   // Zero Page System and User Variables
    ZERO_PAGE_END         = 0x00FF,   // Zero Page System and User Variables
    EDT_BUFFER            = 0x0100,   // START: Line Edit Character Buffer
    KEY_END               = 0x017F,   // END: Line Edit Character Buffer
    FIO_BUFFER            = 0x0180,   // START: Input/Output Buffer
    FIO_BFR_END           = 0x01FF,   // END: Input/Output Buffer
    SYSTEM_STACK          = 0x0200,   // Bottom of System Stack Spcace
    SSTACK_END            = 0x03FF,   // END: System Stack Space
    SSTACK_TOP            = 0x0400,   // TOP: System Stack Space
// _______________________________________________________________________

    VIDEO_BUFFER_DEVICE   = 0x0400,   // START: Video Buffer (8K)
    VIDEO_START           = 0x0400,   // Start of standard video buffer
    VIDEO_END             = 0x23FF,   // End of standard video buffer
    VIDEO_TOP             = 0x2400,   // Top of standard video buffer
// _______________________________________________________________________

    USER_MEMORY_DEVICE    = 0x2400,   // START: User Memory (34K)
    USER_RAM              = 0x2400,   // User Accessable RAM
    USER_RAM_END          = 0xAFFF,   // End User Accessable RAM
    USER_RAM_TOP          = 0xB000,   // Top User Accessable RAM
// _______________________________________________________________________

    BANKED_MEMORY_REGION  = 0xB000,   // START: Banked Memory Region (16K)
    BANKMEM_ONE           = 0xB000,   // Banked Memory Page One (8K)
    BANKMEM_TWO           = 0xD000,   // Banked Memory Page Two (8K)
    BANKMEM_END           = 0xEFFF,   // End of Banked Memory Region
    BANKMEM_TOP           = 0xF000,   // TOP of Banked Memory Region
// _______________________________________________________________________

    KERNEL_ROM_DEVICE     = 0xF000,   // START: Kernel Rom (3.5K)
    KERNEL_START          = 0xF000,   // Start of Kernel Rom Space
    KERNEL_END            = 0xFDFF,   // End of Kernel Rom Space
    KERNEL_TOP            = 0xFE00,   // Top of Kernel Rom Space
// _______________________________________________________________________

    SYS_DEVICE            = 0xFE00,   // START: System and Debug Hardware Registers:
    SYS_BEGIN             = 0xFE00,   // Start of System Registers
    SYS_STATE             = 0xFE00,   // (Byte) System State Register
                                      // SYS_STATE: ABCD.SSSS                          
                                      // - bit  7   = Error: Standard Buffer Overflow  
                                      // - bit  6   = Error: Extended Buffer Overflow  
                                      // - bit  5   = Error: Reserved                  
                                      // - bit  4   = Error: Reserved                  
                                      // - bits 0-3 = CPU Speed (0-15):                
                                      //    0 ($0)  = CPU Clock   10 kHz 
                                      //    1 ($1)  = CPU Clock   25 kHz 
                                      //    2 ($2)  = CPU Clock   50 kHz 
                                      //    3 ($3)  = CPU Clock   75 kHz 
                                      //    4 ($4)  = CPU Clock  100 kHz 
                                      //    5 ($5)  = CPU Clock  150 kHz 
                                      //    6 ($6)  = CPU Clock  225 kHz 
                                      //    7 ($7)  = CPU Clock  350 kHz 
                                      //    8 ($8)  = CPU Clock  500 kHz 
                                      //    9 ($9)  = CPU Clock  750 kHz 
                                      //   10 ($A)  = CPU Clock  900 kHz 
                                      //   11 ($B)  = CPU Clock 1000 khz 
                                      //   12 ($C)  = CPU Clock 2000 khz 
                                      //   13 ($D)  = CPU Clock 3000 khz 
                                      //   14 ($E)  = CPU Clock 4000 khz 
                                      //   15 ($F)  = CPU Clock ~10.0 mhz. (unmetered) 
                                      // 
    SYS_SPEED             = 0xFE01,   // (Word) Average CPU Clock Speed (Read Only)
    SYS_CLOCK_DIV         = 0xFE03,   // (Byte) 60 hz Clock Divider Register (Read Only)
                                      // - bit 7: 0.546875 hz
                                      // - bit 6: 1.09375 hz
                                      // - bit 5: 2.1875 hz
                                      // - bit 4: 4.375 hz
                                      // - bit 3: 8.75 hz
                                      // - bit 2: 17.5 hz
                                      // - bit 1: 35.0 hz
                                      // - bit 0: 70.0 hz
                                      // 
    SYS_UPDATE_COUNT      = 0xFE04,   // (DWord) Update Count (Read Only)
    SYS_DBG_BRK_ADDR      = 0xFE08,   // (Word) Address of current debug breakpoint
    SYS_DBG_FLAGS         = 0xFE0A,   // (Byte) Debug Specific Hardware Flags:
                                      // - bit 7: Debug Enable
                                      // - bit 6: Single Step Enable
                                      // - bit 5: Clear All Breakpoints
                                      // - bit 4: Update Breakpoint at DEBUG_BRK_ADDR
                                      // - bit 3: FIRQ  (on low {0} to high {1} edge)
                                      // - bit 2: IRQ   (on low {0} to high {1} edge)
                                      // - bit 1: NMI   (on low {0} to high {1} edge)
                                      // - bit 0: RESET (on low {0} to high {1} edge)
                                      // 
    SYS_END               = 0xFE0A,   // End of System Registers
    SYS_TOP               = 0xFE0B,   // Top of System Registers
// _______________________________________________________________________

    GPU_DEVICE            = 0xFE0B,   // START: GPU Device Hardware Registers
    GPU_MODE              = 0xFE0B,   // 
    GPU_MODE_MSB          = 0xFE0B,   // (Byte) Graphics Display Mode
                                      // - bit  7   = Extended Display Enable:
                                      //               0: Disabled
                                      //               1: Enabled
                                      // - bit  6   = (reserved)
                                      // - bits 4-5 = Extended Color Depth:
                                      //               00: 2-Colors
                                      //               01: 4-Colors
                                      //               10: 16-Colors
                                      //               11: 256-Colors
                                      // - bit  3   = Extended Rendering Mode
                                      //               0: Tilemap Display
                                      //               1: Bitmap Display
                                      // - bit  2   = Emulation Screen Mode
                                      //               0: Windowed
                                      //               1: Fullscreen
                                      // - bit  1   = VSync Enable
                                      //               0: Disabled
                                      //               1: Enabled
                                      // - bit  0   = Presentation
                                      //               0: Letterbox
                                      //               1: Overscan / Stretch
                                      // 
    GPU_MODE_LSB          = 0xFE0C,   // - bit  7   = Standard Display Enable
                                      //              0: Disabled
                                      //              1: Enabled
                                      // - bit  6    = (reserved)
                                      // - bits 4-5 = Standard Bitmap Color Depth:
                                      //               00: 2-Colors
                                      //               01: 4-Colors
                                      //               10: 16-Colors
                                      //               11: 256-Colors
                                      // - bit  3    = Standard Bitmap:
                                      //               0: Text Display
                                      //               1: Bitmap Display
                                      // MODE (bits 0-2):
                                      // - bit  2    = 0: 320/256 width,  1: 160/128 width
                                      // - bit  1    = 0: 200/160 height, 1: 160/80 height
                                      // - bit  0    = Base Resolution: 0:320x200, 1:256x160
                                      // 
                                      //               Text Mode Table:
                                      //    | MODE | COLUMNS |  ROWS  | BUFFER |
                                      //    |:----:|:-------:|:------:|:------:|
                                      //    |  $00 |    40   |   25   |  2000  |
                                      //    |  $01 |    32   |   20   |  1280  |
                                      //    |  $02 |    40   |   12   |   960  |
                                      //    |  $03 |    32   |   10   |   640  |
                                      //    |  $04 |    20   |   25   |  1000  |
                                      //    |  $05 |    16   |   20   |   640  |
                                      //    |  $06 |    20   |   12   |   480  |
                                      //    |  $07 |    16   |   10   |   320  |
                                      // 
                                      //         Bitmap Display Mode Table:
                                      // | MODE | WIDTH | HEIGHT | COLORS | BUFFER |
                                      // |------|-------|--------|--------|--------|
                                      // | $00  |  320  |   200  |    2   |  8000  |
                                      // | $01  |  256  |   160  |    2   |  5120  |
                                      // | $02  |  320  |   100  |    2   |  4000  |
                                      // | $03  |  256  |    80  |    2   |  2560  |
                                      // | $04  |  160  |   200  |    2   |  4000  |
                                      // | $05  |  128  |   160  |    2   |  2560  |
                                      // | $06  |  160  |   100  |    2   |  2000  |
                                      // | $07  |  128  |    80  |    2   |  1280  |
                                      // | $00  |  320  |   200  |    4   | 16000  | (bgnd only)
                                      // | $01  |  256  |   160  |    4   | 10240  | (bgnd only)
                                      // | $02  |  320  |   100  |    4   |  8000  |
                                      // | $03  |  256  |    80  |    4   |  5120  |
                                      // | $04  |  160  |   200  |    4   |  8000  |
                                      // | $05  |  128  |   160  |    4   |  5120  |
                                      // | $06  |  160  |   100  |    4   |  4000  |
                                      // | $07  |  128  |    80  |    4   |  2560  |
                                      // | $00  |  320  |   200  |   16   | 32000  | (bgnd only)
                                      // | $01  |  256  |   160  |   16   | 20480  | (bgnd only)
                                      // | $02  |  320  |   100  |   16   | 16000  | (bgnd only)
                                      // | $03  |  256  |    80  |   16   | 10240  | (bgnd only)
                                      // | $04  |  160  |   200  |   16   | 16000  | (bgnd only)
                                      // | $05  |  128  |   160  |   16   | 10240  | (bgnd only)
                                      // | $06  |  160  |   100  |   16   |  8000  |
                                      // | $07  |  128  |    80  |   16   |  5120  |
                                      // | $00  |  320  |   200  |  256   | 64000  | (bgnd only)
                                      // | $01  |  256  |   160  |  256   | 40960  | (bgnd only)
                                      // | $02  |  320  |   100  |  256   | 32000  | (bgnd only)
                                      // | $03  |  256  |    80  |  256   | 20480  | (bgnd only)
                                      // | $04  |  160  |   200  |  256   | 32000  | (bgnd only)
                                      // | $05  |  128  |   160  |  256   | 20480  | (bgnd only)
                                      // | $06  |  160  |   100  |  256   | 16000  | (bgnd only)
                                      // | $07  |  128  |    80  |  256   | 10240  | (bgnd only)
                                      // 
    GPU_VIDEO_MAX         = 0xFE0D,   // (Word) Video Buffer Maximum (Read Only)
                                      // Note: This will change to reflect
                                      //       the size of the last cpu
                                      //       accessible memory location
                                      //       of the currently active
                                      //       standard video mode.
                                      // 
    GPU_HRES              = 0xFE0F,   // (Word) Horizontal Pixel Resolution (Read Only)
                                      // Note: This will reflect the number of
                                      //       pixel columns for bitmap modes.
                                      // 
    GPU_VRES              = 0xFE11,   // (Word) Vertical Pixel Resolution (Read Only)
                                      // Note: This will reflect the number of
                                      //       pixel rows for bitmap modes.
                                      // 
    GPU_TCOLS             = 0xFE13,   // (Byte) Text Horizontal Columns (Read Only)
                                      // Note: This will reflect the number of
                                      //       glyph columns for text modes.
                                      // 
    GPU_TROWS             = 0xFE14,   // (Byte) Text Vertical Rows (Read Only)
                                      // Note: This will reflect the number of
                                      //       glyph rows for text modes.
                                      // 
    GPU_PAL_INDEX         = 0xFE15,   // (Byte) Color Palette Index
                                      // Note: Use this register to set the
                                      //       index into the Color Palette.
                                      //       Set this value prior referencing
                                      //       the color data (GPU_PAL_COLOR).
                                      // 
    GPU_PAL_COLOR         = 0xFE16,   // (Word) Color Palette Data (A4R4G4B4 format)
                                      // Note: This is the color data for an
                                      //       individual palette entry. Write to 
                                      //       DSP_PAL_IDX with the index within the
                                      //       color palette prior to reading or
                                      //       writing the color data in the
                                      //       GFX_PAL_CLR register.
                                      // 
    GPU_GLYPH_IDX         = 0xFE18,   // (Byte) Text Glyph Index
                                      // Note: Use this register to set the
                                      //       index of a specific text glyph.
                                      //       Set this value prior to updating
                                      //       the glyph data (GPU_GLYPH_DATA).
                                      // 
    GPU_GLYPH_DATA        = 0xFE19,   // (8-Bytes) 8 rows of binary encoded glyph pixel data
                                      // Note: This is the pixel data for a
                                      //       specific text glyph. Each 8x8
                                      //       text glyph is composed of 8 bytes.
                                      //       The first byte in this array
                                      //       represents the top line of 8 pixels.
                                      //       Each array entry represents a row of 8 pixels.
                                      // 
    GPU_BGND_SIZE         = 0xFE21,   // (Word) Extended Graphics Buffer Size (Read Only)
                                      // Note: The primary extended graphics buffer
                                      //       always begins at $0000. This is also highest
                                      //       accessible memory location of the currently
                                      //       active background video mode.
                                      // 
    GPU_BLIT_ADDR         = 0xFE23,   // (Word) Graphics Memory Address Port 
                                      // Note: When GPU_BLIT_DATA is accessed, this register
                                      //       is automatically incremented to point to the  
                                      //       next byte to be read or written based on the 
                                      //       values in GPU_BLIT_PITCH and GPU_BLIT_WIDTH.
                                      // 
    GPU_BLIT_PITCH        = 0xFE25,   // (Word) Number of Bytes Per Display Line
                                      // Note: This value represents the number of displayed
                                      //       pixels per line.
                                      // 
    GPU_BLIT_WIDTH        = 0xFE27,   // (Word) Width of the Image Block in Pixels
    GPU_BLIT_DATA         = 0xFE29,   // (Byte) GPU Memory Data Port
                                      // 
    GPU_CMD_ARG_1         = 0xFE2A,   // (Word) GPU Command Argument 1
    GPU_CMD_ARG_2         = 0xFE2C,   // (Word) GPU Command Argument 2
    GPU_CMD_ARG_3         = 0xFE2E,   // (Word) GPU Command Argument 3
    GPU_CMD_ARG_4         = 0xFE30,   // (Word) GPU Command Argument 4
    GPU_CMD_ARG_5         = 0xFE32,   // (Word) GPU Command Argument 5
                                      // 
    GPU_COMMAND           = 0xFE34,   // (Byte) Memory Management Unit Command:
                                      // 
    GPU_CMD_NOP           = 0x0000,   //    $00 = No Operation / Error
    GPU_CMD_SIZE          = 0x0001,   //    $01 = Total Number of MMU Commands
                                      // 
    GPU_BMP_IDX           = 0xFE35,   // (Byte) Bitmap Image Index (0-255)
    GPU_BMP_OFFSET        = 0xFE36,   // (Byte) Offset Within the Image Buffer    (0-255)
    GPU_BMP_DATA          = 0xFE37,   // (Byte) Bitmap Data      (Read Write)
                                      // 
    GPU_SPR_XPOS          = 0xFE38,   // (Sint16) Sprite X Position
    GPU_SPR_YPOS          = 0xFE3A,   // (Sint16) Sprite X Position
    GPU_SPR_FLAGS         = 0xFE3C,   // (Byte) Sprite Flags:
                                      // 
    GPU_SPR_FL_DBL_WIDTH  = 0x0001,   //    % 0000'0001:  Double Width
    GPU_SPR_FL_DBL_HEIGHT = 0x0002,   //    % 0000'0010:  Double Height
    GPU_SPR_FL_FLP_HORIZ  = 0x0004,   //    % 0000'0100:  Flip Horizontal
    GPU_SPR_FL_FLP_VERT   = 0x0008,   //    % 0000'1000:  Flip Vertical
                                      //    % 0011'0000:  Collision Type:
    GPU_SPR_FL_COL_NONE   = 0x0000,   //         00 = none
    GPU_SPR_FL_COL_BNDS   = 0x0001,   //         01 = bounding box
    GPU_SPR_FL_COL_CNTR   = 0x0002,   //         10 = center box
    GPU_SPR_FL_COL_PXL    = 0x0003,   //         11 = pixel mask
    GPU_SPR_FL_SPR_ENABLE = 0x0040,   //    % 0100'0000:  Sprite Enable
    GPU_SPR_FL_RESERVED   = 0x0080,   //    % 1000'0000:  (reserved)
                                      // 
    GPU_IMG_FLAGS         = 0xFE3D,   // (Byte) Image Flags:
                                      // 
    GPU_IMG_COLOR_MODE    = 0x0003,   //    % 0000'0011:  Color Mode:
    GPU_IMG_FL_2_COLORS   = 0x0000,   //         00 = 2-colors
    GPU_IMG_FL_4_COLORS   = 0x0001,   //         01 = 4-colors
    GPU_IMG_FL_16_COLORS  = 0x0002,   //         10 = 16-colors
    GPU_IMG_FL_256_COLORS = 0x0003,   //         11 = 256-colors
    GPU_IMG_FL_SEC_PAL    = 0x0004,   //    % 0000'0100:  Secondary Palette (rules apply)
    GPU_IMG_FL_32_WIDTH   = 0x0008,   //    % 0000'1000:  32 pixel width (rules apply)
    GPU_IMG_FL_32_HEIGHT  = 0x0010,   //    % 0001'0000:  32 pixel height (rules apply)
    GPU_IMG_FL_DBL_WIDTH  = 0x00E0,   //    % 1110'0000:  (reserved)
                                      // 
    GPU_TMAP_WIDTH        = 0xFE3E,   // (Word) Tilemap Width (in pixels)
    GPU_TMAP_HEIGHT       = 0xFE40,   // (Word) Tilemap Height (in pixels)
    GPU_TMAP_XPOS         = 0xFE42,   // (SInt16) Tilemap X Position (top left corner)
    GPU_TMAP_YPOS         = 0xFE44,   // (SInt16) Tilemap Y Position (top left corner)
    GPU_TMAP_CLIP_X1      = 0xFE46,   // (Word) Tilemap Clip Region X1
    GPU_TMAP_CLIP_Y1      = 0xFE48,   // (Word) Tilemap Clip Region Y1
    GPU_TMAP_CLIP_X2      = 0xFE4A,   // (Word) Tilemap Clip Region X2
    GPU_TMAP_CLIP_Y2      = 0xFE4C,   // (Word) Tilemap Clip Region Y2
                                      // 
    GPU_ERROR             = 0xFE4E,   // (Byte) Graphics Processing Unit Error Code:     (Read Only)
    GPU_ERR_NONE          = 0x0000,   //    $00 = No Error
    GPU_ERR_COMMAND       = 0x0001,   //    $01 = Invalid Command
    GPU_ERR_ADDRESS       = 0x0002,   //    $02 = Invalid Address
    GPU_ERR_HANDLE        = 0x0003,   //    $03 = Invalid Handle
    GPU_ERR_WIDTH         = 0x0004,   //    $04 = Invalid Width
    GPU_ERR_HEIGHT        = 0x0005,   //    $05 = Invalid Height
    GPU_ERR_PITCH         = 0x0006,   //    $06 = Invalid Pitch
    GPU_ERR_DATA          = 0x0007,   //    $07 = Invalid Data
    GPU_ERR_OFFSET        = 0x0008,   //    $08 = Invalid Offset
    GPU_ERR_ARGUMENT      = 0x0009,   //    $09 = Invalid Argument
    GPU_ERR_OOM           = 0x000A,   //    $0A = Out of Memory
    GPU_ERR_SIZE          = 0x000B,   //    $0B = Total Number of GPU Errors
                                      // 
    GPU_END               = 0xFE4E,   // End of GPU Register Space
    GPU_TOP               = 0xFE4F,   // Top of GPU Register Space
// _______________________________________________________________________

    CSR_DEVICE            = 0xFE4F,   // START: Mouse Device Hardware Registers
    CSR_XPOS              = 0xFE4F,   // (Word) Horizontal Mouse Cursor Coordinate
    CSR_YPOS              = 0xFE51,   // (Word) Vertical Mouse Cursor Coordinate
    CSR_XOFS              = 0xFE53,   // (Byte) Horizontal Mouse Cursor Offset
    CSR_YOFS              = 0xFE54,   // (Byte) Vertical Mouse Cursor Offset
    CSR_SCROLL            = 0xFE55,   // (char) MouseWheel Scroll: -1, 0, 1
    CSR_FLAGS             = 0xFE56,   // (Byte) Mouse Device State Flags
                                      //    bits 0-4: button states
                                      //    bits 5-6: number of clicks
                                      //    bits 7:   cursor enable
                                      // 
    CSR_BMP_INDX          = 0xFE57,   // (Byte) Mouse Cursor Bitmap Pixel Offset
    CSR_BMP_DATA          = 0xFE58,   // (Byte) Mouse Cursor Bitmap Pixel Color Data ($0-$F)
    CSR_PAL_INDX          = 0xFE59,   // (Byte) Mouse Cursor Color Palette Index (0-15)
    CSR_PAL_DATA          = 0xFE5A,   // (Word) Mouse Cursor Color Palette Data A4R4G4B4
    CSR_END               = 0xFE5B,   // End of Mouse Device Register Space
    CSR_TOP               = 0xFE5C,   // Top of CSR Register Space
// _______________________________________________________________________

    KEYBOARD_DEVICE       = 0xFE5C,   // START: Keyboard Device Hardware Registers
    CHAR_Q_LEN            = 0xFE5C,   // (Byte) Number of Characters Waiting in Queue   (Read Only)
    CHAR_SCAN             = 0xFE5D,   // (Byte) Read Next Character in Queue (Not Popped When Read)
    CHAR_POP              = 0xFE5E,   // (Byte) Read Next Character in Queue     (Popped When Read)
    XKEY_BUFFER           = 0xFE5F,   // (16 Bytes) 128 bits for XK_KEY data buffer     (Read Only)
    EDT_BFR_CSR           = 0xFE6F,   // (Byte) Cursor Position Within Edit Buffer     (Read/Write)
    EDT_ENABLE            = 0xFE70,   // (Byte) Line Editor Enable Flag                (Read/Write)
    EDT_BFR_LEN           = 0xFE71,   // (Byte) Limit the Line Editor to This Length   (Read/Write)
    KEYBOARD_END          = 0xFE71,   // End of Keyboard Register Space
    KEYBOARD_TOP          = 0xFE72,   // Top of Keyboard Register Space
// _______________________________________________________________________

    JOYSTICK_DEVICE       = 0xFE72,   // START: Joystick/Gamepad Controller Device Hardware Registers
    JOYS_1_FLAGS          = 0xFE72,   // (Byte) Gamepad/Joystick #1 Condition Flags:     (Read Only)
                                      //            0000'0000: Not Connected
                                      //            0000'1111: Controller Type
                                      //            0001'0000: (reserved)
                                      //            0010'0000: (reserved)
                                      //            0100'0000: Is a Gamepad
                                      //            1000'0000: Is a Joystick
                                      // 
                                      //        Gamepad Controller Type (bits 0-3):
                                      //            0:  Unknown
                                      //            1:  Standard
                                      //            2:  Xbox360
                                      //            3:  XboxOne
                                      //            4:  PS3
                                      //            5:  PS4
                                      //            6:  PS5
                                      //            7:  Nintendo Switch Pro
                                      //            8:  Nintendo Switch Joycon Left
                                      //            9:  Nintendo Switch Joycon Right
                                      // 
                                      //        Joystick Controller Type (bits 0-3):
                                      //            0:  Unknown
                                      //            1:  Gamepad
                                      //            2:  Wheel
                                      //            3:  Arcade Stick
                                      //            4:  Flight Stick
                                      //            5:  Dance Pad
                                      //            6:  Guitar
                                      //            7:  Drum Kit
                                      //            8:  Arcade Pad
                                      //            9:  Throttle
                                      // 
    JOYS_1_BTN            = 0xFE73,   // (Word) Gamepad Controller Button Bits:         (Read Only)
                                      //            0000'0000'0000'0000 = Nothing Pressed
                                      //            0000'0000'0000'0001 = A
                                      //            0000'0000'0000'0010 = B
                                      //            0000'0000'0000'0100 = X
                                      //            0000'0000'0000'1000 = Y
                                      //            0000'0000'0001'0000 = L.Shoulder
                                      //            0000'0000'0010'0000 = R.Shoulder
                                      //            0000'0000'0100'0000 = Back
                                      //            0000'0000'1000'0000 = Start
                                      //            0000'0001'0000'0000 = Misc 1
                                      //            0000'0010'0000'0000 = Misc 2
                                      //            0000'0100'0000'0000 = Misc 3
                                      //            0000'1000'0000'0000 = Guide
                                      //            0001'0000'0000'0000 = DPad Up   
                                      //            0010'0000'0000'0000 = DPad Down 
                                      //            0100'0000'0000'0000 = DPad Left 
                                      //            1000'0000'0000'0000 = DPad Right
                                      //            1111'1111'1111'1111 = Not Connected
                                      // 
    JOYS_1_DBND           = 0xFE75,   // (Byte) PAD 1 analog deadband; default is 5   (read/write)
    JOYS_1_LTX            = 0xFE76,   // (char) PAD 1 LThumb-X position (-128 _ +127)   (realtime)
    JOYS_1_LTY            = 0xFE77,   // (char) PAD 1 LThumb-Y position (-128 _ +127)   (realtime)
    JOYS_1_RTX            = 0xFE78,   // (char) PAD 1 RThumb-X position (-128 _ +127)   (realtime)
    JOYS_1_RTY            = 0xFE79,   // (char) PAD 1 RThumb-Y position (-128 _ +127)   (realtime)
    JOYS_1_Z1             = 0xFE7A,   // (char) PAD 1 left analog trigger (0 - 127)     (realtime)
    JOYS_1_Z2             = 0xFE7B,   // (char) PAD 1 left analog trigger (0 - 127)     (realtime)
                                      // 
    JOYS_2_FLAGS          = 0xFE7C,   // (Byte) Gamepad/Joystick #2 Condition Flags:     (Read Only)
                                      //            0000'0000: Not Connected
                                      //            0000'1111: Controller Type
                                      //            0001'0000: (reserved)
                                      //            0010'0000: (reserved)
                                      //            0100'0000: Is a Gamepad
                                      //            1000'0000: Is a Joystick
                                      // 
                                      //        Gamepad Controller Type (bits 0-3):
                                      //            0:  Unknown
                                      //            1:  Standard
                                      //            2:  Xbox360
                                      //            3:  XboxOne
                                      //            4:  PS3
                                      //            5:  PS4
                                      //            6:  PS5
                                      //            7:  Nintendo Switch Pro
                                      //            8:  Nintendo Switch Joycon Left
                                      //            9:  Nintendo Switch Joycon Right
                                      // 
                                      //        Joystick Controller Type (bits 0-3):
                                      //            0:  Unknown
                                      //            1:  Gamepad
                                      //            2:  Wheel
                                      //            3:  Arcade Stick
                                      //            4:  Flight Stick
                                      //            5:  Dance Pad
                                      //            6:  Guitar
                                      //            7:  Drum Kit
                                      //            8:  Arcade Pad
                                      //            9:  Throttle
                                      // 
    JOYS_2_BTN            = 0xFE7D,   // (Word) Button Bits: Room For up to 16 Buttons  (realtime)
                                      //        Joystick Button Bits:
                                      //            0000'0000'0000'0000 = Nothing Pressed
                                      //            0000'0000'0000'0001 = Button 1 
                                      //            0000'0000'0000'0010 = Button 2 
                                      //            0000'0000'0000'0100 = Button 3 
                                      //            0000'0000'0000'1000 = Button 4 
                                      //            0000'0000'0001'0000 = Button 5 
                                      //            0000'0000'0010'0000 = Button 6 
                                      //            0000'0000'0100'0000 = Button 7 
                                      //            0000'0000'1000'0000 = Button 8 
                                      //            0000'0001'0000'0000 = Button 9 
                                      //            0000'0010'0000'0000 = Button 10
                                      //            0000'0100'0000'0000 = Button 11
                                      //            0000'1000'0000'0000 = Button 12
                                      //            0001'0000'0000'0000 = Hat Up   
                                      //            0010'0000'0000'0000 = Hat Down 
                                      //            0100'0000'0000'0000 = Hat Left 
                                      //            1000'0000'0000'0000 = Hat Right
                                      //            1111'1111'1111'1111 = Not Connected
                                      // 
    JOYS_2_DBND           = 0xFE7F,   // (Byte) PAD 2 analog deadband; default is 5   (read/write)
    JOYS_2_LTX            = 0xFE80,   // (char) PAD 2 LThumb-X position (-128 _ +127)   (realtime)
    JOYS_2_LTY            = 0xFE81,   // (char) PAD 2 LThumb-Y position (-128 _ +127)   (realtime)
    JOYS_2_RTX            = 0xFE82,   // (char) PAD 2 RThumb-X position (-128 _ +127)   (realtime)
    JOYS_2_RTY            = 0xFE83,   // (char) PAD 2 RThumb-Y position (-128 _ +127)   (realtime)
    JOYS_2_Z1             = 0xFE84,   // (char) PAD 2 left analog trigger (0 - 127)     (realtime)
    JOYS_2_Z2             = 0xFE85,   // (char) PAD 2 left analog trigger (0 - 127)     (realtime)
    JOYS_END              = 0xFE85,   // End of Joystick/Gamepad Device Register Space
    JOYS_TOP              = 0xFE86,   // Top of Joystick/Gamepad Device Register Space
// _______________________________________________________________________

    FIO_DEVICE            = 0xFE86,   // START: File I/O Device Hardware Registers
    FIO_ERROR             = 0xFE86,   // (Byte) FILE_ERROR enumeration result (FE_<error>)
                                      // 
    FE_BEGIN              = 0x0000,   //   Begin FILE_ERROR enumeration 
    FE_NOERROR            = 0x0000,   //      no error, condition normal
    FE_NOTFOUND           = 0x0001,   //      file or folder not found  
    FE_NOTOPEN            = 0x0002,   //      file not open             
    FE_EOF                = 0x0003,   //      end of file               
    FE_OVERRUN            = 0x0004,   //      buffer overrun            
    FE_WRONGTYPE          = 0x0005,   //      wrong file type           
    FE_BAD_CMD            = 0x0006,   //      invalid command           
    FE_BADSTREAM          = 0x0007,   //      invalid file stream       
    FE_NOT_EMPTY          = 0x0007,   //      directory not empty       
    FE_FILE_EXISTS        = 0x0007,   //      file already exists       
    FE_INVALID_NAME       = 0x0007,   //      invalid file name         
    FE_LAST               = 0x0007,   //   End of FILE_ERROR enumeration
                                      // 
    FIO_COMMAND           = 0xFE87,   // (Byte) Execute a File Command (FC_<cmd>)
                                      // 
    FC_BEGIN              = 0x0000,   //   Begin FIO_COMMAND enumeration           
    FC_RESET              = 0x0000,   //     Reset                                 
    FC_SHUTDOWN           = 0x0001,   //     SYSTEM: Shutdown                      
    FC_COMPDATE           = 0x0002,   //     SYSTEM: Load Compilation Date         
    FC_FILEEXISTS         = 0x0003,   //     Does File Exist (return in FIO_IODATA)
    FC_OPENREAD           = 0x0004,   //     Open Binary File For Reading          
    FC_OPENWRITE          = 0x0005,   //     Open Binary File For Writing          
    FC_OPENAPPEND         = 0x0006,   //     Open Binary File For Appending        
    FC_CLOSEFILE          = 0x0007,   //     Close File                            
    FC_READBYTE           = 0x0008,   //     Read Byte (into FIO_IOBYTE)           
    FC_WRITEBYTE          = 0x0009,   //     Write Byte (from FIO_IOBYTE)          
    FC_LOADHEX            = 0x000A,   //     Load Hex Format File                  
    FC_GETLENGTH          = 0x000B,   //     Get File Length (into FIO_IOWORD)     
    FC_LISTDIR            = 0x000C,   //     List Directory                        
    FC_MAKEDIR            = 0x000D,   //     Make Directory                        
    FC_CHANGEDIR          = 0x000E,   //     Change Directory                      
    FC_GETPATH            = 0x000F,   //     Fetch Current Path                    
    FC_REN_DIR            = 0x0010,   //     Rename Directory                      
    FC_DEL_DIR            = 0x0011,   //     Delete Directory                      
    FC_DEL_FILE           = 0x0012,   //     Delete File                           
    FC_REN_FILE           = 0x0013,   //     Rename File                           
    FC_COPY_FILE          = 0x0014,   //     Copy File                             
    FC_SEEK_START         = 0x0015,   //     Seek Start                            
    FC_SEEK_END           = 0x0016,   //     Seek End                              
    FC_SET_SEEK           = 0x0017,   //     Set Seek Position (from FIO_IOWORD)   
    FC_GET_SEEK           = 0x0018,   //     Get Seek Position (into FIO_IOWORD)   
    FC_LAST               = 0x0018,   //   End FIO_COMMAND enumeration             
                                      // 
    FIO_HANDLE            = 0xFE88,   // (Byte) Current File Stream HANDLE (0=NONE)
    FIO_SEEKPOS           = 0xFE89,   // (DWord) File Seek Position
    FIO_IODATA            = 0xFE8D,   // (Byte) Input / Output Data
                                      // 
    FIO_PATH_LEN          = 0xFE8E,   // (Byte) Length of the Primary Filepath        (Read Only)
    FIO_PATH_POS          = 0xFE8F,   // (Byte) Character Position Within the Primary Filepath
    FIO_PATH_DATA         = 0xFE90,   // (Byte) Data at the Character Position of the Primary Path
                                      // 
    FIO_ALT_PATH_LEN      = 0xFE91,   // (Byte) Length of the alternate Filepath        (Read Only)
    FIO_ALT_PATH_POS      = 0xFE92,   // (Byte) Character Position Within the Alternate Filepath
    FIO_ALT_PATH_DATA     = 0xFE93,   // (Byte) Data at the Character Position of the Alternate Path
                                      // 
    FIO_DIR_DATA          = 0xFE94,   // (Byte) A Series of Null-Terminated Filenames
                                      //   NOTE: Current read-position is reset to the beginning
                                      //     following a List Directory command. The read-position
                                      //     is automatically advanced on read from this register.
                                      //     Each filename is $0A-terminated. The list itself is
                                      //     null-terminated.
                                      // 
    FIO_END               = 0xFE94,   // End of FIO Device Register Space
    FIO_TOP               = 0xFE95,   // Top of FIO Device Register Space
// _______________________________________________________________________

    MATH_DEVICE           = 0xFE95,   // START: Math Co-Processor Device Hardware Registers
    MATH_ACA_POS          = 0xFE95,   // (Byte) Character Position Within the ACA Float String
    MATH_ACA_DATA         = 0xFE96,   // (Byte) ACA Float String Character Port
    MATH_ACA_RAW          = 0xFE97,   // (4-Bytes) ACA Raw Float Data
    MATH_ACA_INT          = 0xFE9B,   // (4-Bytes) ACA Integer Data
                                      // 
    MATH_ACB_POS          = 0xFE9F,   // (Byte) Character Position Within the ACB Float String
    MATH_ACB_DATA         = 0xFEA0,   // (Byte) ACB Float String Character Port
    MATH_ACB_RAW          = 0xFEA1,   // (4-Bytes) ACB Raw Float Data
    MATH_ACB_INT          = 0xFEA5,   // (4-Bytes) ACB Integer Data
                                      // 
    MATH_ACR_POS          = 0xFEA9,   // (Byte) Character Position Within the ACR Float String
    MATH_ACR_DATA         = 0xFEAA,   // (Byte) ACR Float String Character Port
    MATH_ACR_RAW          = 0xFEAB,   // (4-Bytes) ACR Raw Float Data
    MATH_ACR_INT          = 0xFEAF,   // (4-Bytes) ACR Integer Data
                                      // 
    MATH_OPERATION        = 0xFEB3,   // (Byte) ACA Float String Character Port   (On Write)
    MOP_BEGIN             = 0x0000,   //   BEGIN Math Operation Enumeration:
    MOP_RANDOM            = 0x0000,   //     ACA, ACB, and ACR are set to randomized values
    MOP_RND_SEED          = 0x0001,   //     MATH_ACA_INT seeds the pseudo-random number generator
    MOP_IS_EQUAL          = 0x0002,   //     (bool)ACR = (ACA == ACB)
    MOP_IS_NOT_EQUAL      = 0x0003,   //     (bool)ACR = (ACA != ACB)
    MOP_IS_LESS           = 0x0004,   //     (bool)ACR = std::isless(ACA, ACB)
    MOP_IS_GREATER        = 0x0005,   //     (bool)ACR = std::isgreater(ACA, ACB)
    MOP_IS_LTE            = 0x0006,   //     (bool)ACR = std::islessequal(ACA, ACB)
    MOP_IS_GTE            = 0x0007,   //     (bool)ACR = std::islessgreater(ACA, ACB)
    MOP_IS_FINITE         = 0x0008,   //     (bool)ACR = std::isfinite(ACA)
    MOP_IS_INF            = 0x0009,   //     (bool)ACR = std::isinf(ACA)
    MOP_IS_NAN            = 0x000A,   //     (bool)ACR = std::isnan(ACA)
    MOP_IS_NORMAL         = 0x000B,   //     (bool)ACR = std::isnormal(ACA)
    MOP_SIGNBIT           = 0x000C,   //     (bool)ACR = std::signbit(ACA)
    MOP_SUBTRACT          = 0x000D,   //     ACR = ACA - ACB
    MOP_ADD               = 0x000E,   //     ACR = ACA + ACB
    MOP_MULTIPLY          = 0x000F,   //     ACR = ACA * ACB
    MOP_DIVIDE            = 0x0010,   //     ACR = ACA / ACB
    MOP_FMOD              = 0x0011,   //     ACR = std::fmod(ACA, ACB)
    MOP_REMAINDER         = 0x0012,   //     ACR = std::remainder(ACA, ACB)
    MOP_FMAX              = 0x0013,   //     ACR = std::fmax(ACA, ACB)
    MOP_FMIN              = 0x0014,   //     ACR = std::fmin(ACA, ACB)
    MOP_FDIM              = 0x0015,   //     ACR = std::fdim(ACA, ACB)
    MOP_EXP               = 0x0016,   //     ACR = std::exp(ACA)
    MOP_EXP2              = 0x0017,   //     ACR = std::exp2(ACA)
    MOP_EXPM1             = 0x0018,   //     ACR = std::expm1(ACA)
    MOP_LOG               = 0x0019,   //     ACR = std::log(ACA)
    MOP_LOG10             = 0x001A,   //     ACR = std::log10(ACA)
    MOP_LOG2              = 0x001B,   //     ACR = std::log2(ACA)
    MOP_LOG1P             = 0x001C,   //     ACR = std::log1p(ACA)
    MOP_SQRT              = 0x001D,   //     ACR = std::sqrt(ACA)
    MOP_CBRT              = 0x001E,   //     ACR = std::cbrt(ACA)
    MOP_HYPOT             = 0x001F,   //     ACR = std::hypot(ACA, ACB)
    MOP_POW               = 0x0020,   //     ACR = std::pow(ACA, ACB)
    MOP_SIN               = 0x0021,   //     ACR = std::sin(ACA)
    MOP_COS               = 0x0022,   //     ACR = std::cos(ACA)
    MOP_TAN               = 0x0023,   //     ACR = std::tan(ACA)
    MOP_ASIN              = 0x0024,   //     ACR = std::asin(ACA)
    MOP_ACOS              = 0x0025,   //     ACR = std::acos(ACA)
    MOP_ATAN              = 0x0026,   //     ACR = std::atan(ACA)
    MOP_ATAN2             = 0x0027,   //     ACR = std::atan2(ACA, ACB)
    MOP_SINH              = 0x0028,   //     ACR = std::sinh(ACA)
    MOP_COSH              = 0x0029,   //     ACR = std::cosh(ACA)
    MOP_ASINH             = 0x002A,   //     ACR = std::asinh(ACA)
    MOP_ACOSH             = 0x002B,   //     ACR = std::acosh(ACA)
    MOP_ATANH             = 0x002C,   //     ACR = std::atanh(ACA)
    MOP_ERF               = 0x002D,   //     ACR = std::erf(ACA)
    MOP_ERFC              = 0x002E,   //     ACR = std::erfc(ACA)
    MOP_LGAMMA            = 0x002F,   //     ACR = std::lgamma(ACA)
    MOP_TGAMMA            = 0x0030,   //     ACR = std::tgamma(ACA)
    MOP_CEIL              = 0x0031,   //     ACR = std::ceil(ACA)
    MOP_FLOOR             = 0x0032,   //     ACR = std::floor(ACA)
    MOP_TRUNC             = 0x0033,   //     ACR = std::trunc(ACA)
    MOP_ROUND             = 0x0034,   //     ACR = std::round(ACA)
    MOP_LROUND            = 0x0035,   //     ACR = std::lround(ACA)
    MOP_NEARBYINT         = 0x0036,   //     ACR = std::nearbyint(ACA)
    MOP_ILOGB             = 0x0037,   //     ACR = std::ilogb(ACA)
    MOP_LOGB              = 0x0038,   //     ACR = std::logb(ACA)
    MOP_NEXTAFTER         = 0x0039,   //     ACR = std::nextafter(ACA, ACB)
    MOP_COPYSIGN          = 0x003A,   //     ACR = std::copysign(ACA, ACB)
    MOP_LASTOP            = 0x003B,   //   END Math Operation Enumeration
    MATH_END              = 0xFEB3,   // End of Math Co-Processor Register Space
    MATH_TOP              = 0xFEB4,   // Top of Math Co-Processor Register Space
// _______________________________________________________________________

    MMU_DEVICE            = 0xFEB4,   // START: Memory Management Unit Hardware Registers
    MMU_PAGE_1_SELECT     = 0xFEB4,   // (Word) Page Select for 8K Memory Bank 1
    MMU_PAGE_2_SELECT     = 0xFEB6,   // (Word) Page Select for 8K Memory Bank 2
    MMU_BLOCKS_FREE       = 0xFEB8,   // (Word) Number of 32-Byte Blocks Available for Allocation (Read Only)
    MMU_BLOCKS_ALLOCATED  = 0xFEBA,   // (Word) Number of 32-Byte Blocks Currently Allocated  (Read Only)
    MMU_BLOCKS_FRAGGED    = 0xFEBC,   // (Word) Number of 32-Byte Blocks Currently Fragmented  (Read Only)
    MMU_ARG_1             = 0xFEBE,   // (Word) Argument 1 for MMU Command
    MMU_ARG_1_MSB         = 0xFEBE,   // (Byte) Argument 1 Most Significant Byte for MMU Command
    MMU_ARG_1_LSB         = 0xFEBF,   // (Byte) Argument 1 Least Significant Byte for MMU Command
    MMU_ARG_2             = 0xFEC0,   // (Word) Argument 2 for MMU Command
    MMU_ARG_2_MSB         = 0xFEC0,   // (Byte) Argument 2 Most Significant Byte for MMU Command
    MMU_ARG_2_LSB         = 0xFEC1,   // (Byte) Argument 2 Least Significant Byte for MMU Command
                                      // 
    MMU_COMMAND           = 0xFEC2,   // (Byte) Memory Management Unit Command:
    MMU_CMD_NOP           = 0x0000,   //    $00 = No Operation / Error
    MMU_CMD_PG_ALLOC      = 0x0001,   //    $01 = Page Allocate (8K Bytes)
    MMU_CMD_PG_FREE       = 0x0002,   //    $02 = Page Deallocate (8K Bytes)
    MMU_CMD_ALLOC         = 0x0003,   //    $03 = Allocate Chain (< 8K Bytes)
    MMU_CMD_LOAD_ROOT     = 0x0004,   //    $04 = Load Root Node
    MMU_CMD_LOAD_NEXT     = 0x0005,   //    $05 = Load Next Node
    MMU_CMD_LOAD_PREV     = 0x0006,   //    $06 = Load Prev Node
    MMU_CMD_LOAD_LAST     = 0x0007,   //    $07 = Load Last Node
    MMU_CMD_DEL_NODE      = 0x0008,   //    $08 = Remove Current Node (and Adjust Links)
    MMU_CMD_INS_BEFORE    = 0x0009,   //    $09 = Insert Node Before (and activate)
    MMU_CMD_INS_AFTER     = 0x000A,   //    $0A = Insert Node After (and activate)
    MMU_CMD_PUSH_BACK     = 0x000B,   //    $0B = Push Back (and activate)
    MMU_CMD_PUSH_FRONT    = 0x000C,   //    $0C = Push Front (and activate)
    MMU_CMD_POP_BACK      = 0x000D,   //    $0D = Pop Back (and activate)
    MMU_CMD_POP_FRONT     = 0x000E,   //    $0E = Pop Front (and activate)
    MMU_CMD_LOCK_NODE     = 0x000F,   //    $0F = Lock Node
    MMU_CMD_UNLOCK_NODE   = 0x0010,   //    $10 = Unlock Node
    MMU_CMD_FREE          = 0x0011,   //    $11 = Deallocate Chain (< 8K Bytes)
    MMU_CMD_DEFRAG        = 0x0012,   //    $12 = Defragment / Collect Garbage
    MMU_CMD_RESET         = 0x0013,   //    $13 = Reset Memory Management Unit
    MMU_CMD_SIZE          = 0x0014,   //    $14 = Total Number of MMU Commands
                                      // 
    MMU_ERROR             = 0xFEC3,   // (Byte) Memory Management Unit Error Code:     (Read Only)
    MMU_ERR_NONE          = 0x0000,   //    $00 = No Error
    MMU_ERR_ALLOC         = 0x0001,   //    $01 = Failed to Allocate Memory
    MMU_ERR_FREE          = 0x0002,   //    $02 = Failed to Deallocate Memory
    MMU_ERR_PG_FREE       = 0x0003,   //    $03 = Error Deallocating Page
    MMU_ERR_INVALID       = 0x0004,   //    $04 = Invalid Command
    MMU_ERR_HANDLE        = 0x0005,   //    $05 = Invalid Handle
    MMU_ERR_NODE          = 0x0006,   //    $06 = Invalid Node
    MMU_ERR_RAW_INDEX     = 0x0007,   //    $07 = Invalid Raw Index
    MMU_ERR_SIZE          = 0x0008,   //    $08 = Total Number of MMU Errors
                                      // 
    MMU_META_HANDLE       = 0xFEC4,   // (Word) Handle for the current allocation chain
                                      // 
    MMU_META_STATUS       = 0xFEC6,   // (Byte) Status Flags:
    MMU_STFLG_ALLOC       = 0x0001,   //    0000'0001: Is Allocated: 0 = Free, 1 = Allocated
    MMU_STFLG_PAGED       = 0x0002,   //    0000'0010: Paged Memory: 0 = No,   1 = Yes
    MMU_STFLG_READONLY    = 0x0004,   //    0000'0100: Memory Type:  0 = RAM,  1 = ROM
    MMU_STFLG_FRAGD       = 0x0008,   //    0000'1000: Fragmented:   0 = No,   1 = Yes
    MMU_STFLG_LOCKED      = 0x0010,   //    0001'0000: Locked:       0 = No,   1 = Yes
    MMU_STFLG_RES_1       = 0x0020,   //    0010'0000:   (reserved)
    MMU_STFLG_RES_2       = 0x0040,   //    0100'0000:   (reserved)
    MMU_STFLG_ERROR       = 0x0080,   //    1000'0000: Error:        0 = No,   1 = Yes
                                      // 
    MMU_META_DATA         = 0xFEC7,   // (32-Bytes) Data Window for the Current Allocation
    MMU_META_ROOT         = 0xFEE7,   // (Word) Root node of the current allocation       (Read Only)
    MMU_META_PREV         = 0xFEE9,   // (Word) Previous node of the current allocation   (Read Only)
    MMU_META_NEXT         = 0xFEEB,   // (Word) Next node of the current allocation       (Read Only)
    MMU_RAW_INDEX         = 0xFEED,   // (Word) Raw Index of the current memory node  (Node Window)
                                      // 
    MMU_END               = 0xFEEE,   // End of Banked Memory Register Space
    MMU_TOP               = 0xFEEF,   // Top of Banked Memory Register Space
// _______________________________________________________________________

    HDW_RESERVED_DEVICE   = 0xFEEF,   // START: Reserved Register Space
    HDW_REG_END           = 0xFFF0,   // 257 bytes reserved for future use.
// _______________________________________________________________________

    ROM_VECTS_DEVICE      = 0xFFF0,   // START: Hardware Interrupt Vectors
    HARD_EXEC             = 0xFFF0,   // EXEC Hardware Interrupt Vector
    HARD_SWI3             = 0xFFF2,   // SWI3 Hardware Interrupt Vector
    HARD_SWI2             = 0xFFF4,   // SWI2 Hardware Interrupt Vector
    HARD_FIRQ             = 0xFFF6,   // FIRQ Hardware Interrupt Vector
    HARD_IRQ              = 0xFFF8,   // IRQ Hardware Interrupt Vector
    HARD_SWI              = 0xFFFA,   // SWI / SYS Hardware Interrupt Vector
    HARD_NMI              = 0xFFFC,   // NMI Hardware Interrupt Vector
    HARD_RESET            = 0xFFFE,   // RESET Hardware Interrupt Vector
    MEMMAP_END
}; // END: enum MEMMAP


#endif // __MEMORY_MAP_H__


