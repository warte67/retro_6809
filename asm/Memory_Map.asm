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

SYSTEM_MEMORY_DEVICE  equ    $0010    ; START: System Memory
ZERO_PAGE             equ    $0010    ; Zero Page System and User Variables
ZERO_PAGE_END         equ    $00FF    ; Zero Page System and User Variables
FIO_FILE_BUFFER       equ    $0100    ; START: File Input/Output Buffer
FIO_FILE_BFR_END      equ    $017F    ; END: File Input/Output Buffers
FIO_LN_EDT_BUFFER     equ    $0180    ; START: Line Edit Character Buffer
FIO_LN_EDT_END        equ    $01FF    ; END: Line Edit Character Buffer
SYSTEM_STACK          equ    $0200    ; Bottom of System Stack Spcace
SSTACK_END            equ    $03FF    ; END: System Stack Space
SSTACK_TOP            equ    $0400    ; TOP: System Stack Space
; _______________________________________________________________________

VIDEO_BUFFER_DEVICE   equ    $0400    ; START: Video Buffer (8K)
VIDEO_START           equ    $0400    ; Start of standard video buffer
VIDEO_END             equ    $23FF    ; End of standard video buffer
VIDEO_TOP             equ    $2400    ; Top of standard video buffer
; _______________________________________________________________________

USER_MEMORY_DEVICE    equ    $2400    ; START: User Memory (34K)
USER_RAM              equ    $2400    ; User Accessable RAM
USER_RAM_END          equ    $AFFF    ; End User Accessable RAM
USER_RAM_TOP          equ    $B000    ; Top User Accessable RAM
; _______________________________________________________________________

MEMBANK_DEVICE        equ    $B000    ; START: Banked Memory Region (16K)
MEMBANK_ONE           equ    $B000    ; Banked Memory Page One (8K)
MEMBANK_TWO           equ    $D000    ; Banked Memory Page Two (8K)
MEMBANK_END           equ    $EFFF    ; End of Banked Memory Region
MEMBANK_TOP           equ    $F000    ; TOP of Banked Memory Region
; _______________________________________________________________________

KERNEL_ROM_DEVICE     equ    $F000    ; START: Kernel Rom (3.5K)
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
                                      ; - bits 3   = Emulation Screen Mode
                                      ;               (temporary)
                                      ;               0: Windowed
                                      ;               1: Fullscreen
                                      ; - bits 2   = VSync Enable
                                      ;               0: Disabled
                                      ;               1: Enabled
                                      ; - bit  1   = Presentation
                                      ;               0: Overscan / Stretch
                                      ;               1: Letterbox
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

CSR_DEVICE            equ    $FE23    ; START: Mouse Device Hardware Registers
CSR_XPOS              equ    $FE23    ; (Word) Horizontal Mouse Cursor Coordinate
CSR_YPOS              equ    $FE25    ; (Word) Vertical Mouse Cursor Coordinate
CSR_XOFS              equ    $FE27    ; (Byte) Horizontal Mouse Cursor Offset
CSR_YOFS              equ    $FE28    ; (Byte) Vertical Mouse Cursor Offset
CSR_SCROLL            equ    $FE29    ; (char) MouseWheel Scroll: -1, 0, 1
CSR_FLAGS             equ    $FE2A    ; (Byte) Mouse Device State Flags
                                      ;    bits 0-4: button states
                                      ;    bits 5-6: number of clicks
                                      ;    bits 7:   cursor enable
                                      ; 
CSR_BMP_INDX          equ    $FE2B    ; (Byte) Mouse Cursor Bitmap Pixel Offset
CSR_BMP_DATA          equ    $FE2C    ; (Byte) Mouse Cursor Bitmap Pixel Color Data ($0-$F)
CSR_PAL_INDX          equ    $FE2D    ; (Byte) Mouse Cursor Color Palette Index (0-15)
CSR_PAL_DATA          equ    $FE2E    ; (Word) Mouse Cursor Color Palette Data A4R4G4B4
CSR_END               equ    $FE2F    ; End of Mouse Device Register Space
CSR_TOP               equ    $FE30    ; Top of CSR Register Space
; _______________________________________________________________________

KEYBOARD_DEVICE       equ    $FE30    ; START: Keyboard Device Hardware Registers
CHAR_Q_LEN            equ    $FE30    ; (Byte) Number of Characters Waiting in Queue   (Read Only)
CHAR_SCAN             equ    $FE31    ; (Byte) Read Next Character in Queue (Not Popped When Read)
CHAR_POP              equ    $FE32    ; (Byte) Read Next Character in Queue     (Popped When Read)
XKEY_BUFFER           equ    $FE33    ; (16 Bytes) 128 bits for XK_KEY data buffer     (Read Only)
EDT_BFR_CSR           equ    $FE43    ; (Byte) Cursor Position Within Edit Buffer     (Read/Write)
EDT_ENABLE            equ    $FE44    ; (Byte) Line Editor Enable Flag                (Read/Write)
EDT_BFR_LEN           equ    $FE45    ; (Byte) Limit the Line Editor to This Length   (Read/Write)
KEYBOARD_END          equ    $FE45    ; End of Keyboard Register Space
KEYBOARD_TOP          equ    $FE46    ; Top of Keyboard Register Space
; _______________________________________________________________________

JOYSTICK_DEVICE       equ    $FE46    ; START: Joystick/Gamepad Controller Device Hardware Registers
JOYS_1_FLAGS          equ    $FE46    ; (Byte) Gamepad/Joystick #1 Condition Flags:     (Read Only)
                                      ;            0000'0000: Not Connected
                                      ;            0000'1111: Controller Type
                                      ;            0001'0000: (reserved)
                                      ;            0010'0000: (reserved)
                                      ;            0100'0000: Is a Gamepad
                                      ;            1000'0000: Is a Joystick
                                      ; 
                                      ;        Gamepad Controller Type (bits 0-3):
                                      ;            0:  Unknown
                                      ;            1:  Standard
                                      ;            2:  Xbox360
                                      ;            3:  XboxOne
                                      ;            4:  PS3
                                      ;            5:  PS4
                                      ;            6:  PS5
                                      ;            7:  Nintendo Switch Pro
                                      ;            8:  Nintendo Switch Joycon Left
                                      ;            9:  Nintendo Switch Joycon Right
                                      ; 
                                      ;        Joystick Controller Type (bits 0-3):
                                      ;            0:  Unknown
                                      ;            1:  Gamepad
                                      ;            2:  Wheel
                                      ;            3:  Arcade Stick
                                      ;            4:  Flight Stick
                                      ;            5:  Dance Pad
                                      ;            6:  Guitar
                                      ;            7:  Drum Kit
                                      ;            8:  Arcade Pad
                                      ;            9:  Throttle
                                      ; 
JOYS_1_BTN            equ    $FE47    ; (Word) Gamepad Controller Button Bits:         (Read Only)
                                      ;            0000'0000'0000'0000 = Nothing Pressed
                                      ;            0000'0000'0000'0001 = A
                                      ;            0000'0000'0000'0010 = B
                                      ;            0000'0000'0000'0100 = X
                                      ;            0000'0000'0000'1000 = Y
                                      ;            0000'0000'0001'0000 = L.Shoulder
                                      ;            0000'0000'0010'0000 = R.Shoulder
                                      ;            0000'0000'0100'0000 = Back
                                      ;            0000'0000'1000'0000 = Start
                                      ;            0000'0001'0000'0000 = Misc 1
                                      ;            0000'0010'0000'0000 = Misc 2
                                      ;            0000'0100'0000'0000 = Misc 3
                                      ;            0000'1000'0000'0000 = Guide
                                      ;            0001'0000'0000'0000 = DPad Up   
                                      ;            0010'0000'0000'0000 = DPad Down 
                                      ;            0100'0000'0000'0000 = DPad Left 
                                      ;            1000'0000'0000'0000 = DPad Right
                                      ;            1111'1111'1111'1111 = Not Connected
                                      ; 
JOYS_1_DBND           equ    $FE49    ; (Byte) PAD 1 analog deadband; default is 5   (read/write)
JOYS_1_LTX            equ    $FE4A    ; (char) PAD 1 LThumb-X position (-128 _ +127)   (realtime)
JOYS_1_LTY            equ    $FE4B    ; (char) PAD 1 LThumb-Y position (-128 _ +127)   (realtime)
JOYS_1_RTX            equ    $FE4C    ; (char) PAD 1 RThumb-X position (-128 _ +127)   (realtime)
JOYS_1_RTY            equ    $FE4D    ; (char) PAD 1 RThumb-Y position (-128 _ +127)   (realtime)
JOYS_1_Z1             equ    $FE4E    ; (char) PAD 1 left analog trigger (0 - 127)     (realtime)
JOYS_1_Z2             equ    $FE4F    ; (char) PAD 1 left analog trigger (0 - 127)     (realtime)
                                      ; 
JOYS_2_FLAGS          equ    $FE50    ; (Byte) Gamepad/Joystick #2 Condition Flags:     (Read Only)
                                      ;            0000'0000: Not Connected
                                      ;            0000'1111: Controller Type
                                      ;            0001'0000: (reserved)
                                      ;            0010'0000: (reserved)
                                      ;            0100'0000: Is a Gamepad
                                      ;            1000'0000: Is a Joystick
                                      ; 
                                      ;        Gamepad Controller Type (bits 0-3):
                                      ;            0:  Unknown
                                      ;            1:  Standard
                                      ;            2:  Xbox360
                                      ;            3:  XboxOne
                                      ;            4:  PS3
                                      ;            5:  PS4
                                      ;            6:  PS5
                                      ;            7:  Nintendo Switch Pro
                                      ;            8:  Nintendo Switch Joycon Left
                                      ;            9:  Nintendo Switch Joycon Right
                                      ; 
                                      ;        Joystick Controller Type (bits 0-3):
                                      ;            0:  Unknown
                                      ;            1:  Gamepad
                                      ;            2:  Wheel
                                      ;            3:  Arcade Stick
                                      ;            4:  Flight Stick
                                      ;            5:  Dance Pad
                                      ;            6:  Guitar
                                      ;            7:  Drum Kit
                                      ;            8:  Arcade Pad
                                      ;            9:  Throttle
                                      ; 
JOYS_2_BTN            equ    $FE51    ; (Word) Button Bits: Room For up to 16 Buttons  (realtime)
                                      ;        Joystick Button Bits:
                                      ;            0000'0000'0000'0000 = Nothing Pressed
                                      ;            0000'0000'0000'0001 = Button 1 
                                      ;            0000'0000'0000'0010 = Button 2 
                                      ;            0000'0000'0000'0100 = Button 3 
                                      ;            0000'0000'0000'1000 = Button 4 
                                      ;            0000'0000'0001'0000 = Button 5 
                                      ;            0000'0000'0010'0000 = Button 6 
                                      ;            0000'0000'0100'0000 = Button 7 
                                      ;            0000'0000'1000'0000 = Button 8 
                                      ;            0000'0001'0000'0000 = Button 9 
                                      ;            0000'0010'0000'0000 = Button 10
                                      ;            0000'0100'0000'0000 = Button 11
                                      ;            0000'1000'0000'0000 = Button 12
                                      ;            0001'0000'0000'0000 = Hat Up   
                                      ;            0010'0000'0000'0000 = Hat Down 
                                      ;            0100'0000'0000'0000 = Hat Left 
                                      ;            1000'0000'0000'0000 = Hat Right
                                      ;            1111'1111'1111'1111 = Not Connected
                                      ; 
JOYS_2_DBND           equ    $FE53    ; (Byte) PAD 2 analog deadband; default is 5   (read/write)
JOYS_2_LTX            equ    $FE54    ; (char) PAD 2 LThumb-X position (-128 _ +127)   (realtime)
JOYS_2_LTY            equ    $FE55    ; (char) PAD 2 LThumb-Y position (-128 _ +127)   (realtime)
JOYS_2_RTX            equ    $FE56    ; (char) PAD 2 RThumb-X position (-128 _ +127)   (realtime)
JOYS_2_RTY            equ    $FE57    ; (char) PAD 2 RThumb-Y position (-128 _ +127)   (realtime)
JOYS_2_Z1             equ    $FE58    ; (char) PAD 2 left analog trigger (0 - 127)     (realtime)
JOYS_2_Z2             equ    $FE59    ; (char) PAD 2 left analog trigger (0 - 127)     (realtime)
JOYS_END              equ    $FE59    ; End of Joystick/Gamepad Device Register Space
JOYS_TOP              equ    $FE5A    ; Top of Joystick/Gamepad Device Register Space
; _______________________________________________________________________

FIO_DEVICE            equ    $FE5A    ; START: File I/O Device Hardware Registers
FIO_ERROR             equ    $FE5A    ; (Byte) FILE_ERROR enumeration result (FE_<error>)
                                      ; 
FE_BEGIN              equ    $0000    ;   Begin FILE_ERROR enumeration 
FE_NOERROR            equ    $0000    ;      no error, condition normal
FE_NOTFOUND           equ    $0001    ;      file or folder not found  
FE_NOTOPEN            equ    $0002    ;      file not open             
FE_EOF                equ    $0003    ;      end of file               
FE_OVERRUN            equ    $0004    ;      buffer overrun            
FE_WRONGTYPE          equ    $0005    ;      wrong file type           
FE_BAD_CMD            equ    $0006    ;      invalid command           
FE_BADSTREAM          equ    $0007    ;      invalid file stream       
FE_NOT_EMPTY          equ    $0007    ;      directory not empty       
FE_FILE_EXISTS        equ    $0007    ;      file already exists       
FE_INVALID_NAME       equ    $0007    ;      invalid file name         
FE_LAST               equ    $0007    ;   End of FILE_ERROR enumeration
                                      ; 
FIO_COMMAND           equ    $FE5B    ; (Byte) Execute a File Command (FC_<cmd>)
                                      ; 
FC_BEGIN              equ    $0000    ;   Begin FIO_COMMAND enumeration           
FC_RESET              equ    $0000    ;     Reset                                 
FC_SHUTDOWN           equ    $0001    ;     SYSTEM: Shutdown                      
FC_COMPDATE           equ    $0002    ;     SYSTEM: Load Compilation Date         
FC_FILEEXISTS         equ    $0003    ;     Does File Exist (return in FIO_IODATA)
FC_OPENREAD           equ    $0004    ;     Open Binary File For Reading          
FC_OPENWRITE          equ    $0005    ;     Open Binary File For Writing          
FC_OPENAPPEND         equ    $0006    ;     Open Binary File For Appending        
FC_CLOSEFILE          equ    $0007    ;     Close File                            
FC_READBYTE           equ    $0008    ;     Read Byte (into FIO_IOBYTE)           
FC_WRITEBYTE          equ    $0009    ;     Write Byte (from FIO_IOBYTE)          
FC_LOADHEX            equ    $000A    ;     Load Hex Format File                  
FC_GETLENGTH          equ    $000B    ;     Get File Length (into FIO_IOWORD)     
FC_LISTDIR            equ    $000C    ;     List Directory                        
FC_MAKEDIR            equ    $000D    ;     Make Directory                        
FC_CHANGEDIR          equ    $000E    ;     Change Directory                      
FC_GETPATH            equ    $000F    ;     Fetch Current Path                    
FC_REN_DIR            equ    $0010    ;     Rename Directory                      
FC_DEL_DIR            equ    $0011    ;     Delete Directory                      
FC_DEL_FILE           equ    $0012    ;     Delete File                           
FC_REN_FILE           equ    $0013    ;     Rename File                           
FC_COPY_FILE          equ    $0014    ;     Copy File                             
FC_SEEK_START         equ    $0015    ;     Seek Start                            
FC_SEEK_END           equ    $0016    ;     Seek End                              
FC_SET_SEEK           equ    $0017    ;     Set Seek Position (from FIO_IOWORD)   
FC_GET_SEEK           equ    $0018    ;     Get Seek Position (into FIO_IOWORD)   
FC_LAST               equ    $0018    ;   End FIO_COMMAND enumeration             
                                      ; 
FIO_HANDLE            equ    $FE5C    ; (Byte) Current File Stream HANDLE (0=NONE)
FIO_SEEKPOS           equ    $FE5D    ; (DWord) File Seek Position
FIO_IODATA            equ    $FE61    ; (Byte) Input / Output Data
                                      ; 
FIO_PATH_LEN          equ    $FE62    ; (Byte) Length of the Primary Filepath        (Read Only)
FIO_PATH_POS          equ    $FE63    ; (Byte) Character Position Within the Primary Filepath
FIO_PATH_DATA         equ    $FE64    ; (Byte) Data at the Character Position of the Primary Path
                                      ; 
FIO_PATH_LEN          equ    $FE65    ; (Byte) Length of the Primary Filepath        (Read Only)
FIO_ALT_PATH_POS      equ    $FE66    ; (Byte) Character Position Within the Alternate Filepath
FIO_ALT_PATH_DATA     equ    $FE67    ; (Byte) Data at the Character Position of the Alternate Path
                                      ; 
FIO_DIR_DATA          equ    $FE68    ; (Byte) A Series of Null-Terminated Filenames
                                      ;   NOTE: Current read-position is reset to the beginning
                                      ;     following a List Directory command. The read-position
                                      ;     is automatically advanced on read from this register.
                                      ;     Each filename is $0A-terminated. The list itself is
                                      ;     null-terminated.
                                      ; 
FIO_END               equ    $FE68    ; End of FIO Device Register Space
FIO_TOP               equ    $FE69    ; Top of FIO Device Register Space
; _______________________________________________________________________

MATH_DEVICE           equ    $FE69    ; START: Math Co-Processor Device Hardware Registers
MATH_ACA_POS          equ    $FE69    ; (Byte) Character Position Within the ACA Float String
MATH_ACA_DATA         equ    $FE6A    ; (Byte) ACA Float String Character Port
MATH_ACA_RAW          equ    $FE6B    ; (4-Bytes) ACA Raw Float Data
MATH_ACA_INT          equ    $FE6F    ; (4-Bytes) ACA Integer Data
                                      ; 
MATH_ACB_POS          equ    $FE73    ; (Byte) Character Position Within the ACB Float String
                                      ; 
MATH_ACB_DATA         equ    $FE74    ; (Byte) ACB Float String Character Port
MATH_ACB_RAW          equ    $FE75    ; (4-Bytes) ACB Raw Float Data
MATH_ACB_INT          equ    $FE79    ; (4-Bytes) ACB Integer Data
                                      ; 
MATH_ACR_POS          equ    $FE7D    ; (Byte) Character Position Within the ACR Float String
                                      ; 
MATH_ACR_DATA         equ    $FE7E    ; (Byte) ACR Float String Character Port
MATH_ACR_RAW          equ    $FE7F    ; (4-Bytes) ACR Raw Float Data
MATH_ACR_INT          equ    $FE83    ; (4-Bytes) ACR Integer Data
                                      ; 
MATH_OPERATION        equ    $FE87    ; (Byte) ACA Float String Character Port   (On Write)
MOP_BEGIN             equ    $0000    ;   BEGIN Math Operation Enumeration:
MOP_RANDOM            equ    $0001    ;     ACA, ACB, and ACR are set to randomized values
MOP_RND_SEED          equ    $0002    ;     MATH_ACA_INT seeds the pseudo-random number generator
MOP_IS_EQUAL          equ    $0003    ;     (bool)ACR = (ACA == ACB)
MOP_IS_NOT_EQUAL      equ    $0004    ;     (bool)ACR = (ACA != ACB)
MOP_IS_LESS           equ    $0005    ;     (bool)ACR = std::isless(ACA, ACB)
MOP_IS_GREATER        equ    $0006    ;     (bool)ACR = std::isgreater(ACA, ACB)
MOP_IS_LTE            equ    $0007    ;     (bool)ACR = std::islessequal(ACA, ACB)
MOP_IS_GTE            equ    $0008    ;     (bool)ACR = std::islessgreater(ACA, ACB)
MOP_IS_FINITE         equ    $0009    ;     (bool)ACR = std::isfinite(ACA)
MOP_IS_INF            equ    $000A    ;     (bool)ACR = std::isinf(ACA)
MOP_IS_NAN            equ    $000B    ;     (bool)ACR = std::isnan(ACA)
MOP_IS_NORMAL         equ    $000C    ;     (bool)ACR = std::isnormal(ACA)
MOP_SIGNBIT           equ    $000D    ;     (bool)ACR = std::signbit(ACA)
MOP_SUBTRACT          equ    $000E    ;     ACR = ACA - ACB
MOP_ADD               equ    $000F    ;     ACR = ACA + ACB
MOP_MULTIPLY          equ    $0010    ;     ACR = ACA * ACB
MOP_DIVIDE            equ    $0011    ;     ACR = ACA / ACB
MOP_FMOD              equ    $0012    ;     ACR = std::fmod(ACA, ACB)
MOP_REMAINDER         equ    $0013    ;     ACR = std::remainder(ACA, ACB)
MOP_FMAX              equ    $0014    ;     ACR = std::fmax(ACA, ACB)
MOP_FMIN              equ    $0015    ;     ACR = std::fmin(ACA, ACB)
MOP_FDIM              equ    $0016    ;     ACR = std::fdim(ACA, ACB)
MOP_EXP               equ    $0017    ;     ACR = std::exp(ACA)
MOP_EXP2              equ    $0018    ;     ACR = std::exp2(ACA)
MOP_EXPM1             equ    $0019    ;     ACR = std::expm1(ACA)
MOP_LOG               equ    $001A    ;     ACR = std::log(ACA)
MOP_LOG10             equ    $001B    ;     ACR = std::log10(ACA)
MOP_LOG2              equ    $001C    ;     ACR = std::log2(ACA)
MOP_LOG1P             equ    $001D    ;     ACR = std::log1p(ACA)
MOP_SQRT              equ    $001E    ;     ACR = std::sqrt(ACA)
MOP_CBRT              equ    $001F    ;     ACR = std::cbrt(ACA)
MOP_HYPOT             equ    $0020    ;     ACR = std::hypot(ACA, ACB)
MOP_POW               equ    $0021    ;     ACR = std::pow(ACA, ACB)
MOP_SIN               equ    $0022    ;     ACR = std::sin(ACA)
MOP_COS               equ    $0023    ;     ACR = std::cos(ACA)
MOP_TAN               equ    $0024    ;     ACR = std::tan(ACA)
MOP_ASIN              equ    $0025    ;     ACR = std::asin(ACA)
MOP_ACOS              equ    $0026    ;     ACR = std::acos(ACA)
MOP_ATAN              equ    $0027    ;     ACR = std::atan(ACA)
MOP_ATAN2             equ    $0028    ;     ACR = std::atan2(ACA, ACB)
MOP_SINH              equ    $0029    ;     ACR = std::sinh(ACA)
MOP_COSH              equ    $002A    ;     ACR = std::acosh(ACA)
MOP_ATANH             equ    $002B    ;     ACR = std::atanh(ACA)
MOP_ERF               equ    $002C    ;     ACR = std::erf(ACA)
MOP_ERFC              equ    $002D    ;     ACR = std::erfc(ACA)
MOP_LGAMMA            equ    $002E    ;     ACR = std::lgamma(ACA)
MOP_TGAMMA            equ    $002F    ;     ACR = std::tgamma(ACA)
MOP_CEIL              equ    $0030    ;     ACR = std::ceil(ACA)
MOP_FLOOR             equ    $0031    ;     ACR = std::floor(ACA)
MOP_TRUNC             equ    $0032    ;     ACR = std::trunc(ACA)
MOP_ROUND             equ    $0033    ;     ACR = std::round(ACA)
MOP_LROUND            equ    $0034    ;     ACR = std::lround(ACA)
MOP_NEARBYINT         equ    $0035    ;     ACR = std::nearbyint(ACA)
MOP_ILOGB             equ    $0036    ;     ACR = std::ilogb(ACA)
MOP_LOGB              equ    $0037    ;     ACR = std::logb(ACA)
MOP_NEXTAFTER         equ    $0038    ;     ACR = std::nextafter(ACA, ACB)
MOP_COPYSIGN          equ    $0039    ;     ACR = std::copysign(ACA, ACB)
MOP_LASTOP            equ    $003A    ;   END Math Operation Enumeration
MATH_END              equ    $FE87    ; End of Math Co-Processor Register Space
MATH_TOP              equ    $FE88    ; Top of Math Co-Processor Register Space
; _______________________________________________________________________

HDW_RESERVED_DEVICE   equ    $FE88    ; START: Reserved Register Space
HDW_REG_END           equ    $FFEF    ; 359 bytes reserved for future use.
; _______________________________________________________________________

ROM_VECTS_DEVICE      equ    $FFEF    ; START: Hardware Interrupt Vectors
HARD_EXEC             equ    $FFEF    ; EXEC Hardware Interrupt Vector
HARD_SWI3             equ    $FFF1    ; SWI3 Hardware Interrupt Vector
HARD_SWI2             equ    $FFF3    ; SWI2 Hardware Interrupt Vector
HARD_FIRQ             equ    $FFF5    ; FIRQ Hardware Interrupt Vector
HARD_IRQ              equ    $FFF7    ; IRQ Hardware Interrupt Vector
HARD_SWI              equ    $FFF9    ; SWI / SYS Hardware Interrupt Vector
HARD_NMI              equ    $FFFB    ; NMI Hardware Interrupt Vector
HARD_RESET            equ    $FFFD    ; RESET Hardware Interrupt Vector


; END of memory_map.asm definitions


