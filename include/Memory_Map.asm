

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

SYSTEM_MEMORY_DEVICE  equ   0x0010    ; START: System Memory
ZERO_PAGE             equ   0x0010    ; Zero Page System and User Variables
FIO_BUFFER            equ   0x0100    ; START: File Input/Output Buffer
FIO_BFR_END           equ   0x01FF    ; END: File Input/Output Buffer
SYSTEM_STACK          equ   0x0200    ; Bottom of the system stack spcace
SSTACK_TOP            equ   0x0400    ; Top of the system statck space

VIDEO_BUFFER_DEVICE   equ   0x0400    ; START: Video Buffer (8K)
VIDEO_START           equ   0x0400    ; Start of standard video buffer
VIDEO_END             equ   0x23FF    ; End of standard video buffer
VIDEO_TOP             equ   0x2400    ; Top of standard video buffer

USER_MEMORY_DEVICE    equ   0x2400    ; START: User Memory (34K)
USER_RAM              equ   0x2400    ; User Accessable RAM
USER_RAM_END          equ   0xAFFF    ; End User Accessable RAM
USER_RAM_TOP          equ   0xB000    ; Top User Accessable RAM

MEMBANK_DEVICE        equ   0xB000    ; START: Banked Memory Region (16K)
MEMBANK_ONE           equ   0xB000    ; Banked Memory Page One (8K)
MEMBANK_TWO           equ   0xD000    ; Banked Memory Page Two (8K)
MEMBANK_END           equ   0xEFFF    ; End of Banked Memory Region
MEMBANK_TOP           equ   0xF000    ; TOP of Banked Memory Region

KERNEL_ROM_DEVICE     equ   0xF000    ; START: Kernel Rom (3.5K)
KERNEL_START          equ   0xF000    ; Start of Kernel Rom Space
KERNEL_END            equ   0xFDFF    ; End of Kernel Rom Space
KERNEL_TOP            equ   0xFE00    ; Top of Kernel Rom Space

GPU_DEVICE            equ   0xFE00    ; START: GPU Device Hardware Registers
GPU_ENABLE            equ   0xFE00    ; (Byte) Bitflag Enables
                                      ;    - bits 5-7 = reserved
                                      ;    - bit  4   = 0: disable ext display,
                                      ;                 1: enable ext display
                                      ;    - bit  3   = 0: disable std display,
                                      ;                 1: enable std display
                                      ;    - bit  2   = 0: disable sprites,
                                      ;                 1: enable sprites
                                      ;    - bit  1   = 0: disable tilemap,
                                      ;                 1: enable tilemap
                                      ;    - bit  0   = 0: disable mouse cursor,
                                      ;                 1: enable mouse cursor
                                      ; 
GPU_STD_MODE          equ   0xFE01    ; (Byte) Standard Graphics Mode
                                      ;    - bit  7   = 0: screen is text, 
                                      ;                 1: screen is bitmap
                                      ;    - bit  6   = video timing: 
                                      ;                 0: H:512 x V:320
                                      ;                 1: H:640 x V:400
                                      ;    - bits 4-5 = horizontal overscan: 
                                      ;                 00:H/1 (512 or 640)
                                      ;                 01:H/2 (256 or 320)
                                      ;                 10:H/3 (170 or 213)
                                      ;                 11:H/4 (128 or 160)
                                      ;    - bits 2-3 = vertical overscan: 
                                      ;                 00:V/1 (320 or 400)
                                      ;                 01:V/2 (160 or 200)
                                      ;                 10:V/3 (106 or 133)
                                      ;                 11:V/4 (80 or 100)
                                      ;    - bits 0-1 = text mode:
                                      ;                 00:monochrome text
                                      ;                 01:bg/fg + text
                                      ;                 10:clear-bgnd 256-color text
                                      ;                 11:color_0-bgnd 256-color text
                                      ;     ... or ... 
                                      ;    - bits 0-1 = bitmap mode:
                                      ;                 00: 2 colors,
                                      ;                 01: 4 colors,
                                      ;                 10: 16 colors, 
                                      ;                 11: 256 colors
                                      ; 
GPU_EXT_MODE          equ   0xFE02    ; (Byte) Extended Graphics Mode
                                      ;    - bit  7   = 0: screen is tiled,
                                      ;                 1: screen is bitmap
                                      ;    - bit  6   = video timing: 
                                      ;                 0: H:512 x V:320
                                      ;                 1: H:640 x V:400
                                      ;    - bits 4-5 = horizontal overscan: 
                                      ;                 00:H/1 (512 or 640)
                                      ;                 01:H/2 (256 or 320)
                                      ;                 10:H/3 (170 or 213)
                                      ;                 11:H/4 (128 or 160)
                                      ;    - bits 2-3 = vertical overscan: 
                                      ;                 00:V/1 (320 or 400)
                                      ;                 01:V/2 (160 or 200)
                                      ;                 10:V/3 (106 or 133)
                                      ;                 11:V/4 (80 or 100)
                                      ;    - bits 0-1 = Color Mode:
                                      ;                 00: 2 colors
                                      ;                 01: 4 colors
                                      ;                 10: 16 colors
                                      ;                 11: 256 colors
                                      ; 
GPU_EMULATION         equ   0xFE03    ; (Byte) Emulation Flags
                                      ;    - bit  7    = vsync: 0=off, 1=on
                                      ;    - bit  6    = main: 0=windowed,
                                      ;                  1=fullscreen
                                      ;    - bit  5    = debug: 0=off, 1=on
                                      ;    - bit  4    = debug: 0=windowed, 
                                      ;                  1=fullscreen
                                      ;    - bits 2-3  = Active Monitor 0-3
                                      ;    - bits 0-1  = Debug Monitor 0-3
                                      ; 

HDW_RESERVED_DEVICE   equ   0xFE04    ; START: Reserved Register Space
HDW_REG_END           equ   0xFFEF    ; 491 bytes reserved for future use.

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


