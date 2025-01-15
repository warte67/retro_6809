

;    memory_map.asm
;
;   **********************************************
;   * Allocated 64k Memory Mapped System Symbols *
;   **********************************************
;

SOFT_VECTORS_DEVICE   equ   0x0000    ; START: Software Interrupt Vectors
SOFT_EXEC             equ   0x0000    ; Exec Software Interrupt Vector
SOFT_SWI3             equ   0x0002    ; SWI3 Software Interrupt Vector
SOFT_SWI2             equ   0x0004    ; SWI2 Software Interrupt Vector
SOFT_FIRQ             equ   0x0006    ; FIRQ Software Interrupt Vector
SOFT_IRQ              equ   0x0008    ; IRQ Software Interrupt Vector
SOFT_SWI              equ   0x000A    ; SWI / SYS Software Interrupt Vector
SOFT_NMI              equ   0x000C    ; NMI Software Interrupt Vector
SOFT_RESET            equ   0x000E    ; RESET Software Interrupt Vector
; _______________________________________________________________________

SYSTEM_MEMORY_DEVICE  equ   0x0000    ; START: System Memory
ZERO_PAGE             equ   0x0010    ; Zero Page System and User Variables
FIO_BUFFER            equ   0x0100    ; START: File Input/Output Buffer
FIO_BFR_END           equ   0x01FF    ; END: File Input/Output Buffer
SYSTEM_STACK          equ   0x0200    ; Bottom of the system stack spcace
SSTACK_TOP            equ   0x0400    ; Top of the system statck space
; _______________________________________________________________________

VIDEO_BUFFER_DEVICE   equ   0x0000    ; START: Video Buffer (8K)
VIDEO_START           equ   0x0400    ; Start of standard video buffer
VIDEO_END             equ   0x23FF    ; End of standard video buffer
VIDEO_TOP             equ   0x2400    ; Top of standard video buffer
; _______________________________________________________________________

USER_MEMORY_DEVICE    equ   0x0000    ; START: User Memory (34K)
USER_RAM              equ   0x2400    ; User Accessable RAM
USER_RAM_END          equ   0xAFFF    ; End User Accessable RAM
USER_RAM_TOP          equ   0xB000    ; Top User Accessable RAM
; _______________________________________________________________________

SYS_DEVICE            equ   0xB000    ; START: System and Debug Hardware Registers:
SYS_BEGIN             equ   0xB000    ; Start of System Registers
SYS_STATE             equ   0xB000    ; (Byte) System State Register
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
SYS_SPEED             equ   0xB001    ; (Word) Average CPU Clock Speed (Read Only)
                      equ   0xB002    ; 
SYS_CLOCK_DIV         equ   0xB003    ; (Byte) 60 hz Clock Divider Register (Read Only)
                                      ; - bit 7: 0.546875 hz
                                      ; - bit 6: 1.09375 hz
                                      ; - bit 5: 2.1875 hz
                                      ; - bit 4: 4.375 hz
                                      ; - bit 3: 8.75 hz
                                      ; - bit 2: 17.5 hz
                                      ; - bit 1: 35.0 hz
                                      ; - bit 0: 70.0 hz
                                      ; 
SYS_UPDATE_COUNT      equ   0xB004    ; (Byte) Update Count (Read Only)
                      equ   0xB005    ; 
                      equ   0xB006    ; 
                      equ   0xB007    ; 
SYS_DBG_BRK_ADDR      equ   0xB008    ; (Word) Address of current debug breakpoint
                      equ   0xB009    ; 
SYS_DBG_FLAGS         equ   0xB00A    ; (Byte) Debug Specific Hardware Flags:
                                      ; - bit 7: Debug Enable
                                      ; - bit 6: Single Step Enable
                                      ; - bit 5: Clear All Breakpoints
                                      ; - bit 4: Update Breakpoint at DEBUG_BRK_ADDR
                                      ; - bit 3: FIRQ  (on low {0} to high {1} edge)
                                      ; - bit 2: IRQ   (on low {0} to high {1} edge)
                                      ; - bit 1: NMI   (on low {0} to high {1} edge)
                                      ; - bit 0: RESET (on low {0} to high {1} edge)
                                      ; 
SYS_END               equ   0xB00A    ; End of System Registers
SYS_TOP               equ   0xB00B    ; Top of System Registers
                                      ; 

GPU_DEVICE            equ   0xB00B    ; START: GPU Device Hardware Registers
GPU_OPTIONS           equ   0xB00B    ; (Byte) Bitflag Enables
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
GPU_MODE              equ   0xB00C    ; (Byte) Bitflag Enables
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
GPU_VIDEO_MAX         equ   0xB00D    ; (Word) Video Buffer Maximum (Read Only)
                                      ;  Note: This will change to reflect
                                      ;        the size of the last cpu
                                      ;        accessible memory location
                                      ;        of the currently active
                                      ;        standard video mode.
                      equ   0xB00E    ; 
GPU_HRES              equ   0xB00F    ; (Word) Horizontal Resolution (Read Only)
                                      ;   Note: This will reflect the number of
                                      ;        pixel columns for bitmap modes.
                      equ   0xB010    ; 
GPU_VRES              equ   0xB011    ; (Word) Vertical Resolution (Read Only)
                                      ;   Note: This will reflect the number of
                                      ;        pixel rows for bitmap modes.
                      equ   0xB012    ; 
GPU_TCOLS             equ   0xB013    ; (Byte) Text Horizontal Columns (Read Only)
                                      ;   Note: This will reflect the number of
                                      ;        glyph columns for text modes.
                                      ; 
                      equ   0xB014    ; 
GPU_TROWS             equ   0xB015    ; (Byte) Text Vertical Rows (Read Only)
                                      ;   Note: This will reflect the number of
                                      ;        glyph rows for text modes.
                                      ; 
                      equ   0xB016    ; 
GPU_END               equ   0xB016    ; End of GPU Register Space
GPU_TOP               equ   0xB017    ; Top of GPU Register Space
; _______________________________________________________________________


; END of memory_map.asm definitions


