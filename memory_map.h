// memory_map.h
#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__


// Simple Memory Map :
//    $0000 - $000F SOFT_VECTORS
//    $0010 - $00FF ZERO_PAGE
//    $0100 - $01FF FIO_BUFFER
//    $0200 - $03FF SYSTEM_STACK
//    $0400 - $1C28 GFX_DEVICE
//    $1C29 - $1C7D Keyboard
//    $1C7E - $1C8F Gamepad
//    $1C90 - $1C9D FileIO
//    $1C9E - $1CBC Math
//    $1CBD - $2FFF RESERVED
//    $3000 - $AFFF USER_RAM
//    $B000 - $CFFF PAGED_RAM
//    $D000 - $EFFF PAGED_ROM
//    $F000 - $FFFF KERNEL_ROM

enum MEMMAP
{
    //  **********************************************
    //  * Allocated 64k Memory Mapped System Symbols *
    //  **********************************************
SOFT_VECTORS     = 0x0000,

        // Software Interrupt Vectors:
SOFT_EXEC        = 0x0000, // Exec Software Interrupt Vector
SOFT_SWI3        = 0x0002, // SWI3 Software Interrupt Vector
SOFT_SWI2        = 0x0004, // SWI2 Software Interrupt Vector
SOFT_FIRQ        = 0x0006, // FIRQ Software Interrupt Vector
SOFT_IRQ         = 0x0008, // IRQ Software Interrupt Vector
SOFT_SWI         = 0x000A, // SWI / SYS Software Interrupt Vector
SOFT_NMI         = 0x000C, // NMI Software Interrupt Vector
SOFT_RESET       = 0x000E, // RESET Software Interrupt Vector

ZERO_PAGE        = 0x0010,
FIO_BUFFER       = 0x0100,
FIO_BFR_TOP      = 0x01FF, // Top of the File Input/Output Buffer
SYSTEM_STACK     = 0x0200,
SSTACK_TOP       = 0x0400, // Top of the system stack space

        // Device Registers:
HDW_REGS         = 0x0400, // Begin Device Hardware Registers

STD_VID_MIN      = 0x0400, // Start of Standard Video Buffer Memory
STD_VID_MAX      = 0x1C00, //  (Word) Standard Video Buffer Max

SYS_STATE        = 0x1C02, //  (Byte) System State Register
        // SYS_STATE: ABCD.SSSS
        //      A:0   = Error: Standard Buffer Overflow
        //      B:0   = Error: Extended Buffer Overflow
        //      C:0   = Error: Reserved
        //      D:0   = Error: Reserved
        //      S:$0  = CPU Clock  25 khz.
        //      S:$1  = CPU Clock  50 khz.
        //      S:$2  = CPU Clock 100 khz.
        //      S:$3  = CPU Clock 200 khz.
        //      S:$4  = CPU Clock 333 khz.
        //      S:$5  = CPU Clock 416 khz.
        //      S:$6  = CPU Clock 500 khz.
        //      S:$7  = CPU Clock 625 khz.
        //      S:$8  = CPU Clock 769 khz.
        //      S:$9  = CPU Clock 833 khz.
        //      S:$A  = CPU Clock 1.0 mhz.
        //      S:$B  = CPU Clock 1.4 mhz.
        //      S:$C  = CPU Clock 2.0 mhz.
        //      S:$D  = CPU Clock 3.3 mhz.
        //      S:$E  = CPU Clock 5.0 mhz.
        //      S:$F  = CPU Clock ~10.0 mhz. (unmetered)

SYS_SPEED        = 0x1C03, //  (Word) Approx. Average CPU Clock Speed

SYS_CLOCK_DIV    = 0x1C05, //  (Byte) 60 hz Clock Divider Register (Read Only)
        // SYS_CLOCK_DIV: ABCD.SSSS
        //      bit 7: 0.46875 hz
        //      bit 6: 0.9375 hz
        //      bit 5: 1.875 hz
        //      bit 4: 3.75 hz
        //      bit 3: 7.5 hz
        //      bit 2: 15.0 hz
        //      bit 1: 30.0 hz
        //      bit 0: 60.0 hz

SYS_TIMER        = 0x1C06, //  (Word) Increments at 0.46875 hz

DSP_GRES         = 0x1C08, //  (Byte) Screen Resolution Register
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

DSP_EXT          = 0x1C09, //  (Byte) Extended Graphics Register
        // DSP_EXT: AABC.DEFG
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
        //      G:0   = Fullscreen Enabled( emulator only )
        //      G:1   = Windowed Enabled ( emulator only )

DSP_TXT_COLS     = 0x1C0A, //  (Byte) READ-ONLY Text Screen Columns
DSP_TXT_ROWS     = 0x1C0B, //  (Byte) READ-ONLY Text Screens Rows

DSP_PAL_IDX      = 0x1C0C, //  (Byte) Color Palette Index
        // DSP_PAL_IDX: 0-255
        // Note: Use this register to set the index into theColor Palette.
        //   Set this value prior referencing color data (DSP_PAL_CLR).

DSP_PAL_CLR      = 0x1C0D, //  (Word) Indexed Color Palette Data
        // DSP_PAL_CLR: Color Data A4R4G4B4 format
        // Note: This is the color data for an individual palette entry.
        //     Write to DSP_PAL_IDX with the index within the color palette
        //     prior to reading or writing the color data in the DSP_PAL_CLR register.

DSP_GLYPH_IDX    = 0x1C0F, //  (Byte) Text Glyph Index
        // DSP_GLYPH_IDX: 0-256
        // Note: Set this register to index a specific text glyph. Set this value
        //     prior to updating glyph pixel data.

DSP_GLYPH_DATA   = 0x1C10, //  (8-Bytes) Text Glyph Pixel Data Array
        // DSP_GLYPH_DATA: 8 rows of binary encoded glyph pixel data
        // Note: Each 8x8 text glyph is composed of 8 bytes. The first byte in this
        //     array represents the top line of 8 pixels. Each array entry represents
        //     a row of 8 pixels.

        // Debug Hardware Registers:
DBG_BEGIN        = 0x1C18, // Start of Debug Hardware Registers
DBG_BRK_ADDR     = 0x1C18, //    (Word) Address of current breakpoint
DBG_FLAGS        = 0x1C1A, //    (Byte) Debug Specific Hardware Flags:
        //     bit 7: Debug Enable
        //     bit 6: Single Step Enable
        //     bit 5: Clear All Breakpoints
        //     bit 4: Update Breakpoint at DEBUG_BRK_ADDR
        //     bit 3: FIRQ  (on low to high edge)
        //     bit 2: IRQ   (on low to high edge)
        //     bit 1: NMI   (on low to high edge)
        //     bit 0: RESET (on low to high edge)
DBG_END          = 0x1C1B, // End Debug Registers

        // Mouse Cursor Hardware Registers:
CSR_BEGIN        = 0x1C1B, //  start of mouse cursor hardware registers
CSR_XPOS         = 0x1C1B, //  (Word) horizontal mouse cursor coordinate
CSR_YPOS         = 0x1C1D, //  (Word) vertical mouse cursor coordinate
CSR_XOFS         = 0x1C1F, //  (Byte) horizontal mouse cursor offset
CSR_YOFS         = 0x1C20, //  (Byte) vertical mouse cursor offset
CSR_SCROLL       = 0x1C21, //  (Signed) MouseWheel Scroll: -1, 0, 1
CSR_FLAGS        = 0x1C22, //  (Byte) mouse button flags:
        //  CSR_FLAGS:
        //       bits 0-4: button states
        //       bits 5:   cursor enable
        //       bits 6-7: number of clicks
CSR_BMP_INDX     = 0x1C23, //  (Byte) mouse cursor bitmap pixel offset
CSR_BMP_DATA     = 0x1C24, //  (Byte) mouse cursor bitmap pixel index color
CSR_PAL_INDX     = 0x1C26, //  (Byte) mouse cursor color palette index (0-15)
CSR_PAL_DATA     = 0x1C27, //  (Word) mouse cursor color palette data RGBA4444
CSR_END          = 0x1C29, // End Mouse Registers

GFX_END          = 0x1C29, // End of GFX Device Registers

KEY_BEGIN        = 0x1C29, // Start of the Keyboard Register space
CHAR_Q_LEN       = 0x1C29, //   (Byte) # of characters waiting in queue        (Read Only)
CHAR_SCAN        = 0x1C2A, //   (Byte) read next character in queue (not popped when read)
CHAR_POP         = 0x1C2B, //   (Byte) read next character in queue (not popped when read)
XKEY_BUFFER      = 0x1C2C, //   (128 bits) 16 bytes for XK_KEY data buffer     (Read Only)
EDT_BFR_CSR      = 0x1C3C, //   (Byte) cursor position within edit buffer     (Read/Write)
EDT_ENABLE       = 0x1C3D, //   (Byte) line editor enable flag                 (Read/Write)
EDT_BUFFER       = 0x1C3E, //   line editing character buffer                 (Read/Write)
KEY_END          = 0x1C7E, // End of the Keyboard Register space

JOYS_BEGIN       = 0x1C7E, // Start of the Game Controller Register space
JOYS_1_BTN       = 0x1C7E, //   (Word) button bits: room for up to 16 buttons  (realtime)
JOYS_1_DBND      = 0x1C80, //   (Byte) PAD 1 analog deadband; default is 5   (read/write)
JOYS_1_LTX       = 0x1C81, //   (char) PAD 1 LThumb-X position (-128 _ +127)   (realtime)
JOYS_1_LTY       = 0x1C82, //   (char) PAD 1 LThumb-Y position (-128 _ +127)   (realtime)
JOYS_1_RTX       = 0x1C83, //   (char) PAD 1 RThumb-X position (-128 _ +127)   (realtime)
JOYS_1_RTY       = 0x1C84, //   (char) PAD 1 RThumb-Y position (-128 _ +127)   (realtime)
JOYS_1_Z1        = 0x1C85, //   (char) PAD 1 left analog trigger (0 - 127)     (realtime)
JOYS_1_Z2        = 0x1C86, //   (char) PAD 1 right analog trigger (0 - 127)    (realtime)

JOYS_2_BTN       = 0x1C87, //   (Word) button bits: room for up to 16 buttons  (realtime)
JOYS_2_DBND      = 0x1C89, //   (Byte) PAD 2 analog deadband; default is 5   (read/write)
JOYS_2_LTX       = 0x1C8A, //   (char) PAD 2 LThumb-X position (-128 _ +127)   (realtime)
JOYS_2_LTY       = 0x1C8B, //   (char) PAD 2 LThumb-Y position (-128 _ +127)   (realtime)
JOYS_2_RTX       = 0x1C8C, //   (char) PAD 2 RThumb-X position (-128 _ +127)   (realtime)
JOYS_2_RTY       = 0x1C8D, //   (char) PAD 2 RThumb-Y position (-128 _ +127)   (realtime)
JOYS_2_Z1        = 0x1C8E, //   (char) PAD 2 left analog trigger (0 - 127)     (realtime)
JOYS_2_Z2        = 0x1C8F, //   (char) PAD 2 right analog trigger (0 - 127)    (realtime)
JOYS_END         = 0x1C90, // End of the Game Controller Register space

FIO_BEGIN        = 0x1C90, // Start of the FileIO register space
FIO_ERR_FLAGS    = 0x1C90, // (Byte) File IO error flags
        // FIO_ERR_FLAGS: ABCD.EFGH
        //      A:  file was not found
        //      B:  directory was not found
        //      C:  file not open
        //      D:  end of file
        //      E:  buffer overrun
        //      F:  wrong file type
        //      G:  invalid command
        //      H:  incorrect file stream

FIO_COMMAND      = 0x1C91, // (Byte) OnWrite, execute a file command (FC_<cmd>)
        // Begin FIO_COMMANDS
FC_RESET         = 0x0000, //        Reset
FC_SHUTDOWN      = 0x0001, //        SYSTEM: Shutdown
FC_COMPDATE      = 0x0002, //        SYSTEM: Load Compilation Date
FC_NEWFILE       = 0x0003, //      * New File Stream
FC_OPENFILE      = 0x0004, //      * Open File
FC_ISOPEN        = 0x0005, //      *Is File Open ? (returns FIO_ERR_FLAGS bit - 5)
FC_CLOSEFILE     = 0x0006, //      * Close File
FC_READBYTE      = 0x0007, //      * Read Byte (into FIO_IOBYTE)
FC_WRITEBYTE     = 0x0008, //      * Write Byte (from FIO_IOBYTE)
FC_LOADHEX       = 0x0009, //      * Load Hex Format File
FC_GETLENGTH     = 0x000A, //      * Get File Length (into FIO_IOWORD)
FC_LISTDIR       = 0x000B, //        List Directory
FC_MAKEDIR       = 0x000C, //      * Make Directory
FC_CHANGEDIR     = 0x000D, //        Change Directory
FC_GETPATH       = 0x000E, //        Fetch Current Path
FC_REN_DIR       = 0x000F, //      * Rename Directory
FC_DEL_DIR       = 0x0010, //      * Delete Directory
FC_DEL_FILE      = 0x0011, //      * Delete File
FC_REN_FILE      = 0x0012, //      * Rename file
FC_COPYFILE      = 0x0013, //      * Copy File
FC_SEEKSTART     = 0x0014, //      * Seek Start
FC_SEEKEND       = 0x0015, //      * Seek End
FC_SET_SEEK      = 0x0016, //      * Set Seek Position (from FIO_IOWORD)
FC_GET_SEEK      = 0x0017, //      * Get Seek Position (into FIO_IOWORD)
        // End FIO_COMMANDS

FIO_STREAM       = 0x1C92, // (Byte) current file stream index (0-15)
FIO_MODE         = 0x1C93, // (Byte) Flags describing the I/O mode for the file
        // FIO_MODE: 00AB.CDEF  (indexed by FIO_STREAM)
        //      A:  INPUT - File open for reading
        //      B:  OUTPUT - File open for writing
        //      C:  BINARY - 1: Binary Mode, 0: Text Mode
        //      D:  AT_END - Output starts at the end of the file
        //      E:  APPEND - All output happens at end of the file
        //      F:  TRUNC - discard all previous file data
FIO_SEEKPOS      = 0x1C94, // (DWord) file seek position
FIO_IOBYTE       = 0x1C98, // (Byte) input / output character
FIO_IOWORD       = 0x1C99, // (Byte) input / output character
FIO_PATH_LEN     = 0x1C9A, // (Byte) length of the filepath
FIO_PATH_POS     = 0x1C9B, // (Byte) character position within the filepath
FIO_PATH_DATA    = 0x1C9C, // (Byte) data at the character position of the path
FIO_DIR_DATA     = 0x1C9D, // (Byte) a series of null-terminated filenames
        //     NOTES: Current read-position is reset to the beginning following a
        //             List Directory command. The read-position is automatically
        //             advanced on read from this register. Each filename is
        //             $0a-terminated. The list itself is null-terminated.
FIO_END          = 0x1C9E, // End of the FileIO register space

        // Math Co-Processor Hardware Registers:
MATH_BEGIN       = 0x1C9E, //  start of math co-processor  hardware registers
MATH_ACA_POS     = 0x1C9E, //  (Byte) character position within the ACA float string
MATH_ACA_DATA    = 0x1C9F, //  (Byte) ACA float string character port
MATH_ACA_RAW     = 0x1CA0, //  (4-Bytes) ACA raw float data
MATH_ACA_INT     = 0x1CA4, //  (4-Bytes) ACA integer data
MATH_ACB_POS     = 0x1CA8, //  (Byte) character position within the ACB float string
MATH_ACB_DATA    = 0x1CA9, //  (Byte) ACB float string character port
MATH_ACB_RAW     = 0x1CAA, //  (4-Bytes) ACB raw float data
MATH_ACB_INT     = 0x1CAE, //  (4-Bytes) ACB integer data
MATH_ACR_POS     = 0x1CB2, //  (Byte) character position within the ACR float string
MATH_ACR_DATA    = 0x1CB3, //  (Byte) ACR float string character port
MATH_ACR_RAW     = 0x1CB4, //  (4-Bytes) ACR raw float data
MATH_ACR_INT     = 0x1CB8, //  (4-Bytes) ACR integer data
MATH_OPERATION   = 0x1CBC, //  (Byte) Operation 'command' to be issued
        // Begin MATH_OPERATION's (MOPS)
MOP_RANDOM       = 0x0000, //        ACA, ACB, and ACR are set to randomized values
MOP_RND_SEED     = 0x0001, //        MATH_ACA_INT seeds the pseudo-random number generator
MOP_IS_EQUAL     = 0x0002, //        (bool)ACR = (ACA == ACB)
MOP_IS_NOT_EQUAL = 0x0003, //        (bool)ACR = (ACA != ACB)
MOP_IS_LESS      = 0x0004, //        (bool)ACR = std::isless(ACA, ACB);
MOP_IS_GREATER   = 0x0005, //        (bool)ACR = std::isgreater(ACA, ACB);
MOP_IS_LTE       = 0x0006, //        (bool)ACR = std::islessequal(ACA, ACB);
MOP_IS_GTE       = 0x0007, //        (bool)ACR = std::islessgreater(ACA, ACB);
MOP_IS_FINITE    = 0x0008, //        (bool)ACR = std::isfinite(ACA);
MOP_IS_INF       = 0x0009, //        (bool)ACR = std::isinf(ACA);
MOP_IS_NAN       = 0x000A, //        (bool)ACR = std::isnan(ACA);
MOP_IS_NORMAL    = 0x000B, //        (bool)ACR = std::isnormal(ACA);
MOP_SIGNBIT      = 0x000C, //        (bool)ACR = std::signbit(ACA);
MOP_SUBTRACT     = 0x000D, //        ACR = ACA - ACB
MOP_ADD          = 0x000E, //        ACR = ACA + ACB
MOP_MULTIPLY     = 0x000F, //        ACR = ACA * ACB
MOP_DIVIDE       = 0x0010, //        ACR = ACA / ACB
MOP_FMOD         = 0x0011, //        ACR = std::fmod(ACA, ACB);
MOP_REMAINDER    = 0x0012, //        ACR = std::remainder(ACA, ACB);
MOP_FMAX         = 0x0013, //        ACR = std::fmax(ACA, ACB);
MOP_FMIN         = 0x0014, //        ACR = std::fmin(ACA, ACB);
MOP_FDIM         = 0x0015, //        ACR = std::fdim(ACA, ACB);
MOP_EXP          = 0x0016, //        ACR = std::exp(ACA);
MOP_EXP2         = 0x0017, //        ACR = std::exp2(ACA);
MOP_EXPM1        = 0x0018, //        ACR = std::expm1(ACA);
MOP_LOG          = 0x0019, //        ACR = std::log(ACA);
MOP_LOG10        = 0x001A, //        ACR = std::log10(ACA);
MOP_LOG2         = 0x001B, //        ACR = std::log2(ACA);
MOP_LOG1P        = 0x001C, //        ACR = std::log1p(ACA);
MOP_SQRT         = 0x001D, //        ACR = std::sqrt(ACA);
MOP_CBRT         = 0x001E, //        ACR = std::cbrt(ACA);
MOP_HYPOT        = 0x001F, //        ACR = std::hypot(ACA, ACB);
MOP_POW          = 0x0020, //        ACR = std::pow(ACA, ACB);
MOP_SIN          = 0x0021, //        ACR = std::sin(ACA);
MOP_COS          = 0x0022, //        ACR = std::cos(ACA);
MOP_TAN          = 0x0023, //        ACR = std::tan(ACA);
MOP_ASIN         = 0x0024, //        ACR = std::asin(ACA);
MOP_ACOS         = 0x0025, //        ACR = std::acos(ACA);
MOP_ATAN         = 0x0026, //        ACR = std::atan(ACA);
MOP_ATAN2        = 0x0027, //        ACR = std::atan2(ACA, ACB);
MOP_SINH         = 0x0028, //        ACR = std::sinh(ACA);
MOP_COSH         = 0x0029, //        ACR = std::acosh(ACA);
MOP_ATANH        = 0x002A, //        ACR = std::atanh(ACA);
MOP_ERF          = 0x002B, //        ACR = std::erf(ACA);
MOP_ERFC         = 0x002C, //        ACR = std::erfc(ACA);
MOP_LGAMMA       = 0x002D, //        ACR = std::lgamma(ACA);
MOP_TGAMMA       = 0x002E, //        ACR = std::tgamma(ACA);
MOP_CEIL         = 0x002F, //        ACR = std::ceil(ACA);
MOP_FLOOR        = 0x0030, //        ACR = std::floor(ACA);
MOP_TRUNC        = 0x0031, //        ACR = std::trunc(ACA);
MOP_ROUND        = 0x0032, //        ACR = std::round(ACA);
MOP_LROUND       = 0x0033, //        ACR = std::lround(ACA);
MOP_NEARBYINT    = 0x0034, //        ACR = std::nearbyint(ACA);
MOP_ILOGB        = 0x0035, //        ACR = std::ilogb(ACA);
MOP_LOGB         = 0x0036, //        ACR = std::logb(ACA);
MOP_NEXTAFTER    = 0x0037, //        ACR = std::nextafter(ACA, ACB);
MOP_COPYSIGN     = 0x0038, //        ACR = std::copysign(ACA, ACB);
MOP_LASTOP       = 0x0038, //        last implemented math operation
        // End MATH_OPERATION's (MOPS)
MATH_END         = 0x1CBD, // end of math co-processor registers

    // 4931 ($1343) bytes remaining for additional registers.
RESERVED         = 0x1CBD,

        // User RAM (32K)
USER_RAM         = 0x3000,

        // Paged RAM (8K)
PAGED_RAM        = 0xB000,

        // PAGED ROM (8K bytes)
PAGED_ROM        = 0xD000,

        // KERNEL ROM (4K bytes)
KERNEL_ROM       = 0xF000,

        // Hardware Interrupt Vectors:
HARD_EXEC        = 0xFFF0, // EXEC Hardware Interrupt Vector
HARD_SWI3        = 0xFFF2, // SWI3 Hardware Interrupt Vector
HARD_SWI2        = 0xFFF4, // SWI2 Hardware Interrupt Vector
HARD_FIRQ        = 0xFFF6, // FIRQ Hardware Interrupt Vector
HARD_IRQ         = 0xFFF8, // IRQ Hardware Interrupt Vector
HARD_SWI         = 0xFFFA, // SWI / SYS Hardware Interrupt Vector
HARD_NMI         = 0xFFFC, // NMI Hardware Interrupt Vector
HARD_RESET       = 0xFFFE, // RESET Hardware Interrupt Vector
};  // END: enum MEMMAP


#endif // __MEMORY_MAP_H__
