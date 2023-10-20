;  **********************************************
;  * Allocated 64k Memory Mapped System Symbols *
;  **********************************************
SOFT_VECTORS = $0000  
        
          ; Software Interrupt Vectors:
SOFT_RSRVD   = $0000    ; Motorola RESERVED Software Interrupt Vector
SOFT_SWI3    = $0002    ; SWI3 Software Interrupt Vector
SOFT_SWI2    = $0004    ; SWI2 Software Interrupt Vector
SOFT_FIRQ    = $0006    ; FIRQ Software Interrupt Vector
SOFT_IRQ     = $0008    ; IRQ Software Interrupt Vector
SOFT_SWI     = $000A    ; SWI / SYS Software Interrupt Vector
SOFT_NMI     = $000C    ; NMI Software Interrupt Vector
SOFT_RESET   = $000E    ; RESET Software Interrupt Vector
        
ZERO_PAGE    = $0010  
SYSTEM_STACK = $0100  
SSTACK_TOP   = $0300    ; Top of the system stack space
USER_STACK   = $0300  
USTACK_TOP   = $0400    ; Top of the user stack space
        
          ; Display Buffer
SCREEN_BUFFER = $0400  
        
          ; Device Registers:
HDW_REGS     = $1C00    ; Begin Device Hardware Registers
        
STD_VID_MAX  = $1C00    ;  (Word) Standard Video Buffer Max
        
DSP_GRES     = $1C02    ;  (Byte) Screen Resolution Register
          ; DSP_GRES: BBRR.HHVV
          ;     BB:00 = Standard Graphics 1-bpp (2-color mode)
          ;     BB:01 = Standard Graphics 2-bpp (4-color mode)
          ;     BB:10 = Standard Graphics 4-bpp (16-color mode)
          ;     BB:11 = Standard Graphics 8-bpp (256-color mode)
          ;     RR:00 = 16:9  aspect (1.777778)
          ;     RR:01 = 16:10 aspect (1.600000)
          ;     RR:10 = 16:11 aspect (1.454545)
          ;     RR:11 = 16:12 aspect (1.333333)
          ;     HH:00 = 4x Horizontal Overscan Multiplier
          ;     HH:01 = 3x Horizontal Overscan Multiplier
          ;     HH:10 = 2x Horizontal Overscan Multiplier
          ;     HH:11 = 1x Horizontal Overscan Multiplier
          ;     VV:00 = 4x Vertical Overscan Multiplier
          ;     VV:01 = 3x Vertical Overscan Multiplier
          ;     VV:10 = 2x Vertical Overscan Multiplier
          ;     VV:11 = 1x Vertical Overscan Multiplier
        
DSP_EXT      = $1C03    ;  (Byte) Extended Graphics Register
          ; DSP_EXT: ABCD.EFGG
          ;      AA:00 = Extended Graphics 1bpp (2-color mode) 
          ;      AA:01 = Extended Graphics 2bpp (4-color mode) 
          ;      AA:10 = Extended Graphics 4bpp (16-color mode) 
          ;      AA:11 = Extended Graphics 4bpp (16-color mode) 
          ;      B:0   = Extended Graphics: DISABLED 
          ;      B:1   = Extended Graphics: ENABLED 
          ;      C:0   = Extended Extended Mode: BITMAP 
          ;      C:1   = Extended Extended Mode: TILES 
          ;      D:0   = Standard Graphics: DISABLED 
          ;      D:1   = Standard Graphics: ENABLED 
          ;      E:0   = Standard Display Mode: TEXT 
          ;      E:1   = Standard Display Mode: BITMAP 
          ;      F:0   = VSYNC OFF 
          ;      F:1   = VSYNC ON 
          ;      B:0   = Fullscreen Enabled( emulator only ) 
          ;      B:1   = Windowed Enabled ( emulator only ) 
        
DSP_ERR      = $1C04    ;  (Byte) Display Sub-System Error Code Register
          ; DSP_ERR: ABCD.EFGH
          ;      A:0   = Standard Buffer Overflow 
          ;      B:0   = Extended Buffer Overflow 
          ;      C:0   = Reserved 
          ;      D:0   = Reserved 
          ;      E:0   = Reserved 
          ;      F:0   = Reserved 
          ;      G:0   = Reserved 
          ;      H:0   = Reserved 
        
DSP_TXT_COLS = $1C05    ;  (Byte) READ-ONLY Text Screen Columns
DSP_TXT_ROWS = $1C06    ;  (Byte) READ-ONLY Text Screens Rows
        
DSP_PAL_IDX  = $1C07    ;  (Byte) Color Palette Index
          ; DSP_PAL_IDX: 0-255
          ; Note: Use this register to set the index into theColor Palette. 
          ;   Set this value prior referencing color data (DSP_PAL_CLR).
        
DSP_PAL_CLR  = $1C08    ;  (Word) Indexed Color Palette Data
          ; DSP_PAL_CLR: Color Data A4R4G4B4 format
          ; Note: This is the color data for an individual palette entry.
          ;     Write to DSP_PAL_IDX with the index within the color palette
          ;     prior to reading or writing the color data in the DSP_PAL_CLR register.
        
DSP_GLYPH_IDX = $1C0A    ;  (Byte) Text Glyph Index
          ; DSP_GLYPH_IDX: 0-256
          ; Note: Set this register to index a specific text glyph. Set this value
          ;     prior to updating glyph pixel data.
        
DSP_GLYPH_DATA = $1C0B    ;  (8-Bytes) Text Glyph Pixel Data Array
          ; DSP_GLYPH_DATA: 8 rows of binary encoded glyph pixel data
          ; Note: Each 8x8 text glyph is composed of 8 bytes. The first byte in this 
          ;     array represents the top line of 8 pixels. Each array entry represents
          ;     a row of 8 pixels. 
        
    ; 5101 ($13ED) bytes remaining for additional registers.
RESERVED     = $1C13  
        
          ; User RAM (32K)
USER_RAM     = $3000  
        
          ; Paged RAM (8K)
PAGED_RAM    = $B000  
        
          ; PAGED ROM (8K bytes)
PAGED_ROM    = $D000  
        
          ; KERNEL ROM (4K bytes)
KERNEL_ROM   = $F000  
        
          ; Hardware Interrupt Vectors:
HARD_RSRVD   = $FFF0    ; Motorola RESERVED Hardware Interrupt Vector
HARD_SWI3    = $FFF2    ; SWI3 Hardware Interrupt Vector
HARD_SWI2    = $FFF4    ; SWI2 Hardware Interrupt Vector
HARD_FIRQ    = $FFF6    ; FIRQ Hardware Interrupt Vector
HARD_IRQ     = $FFF8    ; IRQ Hardware Interrupt Vector
HARD_SWI     = $FFFA    ; SWI / SYS Hardware Interrupt Vector
HARD_NMI     = $FFFC    ; NMI Hardware Interrupt Vector
HARD_RESET   = $FFFE    ; RESET Hardware Interrupt Vector
; END of definitions
