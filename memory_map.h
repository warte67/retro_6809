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
    SOFT_RSRVD = 0x0000, // Motorola RESERVED Software Interrupt Vector
    SOFT_SWI3 = 0x0002, // SWI3 Software Interrupt Vector
    SOFT_SWI2 = 0x0004, // SWI2 Software Interrupt Vector
    SOFT_FIRQ = 0x0006, // FIRQ Software Interrupt Vector
    SOFT_IRQ = 0x0008, // IRQ Software Interrupt Vector
    SOFT_SWI = 0x000A, // SWI / SYS Software Interrupt Vector
    SOFT_NMI = 0x000C, // NMI Software Interrupt Vector
    SOFT_RESET = 0x000E, // RESET Software Interrupt Vector

    ZERO_PAGE = 0x0010,
    SYSTEM_STACK = 0x0100,
    SSTACK_TOP = 0x0300, // Top of the system stack space
    USER_STACK = 0x0300,
    USTACK_TOP = 0x0400, // Top of the user stack space

    // Display Buffer
    SCREEN_BUFFER = 0x0400,

    // Device Registers:
    HDW_REGS = 0x1C00, // Begin Device Hardware Registers

    STD_VID_MAX = 0x1C00, //  (Word) Standard Video Buffer Max

    DSP_GRES = 0x1C02, //  (Byte) Screen Resolution Register
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

    DSP_EXT = 0x1C03, //  (Byte) Extended Graphics Register
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

    DSP_ERR = 0x1C04, //  (Byte) Display Sub-System Error Code Register
    // DSP_ERR: ABCD.EFGH
    //      A:0   = Standard Buffer Overflow
    //      B:0   = Extended Buffer Overflow
    //      C:0   = Reserved
    //      D:0   = Reserved
    //      E:0   = Reserved
    //      F:0   = Reserved
    //      G:0   = Reserved
    //      H:0   = Reserved

    DSP_TXT_COLS = 0x1C05, //  (Byte) READ-ONLY Text Screen Columns
    DSP_TXT_ROWS = 0x1C06, //  (Byte) READ-ONLY Text Screens Rows

    DSP_PAL_IDX = 0x1C07, //  (Byte) Color Palette Index
    // DSP_PAL_IDX: 0-255
    // Note: Use this register to set the index into theColor Palette.
    //   Set this value prior referencing color data (DSP_PAL_CLR).

    DSP_PAL_CLR = 0x1C08, //  (Word) Indexed Color Palette Data
    // DSP_PAL_CLR: Color Data A4R4G4B4 format
    // Note: This is the color data for an individual palette entry.
    //     Write to DSP_PAL_IDX with the index within the color palette
    //     prior to reading or writing the color data in the DSP_PAL_CLR register.

    DSP_GLYPH_IDX = 0x1C0A, //  (Byte) Text Glyph Index
    // DSP_GLYPH_IDX: 0-256
    // Note: Set this register to index a specific text glyph. Set this value
    //     prior to updating glyph pixel data.

    DSP_GLYPH_DATA = 0x1C0B, //  (8-Bytes) Text Glyph Pixel Data Array
    // DSP_GLYPH_DATA: 8 rows of binary encoded glyph pixel data
    // Note: Each 8x8 text glyph is composed of 8 bytes. The first byte in this
    //     array represents the top line of 8 pixels. Each array entry represents
    //     a row of 8 pixels.

    // Mouse Cursor Hardware Registers:
    CSR_BEGIN = 0x1C13, //  start of mouse cursor hardware registers
    CSR_XPOS = 0x1C13, //  (Word) horizontal mouse cursor coordinate
    CSR_YPOS = 0x1C15, //  (Word) vertical mouse cursor coordinate
    CSR_XOFS = 0x1C17, //  (Byte) horizontal mouse cursor offset
    CSR_YOFS = 0x1C18, //  (Byte) vertical mouse cursor offset
    CSR_SCROLL = 0x1C19, //  (Signed) MouseWheel Scroll: -1, 0, 1
    CSR_FLAGS = 0x1C1A, //  (Byte) mouse button flags:
    //  CSR_FLAGS:
    //       bits 0-5: button states
    //       bits 6-7: number of clicks
    CSR_BMP_INDX = 0x1C1B, //  (Byte) mouse cursor bitmap pixel offset
    CSR_BMP_DATA = 0x1C1C, //  (Byte) mouse cursor bitmap pixel index color
    CSR_END = 0x1C1E, // End Mouse Registers

    // 5090 ($13E2) bytes remaining for additional registers.
    RESERVED = 0x1C1E,

    // User RAM (32K)
    USER_RAM = 0x3000,

    // Paged RAM (8K)
    PAGED_RAM = 0xB000,

    // PAGED ROM (8K bytes)
    PAGED_ROM = 0xD000,

    // KERNEL ROM (4K bytes)
    KERNEL_ROM = 0xF000,

    // Hardware Interrupt Vectors:
    HARD_RSRVD = 0xFFF0, // Motorola RESERVED Hardware Interrupt Vector
    HARD_SWI3 = 0xFFF2, // SWI3 Hardware Interrupt Vector
    HARD_SWI2 = 0xFFF4, // SWI2 Hardware Interrupt Vector
    HARD_FIRQ = 0xFFF6, // FIRQ Hardware Interrupt Vector
    HARD_IRQ = 0xFFF8, // IRQ Hardware Interrupt Vector
    HARD_SWI = 0xFFFA, // SWI / SYS Hardware Interrupt Vector
    HARD_NMI = 0xFFFC, // NMI Hardware Interrupt Vector
    HARD_RESET = 0xFFFE, // RESET Hardware Interrupt Vector
};  // END: enum MEMMAP


#endif // __MEMORY_MAP_H__
