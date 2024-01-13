// memory_map.h

#ifndef __MEMMAP_H__

enum MEMMAP
{
    //  **********************************************
    //  * Allocated 64k Memory Mapped System Symbols *
    //  **********************************************
     ZERO_PAGE = 0x0000,
     USER_PAGE = 0x0100,
      USER_TOP = 0x0200,        // Top of user variable space
  SYSTEM_STACK = 0x0200,
     STACK_TOP = 0x0400,        // Top of the system stack space
    
    // Display Buffer (9600 bytes)
  SCREEN_BUFFER = 0x0400,
    
    // Device Registers:
      HDW_REGS = 0x2980,        // Begin Device Hardware Registers
    
  DSP_EMUFLAGS = 0x2980,        //  (Byte) Auxillary Emulation Flags
    // DSP_EMUFLAGS: A000.0000
    // A=0:WINDOWED 1:FULLSCREEN
    
     DSP_GMODE = 0x2981,        //  (Byte) Display Mode Register
    // DSP_GMODE: ABCC.DDEE
    // A = VSYNC     0:off       1:on
    // B = gfx_enable   0:off           1:on
    // C = bit depth 00:1bpp    01:2bpp  10:4bpp 11:8bpp (text00:mono)
    // D = h_scan    00:1x      01:2x    10:4x           (text: lsb only)
    // E = v_scan    00:1x      01:2x    10:4x           (text: lsb only)
    
     DSP_TBASE = 0x2982,        //  (Word) Text Glyph Base Address
    // DSP_TBASE:  ($0400 default)
    // NOTE: While this can be changed to any valid 16-bit address, 
    //     care should be taken to ensure the screen remains within 
    //     the text buffer ($0400-$1800).
    
  DSP_TXT_COLS = 0x2984,        //  (Byte) READ-ONLY Text Screen Columns
  DSP_TXT_ROWS = 0x2985,        //  (Byte) READ-ONLY Text Screens Rows
  DSP_TXT_PITCH = 0x2986,       //  (Word) Text Screen Pitch
    
  DSP_TXT_PXLOFS = 0x2988,      //  (Byte) Text Screen Pixel Offset
    // DSP_TXT_PXLOFS: AAAA.BBBB
    // A = horizontal pixel offset (signed 4-bit)
    // B = vertical pixel offset (signed 4-bit)
    // Smooth scrolling affects only text modes.
    
     DSP_GBASE = 0x2989,        //  (Word) Graphics Base Address
    // DSP_GBASE: Can be used for page swapping ($0000 default)
    // Note: This is the base address to begin displaying pixel
    //     graphics information within the external serial QSPI RAM.
    
  DSP_SCN_WIDTH = 0x298B,       //  (Word) READ-ONLY Screen Pixel Width
  DSP_SCN_HEIGHT = 0x298D,      //  (Word) READ-ONLY Screen Pixel Height
  DSP_SCN_PITCH = 0x298F,       //  (Word) Graphics Screen Pitch
    
     DSP_GADDR = 0x2991,        //  (Word) Pixel Address
    // DSP_GADDR: $0000-$FFFF (16-bit address)
    // Note: This is the 16-bit address register for the graphics modes.
    //     Write to set or read to retrieve the current register value.
    
     DSP_GDATA = 0x2993,        //  (Byte) Pixel Data
    // DSP_GDATA: $00-$FF (8-bit data)
    // Note: Current pixel data stored at where the address register points.
    //     Can be used for both read and write operations.
    
   DSP_PAL_IDX = 0x2994,        //  (Byte) Color Palette Index
    // DSP_PAL_IDX: 0-255
    // Note: Use this register to set the index into theColor Palette. 
    //   Set this value prior referencing color data (DSP_PAL_CLR).
    
   DSP_PAL_CLR = 0x2995,        //  (Word) Indexed Color Palette Data
    // DSP_PAL_CLR: Color Data A4R4G4B4 format
    // Note: This is the color data for an individual palette entry.
    //     Write to DSP_PAL_IDX with the index within the color palette
    //     prior to reading or writing the color data in the DSP_PAL_CLR register.
    
  DSP_GLYPH_IDX = 0x2997,       //  (Byte) Text Glyph Index
    // DSP_GLYPH_IDX: 0-256
    // Note: Set this register to index a specific text glyph. Set this value
    //     prior to updating glyph pixel data.
    
  DSP_GLYPH_DATA = 0x2998,      //  (8-Bytes) Text Glyph Pixel Data Array
    // DSP_GLYPH_DATA: 8 rows of binary encoded glyph pixel data
    // Note: Each 8x8 text glyph is composed of 8 bytes. The first byte in this 
    //     array represents the top line of 8 pixels. Each array entry represents
    //     a row of 8 pixels. 
    
    // Mouse Cursor Hardware Registers:
     CSR_BEGIN = 0x29A0,        //  start of mouse cursor hardware registers
      CSR_XPOS = 0x29A0,        //  (Word) horizontal mouse cursor coordinate
      CSR_YPOS = 0x29A2,        //  (Word) vertical mouse cursor coordinate
      CSR_XOFS = 0x29A4,        //  (Byte) horizontal mouse cursor offset
      CSR_YOFS = 0x29A5,        //  (Byte) vertical mouse cursor offset
      CSR_SIZE = 0x29A6,        //  (Byte) cursor size (0-15) 0:off
    CSR_SCROLL = 0x29A7,        //  (Signed) MouseWheel Scroll: -1, 0, 1
     CSR_FLAGS = 0x29A8,        //  (Byte) mouse button flags:
    //  CSR_FLAGS:
    //       bits 0-5: button states
    //       bits 6-7: number of clicks
  CSR_PAL_INDX = 0x29A9,        //  (Byte) mouse cursor color palette index (0-15)
  CSR_PAL_DATA = 0x29AA,        //  (Word) mouse cursor color palette data RGBA4444
  CSR_BMP_INDX = 0x29AC,        //  (Byte) mouse cursor bitmap pixel offset
  CSR_BMP_DATA = 0x29AD,        //  (Byte) mouse cursor bitmap pixel index color
       CSR_END = 0x29AF,        //  end of mouse cursor hardware registers
    
    // 1616 ($0650) bytes remaining for additional registers.
      RESERVED = 0x29B0,
    
    // User RAM (32K)
      USER_RAM = 0x3000,
    
    // Paged Memory (8K)
  PAGED_MEMORY = 0xE000,
    
    // KERNEL ROM (4K bytes)
    KERNEL_ROM = 0x1000,
    
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



#endif // __MEMMAP_H__