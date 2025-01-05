

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

SYSTEM_MEMORY_DEVICE  equ   0x0010    ; START: System Memory
ZERO_PAGE             equ   0x0010    ; Zero Page System and User Variables
FIO_BUFFER            equ   0x0100    ; START: File Input/Output Buffer
FIO_BFR_END           equ   0x01FF    ; END: File Input/Output Buffer
SYSTEM_STACK          equ   0x0200    ; Bottom of the system stack spcace
SSTACK_TOP            equ   0x0400    ; Top of the system statck space
; _______________________________________________________________________

VIDEO_BUFFER_DEVICE   equ   0x0400    ; START: Video Buffer (8K)
VIDEO_START           equ   0x0400    ; Start of standard video buffer
VIDEO_END             equ   0x23FF    ; End of standard video buffer
VIDEO_TOP             equ   0x2400    ; Top of standard video buffer
; _______________________________________________________________________

USER_MEMORY_DEVICE    equ   0x2400    ; START: User Memory (34K)
USER_RAM              equ   0x2400    ; User Accessable RAM
USER_RAM_END          equ   0xAFFF    ; End User Accessable RAM
USER_RAM_TOP          equ   0xB000    ; Top User Accessable RAM
; _______________________________________________________________________

MEMBANK_DEVICE        equ   0xB000    ; START: Banked Memory Region (16K)
MEMBANK_ONE           equ   0xB000    ; Banked Memory Page One (8K)
MEMBANK_TWO           equ   0xD000    ; Banked Memory Page Two (8K)
MEMBANK_END           equ   0xEFFF    ; End of Banked Memory Region
MEMBANK_TOP           equ   0xF000    ; TOP of Banked Memory Region
; _______________________________________________________________________

KERNEL_ROM_DEVICE     equ   0xF000    ; START: Kernel Rom (3.5K)
KERNEL_START          equ   0xF000    ; Start of Kernel Rom Space
KERNEL_END            equ   0xFDFF    ; End of Kernel Rom Space
KERNEL_TOP            equ   0xFE00    ; Top of Kernel Rom Space
; _______________________________________________________________________

TESTS_DEVICE          equ   0xFE00    ; START: TESTS_DEVICE
TESTS_START           equ   0xFE00    ; Start of Test Registers Space
TESTS_ONE             equ   0xFE00    ; (Byte) First Test Register
                                      ; - bit 7    = TEST_ENABLE
                                      ; - bit 6    = (reserved)
                                      ; - bit 5    = (reserved)
                                      ; - bit 4    = (reserved)
                                      ; - bit 3    = (reserved)
                                      ; - bit 2    = (reserved)
                                      ; - bit 1    = (reserved)
                                      ; - bit 0    = TEST_INC_VID_MODES
                                      ; 
TESTS_TWO             equ   0xFE01    ; (Byte) Second Test Register
                                      ; - bits 0-7  = (reserved)
                                      ; 
TESTS_END             equ   0xFE02    ; End of Tests Registers Space
TESTS_TOP             equ   0xFE03    ; Top of Tests Registers Space
; _______________________________________________________________________

GPU_DEVICE            equ   0xFE03    ; START: GPU Device Hardware Registers
GPU_OPTIONS           equ   0xFE03    ; (Byte) Bitflag Enables
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
GPU_MODE              equ   0xFE04    ; (Byte) Bitflag Enables
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
GPU_END               equ   0xFE04    ; End of GPU Register Space
GPU_TOP               equ   0xFE05    ; Top of GPU Register Space
; _______________________________________________________________________

HDW_RESERVED_DEVICE   equ   0xFE05    ; START: Reserved Register Space
HDW_REG_END           equ   0xFFEF    ; 490 bytes reserved for future use.
; _______________________________________________________________________

ROM_VECTS_DEVICE      equ   0xFFF0    ; START: Hardware Interrupt Vectors
HARD_EXEC             equ   0xFFF0    ; EXEC Hardware Interrupt Vector
HARD_SWI3             equ   0xFFF2    ; SWI3 Hardware Interrupt Vector
HARD_SWI2             equ   0xFFF4    ; SWI2 Hardware Interrupt Vector
HARD_FIRQ             equ   0xFFF6    ; FIRQ Hardware Interrupt Vector
HARD_IRQ              equ   0xFFF8    ; IRQ Hardware Interrupt Vector
HARD_SWI              equ   0xFFFA    ; SWI / SYS Hardware Interrupt Vector
HARD_NMI              equ   0xFFFC    ; NMI Hardware Interrupt Vector
HARD_RESET            equ   0xFFFE    ; RESET Hardware Interrupt Vector


; END of memory_map.asm definitions


