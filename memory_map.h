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
    
    // 5120 ($1400) bytes remaining for additional registers.
      RESERVED = 0x1C00,
    
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
