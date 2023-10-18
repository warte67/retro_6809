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
      HDW_REGS = 0x2980,        // Begin Device Hardware Registers
    
      DSP_GRES = 0x1C00,        //  (Byte) Screen Resolution Register
    // DSP_GRES: BBRR.HHVV
    //     BB:00 = 1-bpp (2-colors)
    //     BB:01 = 2-bpp (4-colors)
    //     BB:10 = 4-bpp (16-colors)
    //     BB:11 = 8-bpp (256-colors)
    //     RR:00 = 16:9  aspect (1.777778)
    //     RR:01 = 16:10 aspect (1.600000)
    //     RR:10 = 16:11 aspect (1.454545)
    //     RR:11 = 16:12 aspect (1.333333)
    //     HH:00 = 1x Horizontal Multiplier
    //     HH:01 = 2x Horizontal Multiplier
    //     HH:10 = 3x Horizontal Multiplier
    //     HH:11 = 4x Horizontal Multiplier
    //     VV:00 = 1x Vertical Multiplier
    //     VV:01 = 2x Vertical Multiplier
    //     VV:10 = 3x Vertical Multiplier
    //     VV:11 = 4x Vertical Multiplier
    
     DSP_GMODE = 0x1C01,        //  (Byte) Graphics Mode Register
    // DSP_GRES: ABCD.EFGG
    //     A:0 = VSYNC OFF
    //     A:1 = VSYNC ON
    //     B:0 = Fullscreen Enabled (emulator only)
    //     B:1 = Windowed Enabled (emulator only)
    //     C:0 = Extended Graphics DISABLED
    //     C:1 = Extended Graphics ENABLED
    //     D:0 = Extended Bitmap Mode Active
    //     D:1 = Extended Tile Graphics Active
    //     E:0 = Standard Text / Bitmap DISABLED
    //     E:1 = Standard Text / Bitmap ENABLED
    //     F:0 = Standard Text Mode ENABLED
    //     F:1 = Standard Bitmap Mode ENABLED
    //     GG:00 = Standard Graphics 1-bpp (2-colors)
    //     GG:01 = Standard Graphics 2-bpp (4-colors)
    //     GG:10 = Standard Graphics 4-bpp (16-colors)
    //     GG:11 = Standard Graphics 8-bpp (256-colors)
    
    // 5118 ($13FE) bytes remaining for additional registers.
      RESERVED = 0x1C02,
    
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


#endif // __MEMORY_MAP_H__
