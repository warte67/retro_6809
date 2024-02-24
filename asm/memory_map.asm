;  **********************************************
;  * Allocated 64k Memory Mapped System Symbols *
;  **********************************************
SOFT_VECTORS        equ   $0000  
        
          ; Software Interrupt Vectors:
SOFT_EXEC           equ   $0000    ; Exec Software Interrupt Vector
SOFT_SWI3           equ   $0002    ; SWI3 Software Interrupt Vector
SOFT_SWI2           equ   $0004    ; SWI2 Software Interrupt Vector
SOFT_FIRQ           equ   $0006    ; FIRQ Software Interrupt Vector
SOFT_IRQ            equ   $0008    ; IRQ Software Interrupt Vector
SOFT_SWI            equ   $000A    ; SWI / SYS Software Interrupt Vector
SOFT_NMI            equ   $000C    ; NMI Software Interrupt Vector
SOFT_RESET          equ   $000E    ; RESET Software Interrupt Vector
        
ZERO_PAGE           equ   $0010  
FIO_BUFFER          equ   $0100  
FIO_BFR_TOP         equ   $01FF    ; Top of the File Input/Output Buffer
SYSTEM_STACK        equ   $0200  
SSTACK_TOP          equ   $0400    ; Top of the system stack space
        
          ; Device Registers:
HDW_REGS            equ   $0400    ; Begin Device Hardware Registers
STD_VID_MIN         equ   $0400    ; Start of Standard Video Buffer Memory
STD_VID_MAX         equ   $2400    ;  (Word) Standard Video Buffer Max
SYS_STATE           equ   $2402    ;  (Byte) System State Register
          ; SYS_STATE: ABCD.SSSS
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
        
SYS_SPEED           equ   $2403    ;  (Word) Approx. Average CPU Clock Speed
SYS_CLOCK_DIV       equ   $2405    ;  (Byte) 60 hz Clock Divider Register (Read Only) 
          ; SYS_CLOCK_DIV:
          ;      bit 7: 0.46875 hz
          ;      bit 6: 0.9375 hz
          ;      bit 5: 1.875 hz
          ;      bit 4: 3.75 hz
          ;      bit 3: 7.5 hz
          ;      bit 2: 15.0 hz
          ;      bit 1: 30.0 hz
          ;      bit 0: 60.0 hz
        
SYS_TIMER           equ   $2406    ;  (Word) Increments at 0.46875 hz
EMU_FLAGS           equ   $2408    ;  (Byte) Emulation Flags 
          ; EMU_FLAGS: xxxx.xxFG
          ;      x     = reserved 
          ;      F:0   = VSYNC OFF 
          ;      F:1   = VSYNC ON 
          ;      G:0   = Fullscreen Enabled ( emulator only ) 
          ;      G:1   = Windowed Enabled ( emulator only ) 
        
DSP_RES             equ   $2409    ;  (Byte) Display Resolution Timing 0-255
DSP_MODE            equ   $240A    ;  (Byte) Graphics Display Mode
          ; DSP_MODE: ABCD.EEFF
          ;      A:0   = Extended Bitmap Mode 
          ;      A:1   = Extended Tile Mode 
          ;      B:1   = Standard Bitmap Mode 
          ;      B:0   = Standard Text Mode 
          ;      C:1   = Extended Graphics Enabled 
          ;      C:0   = Extended Graphics Disabled 
          ;      D:1   = Standard Graphics Enabled 
          ;      D:0   = Standard Graphics Disabled 
          ;     EE:00  = Extended Graphics   2-Color 
          ;     EE:01  = Extended Graphics   4-Color 
          ;     EE:10  = Extended Graphics  16-Color 
          ;     EE:11  = Extended Graphics 256-Color 
          ;     FF:00  = Standard Graphics   2-Color 
          ;     FF:01  = Standard Graphics   4-Color 
          ;     FF:10  = Standard Graphics  16-Color 
          ;     FF:11  = Standard Graphics 256-Color 
        
DSP_TXT_COLS        equ   $240B    ;  (Byte) READ-ONLY Text Screen Columns
DSP_TXT_ROWS        equ   $240C    ;  (Byte) READ-ONLY Text Screens Rows
        
DSP_PAL_IDX         equ   $240D    ;  (Byte) Color Palette Index
          ; DSP_PAL_IDX: 0-255
          ; Note: Use this register to set the index into theColor Palette. 
          ;   Set this value prior referencing color data (DSP_PAL_CLR).
        
DSP_PAL_CLR         equ   $240E    ;  (Word) Indexed Color Palette Data
          ; DSP_PAL_CLR: Color Data A4R4G4B4 format
          ; Note: This is the color data for an individual palette entry.
          ;     Write to DSP_PAL_IDX with the index within the color palette
          ;     prior to reading or writing the color data in the DSP_PAL_CLR register.
        
DSP_GLYPH_IDX       equ   $2410    ;  (Byte) Text Glyph Index
          ; DSP_GLYPH_IDX: 0-256
          ; Note: Set this register to index a specific text glyph. Set this value
          ;     prior to updating glyph pixel data.
        
DSP_GLYPH_DATA      equ   $2411    ;  (8-Bytes) Text Glyph Pixel Data Array
          ; DSP_GLYPH_DATA: 8 rows of binary encoded glyph pixel data
          ; Note: Each 8x8 text glyph is composed of 8 bytes. The first byte in this 
          ;     array represents the top line of 8 pixels. Each array entry represents
          ;     a row of 8 pixels. 
        
          ; Debug Hardware Registers:
DBG_BEGIN           equ   $2419    ; Start of Debug Hardware Registers
DBG_BRK_ADDR        equ   $2419    ;    (Word) Address of current breakpoint
DBG_FLAGS           equ   $241B    ;    (Byte) Debug Specific Hardware Flags:
          ;     bit 7: Debug Enable
          ;     bit 6: Single Step Enable
          ;     bit 5: Clear All Breakpoints
          ;     bit 4: Update Breakpoint at DEBUG_BRK_ADDR
          ;     bit 3: FIRQ  (on low to high edge)
          ;     bit 2: IRQ   (on low to high edge)
          ;     bit 1: NMI   (on low to high edge)
          ;     bit 0: RESET (on low to high edge)
DBG_END             equ   $241C    ; End Debug Registers
        
          ; Mouse Cursor Hardware Registers:
CSR_BEGIN           equ   $241C    ;  start of mouse cursor hardware registers
CSR_XPOS            equ   $241C    ;  (Word) horizontal mouse cursor coordinate
CSR_YPOS            equ   $241E    ;  (Word) vertical mouse cursor coordinate
CSR_XOFS            equ   $2420    ;  (Byte) horizontal mouse cursor offset
CSR_YOFS            equ   $2421    ;  (Byte) vertical mouse cursor offset
CSR_SCROLL          equ   $2422    ;  (Signed) MouseWheel Scroll: -1, 0, 1
CSR_FLAGS           equ   $2423    ;  (Byte) mouse button flags:
          ;  CSR_FLAGS:
          ;       bits 0-4: button states
          ;       bits 5:   cursor enable
          ;       bits 6-7: number of clicks
CSR_BMP_INDX        equ   $2424    ;  (Byte) mouse cursor bitmap pixel offset
CSR_BMP_DATA        equ   $2425    ;  (Byte) mouse cursor bitmap pixel index color
CSR_PAL_INDX        equ   $2427    ;  (Byte) mouse cursor color palette index (0-15)
CSR_PAL_DATA        equ   $2428    ;  (Word) mouse cursor color palette data RGBA4444
CSR_END             equ   $242A    ; End Mouse Registers
        
GFX_END             equ   $242A    ; End of GFX Device Registers
        
KEY_BEGIN           equ   $242A    ; Start of the Keyboard Register space
CHAR_Q_LEN          equ   $242A    ;   (Byte) # of characters waiting in queue        (Read Only)
CHAR_SCAN           equ   $242B    ;   (Byte) read next character in queue (not popped when read)
CHAR_POP            equ   $242C    ;   (Byte) read next character in queue (not popped when read)
XKEY_BUFFER         equ   $242D    ;   (128 bits) 16 bytes for XK_KEY data buffer     (Read Only)
EDT_BFR_CSR         equ   $243D    ;   (Byte) cursor position within edit buffer     (Read/Write)
EDT_ENABLE          equ   $243E    ;   (Byte) line editor enable flag                 (Read/Write)
EDT_BUFFER          equ   $243F    ;   line editing character buffer                 (Read/Write)
KEY_END             equ   $253F    ; End of the Keyboard Register space
        
JOYS_BEGIN          equ   $253F    ; Start of the Game Controller Register space
JOYS_1_BTN          equ   $253F    ;   (Word) button bits: room for up to 16 buttons  (realtime)
JOYS_1_DBND         equ   $2541    ;   (Byte) PAD 1 analog deadband; default is 5   (read/write)
JOYS_1_LTX          equ   $2542    ;   (char) PAD 1 LThumb-X position (-128 _ +127)   (realtime)
JOYS_1_LTY          equ   $2543    ;   (char) PAD 1 LThumb-Y position (-128 _ +127)   (realtime)
JOYS_1_RTX          equ   $2544    ;   (char) PAD 1 RThumb-X position (-128 _ +127)   (realtime)
JOYS_1_RTY          equ   $2545    ;   (char) PAD 1 RThumb-Y position (-128 _ +127)   (realtime)
JOYS_1_Z1           equ   $2546    ;   (char) PAD 1 left analog trigger (0 - 127)     (realtime)
JOYS_1_Z2           equ   $2547    ;   (char) PAD 1 right analog trigger (0 - 127)    (realtime)
        
JOYS_2_BTN          equ   $2548    ;   (Word) button bits: room for up to 16 buttons  (realtime)
JOYS_2_DBND         equ   $254A    ;   (Byte) PAD 2 analog deadband; default is 5   (read/write)
JOYS_2_LTX          equ   $254B    ;   (char) PAD 2 LThumb-X position (-128 _ +127)   (realtime)
JOYS_2_LTY          equ   $254C    ;   (char) PAD 2 LThumb-Y position (-128 _ +127)   (realtime)
JOYS_2_RTX          equ   $254D    ;   (char) PAD 2 RThumb-X position (-128 _ +127)   (realtime)
JOYS_2_RTY          equ   $254E    ;   (char) PAD 2 RThumb-Y position (-128 _ +127)   (realtime)
JOYS_2_Z1           equ   $254F    ;   (char) PAD 2 left analog trigger (0 - 127)     (realtime)
JOYS_2_Z2           equ   $2550    ;   (char) PAD 2 right analog trigger (0 - 127)    (realtime)
JOYS_END            equ   $2551    ; End of the Game Controller Register space
        
FIO_BEGIN           equ   $2551    ; Start of the FileIO register space
FIO_ERR_FLAGS       equ   $2551    ; (Byte) File IO error flags
          ; FIO_ERR_FLAGS: ABCD.EFGH
          ;      A:  file was not found
          ;      B:  directory was not found
          ;      C:  file not open
          ;      D:  end of file
          ;      E:  buffer overrun
          ;      F:  wrong file type
          ;      G:  invalid command
          ;      H:  incorrect file stream
        
FIO_COMMAND         equ   $2552    ; (Byte) OnWrite, execute a file command (FC_<cmd>)
          ; Begin FIO_COMMANDS
FC_RESET            equ   $0000    ;        Reset
FC_SHUTDOWN         equ   $0001    ;        SYSTEM: Shutdown
FC_COMPDATE         equ   $0002    ;        SYSTEM: Load Compilation Date
FC_NEWFILE          equ   $0003    ;      * New File Stream
FC_OPENFILE         equ   $0004    ;      * Open File
FC_ISOPEN           equ   $0005    ;      *Is File Open ? (returns FIO_ERR_FLAGS bit - 5)
FC_CLOSEFILE        equ   $0006    ;      * Close File
FC_READBYTE         equ   $0007    ;      * Read Byte (into FIO_IOBYTE)
FC_WRITEBYTE        equ   $0008    ;      * Write Byte (from FIO_IOBYTE)
FC_LOADHEX          equ   $0009    ;      * Load Hex Format File
FC_GETLENGTH        equ   $000A    ;      * Get File Length (into FIO_IOWORD)
FC_LISTDIR          equ   $000B    ;        List Directory
FC_MAKEDIR          equ   $000C    ;      * Make Directory
FC_CHANGEDIR        equ   $000D    ;        Change Directory
FC_GETPATH          equ   $000E    ;        Fetch Current Path
FC_REN_DIR          equ   $000F    ;      * Rename Directory
FC_DEL_DIR          equ   $0010    ;      * Delete Directory
FC_DEL_FILE         equ   $0011    ;      * Delete File
FC_REN_FILE         equ   $0012    ;      * Rename file
FC_COPYFILE         equ   $0013    ;      * Copy File
FC_SEEKSTART        equ   $0014    ;      * Seek Start
FC_SEEKEND          equ   $0015    ;      * Seek End
FC_SET_SEEK         equ   $0016    ;      * Set Seek Position (from FIO_IOWORD)
FC_GET_SEEK         equ   $0017    ;      * Get Seek Position (into FIO_IOWORD)
          ; End FIO_COMMANDS
        
FIO_STREAM          equ   $2553    ; (Byte) current file stream index (0-15)
FIO_MODE            equ   $2554    ; (Byte) Flags describing the I/O mode for the file
          ; FIO_MODE: 00AB.CDEF  (indexed by FIO_STREAM)
          ;      A:  INPUT - File open for reading
          ;      B:  OUTPUT - File open for writing
          ;      C:  BINARY - 1: Binary Mode, 0: Text Mode
          ;      D:  AT_END - Output starts at the end of the file
          ;      E:  APPEND - All output happens at end of the file
          ;      F:  TRUNC - discard all previous file data
FIO_SEEKPOS         equ   $2555    ; (DWord) file seek position
FIO_IOBYTE          equ   $2559    ; (Byte) input / output character
FIO_IOWORD          equ   $255A    ; (Byte) input / output character
FIO_PATH_LEN        equ   $255B    ; (Byte) length of the filepath
FIO_PATH_POS        equ   $255C    ; (Byte) character position within the filepath
FIO_PATH_DATA       equ   $255D    ; (Byte) data at the character position of the path
FIO_DIR_DATA        equ   $255E    ; (Byte) a series of null-terminated filenames
          ;     NOTES: Current read-position is reset to the beginning following a 
          ;             List Directory command. The read-position is automatically 
          ;             advanced on read from this register. Each filename is 
          ;             $0a-terminated. The list itself is null-terminated.
FIO_END             equ   $255F    ; End of the FileIO register space
        
          ; Math Co-Processor Hardware Registers:
MATH_BEGIN          equ   $255F    ;  start of math co-processor  hardware registers
MATH_ACA_POS        equ   $255F    ;  (Byte) character position within the ACA float string
MATH_ACA_DATA       equ   $2560    ;  (Byte) ACA float string character port
MATH_ACA_RAW        equ   $2561    ;  (4-Bytes) ACA raw float data
MATH_ACA_INT        equ   $2565    ;  (4-Bytes) ACA integer data
MATH_ACB_POS        equ   $2569    ;  (Byte) character position within the ACB float string
MATH_ACB_DATA       equ   $256A    ;  (Byte) ACB float string character port
MATH_ACB_RAW        equ   $256B    ;  (4-Bytes) ACB raw float data
MATH_ACB_INT        equ   $256F    ;  (4-Bytes) ACB integer data
MATH_ACR_POS        equ   $2573    ;  (Byte) character position within the ACR float string
MATH_ACR_DATA       equ   $2574    ;  (Byte) ACR float string character port
MATH_ACR_RAW        equ   $2575    ;  (4-Bytes) ACR raw float data
MATH_ACR_INT        equ   $2579    ;  (4-Bytes) ACR integer data
MATH_OPERATION      equ   $257D    ;  (Byte) Operation 'command' to be issued
          ; Begin MATH_OPERATION's (MOPS)
MOP_RANDOM          equ   $0000    ;        ACA, ACB, and ACR are set to randomized values
MOP_RND_SEED        equ   $0001    ;        MATH_ACA_INT seeds the pseudo-random number generator
MOP_IS_EQUAL        equ   $0002    ;        (bool)ACR = (ACA == ACB)
MOP_IS_NOT_EQUAL    equ   $0003    ;        (bool)ACR = (ACA != ACB)
MOP_IS_LESS         equ   $0004    ;        (bool)ACR = std::isless(ACA, ACB);
MOP_IS_GREATER      equ   $0005    ;        (bool)ACR = std::isgreater(ACA, ACB);
MOP_IS_LTE          equ   $0006    ;        (bool)ACR = std::islessequal(ACA, ACB);
MOP_IS_GTE          equ   $0007    ;        (bool)ACR = std::islessgreater(ACA, ACB);
MOP_IS_FINITE       equ   $0008    ;        (bool)ACR = std::isfinite(ACA);
MOP_IS_INF          equ   $0009    ;        (bool)ACR = std::isinf(ACA);
MOP_IS_NAN          equ   $000A    ;        (bool)ACR = std::isnan(ACA);
MOP_IS_NORMAL       equ   $000B    ;        (bool)ACR = std::isnormal(ACA);
MOP_SIGNBIT         equ   $000C    ;        (bool)ACR = std::signbit(ACA);
MOP_SUBTRACT        equ   $000D    ;        ACR = ACA - ACB
MOP_ADD             equ   $000E    ;        ACR = ACA + ACB
MOP_MULTIPLY        equ   $000F    ;        ACR = ACA * ACB
MOP_DIVIDE          equ   $0010    ;        ACR = ACA / ACB
MOP_FMOD            equ   $0011    ;        ACR = std::fmod(ACA, ACB);
MOP_REMAINDER       equ   $0012    ;        ACR = std::remainder(ACA, ACB);
MOP_FMAX            equ   $0013    ;        ACR = std::fmax(ACA, ACB);
MOP_FMIN            equ   $0014    ;        ACR = std::fmin(ACA, ACB);
MOP_FDIM            equ   $0015    ;        ACR = std::fdim(ACA, ACB);
MOP_EXP             equ   $0016    ;        ACR = std::exp(ACA);
MOP_EXP2            equ   $0017    ;        ACR = std::exp2(ACA);
MOP_EXPM1           equ   $0018    ;        ACR = std::expm1(ACA);
MOP_LOG             equ   $0019    ;        ACR = std::log(ACA);
MOP_LOG10           equ   $001A    ;        ACR = std::log10(ACA);
MOP_LOG2            equ   $001B    ;        ACR = std::log2(ACA);
MOP_LOG1P           equ   $001C    ;        ACR = std::log1p(ACA);
MOP_SQRT            equ   $001D    ;        ACR = std::sqrt(ACA);
MOP_CBRT            equ   $001E    ;        ACR = std::cbrt(ACA);
MOP_HYPOT           equ   $001F    ;        ACR = std::hypot(ACA, ACB);
MOP_POW             equ   $0020    ;        ACR = std::pow(ACA, ACB);
MOP_SIN             equ   $0021    ;        ACR = std::sin(ACA);
MOP_COS             equ   $0022    ;        ACR = std::cos(ACA);
MOP_TAN             equ   $0023    ;        ACR = std::tan(ACA);
MOP_ASIN            equ   $0024    ;        ACR = std::asin(ACA);
MOP_ACOS            equ   $0025    ;        ACR = std::acos(ACA);
MOP_ATAN            equ   $0026    ;        ACR = std::atan(ACA);
MOP_ATAN2           equ   $0027    ;        ACR = std::atan2(ACA, ACB);
MOP_SINH            equ   $0028    ;        ACR = std::sinh(ACA);
MOP_COSH            equ   $0029    ;        ACR = std::acosh(ACA);
MOP_ATANH           equ   $002A    ;        ACR = std::atanh(ACA);
MOP_ERF             equ   $002B    ;        ACR = std::erf(ACA);
MOP_ERFC            equ   $002C    ;        ACR = std::erfc(ACA);
MOP_LGAMMA          equ   $002D    ;        ACR = std::lgamma(ACA);
MOP_TGAMMA          equ   $002E    ;        ACR = std::tgamma(ACA);
MOP_CEIL            equ   $002F    ;        ACR = std::ceil(ACA);
MOP_FLOOR           equ   $0030    ;        ACR = std::floor(ACA);
MOP_TRUNC           equ   $0031    ;        ACR = std::trunc(ACA);
MOP_ROUND           equ   $0032    ;        ACR = std::round(ACA);
MOP_LROUND          equ   $0033    ;        ACR = std::lround(ACA);
MOP_NEARBYINT       equ   $0034    ;        ACR = std::nearbyint(ACA);
MOP_ILOGB           equ   $0035    ;        ACR = std::ilogb(ACA);
MOP_LOGB            equ   $0036    ;        ACR = std::logb(ACA);
MOP_NEXTAFTER       equ   $0037    ;        ACR = std::nextafter(ACA, ACB);
MOP_COPYSIGN        equ   $0038    ;        ACR = std::copysign(ACA, ACB);
MOP_LASTOP          equ   $0038    ;        last implemented math operation 
          ; End MATH_OPERATION's (MOPS)
MATH_END            equ   $257E    ; end of math co-processor registers
        
    ; 2690 ($0A82) bytes remaining for additional registers.
RESERVED            equ   $257E  
        
          ; User RAM (32K)
USER_RAM            equ   $3000  
        
          ; Paged RAM (8K)
PAGED_RAM           equ   $B000  
        
          ; PAGED ROM (8K bytes)
PAGED_ROM           equ   $D000  
        
          ; KERNEL ROM (4K bytes)
KERNEL_ROM          equ   $F000  
        
          ; Hardware Interrupt Vectors:
HARD_EXEC           equ   $FFF0    ; EXEC Hardware Interrupt Vector
HARD_SWI3           equ   $FFF2    ; SWI3 Hardware Interrupt Vector
HARD_SWI2           equ   $FFF4    ; SWI2 Hardware Interrupt Vector
HARD_FIRQ           equ   $FFF6    ; FIRQ Hardware Interrupt Vector
HARD_IRQ            equ   $FFF8    ; IRQ Hardware Interrupt Vector
HARD_SWI            equ   $FFFA    ; SWI / SYS Hardware Interrupt Vector
HARD_NMI            equ   $FFFC    ; NMI Hardware Interrupt Vector
HARD_RESET          equ   $FFFE    ; RESET Hardware Interrupt Vector
; END of definitions
