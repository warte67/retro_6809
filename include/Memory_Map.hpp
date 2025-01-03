

// memory_map.hpp
#ifndef __MEMORY_MAP_HPP__
#define __MEMORY_MAP_HPP__

enum MEMMAP
{
    //  **********************************************
    //  * Allocated 64k Memory Mapped System Symbols *
    //  **********************************************

    SOFT_VECTORS_DEVICE   = 0x0000, // START: Software Interrupt Vectors
    SOFT_EXEC             = 0x0000, // Exec Software Interrupt Vector
    SOFT_SWI3             = 0x0002, // SWI3 Software Interrupt Vector
    SOFT_SWI2             = 0x0004, // SWI2 Software Interrupt Vector
    SOFT_FIRQ             = 0x0006, // FIRQ Software Interrupt Vector
    SOFT_IRQ              = 0x0008, // IRQ Software Interrupt Vector
    SOFT_SWI              = 0x000A, // SWI / SYS Software Interrupt Vector
    SOFT_NMI              = 0x000C, // NMI Software Interrupt Vector
    SOFT_RESET            = 0x000E, // RESET Software Interrupt Vector

    SYSTEM_MEMORY_DEVICE  = 0x0010, // START: System Memory
    ZERO_PAGE             = 0x0010, // Zero Page System and User Variables
    FIO_BUFFER            = 0x0100, // START: File Input/Output Buffer
    FIO_BFR_END           = 0x01FF, // END: File Input/Output Buffer
    SYSTEM_STACK          = 0x0200, // Bottom of the system stack spcace
    SSTACK_TOP            = 0x0400, // Top of the system statck space

    VIDEO_BUFFER_DEVICE   = 0x0400, // START: Video Buffer (8K)
    VIDEO_START           = 0x0400, // Start of standard video buffer
    VIDEO_END             = 0x23FF, // End of standard video buffer
    VIDEO_TOP             = 0x2400, // Top of standard video buffer

    USER_MEMORY_DEVICE    = 0x2400, // START: User Memory (34K)
    USER_RAM              = 0x2400, // User Accessable RAM
    USER_RAM_END          = 0xAFFF, // End User Accessable RAM
    USER_RAM_TOP          = 0xB000, // Top User Accessable RAM

    MEMBANK_DEVICE        = 0xB000, // START: Banked Memory Region (16K)
    MEMBANK_ONE           = 0xB000, // Banked Memory Page One (8K)
    MEMBANK_TWO           = 0xD000, // Banked Memory Page Two (8K)
    MEMBANK_END           = 0xEFFF, // End of Banked Memory Region
    MEMBANK_TOP           = 0xF000, // TOP of Banked Memory Region

    KERNEL_ROM_DEVICE     = 0xF000, // START: Kernel Rom (3.5K)
    KERNEL_START          = 0xF000, // Start of Kernel Rom Space
    KERNEL_END            = 0xFDFF, // End of Kernel Rom Space
    KERNEL_TOP            = 0xFE00, // Top of Kernel Rom Space

    GPU_DEVICE            = 0xFE00, // START: GPU Device Hardware Registers
    GPU_OPTIONS           = 0xFE00, // (Byte) Bitflag Enables
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
                                    //    - bits 2   = Debug Enable
                                    //                 0: Disabled
                                    //                 1: Enabled
                                    //    - bits 1   = Sprite Enable
                                    //                 0: Disabled
                                    //                 1: Enabled
                                    //    - bit  0   = Standard Display Enable
                                    //                 0: Disabled
                                    //                 1: Enabled
                                    // 
    GPU_MODE              = 0xFE01, // (Byte) Bitflag Enables
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
    GPU_ENABLE            = 0xFE02, // (Byte) Bitflag Enables
                                    //    - bits 5-7 = (reserved)
                                    //    - bits 3   = 0: Disable Standard Mode,
                                    //                 1: Enable Standard Mode
                                    //    - bit  2   = 0: Disable Extended Mode,
                                    //                 1: Enable Extended Mode
                                    //    - bit  1   = 0: Disable Sprites,
                                    //                 1: Enable Sprites
                                    //    - bit  0   = 0: Disable Mouse Cursor,
                                    //                 1: Enable Mouse Cursor
                                    // 
    GPU_STD_MODE          = 0xFE03, // (Byte) Standard Graphics Mode
                                    //    - bit  7   = 0: screen is text, 
                                    //                 1: screen is bitmap
                                    //    - bits 5-6 = bitmap color depth:
                                    //                 00: 2 colors,
                                    //                 01: 4 colors,
                                    //                 10: 16 colors, 
                                    //                 11: 256 colors
                                    //    - bits 3-4 = horizontal overscan: 
                                    //                 00:H/1 (512 or 640)
                                    //                 01:H/2 (256 or 320)
                                    //                 10:H/3 (170 or 213)
                                    //                 11:H/4 (128 or 160)
                                    //    - bits 1-2 = vertical overscan: 
                                    //                 00:V/1 (320 or 400)
                                    //                 01:V/2 (160 or 200)
                                    //                 10:V/3 (106 or 133)
                                    //                 11:V/4 (80 or 100)
                                    //    - bit  0   = Video Timing:
                                    //                 0: H:512 x V:320
                                    //                 1: H:640 x V:400
                                    //                 (Overrides EXT_MODE)
                                    // 
    GPU_EXT_MODE          = 0xFE04, // (Byte) Extended Graphics Mode
                                    //    - bit  7   = 0: screen is tiled,
                                    //                 1: screen is bitmap
                                    //    - bits 5-6 = bitmap color depth:
                                    //                 00: 2 colors,
                                    //                 01: 4 colors,
                                    //                 10: 16 colors, 
                                    //                 11: 256 colors
                                    //    - bits 3-4 = horizontal overscan: 
                                    //                 00:H/1 (512 or 640)
                                    //                 01:H/2 (256 or 320)
                                    //                 10:H/3 (170 or 213)
                                    //                 11:H/4 (128 or 160)
                                    //    - bits 1-2 = vertical overscan: 
                                    //                 00:V/1 (320 or 400)
                                    //                 01:V/2 (160 or 200)
                                    //                 10:V/3 (106 or 133)
                                    //                 11:V/4 (80 or 100)
                                    //    - bit  0   = Video Timing:
                                    //                 0: H:512 x V:320
                                    //                 1: H:640 x V:400
                                    //                 (Overrides STD_MODE)
                                    // 
    GPU_EMULATION         = 0xFE05, // (Byte) Emulation Flags
                                    //    - bit  7    = vsync: 0=off, 1=on
                                    //    - bit  6    = main: 0=windowed,
                                    //                  1=fullscreen
                                    //    - bit  5    = debug: 0=windowed, 
                                    //                  1=fullscreen
                                    //    - bit  4    = debug: 0=off, 1=on
                                    //    - bits 2-3  = Active Monitor 0-3
                                    //    - bits 0-1  = Debug Monitor 0-3
                                    // 

    HDW_RESERVED_DEVICE   = 0xFE06, // START: Reserved Register Space
    HDW_REG_END           = 0xFFEF, // 489 bytes reserved for future use.

    ROM_VECTS_DEVICE      = 0xFFF0, // START: Hardware Interrupt Vectors
    HARD_EXEC             = 0xFFF0, // EXEC Hardware Interrupt Vector
    HARD_SWI3             = 0xFFF2, // SWI3 Hardware Interrupt Vector
    HARD_SWI2             = 0xFFF4, // SWI2 Hardware Interrupt Vector
    HARD_FIRQ             = 0xFFF6, // FIRQ Hardware Interrupt Vector
    HARD_IRQ              = 0xFFF8, // IRQ Hardware Interrupt Vector
    HARD_SWI              = 0xFFFA, // SWI / SYS Hardware Interrupt Vector
    HARD_NMI              = 0xFFFC, // NMI Hardware Interrupt Vector
    HARD_RESET            = 0xFFFE, // RESET Hardware Interrupt Vector
    MEMMAP_END
}; // END: enum MEMMAP


#endif // __MEMORY_MAP_H__


