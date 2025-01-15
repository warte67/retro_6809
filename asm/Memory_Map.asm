;*** Memory_Map.asm *************************************************
;*    __  __                                     __  __                                 
;*   |  \/  |                                   |  \/  |                                
;*   | \  / | ___ _ __ ___   ___  _ __ _   _    | \  / | __ _ _ __   __ _ ___ _ __ ___  
;*   | |\/| |/ _ \ '_ ` _ \ / _ \| '__| | | |   | |\/| |/ _` | '_ \ / _` / __| '_ ` _ \ 
;*   | |  | |  __/ | | | | | (_) | |  | |_| |   | |  | | (_| | |_) | (_| \__ \ | | | | |
;*   |_|  |_|\___|_| |_| |_|\___/|_|   \__, |   |_|  |_|\__,_| .__(_)__,_|___/_| |_| |_|
;*                                      __/ |_____           | |                        
;*                                     |___/______|          |_|                        
;*
;* 6809 Assembly Memory Map Definition File
;*
;* Released under the GPL v3.0 License.
;* Original Author: Jay Faries (warte67)
;*
;************************************



;   **********************************************
;   * Allocated 64k Memory Mapped System Symbols *
;   **********************************************
;

SOFT_VECTORS_DEVICE   equ    $0000    ; START: Software Interrupt Vectors
SOFT_EXEC             equ    $0000    ; Exec Software Interrupt Vector
SOFT_SWI3             equ    $0002    ; SWI3 Software Interrupt Vector
SOFT_SWI2             equ    $0004    ; SWI2 Software Interrupt Vector
SOFT_FIRQ             equ    $0006    ; FIRQ Software Interrupt Vector
SOFT_IRQ              equ    $0008    ; IRQ Software Interrupt Vector
SOFT_SWI              equ    $000A    ; SWI / SYS Software Interrupt Vector
SOFT_NMI              equ    $000C    ; NMI Software Interrupt Vector
SOFT_RESET            equ    $000E    ; RESET Software Interrupt Vector
; _______________________________________________________________________

SYSTEM_MEMORY_DEVICE  equ    $0000    ; START: System Memory
ZERO_PAGE             equ    $0010    ; Zero Page System and User Variables
FIO_BUFFER            equ    $0100    ; START: File Input/Output Buffer
FIO_BFR_END           equ    $01FF    ; END: File Input/Output Buffer
SYSTEM_STACK          equ    $0200    ; Bottom of the system stack spcace
SSTACK_TOP            equ    $0400    ; Top of the system statck space
; _______________________________________________________________________

VIDEO_BUFFER_DEVICE   equ    $0000    ; START: Video Buffer (8K)
VIDEO_START           equ    $0400    ; Start of standard video buffer
VIDEO_END             equ    $23FF    ; End of standard video buffer
VIDEO_TOP             equ    $2400    ; Top of standard video buffer
; _______________________________________________________________________

USER_MEMORY_DEVICE    equ    $0000    ; START: User Memory (34K)
USER_RAM              equ    $2400    ; User Accessable RAM
USER_RAM_END          equ    $AFFF    ; End User Accessable RAM
USER_RAM_TOP          equ    $B000    ; Top User Accessable RAM
; _______________________________________________________________________

MEMBANK_DEVICE        equ    $0000    ; START: Banked Memory Region (16K)
MEMBANK_ONE           equ    $B000    ; Banked Memory Page One (8K)
MEMBANK_TWO           equ    $D000    ; Banked Memory Page Two (8K)
MEMBANK_END           equ    $EFFF    ; End of Banked Memory Region
MEMBANK_TOP           equ    $F000    ; TOP of Banked Memory Region
; _______________________________________________________________________

KERNEL_ROM_DEVICE     equ    $0000    ; START: Kernel Rom (3.5K)
KERNEL_START          equ    $F000    ; Start of Kernel Rom Space
KERNEL_END            equ    $FDFF    ; End of Kernel Rom Space
KERNEL_TOP            equ    $FE00    ; Top of Kernel Rom Space
; _______________________________________________________________________

SYS_DEVICE            equ    $FE00    ; START: System and Debug Hardware Registers:
SYS_BEGIN             equ    $FE00    ; Start of System Registers
SYS_STATE             equ    $FE00    ; (Byte) System State Register
                                      ; SYS_STATE: ABCD.SSSS                          
                                      ; - bit  7   = Error: Standard Buffer Overflow  
                                      ; - bit  6   = Error: Extended Buffer Overflow  
                                      ; - bit  5   = Error: Reserved                  
                                      ; - bit  4   = Error: Reserved                  
                                      ; - bits 0-3 = CPU Speed (0-15):                
                                      ;    0 ($0)  = CPU Clock   10 kHz 
                                      ;    1 ($1)  = CPU Clock   25 kHz 
                                      ;    2 ($2)  = CPU Clock   50 kHz 
                                      ;    3 ($3)  = CPU Clock   75 kHz 
                                      ;    4 ($4)  = CPU Clock  100 kHz 
                                      ;    5 ($5)  = CPU Clock  150 kHz 
                                      ;    6 ($6)  = CPU Clock  225 kHz 
                                      ;    7 ($7)  = CPU Clock  350 kHz 
                                      ;    8 ($8)  = CPU Clock  500 kHz 
                                      ;    9 ($9)  = CPU Clock  750 kHz 
                                      ;   10 ($A)  = CPU Clock  900 kHz 
                                      ;   11 ($B)  = CPU Clock 1000 khz 
                                      ;   12 ($C)  = CPU Clock 2000 khz 
                                      ;   13 ($D)  = CPU Clock 3000 khz 
                                      ;   14 ($E)  = CPU Clock 4000 khz 
                                      ;   15 ($F)  = CPU Clock ~10.0 mhz. (unmetered) 
                                      ; 
SYS_SPEED             equ    $FE01    ; (Word) Average CPU Clock Speed (Read Only)
SYS_CLOCK_DIV         equ    $FE03    ; (Byte) 60 hz Clock Divider Register (Read Only)
                                      ; - bit 7: 0.546875 hz
                                      ; - bit 6: 1.09375 hz
                                      ; - bit 5: 2.1875 hz
                                      ; - bit 4: 4.375 hz
                                      ; - bit 3: 8.75 hz
                                      ; - bit 2: 17.5 hz
                                      ; - bit 1: 35.0 hz
                                      ; - bit 0: 70.0 hz
                                      ; 
SYS_UPDATE_COUNT      equ    $FE04    ; (DWord) Update Count (Read Only)
SYS_DBG_BRK_ADDR      equ    $FE08    ; (Word) Address of current debug breakpoint
SYS_DBG_FLAGS         equ    $FE0A    ; (Byte) Debug Specific Hardware Flags:
                                      ; - bit 7: Debug Enable
                                      ; - bit 6: Single Step Enable
                                      ; - bit 5: Clear All Breakpoints
                                      ; - bit 4: Update Breakpoint at DEBUG_BRK_ADDR
                                      ; - bit 3: FIRQ  (on low {0} to high {1} edge)
                                      ; - bit 2: IRQ   (on low {0} to high {1} edge)
                                      ; - bit 1: NMI   (on low {0} to high {1} edge)
                                      ; - bit 0: RESET (on low {0} to high {1} edge)
                                      ; 
SYS_END               equ    $FE0A    ; End of System Registers
SYS_TOP               equ    $FE0B    ; Top of System Registers
; _______________________________________________________________________

GPU_DEVICE            equ    $FE0B    ; START: GPU Device Hardware Registers
GPU_OPTIONS           equ    $FE0B    ; (Byte) Bitflag Enables
                                      ; - bit 7    = Extended Bitmap:
                                      ;               0: Tilemap Display
                                      ;               1: Bitmap Display
                                      ; - bits 5-6 = Extended Color Mode:
                                      ;               00: 2-Colors
                                      ;               01: 4-Colors
                                      ;               10: 16-Colors
                                      ;               11: 256-Colors
                                      ; - bits 4   = Extended Display Enable
                                      ;               0: Disabled
                                      ;               1: Enabled
                                      ; - bits 3   = Application Screen Mode
                                      ;               0: Windowed
                                      ;               1: Fullscreen
                                      ; - bits 2   = VSync Enable
                                      ;               0: Disabled
                                      ;               1: Enabled
                                      ; - bits 1   = Sprite Enable
                                      ;               0: Disabled
                                      ;               1: Enabled
                                      ; - bit  0   = Standard Display Enable
                                      ;               0: Disabled
                                      ;               1: Enabled
                                      ; 
GPU_MODE              equ    $FE0C    ; (Byte) Standard Display Mode
                                      ; - bit 7    = Standard Bitmap:
                                      ;               0: Text Display
                                      ;               1: Bitmap Display
                                      ; - bits 5-6 = Standard Color Mode:
                                      ;               00: 2-Colors
                                      ;               01: 4-Colors
                                      ;               10: 16-Colors
                                      ;               11: 256-Colors
                                      ; - bits 0-4 = Display Mode (0-31)
                                      ; 
GPU_VIDEO_MAX         equ    $FE0D    ; (Word) Video Buffer Maximum (Read Only)
                                      ;  Note: This will change to reflect
                                      ;        the size of the last cpu
                                      ;        accessible memory location
                                      ;        of the currently active
                                      ;        standard video mode.
                                      ; 
GPU_HRES              equ    $FE0F    ; (Word) Horizontal Pixel Resolution (Read Only)
                                      ;   Note: This will reflect the number of
                                      ;        pixel columns for bitmap modes.
                                      ; 
GPU_VRES              equ    $FE11    ; (Word) Vertical Pixel Resolution (Read Only)
                                      ;   Note: This will reflect the number of
                                      ;        pixel rows for bitmap modes.
                                      ; 
GPU_TCOLS             equ    $FE13    ; (Byte) Text Horizontal Columns (Read Only)
                                      ;   Note: This will reflect the number of
                                      ;        glyph columns for text modes.
                                      ; 
GPU_TROWS             equ    $FE15    ; (Byte) Text Vertical Rows (Read Only)
                                      ;   Note: This will reflect the number of
                                      ;        glyph rows for text modes.
                                      ; 
GPU_PAL_INDEX         equ    $FE17    ; (Byte) Color Palette Index
                                      ;   Note: Use this register to set the
                                      ;        index into the Color Palette.
                                      ;        Set this value prior referencing
                                      ;        the color data (GPU_PAL_COLOR).
                                      ; 
GPU_PAL_COLOR         equ    $FE18    ; (Word) Color Palette Data (A4R4G4B4 format)
                                      ;   Note: This is the color data for an
                                      ;        individual palette entry. Write to 
                                      ;        DSP_PAL_IDX with the index within the
                                      ;        color palette prior to reading or
                                      ;        writing the color data in the
                                      ;        GFX_PAL_CLR register.
                                      ; 
GPU_GLYPH_IDX         equ    $FE1A    ; (Byte) Text Glyph Index
                                      ;   Note: Use this register to set the
                                      ;        index of a specific text glyph.
                                      ;        Set this value prior to updating
                                      ;        the glyph data (GPU_GLYPH_DATA).
                                      ; 
GPU_GLYPH_DATA        equ    $FE1B    ; (8-Bytes) 8 rows of binary encoded glyph pixel data
                                      ;   Note: This is the pixel data for a
                                      ;        specific text glyph. Each 8x8
                                      ;        text glyph is composed of 8 bytes.
                                      ;        The first byte in this array
                                      ;        represents the top line of 8 pixels.
                                      ;        Each array entry represents a row of 8 pixels.
                                      ; 
GPU_END               equ    $FE22    ; End of GPU Register Space
GPU_TOP               equ    $FE23    ; Top of GPU Register Space
; _______________________________________________________________________

HDW_RESERVED_DEVICE   equ    $0000    ; START: Reserved Register Space
HDW_REG_END           equ    $FFEF    ; 460 bytes reserved for future use.
; _______________________________________________________________________

ROM_VECTS_DEVICE      equ    $0000    ; START: Hardware Interrupt Vectors
HARD_EXEC             equ    $FFF0    ; EXEC Hardware Interrupt Vector
HARD_SWI3             equ    $FFF2    ; SWI3 Hardware Interrupt Vector
HARD_SWI2             equ    $FFF4    ; SWI2 Hardware Interrupt Vector
HARD_FIRQ             equ    $FFF6    ; FIRQ Hardware Interrupt Vector
HARD_IRQ              equ    $FFF8    ; IRQ Hardware Interrupt Vector
HARD_SWI              equ    $FFFA    ; SWI / SYS Hardware Interrupt Vector
HARD_NMI              equ    $FFFC    ; NMI Hardware Interrupt Vector
HARD_RESET            equ    $FFFE    ; RESET Hardware Interrupt Vector


; END of memory_map.asm definitions


