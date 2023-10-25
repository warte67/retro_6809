;  **********************************************
;  * Allocated 64k Memory Mapped System Symbols *
;  **********************************************
SOFT_VECTORS    equ   $0000

          ; Software Interrupt Vectors:
SOFT_RSRVD      equ   $0000    ; Motorola RESERVED Software Interrupt Vector
SOFT_SWI3       equ   $0002    ; SWI3 Software Interrupt Vector
SOFT_SWI2       equ   $0004    ; SWI2 Software Interrupt Vector
SOFT_FIRQ       equ   $0006    ; FIRQ Software Interrupt Vector
SOFT_IRQ        equ   $0008    ; IRQ Software Interrupt Vector
SOFT_SWI        equ   $000A    ; SWI / SYS Software Interrupt Vector
SOFT_NMI        equ   $000C    ; NMI Software Interrupt Vector
SOFT_RESET      equ   $000E    ; RESET Software Interrupt Vector

ZERO_PAGE       equ   $0010
SYSTEM_STACK    equ   $0100
SSTACK_TOP      equ   $0300    ; Top of the system stack space
USER_STACK      equ   $0300
USTACK_TOP      equ   $0400    ; Top of the user stack space

          ; Display Buffer
SCREEN_BUFFER    equ   $0400

          ; Device Registers:
HDW_REGS        equ   $1C00    ; Begin Device Hardware Registers

STD_VID_MAX     equ   $1C00    ;  (Word) Standard Video Buffer Max

SYS_STATE       equ   $1C02    ;  (Byte) System State Register
          ; DSP_GRES: ABCD.SSSS
          ;      A:0   = Error: Standard Buffer Overflow
          ;      B:0   = Error: Extended Buffer Overflow
          ;      C:0   = Error: Reserved
          ;      D:0   = Error: Reserved
          ;      S:$0  = CPU Clock  25 khz.
          ;      S:$1  = CPU Clock  50 khz.
          ;      S:$2  = CPU Clock 100 khz.
          ;      S:$3  = CPU Clock 200 khz.
          ;      S:$4  = CPU Clock 333 khz.
          ;      S:$5  = CPU Clock 416 khz.
          ;      S:$6  = CPU Clock 500 khz.
          ;      S:$7  = CPU Clock 625 khz.
          ;      S:$8  = CPU Clock 769 khz.
          ;      S:$9  = CPU Clock 833 khz.
          ;      S:$A  = CPU Clock 1.0 mhz.
          ;      S:$B  = CPU Clock 1.4 mhz.
          ;      S:$C  = CPU Clock 2.0 mhz.
          ;      S:$D  = CPU Clock 3.3 mhz.
          ;      S:$E  = CPU Clock 5.0 mhz.
          ;      S:$F  = CPU Clock ~10.0 mhz. (unmetered)

SYS_SPEED       equ   $1C03    ;  (Word) Approx. Average CPU Clock Speed

DSP_GRES        equ   $1C05    ;  (Byte) Screen Resolution Register
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

DSP_EXT         equ   $1C06    ;  (Byte) Extended Graphics Register
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

DSP_TXT_COLS    equ   $1C07    ;  (Byte) READ-ONLY Text Screen Columns
DSP_TXT_ROWS    equ   $1C08    ;  (Byte) READ-ONLY Text Screens Rows

DSP_PAL_IDX     equ   $1C09    ;  (Byte) Color Palette Index
          ; DSP_PAL_IDX: 0-255
          ; Note: Use this register to set the index into theColor Palette.
          ;   Set this value prior referencing color data (DSP_PAL_CLR).

DSP_PAL_CLR     equ   $1C0A    ;  (Word) Indexed Color Palette Data
          ; DSP_PAL_CLR: Color Data A4R4G4B4 format
          ; Note: This is the color data for an individual palette entry.
          ;     Write to DSP_PAL_IDX with the index within the color palette
          ;     prior to reading or writing the color data in the DSP_PAL_CLR register.

DSP_GLYPH_IDX    equ   $1C0C    ;  (Byte) Text Glyph Index
          ; DSP_GLYPH_IDX: 0-256
          ; Note: Set this register to index a specific text glyph. Set this value
          ;     prior to updating glyph pixel data.

DSP_GLYPH_DATA    equ   $1C0D    ;  (8-Bytes) Text Glyph Pixel Data Array
          ; DSP_GLYPH_DATA: 8 rows of binary encoded glyph pixel data
          ; Note: Each 8x8 text glyph is composed of 8 bytes. The first byte in this
          ;     array represents the top line of 8 pixels. Each array entry represents
          ;     a row of 8 pixels.

          ; Debug Hardware Registers:
DBG_BEGIN       equ   $1C15    ;  start of mouse cursor hardware registers
DBG_TEMP        equ   $1C15    ;  (Byte) Simple Debug test register
DBG_END         equ   $1C16    ; End Debug Registers

          ; Mouse Cursor Hardware Registers:
CSR_BEGIN       equ   $1C16    ;  start of mouse cursor hardware registers
CSR_XPOS        equ   $1C16    ;  (Word) horizontal mouse cursor coordinate
CSR_YPOS        equ   $1C18    ;  (Word) vertical mouse cursor coordinate
CSR_XOFS        equ   $1C1A    ;  (Byte) horizontal mouse cursor offset
CSR_YOFS        equ   $1C1B    ;  (Byte) vertical mouse cursor offset
CSR_SCROLL      equ   $1C1C    ;  (Signed) MouseWheel Scroll: -1, 0, 1
CSR_FLAGS       equ   $1C1D    ;  (Byte) mouse button flags:
          ;  CSR_FLAGS:
          ;       bits 0-5: button states
          ;       bits 6-7: number of clicks
CSR_BMP_INDX    equ   $1C1E    ;  (Byte) mouse cursor bitmap pixel offset
CSR_BMP_DATA    equ   $1C1F    ;  (Byte) mouse cursor bitmap pixel index color
CSR_PAL_INDX    equ   $1C21    ;  (Byte) mouse cursor color palette index (0-15)
CSR_PAL_DATA    equ   $1C22    ;  (Word) mouse cursor color palette data RGBA4444
CSR_END         equ   $1C24    ; End Mouse Registers
GFX_END         equ   $1C24    ; End of GFX Device Registers

KEY_BEGIN       equ   $1C24    ; Start of the Keyboard Register space
CHAR_Q_LEN      equ   $1C24    ;   (Byte) # of characters waiting in queue        (Read Only)
CHAR_SCAN       equ   $1C25    ;   (Byte) read next character in queue (not popped when read)
CHAR_POP        equ   $1C26    ;   (Byte) read next character in queue (not popped when read)
XKEY_BUFFER     equ   $1C27    ;   (128 bits) 16 bytes for XK_KEY data buffer     (Read Only)
EDT_BFR_CSR     equ   $1C37    ;   (Byte) cursor position within edit buffer     (Read/Write)
EDT_BUFFER      equ   $1C38    ;   (256 Bytes) line editing character buffer     (Read/Write)
KEY_END         equ   $1D38    ; End of the Keyboard Register space

    ; 4808 ($12C8) bytes remaining for additional registers.
RESERVED        equ   $1D38

          ; User RAM (32K)
USER_RAM        equ   $3000

          ; Paged RAM (8K)
PAGED_RAM       equ   $B000

          ; PAGED ROM (8K bytes)
PAGED_ROM       equ   $D000

          ; KERNEL ROM (4K bytes)
KERNEL_ROM      equ   $F000

          ; Hardware Interrupt Vectors:
HARD_RSRVD      equ   $FFF0    ; Motorola RESERVED Hardware Interrupt Vector
HARD_SWI3       equ   $FFF2    ; SWI3 Hardware Interrupt Vector
HARD_SWI2       equ   $FFF4    ; SWI2 Hardware Interrupt Vector
HARD_FIRQ       equ   $FFF6    ; FIRQ Hardware Interrupt Vector
HARD_IRQ        equ   $FFF8    ; IRQ Hardware Interrupt Vector
HARD_SWI        equ   $FFFA    ; SWI / SYS Hardware Interrupt Vector
HARD_NMI        equ   $FFFC    ; NMI Hardware Interrupt Vector
HARD_RESET      equ   $FFFE    ; RESET Hardware Interrupt Vector
; END of definitions
