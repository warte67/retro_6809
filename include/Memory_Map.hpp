

// memory_map.hpp
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
    FIO_BUFFER            = 0x0100,   // START: File Input/Output Buffer
    FIO_BFR_END           = 0x01FF,   // END: File Input/Output Buffer
    SYSTEM_STACK          = 0x0200,   // Bottom of the system stack spcace
    SSTACK_TOP            = 0x0400,   // Top of the system statck space
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
                                      // - bits 3   = Application Screen Mode
                                      //               0: Windowed
                                      //               1: Fullscreen
                                      // - bits 2   = VSync Enable
                                      //               0: Disabled
                                      //               1: Enabled
                                      // - bits 1   = Sprite Enable
                                      //               0: Disabled
                                      //               1: Enabled
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
    GPU_END               = 0xFE19,   // End of GPU Register Space
    GPU_TOP               = 0xFE1A,   // GPU_TOP
// _______________________________________________________________________

    HDW_RESERVED_DEVICE   = 0x0000,   // START: Reserved Register Space
    HDW_REG_END           = 0xFFEF,   // 469 bytes reserved for future use.
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


