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
                                      // - bit  2    = 0: 320/256 width,  1: 160/128 width
                                      // - bit  1    = 0: 200/160 height, 1: 160/80 height
                                      // - bit  0    = Base Resolution: 0:320x200, 1:256x160            
                                      // 
    GPU_VIDEO_MAX         = 0xFE0D,   // (Word) Video Buffer Maximum (Read Only)
                                      //  Note: This will change to reflect
                                      //        the size of the last cpu
                                      //        accessible memory location
                                      //        of the currently active
                                      //        standard video mode.
                                      // 
    GPU_HRES              = 0xFE0F,   // (Word) Horizontal Pixel Resolution (Read Only)
                                      //   Note: This will reflect the number of
                                      //        pixel columns for bitmap modes.
                                      // 
    GPU_VRES              = 0xFE11,   // (Word) Vertical Pixel Resolution (Read Only)
                                      //   Note: This will reflect the number of
                                      //        pixel rows for bitmap modes.
                                      // 
    GPU_TCOLS             = 0xFE13,   // (Byte) Text Horizontal Columns (Read Only)
                                      //   Note: This will reflect the number of
                                      //        glyph columns for text modes.
                                      // 
    GPU_TROWS             = 0xFE14,   // (Byte) Text Vertical Rows (Read Only)
                                      //   Note: This will reflect the number of
                                      //        glyph rows for text modes.
                                      // 
    GPU_PAL_INDEX         = 0xFE15,   // (Byte) Color Palette Index
                                      //   Note: Use this register to set the
                                      //        index into the Color Palette.
                                      //        Set this value prior referencing
                                      //        the color data (GPU_PAL_COLOR).
                                      // 
    GPU_PAL_COLOR         = 0xFE16,   // (Word) Color Palette Data (A4R4G4B4 format)
                                      //   Note: This is the color data for an
                                      //        individual palette entry. Write to 
                                      //        DSP_PAL_IDX with the index within the
                                      //        color palette prior to reading or
                                      //        writing the color data in the
                                      //        GFX_PAL_CLR register.
                                      // 
    GPU_GLYPH_IDX         = 0xFE18,   // (Byte) Text Glyph Index
                                      //   Note: Use this register to set the
                                      //        index of a specific text glyph.
                                      //        Set this value prior to updating
                                      //        the glyph data (GPU_GLYPH_DATA).
                                      // 
    GPU_GLYPH_DATA        = 0xFE19,   // (8-Bytes) 8 rows of binary encoded glyph pixel data
                                      //   Note: This is the pixel data for a
                                      //        specific text glyph. Each 8x8
                                      //        text glyph is composed of 8 bytes.
                                      //        The first byte in this array
                                      //        represents the top line of 8 pixels.
                                      //        Each array entry represents a row of 8 pixels.
                                      // 
    GPU_END               = 0xFE20,   // End of GPU Register Space
    GPU_TOP               = 0xFE21,   // Top of GPU Register Space
// _______________________________________________________________________

    CSR_DEVICE            = 0xFE21,   // START: Mouse Device Hardware Registers
    CSR_XPOS              = 0xFE21,   // (Word) Horizontal Mouse Cursor Coordinate
    CSR_YPOS              = 0xFE23,   // (Word) Vertical Mouse Cursor Coordinate
    CSR_XOFS              = 0xFE25,   // (Byte) Horizontal Mouse Cursor Offset
    CSR_YOFS              = 0xFE26,   // (Byte) Vertical Mouse Cursor Offset
    CSR_SCROLL            = 0xFE27,   // (char) MouseWheel Scroll: -1, 0, 1
    CSR_FLAGS             = 0xFE28,   // (Byte) Mouse Device State Flags
                                      //    bits 0-4: button states
                                      //    bits 5-6: number of clicks
                                      //    bits 7:   cursor enable
                                      // 
    CSR_BMP_INDX          = 0xFE29,   // (Byte) Mouse Cursor Bitmap Pixel Offset
    CSR_BMP_DATA          = 0xFE2A,   // (Byte) Mouse Cursor Bitmap Pixel Color Data ($0-$F)
    CSR_PAL_INDX          = 0xFE2B,   // (Byte) Mouse Cursor Color Palette Index (0-15)
    CSR_PAL_DATA          = 0xFE2C,   // (Word) Mouse Cursor Color Palette Data A4R4G4B4
    CSR_END               = 0xFE2D,   // End of Mouse Device Register Space
    CSR_TOP               = 0xFE2E,   // Top of CSR Register Space
// _______________________________________________________________________

    KEYBOARD_DEVICE       = 0xFE2E,   // START: Keyboard Device Hardware Registers
    CHAR_Q_LEN            = 0xFE2E,   // (Byte) Number of Characters Waiting in Queue   (Read Only)
    CHAR_SCAN             = 0xFE2F,   // (Byte) Read Next Character in Queue (Not Popped When Read)
    CHAR_POP              = 0xFE30,   // (Byte) Read Next Character in Queue     (Popped When Read)
    XKEY_BUFFER           = 0xFE31,   // (16 Bytes) 128 bits for XK_KEY data buffer     (Read Only)
    EDT_BFR_CSR           = 0xFE41,   // (Byte) Cursor Position Within Edit Buffer     (Read/Write)
    EDT_ENABLE            = 0xFE42,   // (Byte) Line Editor Enable Flag                (Read/Write)
    EDT_BFR_LEN           = 0xFE43,   // (Byte) Limit the Line Editor to This Length   (Read/Write)
    KEYBOARD_END          = 0xFE43,   // End of Keyboard Register Space
    KEYBOARD_TOP          = 0xFE44,   // Top of Keyboard Register Space
// _______________________________________________________________________

    JOYSTICK_DEVICE       = 0xFE44,   // START: Joystick/Gamepad Controller Device Hardware Registers
    JOYS_1_FLAGS          = 0xFE44,   // (Byte) Gamepad/Joystick #1 Condition Flags:     (Read Only)
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
    JOYS_1_BTN            = 0xFE45,   // (Word) Gamepad Controller Button Bits:         (Read Only)
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
    JOYS_1_DBND           = 0xFE47,   // (Byte) PAD 1 analog deadband; default is 5   (read/write)
    JOYS_1_LTX            = 0xFE48,   // (char) PAD 1 LThumb-X position (-128 _ +127)   (realtime)
    JOYS_1_LTY            = 0xFE49,   // (char) PAD 1 LThumb-Y position (-128 _ +127)   (realtime)
    JOYS_1_RTX            = 0xFE4A,   // (char) PAD 1 RThumb-X position (-128 _ +127)   (realtime)
    JOYS_1_RTY            = 0xFE4B,   // (char) PAD 1 RThumb-Y position (-128 _ +127)   (realtime)
    JOYS_1_Z1             = 0xFE4C,   // (char) PAD 1 left analog trigger (0 - 127)     (realtime)
    JOYS_1_Z2             = 0xFE4D,   // (char) PAD 1 left analog trigger (0 - 127)     (realtime)
                                      // 
    JOYS_2_FLAGS          = 0xFE4E,   // (Byte) Gamepad/Joystick #2 Condition Flags:     (Read Only)
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
    JOYS_2_BTN            = 0xFE4F,   // (Word) Button Bits: Room For up to 16 Buttons  (realtime)
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
    JOYS_2_DBND           = 0xFE51,   // (Byte) PAD 2 analog deadband; default is 5   (read/write)
    JOYS_2_LTX            = 0xFE52,   // (char) PAD 2 LThumb-X position (-128 _ +127)   (realtime)
    JOYS_2_LTY            = 0xFE53,   // (char) PAD 2 LThumb-Y position (-128 _ +127)   (realtime)
    JOYS_2_RTX            = 0xFE54,   // (char) PAD 2 RThumb-X position (-128 _ +127)   (realtime)
    JOYS_2_RTY            = 0xFE55,   // (char) PAD 2 RThumb-Y position (-128 _ +127)   (realtime)
    JOYS_2_Z1             = 0xFE56,   // (char) PAD 2 left analog trigger (0 - 127)     (realtime)
    JOYS_2_Z2             = 0xFE57,   // (char) PAD 2 left analog trigger (0 - 127)     (realtime)
    JOYS_END              = 0xFE57,   // End of Joystick/Gamepad Device Register Space
    JOYS_TOP              = 0xFE58,   // Top of Joystick/Gamepad Device Register Space
// _______________________________________________________________________

    FIO_DEVICE            = 0xFE58,   // START: File I/O Device Hardware Registers
    FIO_ERROR             = 0xFE58,   // (Byte) FILE_ERROR enumeration result (FE_<error>)
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
    FIO_COMMAND           = 0xFE59,   // (Byte) Execute a File Command (FC_<cmd>)
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
    FIO_HANDLE            = 0xFE5A,   // (Byte) Current File Stream HANDLE (0=NONE)
    FIO_SEEKPOS           = 0xFE5B,   // (DWord) File Seek Position
    FIO_IODATA            = 0xFE5F,   // (Byte) Input / Output Data
                                      // 
    FIO_PATH_LEN          = 0xFE60,   // (Byte) Length of the Primary Filepath        (Read Only)
    FIO_PATH_POS          = 0xFE61,   // (Byte) Character Position Within the Primary Filepath
    FIO_PATH_DATA         = 0xFE62,   // (Byte) Data at the Character Position of the Primary Path
                                      // 
    FIO_ALT_PATH_LEN      = 0xFE63,   // (Byte) Length of the alternate Filepath        (Read Only)
    FIO_ALT_PATH_POS      = 0xFE64,   // (Byte) Character Position Within the Alternate Filepath
    FIO_ALT_PATH_DATA     = 0xFE65,   // (Byte) Data at the Character Position of the Alternate Path
                                      // 
    FIO_DIR_DATA          = 0xFE66,   // (Byte) A Series of Null-Terminated Filenames
                                      //   NOTE: Current read-position is reset to the beginning
                                      //     following a List Directory command. The read-position
                                      //     is automatically advanced on read from this register.
                                      //     Each filename is $0A-terminated. The list itself is
                                      //     null-terminated.
                                      // 
    FIO_END               = 0xFE66,   // End of FIO Device Register Space
    FIO_TOP               = 0xFE67,   // Top of FIO Device Register Space
// _______________________________________________________________________

    MATH_DEVICE           = 0xFE67,   // START: Math Co-Processor Device Hardware Registers
    MATH_ACA_POS          = 0xFE67,   // (Byte) Character Position Within the ACA Float String
    MATH_ACA_DATA         = 0xFE68,   // (Byte) ACA Float String Character Port
    MATH_ACA_RAW          = 0xFE69,   // (4-Bytes) ACA Raw Float Data
    MATH_ACA_INT          = 0xFE6D,   // (4-Bytes) ACA Integer Data
                                      // 
    MATH_ACB_POS          = 0xFE71,   // (Byte) Character Position Within the ACB Float String
    MATH_ACB_DATA         = 0xFE72,   // (Byte) ACB Float String Character Port
    MATH_ACB_RAW          = 0xFE73,   // (4-Bytes) ACB Raw Float Data
    MATH_ACB_INT          = 0xFE77,   // (4-Bytes) ACB Integer Data
                                      // 
    MATH_ACR_POS          = 0xFE7B,   // (Byte) Character Position Within the ACR Float String
    MATH_ACR_DATA         = 0xFE7C,   // (Byte) ACR Float String Character Port
    MATH_ACR_RAW          = 0xFE7D,   // (4-Bytes) ACR Raw Float Data
    MATH_ACR_INT          = 0xFE81,   // (4-Bytes) ACR Integer Data
                                      // 
    MATH_OPERATION        = 0xFE85,   // (Byte) ACA Float String Character Port   (On Write)
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
    MOP_COSH              = 0x0029,   //     ACR = std::acosh(ACA)
    MOP_ATANH             = 0x002A,   //     ACR = std::atanh(ACA)
    MOP_ERF               = 0x002B,   //     ACR = std::erf(ACA)
    MOP_ERFC              = 0x002C,   //     ACR = std::erfc(ACA)
    MOP_LGAMMA            = 0x002D,   //     ACR = std::lgamma(ACA)
    MOP_TGAMMA            = 0x002E,   //     ACR = std::tgamma(ACA)
    MOP_CEIL              = 0x002F,   //     ACR = std::ceil(ACA)
    MOP_FLOOR             = 0x0030,   //     ACR = std::floor(ACA)
    MOP_TRUNC             = 0x0031,   //     ACR = std::trunc(ACA)
    MOP_ROUND             = 0x0032,   //     ACR = std::round(ACA)
    MOP_LROUND            = 0x0033,   //     ACR = std::lround(ACA)
    MOP_NEARBYINT         = 0x0034,   //     ACR = std::nearbyint(ACA)
    MOP_ILOGB             = 0x0035,   //     ACR = std::ilogb(ACA)
    MOP_LOGB              = 0x0036,   //     ACR = std::logb(ACA)
    MOP_NEXTAFTER         = 0x0037,   //     ACR = std::nextafter(ACA, ACB)
    MOP_COPYSIGN          = 0x0038,   //     ACR = std::copysign(ACA, ACB)
    MOP_LASTOP            = 0x0039,   //   END Math Operation Enumeration
    MATH_END              = 0xFE85,   // End of Math Co-Processor Register Space
    MATH_TOP              = 0xFE86,   // Top of Math Co-Processor Register Space
// _______________________________________________________________________

    MMU_DEVICE            = 0xFE86,   // START: Memory Management Unit Hardware Registers
    MMU_1_SELECT          = 0xFE86,   // (Byte) Page Select for 8K Memory Bank 1
    MMU_2_SELECT          = 0xFE87,   // (Byte) Page Select for 8K Memory Bank 2 (MMU_META_HANDLE)
    MMU_BLOCKS_FREE       = 0xFE88,   // (Word) Number of 32-Byte Blocks Available for Allocation
    MMU_BLOCKS_ALLOCATED  = 0xFE8A,   // (Word) Number of 32-Byte Blocks Currently Allocated
    MMU_BLOCKS_FRAGGED    = 0xFE8C,   // (Word) Number of 32-Byte Blocks Currently Fragmented
    MMU_ARG_1             = 0xFE8E,   // (Word)  Argument 1 for MMU Command
    MMU_ARG_1_MSB         = 0xFE8E,   // (Byte)  Argument 1 Most Significant Byte for MMU Command
    MMU_ARG_1_LSB         = 0xFE8F,   // (Byte)  Argument 1 Least Significant Byte for MMU Command
    MMU_ARG_2             = 0xFE90,   // (Word)  Argument 2 for MMU Command
    MMU_ARG_2_MSB         = 0xFE90,   // (Byte)  Argument 2 Most Significant Byte for MMU Command
    MMU_ARG_2_LSB         = 0xFE91,   // (Byte)  Argument 2 Least Significant Byte for MMU Command
    MMU_COMMAND           = 0xFE92,   // (Byte) Memory Management Unit Command:
                                      //    $00 = NO OPERATION / ERROR
                                      //    $01 = ALLOCATE PAGE (Always 8192 Bytes)
                                      //    $02 = DEALLOCATE PAGE (Always 8192 Bytes)
                                      //    $03 = ALLOCATE CHAIN (MMU_ARG_1_LSB = # Blocks)
                                      //             MMU_ARG_1_LSB: 0=NONE, 1-255= (32-8160 Bytes)
                                      //    $04 = DEALLOCATE CHAIN (From MMU_META_ROOT)
                                      //    $05 = Load Root Node
                                      //    $06 = Load Next Node
                                      //    $07 = Load Prev Node
                                      //    $08 = Load Last Node
                                      //    $09 = Remove Current Node (and Adjust Links)
                                      //    $0A = Insert Node Before (and activate)
                                      //    $0B = Insert Node After (and activate)
                                      //    $0C = Push Back (and activate)
                                      //    $0D = Push Front (and activate)
                                      //    $0E = Pop Back (and activate)
                                      //    $0F = Pop Front (adjust the handle and activate next)
                                      //    $10 = Lock Node
                                      //    $11 = Unlock Node
                                      //    $12 = Defragment / Collect Garbage
                                      //    ...
                                      //    $FF = RESET; Clear All Allocation Chains
                                      // 
    MMU_ERROR             = 0xFE93,   // (Byte) Memory Management Unit Error Code:
                                      //    $00 = NO ERROR
                                      //    $01 = OUT OF MEMORY
                                      //    $02 = MEMORY ALLOCATION ERROR
                                      //    $03 = MEMORY DEALLOCATION ERROR
                                      //    $04 = MEMORY MAPPING ERROR
                                      //    $05 = MEMORY UNMAPPING ERROR
                                      //    $06 = INVALID COMMAND
                                      //    $07 = INVALID ARGUMENT
                                      //    $08 = INVALID HANDLE
                                      //    $09 = INVALID NODE
                                      //    $0A = INVALID PAGE
                                      //    $0B = INVALID BANK
                                      //    $0C = INVALID ADDRESS
                                      //    $0D = INVALID OFFSET
                                      //    $0E = INVALID LENGTH
                                      //    $0F = INVALID INDEX
                                      //    $FF = UNKNOWN ERROR
                                      // 
    MMU_META_HANDLE       = 0xFE94,   // (Word) Handle for the current allocation chain
    MMU_META_STATUS       = 0xFE96,   // (Byte) Status Flags:
                                      //    bit  0 = Is Allocated:     0 = Free,   1 = Allocated
                                      //    bit  1 = 8k Paged Memory:  0 = No,     1 = Yes
                                      //    bit  2 = Storage Type:     0 = RAM,    1 = ROM
                                      //    bit  3 = Is Fragmented?    0 = No,     1 = Yes
                                      //    bit  4 = Is Locked?        0 = No,     1 = Yes
                                      //    bit  5 = (reserved)
                                      //    bit  6 = (reserved)
                                      //    bit  7 = Error:            0 = No,     1 = Yes
                                      // 
    MMU_META_DATA         = 0xFE97,   // (32-Bytes) Data Window for the Current Allocation
    MMU_META_ROOT         = 0xFEB6,   // (Word) Root node of the current allocation       (Read Only)
    MMU_META_PREV         = 0xFEB8,   // (Word) Previous node of the current allocation   (Read Only)
    MMU_META_NEXT         = 0xFEBA,   // (Word) Next node of the current allocation       (Read Only)
    MMU_RAW_INDEX         = 0xFEBC,   // (Word) Raw Index of the current memory node  (For Debugging)
                                      // 
    MMU_END               = 0xFEBD,   // End of Banked Memory Register Space
    MMU_TOP               = 0xFEBE,   // Top of Banked Memory Register Space
// _______________________________________________________________________

    HDW_RESERVED_DEVICE   = 0xFEBE,   // START: Reserved Register Space
    HDW_REG_END           = 0xFFF0,   // 306 bytes reserved for future use.
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


