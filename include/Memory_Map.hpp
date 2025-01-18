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

    SYSTEM_MEMORY_DEVICE  = 0x0000,   // START: System Memory
    ZERO_PAGE             = 0x0010,   // Zero Page System and User Variables
    ZERO_PAGE_END         = 0x00FF,   // Zero Page System and User Variables
    FIO_FILE_BUFFER       = 0x0100,   // START: File Input/Output Buffer
    FIO_FILE_BFR_END      = 0x017F,   // END: File Input/Output Buffers
    FIO_LN_EDT_BUFFER     = 0x0180,   // START: Line Edit Character Buffer
    FIO_LN_EDT_END        = 0x01FF,   // END: Line Edit Character Buffer
    SYSTEM_STACK          = 0x0200,   // Bottom of System Stack Spcace
    SSTACK_END            = 0x03FF,   // END: System Stack Space
    SSTACK_TOP            = 0x0400,   // TOP: System Stack Space
// _______________________________________________________________________

    VIDEO_BUFFER_DEVICE   = 0x0000,   // START: Video Buffer (8K)
    VIDEO_START           = 0x0400,   // Start of standard video buffer
    VIDEO_END             = 0x23FF,   // End of standard video buffer
    VIDEO_TOP             = 0x2400,   // Top of standard video buffer
// _______________________________________________________________________

    USER_MEMORY_DEVICE    = 0x0000,   // START: User Memory (34K)
    USER_RAM              = 0x2400,   // User Accessable RAM
    USER_RAM_END          = 0xAFFF,   // End User Accessable RAM
    USER_RAM_TOP          = 0xB000,   // Top User Accessable RAM
// _______________________________________________________________________

    MEMBANK_DEVICE        = 0x0000,   // START: Banked Memory Region (16K)
    MEMBANK_ONE           = 0xB000,   // Banked Memory Page One (8K)
    MEMBANK_TWO           = 0xD000,   // Banked Memory Page Two (8K)
    MEMBANK_END           = 0xEFFF,   // End of Banked Memory Region
    MEMBANK_TOP           = 0xF000,   // TOP of Banked Memory Region
// _______________________________________________________________________

    KERNEL_ROM_DEVICE     = 0x0000,   // START: Kernel Rom (3.5K)
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
    GPU_OPTIONS           = 0xFE0B,   // (Byte) Bitflag Enables
                                      // - bit 7    = Extended Bitmap:
                                      //               0: Tilemap Display
                                      //               1: Bitmap Display
                                      // - bits 5-6 = Extended Color Mode:
                                      //               00: 2-Colors
                                      //               01: 4-Colors
                                      //               10: 16-Colors
                                      //               11: 256-Colors
                                      // - bits 4   = Extended Display Enable
                                      //               0: Disabled
                                      //               1: Enabled
                                      // - bits 3   = Emulation Screen Mode
                                      //               (temporary)
                                      //               0: Windowed
                                      //               1: Fullscreen
                                      // - bits 2   = VSync Enable
                                      //               0: Disabled
                                      //               1: Enabled
                                      // - bit  1   = Presentation
                                      //               0: Overscan / Stretch
                                      //               1: Letterbox
                                      // - bit  0   = Standard Display Enable
                                      //               0: Disabled
                                      //               1: Enabled
                                      // 
    GPU_MODE              = 0xFE0C,   // (Byte) Standard Display Mode
                                      // - bit 7    = Standard Bitmap:
                                      //               0: Text Display
                                      //               1: Bitmap Display
                                      // - bits 5-6 = Standard Color Mode:
                                      //               00: 2-Colors
                                      //               01: 4-Colors
                                      //               10: 16-Colors
                                      //               11: 256-Colors
                                      // - bits 0-4 = Display Mode (0-31)
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
    GPU_TROWS             = 0xFE15,   // (Byte) Text Vertical Rows (Read Only)
                                      //   Note: This will reflect the number of
                                      //        glyph rows for text modes.
                                      // 
    GPU_PAL_INDEX         = 0xFE17,   // (Byte) Color Palette Index
                                      //   Note: Use this register to set the
                                      //        index into the Color Palette.
                                      //        Set this value prior referencing
                                      //        the color data (GPU_PAL_COLOR).
                                      // 
    GPU_PAL_COLOR         = 0xFE18,   // (Word) Color Palette Data (A4R4G4B4 format)
                                      //   Note: This is the color data for an
                                      //        individual palette entry. Write to 
                                      //        DSP_PAL_IDX with the index within the
                                      //        color palette prior to reading or
                                      //        writing the color data in the
                                      //        GFX_PAL_CLR register.
                                      // 
    GPU_GLYPH_IDX         = 0xFE1A,   // (Byte) Text Glyph Index
                                      //   Note: Use this register to set the
                                      //        index of a specific text glyph.
                                      //        Set this value prior to updating
                                      //        the glyph data (GPU_GLYPH_DATA).
                                      // 
    GPU_GLYPH_DATA        = 0xFE1B,   // (8-Bytes) 8 rows of binary encoded glyph pixel data
                                      //   Note: This is the pixel data for a
                                      //        specific text glyph. Each 8x8
                                      //        text glyph is composed of 8 bytes.
                                      //        The first byte in this array
                                      //        represents the top line of 8 pixels.
                                      //        Each array entry represents a row of 8 pixels.
                                      // 
    GPU_END               = 0xFE22,   // End of GPU Register Space
    GPU_TOP               = 0xFE23,   // Top of GPU Register Space
// _______________________________________________________________________

    CSR_DEVICE            = 0xFE23,   // START: Mouse Device Hardware Registers
    CSR_XPOS              = 0xFE23,   // (Word) Horizontal Mouse Cursor Coordinate
    CSR_YPOS              = 0xFE25,   // (Word) Vertical Mouse Cursor Coordinate
    CSR_XOFS              = 0xFE27,   // (Byte) Horizontal Mouse Cursor Offset
    CSR_YOFS              = 0xFE28,   // (Byte) Vertical Mouse Cursor Offset
    CSR_SCROLL            = 0xFE29,   // (char) MouseWheel Scroll: -1, 0, 1
    CSR_FLAGS             = 0xFE2A,   // (Byte) Mouse Device State Flags
                                      //    bits 0-4: button states
                                      //    bits 5-6: number of clicks
                                      //    bits 7:   cursor enable
                                      // 
    CSR_BMP_INDX          = 0xFE2B,   // (Byte) Mouse Cursor Bitmap Pixel Offset
    CSR_BMP_DATA          = 0xFE2C,   // (Byte) Mouse Cursor Bitmap Pixel Color Data ($0-$F)
    CSR_PAL_INDX          = 0xFE2D,   // (Byte) Mouse Cursor Color Palette Index (0-15)
    CSR_PAL_DATA          = 0xFE2E,   // (Word) Mouse Cursor Color Palette Data A4R4G4B4
    CSR_END               = 0xFE2F,   // End of Mouse Device Register Space
    CSR_TOP               = 0xFE30,   // Top of CSR Register Space
// _______________________________________________________________________

    KEYBOARD_DEVICE       = 0xFE30,   // START: Keyboard Device Hardware Registers
    CHAR_Q_LEN            = 0xFE30,   // (Byte) Number of Characters Waiting in Queue   (Read Only)
    CHAR_SCAN             = 0xFE31,   // (Byte) Read Next Character in Queue (Not Popped When Read)
    CHAR_POP              = 0xFE32,   // (Byte) Read Next Character in Queue     (Popped When Read)
    XKEY_BUFFER           = 0xFE33,   // (16 Bytes) 128 bits for XK_KEY data buffer     (Read Only)
    EDT_BFR_CSR           = 0xFE43,   // (Byte) Cursor Position Within Edit Buffer     (Read/Write)
    EDT_ENABLE            = 0xFE44,   // (Byte) Line Editor Enable Flag                (Read/Write)
    EDT_BFR_LEN           = 0xFE45,   // (Byte) Limit the Line Editor to This Length   (Read/Write)
    KEYBOARD_END          = 0xFE45,   // End of Keyboard Register Space
    KEYBOARD_TOP          = 0xFE46,   // Top of Keyboard Register Space
// _______________________________________________________________________

    JOYSTICK_DEVICE       = 0xFE46,   // START: Joystick/Gamepad Controller Device Hardware Registers
    JOYS_1_BTN            = 0xFE46,   // (Word) Button Bits: Room For up to 16 Buttons  (realtime)
                                      //        Gamepad Controller Button Bits:
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
    JOYS_1_DBND           = 0xFE48,   // (Byte) PAD 1 analog deadband; default is 5   (read/write)
    JOYS_1_LTX            = 0xFE49,   // (char) PAD 1 LThumb-X position (-128 _ +127)   (realtime)
    JOYS_1_LTY            = 0xFE4A,   // (char) PAD 1 LThumb-Y position (-128 _ +127)   (realtime)
    JOYS_1_RTX            = 0xFE4B,   // (char) PAD 1 RThumb-X position (-128 _ +127)   (realtime)
    JOYS_1_RTY            = 0xFE4C,   // (char) PAD 1 RThumb-Y position (-128 _ +127)   (realtime)
    JOYS_1_Z1             = 0xFE4D,   // (char) PAD 1 left analog trigger (0 - 127)     (realtime)
    JOYS_1_Z2             = 0xFE4E,   // (char) PAD 1 left analog trigger (0 - 127)     (realtime)
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

    HDW_RESERVED_DEVICE   = 0x0000,   // START: Reserved Register Space
    HDW_REG_END           = 0xFFEF,   // 407 bytes reserved for future use.
// _______________________________________________________________________

    ROM_VECTS_DEVICE      = 0x0000,   // START: Hardware Interrupt Vectors
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


