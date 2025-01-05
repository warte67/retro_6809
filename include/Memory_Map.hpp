

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

    SYSTEM_MEMORY_DEVICE  = 0x0010,   // START: System Memory
    ZERO_PAGE             = 0x0010,   // Zero Page System and User Variables
    FIO_BUFFER            = 0x0100,   // START: File Input/Output Buffer
    FIO_BFR_END           = 0x01FF,   // END: File Input/Output Buffer
    SYSTEM_STACK          = 0x0200,   // Bottom of the system stack spcace
    SSTACK_TOP            = 0x0400,   // Top of the system statck space
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

    MEMBANK_DEVICE        = 0xB000,   // START: Banked Memory Region (16K)
    MEMBANK_ONE           = 0xB000,   // Banked Memory Page One (8K)
    MEMBANK_TWO           = 0xD000,   // Banked Memory Page Two (8K)
    MEMBANK_END           = 0xEFFF,   // End of Banked Memory Region
    MEMBANK_TOP           = 0xF000,   // TOP of Banked Memory Region
// _______________________________________________________________________

    KERNEL_ROM_DEVICE     = 0xF000,   // START: Kernel Rom (3.5K)
    KERNEL_START          = 0xF000,   // Start of Kernel Rom Space
    KERNEL_END            = 0xFDFF,   // End of Kernel Rom Space
    KERNEL_TOP            = 0xFE00,   // Top of Kernel Rom Space
// _______________________________________________________________________

    TESTS_DEVICE          = 0xFE00,   // START: TESTS_DEVICE
    TESTS_START           = 0xFE00,   // Start of Test Registers Space
    TESTS_ONE             = 0xFE00,   // (Byte) First Test Register
                                      // - bit 7    = TEST_ENABLE
                                      // - bit 6    = (reserved)
                                      // - bit 5    = (reserved)
                                      // - bit 4    = (reserved)
                                      // - bit 3    = (reserved)
                                      // - bit 2    = (reserved)
                                      // - bit 1    = (reserved)
                                      // - bit 0    = TEST_INC_VID_MODES
                                      // 
    TESTS_TWO             = 0xFE01,   // (Byte) Second Test Register
                                      // - bits 0-7  = (reserved)
                                      // 
    TESTS_END             = 0xFE02,   // End of Tests Registers Space
    TESTS_TOP             = 0xFE03,   // Top of Tests Registers Space
// _______________________________________________________________________

    GPU_DEVICE            = 0xFE03,   // START: GPU Device Hardware Registers
    GPU_OPTIONS           = 0xFE03,   // (Byte) Bitflag Enables
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
    GPU_MODE              = 0xFE04,   // (Byte) Bitflag Enables
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
    GPU_END               = 0xFE04,   // End of GPU Register Space
    GPU_TOP               = 0xFE05,   // Top of GPU Register Space
// _______________________________________________________________________

    DEBUG_DEVICE          = 0xFE05,   // START: Debug Hardware Registers:
    DBG_BRK_ADDR          = 0xFE05,   // (Word) Address of current breakpoint
    DBG_FLAGS             = 0xFE07,   // (Byte) Debug Specific Hardware Flags:
                                      // - bit 7: Debug Enable
                                      // - bit 6: Single Step Enable
                                      // - bit 5: Clear All Breakpoints
                                      // - bit 4: Update Breakpoint at DEBUG_BRK_ADDR
                                      // - bit 3: FIRQ  (on low {0} to high {1} edge)
                                      // - bit 2: IRQ   (on low {0} to high {1} edge)
                                      // - bit 1: NMI   (on low {0} to high {1} edge)
                                      // - bit 0: RESET (on low {0} to high {1} edge)
                                      // 
    DBG_END               = 0xFE07,   // End of Debug Registers
    DBG_TOP               = 0xFE08,   // Top of Debug Registers
// _______________________________________________________________________

    HDW_RESERVED_DEVICE   = 0xFE08,   // START: Reserved Register Space
    HDW_REG_END           = 0xFFEF,   // 487 bytes reserved for future use.
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


