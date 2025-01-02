

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
    GPU_ENABLE            = 0xFE00, // (Byte) Bitflag Enables
                                    //    - bits 5-7 = reserved
                                    //    - bit  4   = 0:disable ext display,
                                    //                 1:enable ext display
                                    //    - bit  3   = 0:disable std display,
                                    //                 1:enable std display
                                    //    - bit  2   = 0:disable sprites,
                                    //                 1:enable sprites
                                    //    - bit  1   = 0:disable tilemap,
                                    //                 1:enable tilemap
                                    //    - bit  0   = 0:disable mouse cursor,
                                    //                 1:enable mouse cursor
                                    // 
    GPU_STD_MODE          = 0xFE01, // (Byte) Standard Graphics Mode
                                    //    - bit  7   = 0:screen is text, 
                                    //                 1:screen is bitmap
                                    //    - bit  6   = video timing: 
                                    //                 0=H:512xV:320, 1=H:640xV:400
                                    //    - bits 4-5 = horizontal overscan: 
                                    //                 00=H/8, 01=H/4, 10=H/2, 11=H/1
                                    //    - bits 2-3 = vertical overscan: 
                                    //                 00=V/8, 01=V/4, 10=V/2, 11=V/1
                                    //    - bits 0-1 = bitmap mode:
                                    //                 00=2-colors,
                                    //                 01=4-colors,
                                    //                 10=16-colors, 
                                    //                 11=256-clr
                                    //          or  
                                    //    - bits 0-1 = text mode:
                                    //                 00=monochrome text
                                    //                 01=bg/fg + text
                                    //                 10=clear-bgnd 256-color text
                                    //                 11=color_0-bgnd 256-color text
                                    // 
    GPU_EXT_MODE          = 0xFE02, // (Byte) Extended Graphics Mode
                                    //    - bit  7   = 0:screen is tiled,
                                    //                 1:screen is bitmap
                                    //    - bit  6   = video timing: 
                                    //                 0=H:512xV:320, 1=H:640xV:400
                                    //    - bits 4-5 = horizontal overscan: 
                                    //                 00=H/8, 01=H/4, 10=H/2, 11=H/1
                                    //    - bits 2-3 = vertical overscan: 
                                    //                 00=V/8, 01=V/4, 10=V/2, 11=V/1
                                    //    - bits 0-1 = Color Mode: 00=2-clr, 
                                    //                 01=4-clr, 10=16-clr, 
                                    //                 11=256-clr
                                    // 
    GPU_EMULATION         = 0xFE03, // (Byte) Emulation Flags
                                    //    - bit  7    = vsync: 0=off, 1=on
                                    //    - bit  6    = main: 0=windowed,
                                    //                  1=fullscreen
                                    //    - bit  5    = debug: 0=off, 1=on
                                    //    - bit  4    = debug: 0=windowed, 
                                    //                  1=fullscreen
                                    //    - bits 2-3  = Active Monitor 0-3
                                    //    - bits 0-1  = Debug Monitor 0-3
                                    // 

    HDW_RESERVED_DEVICE   = 0xFE04, // START: Reserved Register Space
    HDW_REG_END           = 0xFFEF, // 491 bytes reserved for future use.

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

