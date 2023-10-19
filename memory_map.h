// memory_map.h
#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__

enum MEMMAP
{
    //  **********************************************
    //  * Allocated 64k Memory Mapped System Symbols *
    //  **********************************************
  SOFT_VECTORS = 0x0000,
    
    // Software Interrupt Vectors:
    SOFT_RSRVD = 0x0000,        // Motorola RESERVED Software Interrupt Vector
     SOFT_SWI3 = 0x0002,        // SWI3 Software Interrupt Vector
     SOFT_SWI2 = 0x0004,        // SWI2 Software Interrupt Vector
     SOFT_FIRQ = 0x0006,        // FIRQ Software Interrupt Vector
      SOFT_IRQ = 0x0008,        // IRQ Software Interrupt Vector
      SOFT_SWI = 0x000A,        // SWI / SYS Software Interrupt Vector
      SOFT_NMI = 0x000C,        // NMI Software Interrupt Vector
    SOFT_RESET = 0x000E,        // RESET Software Interrupt Vector
    
     ZERO_PAGE = 0x0010,
  SYSTEM_STACK = 0x0100,
    SSTACK_TOP = 0x0300,        // Top of the system stack space
    USER_STACK = 0x0300,
    USTACK_TOP = 0x0400,        // Top of the user stack space
    
    // Display Buffer (6K bytes)
  SCREEN_BUFFER = 0x0400,
    
    // Device Registers:
      HDW_REGS = 0x1C00,        // Begin Device Hardware Registers
    
      DSP_GRES = 0x1C00,        //  (Byte) Screen Resolution Register
    // DSP_GRES: BBRR.HHVV
    //     BB:00 = Standard Graphics 1-bpp (2-color mode)
    //     BB:01 = Standard Graphics 2-bpp (4-color mode)
    //     BB:10 = Standard Graphics 4-bpp (16-color mode)
    //     BB:11 = Standard Graphics 8-bpp (256-color mode)
    //     RR:00 = 16:9  aspect (1.777778)
    //     RR:01 = 16:10 aspect (1.600000)
    //     RR:10 = 16:11 aspect (1.454545)
    //     RR:11 = 16:12 aspect (1.333333)
    //     HH:00 = 4x Horizontal Overscan Multiplier
    //     HH:01 = 3x Horizontal Overscan Multiplier
    //     HH:10 = 2x Horizontal Overscan Multiplier
    //     HH:11 = 1x Horizontal Overscan Multiplier
    //     VV:00 = 4x Vertical Overscan Multiplier
    //     VV:01 = 3x Vertical Overscan Multiplier
    //     VV:10 = 2x Vertical Overscan Multiplier
    //     VV:11 = 1x Vertical Overscan Multiplier
    
       DSP_EXT = 0x1C01,        //  (Byte) Extended Graphics Register
    // DSP_EXT: ABCD.EFGG
    //      AA:00 = Extended Graphics 1bpp (2-color mode) 
    //      AA:01 = Extended Graphics 2bpp (4-color mode) 
    //      AA:10 = Extended Graphics 4bpp (16-color mode) 
    //      AA:11 = Extended Graphics 4bpp (16-color mode) 
    //      B:0   = Extended Graphics: DISABLED 
    //      B:1   = Extended Graphics: ENABLED 
    //      C:0   = Extended Extended Mode: BITMAP 
    //      C:1   = Extended Extended Mode: TILES 
    //      D:0   = Standard Graphics: DISABLED 
    //      D:1   = Standard Graphics: ENABLED 
    //      E:0   = Standard Display Mode: TEXT 
    //      E:1   = Standard Display Mode: BITMAP 
    //      F:0   = VSYNC OFF 
    //      F:1   = VSYNC ON 
    //      B:0   = Fullscreen Enabled( emulator only ) 
    //      B:1   = Windowed Enabled ( emulator only ) 
    
       DSP_ERR = 0x1C02,        //  (Byte) Display Sub-System Error Code Register
    // DSP_ERR: ABCD.EFGH
    //      A:0   = Standard Buffer Overflow 
    //      B:0   = Extended Buffer Overflow 
    //      C:0   = Reserved 
    //      D:0   = Reserved 
    //      E:0   = Reserved 
    //      F:0   = Reserved 
    //      G:0   = Reserved 
    //      H:0   = Reserved 
    
    // 5117 ($13FD) bytes remaining for additional registers.
      RESERVED = 0x1C03,
    
    // User RAM (32K)
      USER_RAM = 0x3000,
    
    // Paged RAM (8K)
     PAGED_RAM = 0xB000,
    
    // PAGED ROM (8K bytes)
     PAGED_ROM = 0xD000,
    
    // KERNEL ROM (4K bytes)
    KERNEL_ROM = 0xF000,
    
    // Hardware Interrupt Vectors:
    HARD_RSRVD = 0xFFF0,        // Motorola RESERVED Hardware Interrupt Vector
     HARD_SWI3 = 0xFFF2,        // SWI3 Hardware Interrupt Vector
     HARD_SWI2 = 0xFFF4,        // SWI2 Hardware Interrupt Vector
     HARD_FIRQ = 0xFFF6,        // FIRQ Hardware Interrupt Vector
      HARD_IRQ = 0xFFF8,        // IRQ Hardware Interrupt Vector
      HARD_SWI = 0xFFFA,        // SWI / SYS Hardware Interrupt Vector
      HARD_NMI = 0xFFFC,        // NMI Hardware Interrupt Vector
    HARD_RESET = 0xFFFE,        // RESET Hardware Interrupt Vector
};  // END: enum MEMMAP

#endif //__MEMORY_MAP_H__