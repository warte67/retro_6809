/*  coco.h - Utility functions for interface with CoCo Disk Basic

    By Pierre Sarrazin <http://sarrazip.com/>.
    This file is in the public domain.
*/

#ifndef _coco_h_
#define _coco_h_

#include <cmoc.h>


#ifndef _CMOC_HAVE_FALSE_TRUE_
enum { FALSE, TRUE };
#define _CMOC_HAVE_FALSE_TRUE_
#endif

#ifndef _CMOC_HAVE_BOOL_
typedef unsigned char BOOL;
#define _CMOC_HAVE_BOOL_
#endif


#ifndef _CMOC_BASIC_TYPES_
#define _CMOC_BASIC_TYPES_

typedef unsigned char byte;
typedef signed char   sbyte;
typedef unsigned int  word;
typedef signed int    sword;
typedef unsigned long dword;
typedef signed long   sdword;

#endif

#ifndef _CMOC_STDINT_
#define _CMOC_STDINT_

typedef unsigned char uint8_t;
typedef signed char   int8_t;
typedef unsigned int  uint16_t;
typedef signed int    int16_t;
typedef unsigned long uint32_t;
typedef signed long   int32_t;

#endif


// initCoCoSupport() must have been called for these variables to be valid.
//
extern byte isCoCo3;  // non-zero iff the machine is a CoCo 3
extern byte textScreenWidth;
extern byte textScreenHeight;


// May be called more than once.
// Sets isCoCo3, textScreenWidth, textScreenHeight.
//
void initCoCoSupport(void);


#if defined(_COCO_BASIC_) || defined(DRAGON)


void setHighSpeed(BOOL fast);


// Does not require the presence of Basic.
// isRGB: If false, the CMP palette is used.
// Does nothing if not on a CoCo 3.
//
byte resetPalette(byte isRGB);


// These functions do not require Basic to be present.
// They do nothing if not on a CoCo 3.
//
void rgb(void);
void cmp(void);



// slot: 0..15.
// color: 0..63.
// Returns true for success, false if args are invalid.
// See also paletteRGB().
//
byte palette(byte slot, byte color);


// Easier interface when assuming an RGB monitor.
// slot: 0..15.
// red, green, blue: 0..3.
//
void paletteRGB(byte slot, byte red, byte green, byte blue);


// Sets the given 6-bit color code as the border color
// for the 40 and 80 column modes.
//
byte setBorderColor(byte color);


// Returns true for success, false if arg is invalid.
// Assumes that Basic is present.
//
byte width(byte columns);


// Returns 32, 40 or 80.
// Assumes that Basic is present.
//
byte getTextMode();


// color: Argument that would be passed to BASIC's CLS command.
//        Pass 255 to signify no argument.
// Requires Color Basic to be present in memory.
//
void cls(byte color);


// CoCo 3 only.
// foreColor: 0-7.
// backColor: 0-7.
// blink, underline: Booleans.
// Requires Super Extended Color Basic to be present in memory.
//
byte attr(byte foreColor, byte backColor, byte blink, byte underline);


// Only works in 32x16 mode on a CoCo that has a 6847T1 VDG chip.
// It does not work on the original CoCo 1 and 2 models.
// N.B.: Lower-case may go back to inverted colors lower-case mode
//       as soon as Basic's Console Out routine ([$A002]) gets used.
//       The RVEC3 vector at $0167 can be modified to get around that:
//           * (char *) 0x0167 = 57;  /* write RTS instruction */
//
#define enable6847T1TrueLowerCase() (* (byte *) 0xFF22 |= 0x10)


// Positions the current printing position on the text screen.
// Works in the 32x16 mode and, on the CoCo 3, in the 40x24 and 80x24 modes.
// column: Zero-based.
// row: Zero-based.
// Assumes that Basic is present.
//
byte locate(byte column, byte row);


byte hscreen(byte mode);


byte hset(word x, word y, byte color);


// Change Color Basic's uppercase/lowercase flag.
//
void setCaseFlag(byte upperCase);


// Requires Color Basic to be present in memory.
//
void coldStart();


// Returns 0 if no key is currently pressed.
// Requires Color Basic to be present in memory.
//
byte inkey();


// Waits for a key to be pressed and returns its code.
// Requires Color Basic to be present in memory.
//
byte waitkey(byte blinkCursor);


// Names for values that can be passed to isKeyPressed()
// to test if a CoCo key is down or not.
// These values do not apply on the Dragon, whose keyboard grid is different.
//
enum KeyboardBits
{
    KEY_PROBE_AT        = 0xFE, KEY_BIT_AT        = 0x01,
    KEY_PROBE_A         = 0xFD, KEY_BIT_A         = 0x01,
    KEY_PROBE_B         = 0xFB, KEY_BIT_B         = 0x01,
    KEY_PROBE_C         = 0xF7, KEY_BIT_C         = 0x01,
    KEY_PROBE_D         = 0xEF, KEY_BIT_D         = 0x01,
    KEY_PROBE_E         = 0xDF, KEY_BIT_E         = 0x01,
    KEY_PROBE_F         = 0xBF, KEY_BIT_F         = 0x01,
    KEY_PROBE_G         = 0x7F, KEY_BIT_G         = 0x01,
    KEY_PROBE_H         = 0xFE, KEY_BIT_H         = 0x02,
    KEY_PROBE_I         = 0xFD, KEY_BIT_I         = 0x02,
    KEY_PROBE_J         = 0xFB, KEY_BIT_J         = 0x02,
    KEY_PROBE_K         = 0xF7, KEY_BIT_K         = 0x02,
    KEY_PROBE_L         = 0xEF, KEY_BIT_L         = 0x02,
    KEY_PROBE_M         = 0xDF, KEY_BIT_M         = 0x02,
    KEY_PROBE_N         = 0xBF, KEY_BIT_N         = 0x02,
    KEY_PROBE_O         = 0x7F, KEY_BIT_O         = 0x02,
    KEY_PROBE_P         = 0xFE, KEY_BIT_P         = 0x04,
    KEY_PROBE_Q         = 0xFD, KEY_BIT_Q         = 0x04,
    KEY_PROBE_R         = 0xFB, KEY_BIT_R         = 0x04,
    KEY_PROBE_S         = 0xF7, KEY_BIT_S         = 0x04,
    KEY_PROBE_T         = 0xEF, KEY_BIT_T         = 0x04,
    KEY_PROBE_U         = 0xDF, KEY_BIT_U         = 0x04,
    KEY_PROBE_V         = 0xBF, KEY_BIT_V         = 0x04,
    KEY_PROBE_W         = 0x7F, KEY_BIT_W         = 0x04,
    KEY_PROBE_X         = 0xFE, KEY_BIT_X         = 0x08,
    KEY_PROBE_Y         = 0xFD, KEY_BIT_Y         = 0x08,
    KEY_PROBE_Z         = 0xFB, KEY_BIT_Z         = 0x08,
    KEY_PROBE_UP        = 0xF7, KEY_BIT_UP        = 0x08,
    KEY_PROBE_DOWN      = 0xEF, KEY_BIT_DOWN      = 0x08,
    KEY_PROBE_LEFT      = 0xDF, KEY_BIT_LEFT      = 0x08,
    KEY_PROBE_RIGHT     = 0xBF, KEY_BIT_RIGHT     = 0x08,
    KEY_PROBE_SPACE     = 0x7F, KEY_BIT_SPACE     = 0x08,
    KEY_PROBE_0         = 0xFE, KEY_BIT_0         = 0x10,
    KEY_PROBE_1         = 0xFD, KEY_BIT_1         = 0x10,
    KEY_PROBE_2         = 0xFB, KEY_BIT_2         = 0x10,
    KEY_PROBE_3         = 0xF7, KEY_BIT_3         = 0x10,
    KEY_PROBE_4         = 0xEF, KEY_BIT_4         = 0x10,
    KEY_PROBE_5         = 0xDF, KEY_BIT_5         = 0x10,
    KEY_PROBE_6         = 0xBF, KEY_BIT_6         = 0x10,
    KEY_PROBE_7         = 0x7F, KEY_BIT_7         = 0x10,
    KEY_PROBE_8         = 0xFE, KEY_BIT_8         = 0x20,
    KEY_PROBE_9         = 0xFD, KEY_BIT_9         = 0x20,
    KEY_PROBE_COLON     = 0xFB, KEY_BIT_COLON     = 0x20,
    KEY_PROBE_SEMICOLON = 0xF7, KEY_BIT_SEMICOLON = 0x20,
    KEY_PROBE_COMMA     = 0xEF, KEY_BIT_COMMA     = 0x20,
    KEY_PROBE_HYPHEN    = 0xDF, KEY_BIT_HYPHEN    = 0x20,
    KEY_PROBE_PERIOD    = 0xBF, KEY_BIT_PERIOD    = 0x20,
    KEY_PROBE_SLASH     = 0x7F, KEY_BIT_SLASH     = 0x20,
    KEY_PROBE_ENTER     = 0xFE, KEY_BIT_ENTER     = 0x40,
    KEY_PROBE_CLEAR     = 0xFD, KEY_BIT_CLEAR     = 0x40,
    KEY_PROBE_BREAK     = 0xFB, KEY_BIT_BREAK     = 0x40,
    KEY_PROBE_ALT       = 0xF7, KEY_BIT_ALT       = 0x40,
    KEY_PROBE_CTRL      = 0xEF, KEY_BIT_CTRL      = 0x40,
    KEY_PROBE_F1        = 0xDF, KEY_BIT_F1        = 0x40,
    KEY_PROBE_F2        = 0xBF, KEY_BIT_F2        = 0x40,
    KEY_PROBE_SHIFT     = 0x7F, KEY_BIT_SHIFT     = 0x40,
};


// Writes 'probe' to $FF02, reads $FF00 and tests the bit specified by 'testBit'.
// Returns non-zero iff the designated key is currently pressed.
// See the KeyboardBits enumeration above.
// Example: isKeyPressed(KEY_PROBE_SPACE, KEY_BIT_SPACE) checks
//          for the space key.
// For details, look for a CoCo keyboard grid diagram.
// Does not require Color Basic to be present in memory.
//
byte isKeyPressed(byte probe, byte testBit);


// Indices into the POTVAL array whose address is returned
// by readJoystickPositions().
//
enum
{
    JOYSTK_RIGHT_HORIZ = 0,
    JOYSTK_RIGHT_VERT  = 1,
    JOYSTK_LEFT_HORIZ  = 2,
    JOYSTK_LEFT_VERT   = 3,
};


enum
{
    JOYSTK_MAX = 63,  // max value in a POTVAL entry (min is 0)
};


// Reads the joysticks and returns the address of a 4-byte array
// that contains the 0..63 values that would be returned by
// JOYSTK(0..3) in Color Basic.
//
const byte *readJoystickPositions();


// Bit names to be used on the value returned by readJoystickButtons().
//
enum
{
    JOYSTK_BUTTON_1_RIGHT = 0x01,
    JOYSTK_BUTTON_2_RIGHT = 0x02,
    JOYSTK_BUTTON_1_LEFT  = 0x04,
    JOYSTK_BUTTON_2_LEFT  = 0x08,
};


// Reads the state of all 4 supported joystick buttons.
// Returns a 4-bit value:
// - bit 0: button 1 of right joystick;
// - bit 1: button 2 of right joystick;
// - bit 2: button 1 of left joystick;
// - bit 3: button 2 of left joystick.
// Does not rely on Basic.
//
byte readJoystickButtons();


// Same arguments as Color Basic's SOUND command.
// Color Basic must be present in memory.
//
void sound(byte tone, byte duration);


// Device numbers.
//
enum
{
    DEV_SCREEN = 0,
    DEV_CASSETTE = -1,
    DEV_SERIAL = -2,
    DEV_PRINTER = DEV_SERIAL,
};


void setOutputDevice(sbyte deviceNum);


// newValue: word value.
//
#define setTimer(newValue) (* (word *) 0x112 = (newValue))

// Returns a word.
//
#define getTimer() (* (word *) 0x112)


// seconds: 0..1092.
//
void sleep(int seconds);


// samAddr: Base address to write to (0xFFC0 for screen mode,
//          0xFFC6 for graphics page address).
// value:   Value to write.
// numBits: Number of bits to write to samAddr. (Refers to the
//          least significant bits of 'value'.)
// 
void setSAMRegisters(byte *samAddr, byte value, byte numBits);


// Based on Basic'S PMODE command.
// Does not require the presence of Basic in memory.
// mode: 0..4. Same as the first parameter of Basic's PMODE command.
// screenBuffer: Must be divisible by 512. This address is remembered,
//               for pcls() and screen().
//               Extended Color Basic's current PMODE screen address
//               can be obtained with this fancy expression:
//                  (byte *) * (byte *) 0x00BC << 8
//
// Returns FALSE upon error (bad mode or bad screenBuffer).
//
// pcls() and screen() should typically be called after having called this function.
//
// Since version 0.1.89 of CMOC, this function does not change the screen mode anymore;
// this is now done by screen(), as in Extended Color Basic.
//
// To display a different screen buffer after having called pmode() and screen(),
// setSAMRegisters((byte *) 0xFFC6, pageNum, 7) can be called, where pageNum is
// the 512-byte page number of the screen buffer to be displayed, i.e., the address
// of that buffer divided by 512.
//
BOOL pmode(byte mode, byte *screenBuffer);


// Writes the given byte in the PMODE screen buffer.
// pmode() must have been called before.
// Does not require the presence of Basic in memory.
//
void pcls(byte byteToClearWith);


// Like Basic's SCREEN command.
// Does not require the presence of Basic in memory.
//
// type: 0 for text, 1 for graphics.
//       If 'type' is 1, this function switches to graphics mode and sets the
//       address of the graphics screen. (This address can be changed later
//       by calling setPmodeGraphicsAddress().)
//       If 'type' is 0, this function switches to text mode and sets the address
//       of the text screen to 1024.
// colorset: 0 or 1.
//
// When switching to graphics, the screen buffer at the address passed to pmode()
// is displayed. pmode() must have been called before, to set the graphics mode and
// screen buffer address.
// When switching to text, the default 32x16 screen at 0x0400 is shown on screen.
//
void screen(byte type, byte colorset);


// Sets the address of the graphics screen.
// Does not change the graphics mode.
//
// newPmodeScreenBuffer: Must be an unsigned char pointer on a 512-byte boundary.
//
#define setPmodeGraphicsAddress(newPmodeScreenBuffer) (setPmodeGraphicsAddressEx((newPmodeScreenBuffer), TRUE))


// Like setPmodeGraphicsAddress(), but if FALSE is passed for makeVisible,
// only the internal screen buffer pointer changes, i.e., the displayed
// screen does not change. This is useful for double-buffering.
// Does not require the presence of Basic in memory.
//
void setPmodeGraphicsAddressEx(byte *newPmodeScreenBuffer, BOOL makeVisible);


// Sets the SAM registers to show the PMODE graphics specified by
// 'samVRegisterValue' at address pageNum * 512.
// Does not require the presence of Basic in memory.
//
// samVRegisterValue: 0..7. For PMODEs 0..4, use 3, 4, 5, 6, 6 respectively.
//                    For PMODE 0 (32x16 text screen), use 0.
// pageNum: 512-byte page index in 0..127. The displayed screen will reflect
//          the contents of memory at pageNum * 512. Pass 7 for the default
//          graphics address of 0x0E00.
//
void showGraphicsAddress(byte samVRegisterValue, byte pageNum);


// Deprecated in favor of pmode(4, screenBuffer) and screen(1, colorset).
// Set "PMODE 4 : SCREEN 1,colorset", where colorset is 0 (green/black)
// or 1 (white/black).
//
void showPmode4(byte colorset);


// Deprecated in favor of screen(0, 0) (see above).
// Select the 32x16 text mode and position the screen buffer at address 1024.
//
void showLowResTextAddress(void);


#endif  /* defined(_COCO_BASIC_) || defined(DRAGON) */


#ifdef _COCO_BASIC_


// Reads a Disk Basic file into the memory region designated by 'dest'.
// Does not require the presence of Disk Basic in memory.
//
// dest: Must be at least 256 bytes, even if the file to be read is shorter than that.
// filename: Space-padded 8-character filename followed by a space-padded
//           3-character extension. Example: "FOOBAR  DAT".
//           DO NOT use the dot notation, e.g., "FOOBAR.DAT".
// workBuffer: Buffer to be used to read the directory and the FAT.
//             Contents can be discarded after return.
//             Must not intersect with the region at 'dest' where the file will be loaded.
// sizePtr: Pointer to a size_t that receives the file length in bytes
//          upon success. Note that complete sectors are read in memory,
//          so up to 255 more bytes may get overwritten after the file's last
//          valid byte.
// Returns:
//   0 = success;
//   1 = read error when reading directory sector(s);
//   2 = file not found;
//   3 = read error when reading the FAT;
//   4 = read error when reading a file sector.
//
// Temporarily redirects the IRQ and NMI interrupts to internal routines.
//
// Supports reading an empty file. *sizePtr becomes zero.
//
// CAUTION: Upon return, interrupts are masked.
//          Call enableInterrupts() when interrupts are wanted again.
//
byte readDECBFile(void *dest,
                  byte driveNo, const char filename[11],
                  byte workBuffer[256],
                  size_t *sizePtr);

#endif  /* defined(_COCO_BASIC_) */


#if defined(_COCO_BASIC_) || defined(DRAGON)

// Math functions.
// Extended Color Basic (or Dragon Basic) must be present when calling these functions.
// Passing an invalid argument may display a Basic error message and
// end the execution of the program.

// logf() is declared in <cmoc.h>.

float sinf(float radians);
float cosf(float radians);
float tanf(float radians);  // argument must not be pi/2 or 3*pi/2 or any other equivalent angle
float atanf(float radians);  // returns value in [-pi/2, pi/2]
float expf(float x);
float sqrtf(float x);  // x >= 0
float fabsf(float radians);
float floorf(float x);  // returns the largest integral value that is not greater than x
float truncf(float x);  // returns x rounded to the nearest integer value that is not larger in magnitude than x
float roundf(float x);  // returns x rounded to the nearest integer, but halfway cases are rounded away from zero

#endif  /* defined(_COCO_BASIC_) */


#define disableInterrupts() asm("ORCC",  "#$50")
#define enableInterrupts()  asm("ANDCC", "#$AF")


#endif  /* _coco_h_ */
