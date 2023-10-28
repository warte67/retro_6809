

// memory_map.h
#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__

enum MEMMAP
{
    //  **********************************************
    //  * Allocated 64k Memory Mapped System Symbols *
    //  **********************************************
SOFT_VECTORS = 0x0000,

        // Software Interrupt Vectors:
SOFT_RSRVD   = 0x0000, // Motorola RESERVED Software Interrupt Vector
SOFT_SWI3    = 0x0002, // SWI3 Software Interrupt Vector
SOFT_SWI2    = 0x0004, // SWI2 Software Interrupt Vector
SOFT_FIRQ    = 0x0006, // FIRQ Software Interrupt Vector
SOFT_IRQ     = 0x0008, // IRQ Software Interrupt Vector
SOFT_SWI     = 0x000A, // SWI / SYS Software Interrupt Vector
SOFT_NMI     = 0x000C, // NMI Software Interrupt Vector
SOFT_RESET   = 0x000E, // RESET Software Interrupt Vector

ZERO_PAGE    = 0x0010,
FIO_BUFFER   = 0x0100,
FIO_BFR_TOP  = 0x01FF, // Top of the File Input/Output Buffer
SYSTEM_STACK = 0x0200,
SSTACK_TOP   = 0x0400, // Top of the system stack space

        // Display Buffer
SCREEN_BUFFER = 0x0400,

        // Device Registers:
HDW_REGS     = 0x1C00, // Begin Device Hardware Registers

STD_VID_MAX  = 0x1C00, //  (Word) Standard Video Buffer Max

SYS_STATE    = 0x1C02, //  (Byte) System State Register
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

SYS_SPEED    = 0x1C03, //  (Word) Approx. Average CPU Clock Speed

SYS_CLOCK_DIV = 0x1C05, //  (Byte) 60 hz Clock Divider Register (Read Only)
        // SYS_CLOCK_DIV: ABCD.SSSS
        //      bit 7: 0.46875 hz
        //      bit 6: 0.9375 hz
        //      bit 5: 1.875 hz
        //      bit 4: 3.75 hz
        //      bit 3: 7.5 hz
        //      bit 2: 15.0 hz
        //      bit 1: 30.0 hz
        //      bit 0: 60.0 hz

SYS_TIMER    = 0x1C06, //  (Word) Increments at 0.46875 hz

DSP_GRES     = 0x1C08, //  (Byte) Screen Resolution Register
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

DSP_EXT      = 0x1C09, //  (Byte) Extended Graphics Register
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

DSP_TXT_COLS = 0x1C0A, //  (Byte) READ-ONLY Text Screen Columns
DSP_TXT_ROWS = 0x1C0B, //  (Byte) READ-ONLY Text Screens Rows

DSP_PAL_IDX  = 0x1C0C, //  (Byte) Color Palette Index
        // DSP_PAL_IDX: 0-255
        // Note: Use this register to set the index into theColor Palette.
        //   Set this value prior referencing color data (DSP_PAL_CLR).

DSP_PAL_CLR  = 0x1C0D, //  (Word) Indexed Color Palette Data
        // DSP_PAL_CLR: Color Data A4R4G4B4 format
        // Note: This is the color data for an individual palette entry.
        //     Write to DSP_PAL_IDX with the index within the color palette
        //     prior to reading or writing the color data in the DSP_PAL_CLR register.

DSP_GLYPH_IDX = 0x1C0F, //  (Byte) Text Glyph Index
        // DSP_GLYPH_IDX: 0-256
        // Note: Set this register to index a specific text glyph. Set this value
        //     prior to updating glyph pixel data.

DSP_GLYPH_DATA = 0x1C10, //  (8-Bytes) Text Glyph Pixel Data Array
        // DSP_GLYPH_DATA: 8 rows of binary encoded glyph pixel data
        // Note: Each 8x8 text glyph is composed of 8 bytes. The first byte in this
        //     array represents the top line of 8 pixels. Each array entry represents
        //     a row of 8 pixels.

        // Debug Hardware Registers:
DBG_BEGIN    = 0x1C18, //  start of mouse cursor hardware registers
DBG_TEMP     = 0x1C18, //  (Byte) Simple Debug test register
DBG_END      = 0x1C19, // End Debug Registers

        // Mouse Cursor Hardware Registers:
CSR_BEGIN    = 0x1C19, //  start of mouse cursor hardware registers
CSR_XPOS     = 0x1C19, //  (Word) horizontal mouse cursor coordinate
CSR_YPOS     = 0x1C1B, //  (Word) vertical mouse cursor coordinate
CSR_XOFS     = 0x1C1D, //  (Byte) horizontal mouse cursor offset
CSR_YOFS     = 0x1C1E, //  (Byte) vertical mouse cursor offset
CSR_SCROLL   = 0x1C1F, //  (Signed) MouseWheel Scroll: -1, 0, 1
CSR_FLAGS    = 0x1C20, //  (Byte) mouse button flags:
        //  CSR_FLAGS:
        //       bits 0-5: button states
        //       bits 6-7: number of clicks
CSR_BMP_INDX = 0x1C21, //  (Byte) mouse cursor bitmap pixel offset
CSR_BMP_DATA = 0x1C22, //  (Byte) mouse cursor bitmap pixel index color
CSR_PAL_INDX = 0x1C24, //  (Byte) mouse cursor color palette index (0-15)
CSR_PAL_DATA = 0x1C25, //  (Word) mouse cursor color palette data RGBA4444
CSR_END      = 0x1C27, // End Mouse Registers
GFX_END      = 0x1C27, // End of GFX Device Registers

KEY_BEGIN    = 0x1C27, // Start of the Keyboard Register space
CHAR_Q_LEN   = 0x1C27, //   (Byte) # of characters waiting in queue        (Read Only)
CHAR_SCAN    = 0x1C28, //   (Byte) read next character in queue (not popped when read)
CHAR_POP     = 0x1C29, //   (Byte) read next character in queue (not popped when read)
XKEY_BUFFER  = 0x1C2A, //   (128 bits) 16 bytes for XK_KEY data buffer     (Read Only)
EDT_BFR_CSR  = 0x1C3A, //   (Byte) cursor position within edit buffer     (Read/Write)
EDT_ENABLE   = 0x1C3B, //   (Byte) line editor enable flag                 (Read/Write)
EDT_BUFFER   = 0x1C3C, //   line editing character buffer                 (Read/Write)
KEY_END      = 0x1C7C, // End of the Keyboard Register space

FIO_BEGIN    = 0x1C7C, // Start of the FileIO register space
FIO_ERR_FLAGS = 0x1C7C, // (Byte) File IO error flags
        // FIO_ERR_FLAGS: ABCD.EFGH
        //      A:  file was not found
        //      B:  directory was not found
        //      C:  file not open
        //      D:  end of file
        //      E:  buffer overrun
        //      F:  wrong file type
        //      G:  invalid command
        //      H:  incorrect file stream
FIO_COMMAND  = 0x1C7D, // (Byte) OnWrite, execute a file command
        //      $00:  Reset/Null
        //      $01:  SYSTEM: Shutdown
        //      $02:  SYSTEM: Load Compilation Date
        //      $03:  New File Stream
        //      $04:  Open File
        //      $05:  Is File Open? (returns FIO_ERR_FLAGS bit-5)
        //      $06:  Close File
        //      $07:  Read Byte
        //      $08:  Write Byte
        //      $09:  Load Hex Format File
        //      $0A:  Get File Length
        //      $0B:  List Directory
        //      $0C:  Make Directory
        //      $0D:  Change Directory
        //      $0E:  Rename Directory
        //      $0F:  Remove Directory
        //      $10:  Delete File
        //      $11:  Rename file
        //      $12:  Copy File
        //      $13:  Seek Start
        //      $14:  Seek End
        //      $15:  Set Seek Position
        //      $16:  Get Seek Position
FIO_STREAM   = 0x1C7E, // (Byte) current file stream index (0-15)
FIO_MODE     = 0x1C7F, // (Byte) Flags describing the I/O mode for the file
        // FIO_MODE: 00AB.CDEF  (indexed by FIO_STREAM)
        //      A:  INPUT - File open for reading
        //      B:  OUTPUT - File open for writing
        //      C:  BINARY - 1: Binary Mode, 0: Text Mode
        //      D:  AT_END - Output starts at the end of the file
        //      E:  APPEND - All output happens at end of the file
        //      F:  TRUNC - discard all previous file data
FIO_SEEKPOS  = 0x1C80, // (DWord) file seek position
FIO_IODATA   = 0x1C84, // (Byte) input / output character
FIO_PATH_LEN = 0x1C85, // (Byte) length of the filepath
FIO_PATH_POS = 0x1C86, // (Byte) character position within the filepath
FIO_PATH_DATA = 0x1C87, // (Byte) data at the character position of the path
FIO_DIR_DATA = 0x1C88, // (Byte) a series of null-terminated filenames
        //     NOTES: Current read-position is reset to the beginning following a
        //             List Directory command. The read-position is automatically
        //             advanced on read from this register. Each filename is
        //             $0a-terminated. The list itself is null-terminated.
FIO_END      = 0x1C89, // End of the FileIO register space

    // 4983 ($1377) bytes remaining for additional registers.
RESERVED     = 0x1C89,

        // User RAM (32K)
USER_RAM     = 0x3000,

        // Paged RAM (8K)
PAGED_RAM    = 0xB000,

        // PAGED ROM (8K bytes)
PAGED_ROM    = 0xD000,

        // KERNEL ROM (4K bytes)
KERNEL_ROM   = 0xF000,

        // Hardware Interrupt Vectors:
HARD_RSRVD   = 0xFFF0, // Motorola RESERVED Hardware Interrupt Vector
HARD_SWI3    = 0xFFF2, // SWI3 Hardware Interrupt Vector
HARD_SWI2    = 0xFFF4, // SWI2 Hardware Interrupt Vector
HARD_FIRQ    = 0xFFF6, // FIRQ Hardware Interrupt Vector
HARD_IRQ     = 0xFFF8, // IRQ Hardware Interrupt Vector
HARD_SWI     = 0xFFFA, // SWI / SYS Hardware Interrupt Vector
HARD_NMI     = 0xFFFC, // NMI Hardware Interrupt Vector
HARD_RESET   = 0xFFFE, // RESET Hardware Interrupt Vector
};  // END: enum MEMMAP


#endif // __MEMORY_MAP_H__
