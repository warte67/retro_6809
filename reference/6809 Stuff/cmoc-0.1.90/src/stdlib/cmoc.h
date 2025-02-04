// cmoc.h - CMOC's standard library functions.
//
// By Pierre Sarrazin <http://sarrazip.com/>.
// This file is in the public domain.
//
// Functions not documented here should be assumed to behave as in C.

#ifndef _H_CMOC
#define _H_CMOC

#include <stdarg.h>  /* for va_list */


#ifndef __GNUC__


#ifndef NULL
#define NULL ((void *) 0)
#endif


#ifndef _CMOC_CONST_
#define _CMOC_CONST_ const
#endif


// Gives the offset in bytes of the specified 'member' in the struct
// or union named 'Type'.
//
#define offsetof(Type, member) ((unsigned) &((Type *) 0)->member)

typedef unsigned size_t;

#define SIZE_MAX 0xFFFFu

typedef signed ssize_t;

#define SSIZE_MAX 0x7FFF


#ifndef VECTREX
// Supports %u, %d, %x, %X, %p, %s, %c and %%. Specifying a field width is
// allowed, but a left justification is only supported for strings, i.e.,
// %-15s will work, but %-6u will not. Zero padding for a number is supported
// (e.g., %04x).
//
int printf(_CMOC_CONST_ char *format, ...);
#endif


// Writes to 'dest'. Not thread-safe. Does not check for buffer overflow.
// The written string is ended with a '\0' character.
//
int sprintf(char *dest, _CMOC_CONST_ char *format, ...);


// Like printf() but the variable arguments are provided by 'ap'.
//
int vprintf(_CMOC_CONST_ char *format, va_list ap);


// Like vprintf() but writes to 'dest' like sprintf().
// Not thread-safe. Does not check for buffer overflow.
// The written string is ended with a '\0' character.
//
int vsprintf(char *dest, _CMOC_CONST_ char *format, va_list ap);


#ifndef VECTREX
// Writes the first 'n' characters designated by 's', regardless of any
// null characters encountered among them.
//
void putstr(_CMOC_CONST_ char *s, size_t n);

void putchar(int c);
#endif

int strcmp(_CMOC_CONST_ char *s1, _CMOC_CONST_ char *s2);
int stricmp(_CMOC_CONST_ char *s1, _CMOC_CONST_ char *s2);
int strncmp(_CMOC_CONST_ char *s1, _CMOC_CONST_ char *s2, size_t n);
int memcmp(_CMOC_CONST_ void *s1, _CMOC_CONST_ void *s2, size_t n);
int memicmp(_CMOC_CONST_ void *s1, _CMOC_CONST_ void *s2, size_t n);
void *memchr(_CMOC_CONST_ void *s, int c, size_t n);
void *memichr(_CMOC_CONST_ void *s, int c, size_t n);
void *memcpy(void *dest, _CMOC_CONST_ void *src, size_t n);
void *memmove(void *dest, _CMOC_CONST_ void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memset16(void *dest, unsigned short w, size_t num16BitWords);
size_t strlen(_CMOC_CONST_ char *s);
char *strcpy(char *dest, _CMOC_CONST_ char *src);
char *strcat(char *dest, _CMOC_CONST_ char *src);
char *strncpy(char *dest, _CMOC_CONST_ char *src, size_t n);
char *strchr(_CMOC_CONST_ char *s, int c);
char *strrchr(_CMOC_CONST_ char *s, int c);
char *strstr(_CMOC_CONST_ char *haystack, _CMOC_CONST_ char *needle);
char *strlwr(char *s);
char *strupr(char *s);
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
char *strtok(char *str, const char *delim);
char *strpbrk(const char *s, const char *accept);
int isspace(int c);
int isalpha(int c);
int isalnum(int c);
int isdigit(int c);


// Converts an ASCII unsigned decimal string into an unsigned word.
//
unsigned atoui(_CMOC_CONST_ char *s);


// Converts an ASCII signed decimal string into a signed word.
//
int atoi(_CMOC_CONST_ char *s);


// Converts an integer to a NUL-terminated ASCII signed decimal string.
// Returns 'str'.
// The caller must be careful to pass an array of sufficient size,
// including room for the terminating '\0'.
//
char *itoa10(int value, char *str);


// Converts an integer to a NUL-terminated ASCII signed decimal string.
// N.B.: 'base' must be 10. No other base is supported by this implementation.
// Returns 'str'.
// The caller must be careful to pass an array of sufficient size,
// including room for the terminating '\0'.
//
#define itoa(value, str, base) (itoa10((value), (str)))


// Like itoa10(), but 'value' can be in the range 32768..65535.
//
char *utoa10(unsigned value, char *str);


// Like itoa(), but 'value' can be in the range 32768..65535.
// N.B.: 'base' must be 10. No other base is supported by this implementation.
//
#define utoa(value, str, base) (utoa10((value), (str)))


// Like itoa10(), but 'value' can be 32 bits.
//
char *ltoa10(long value, char *str);


// Like itoa(), but 'value' can be 32 bits.
// N.B.: 'base' must be 10. No other base is supported by this implementation.
//
#define ltoa(value, str, base) (ltoa10((value), (str)))


// Like utoa10(), but 'value' can be 32 bits.
//
char *ultoa10(unsigned long value, char *str);


// Like utoa(), but 'value' can be 32 bits.
// N.B.: 'base' must be 10. No other base is supported by this implementation.
//
#define ultoa(value, str, base) (ultoa10((value), (str)))


// Double-word to ASCII.
// Converts the unsigned 32-bit integer formed by hi * 65536 + lo into
// an ASCII decimal representation that gets written to 'out'.
// 'out' must point to at least 11 bytes. The string written there will
// be terminated by a null character.
// Returns the address of the first non-'0' character in the 11-byte
// buffer, or to "0" if hi and lo are both zero.
// Example: char s[11]; char *p = dwtoa(s, 1, 2);
//          s will get the string "
// NOTE: This operation can also be done with the 'long' type and by
//       calling sprintf() with the "%lu" or "%ld" placeholders.
//
char *dwtoa(char *out, unsigned hi, unsigned lo);


// Returns the absolute value of a short integer.
// Trying to take the absolute value of the most negative short integer is not defined.
//
int abs(int j);


// Returns the absolute value of a long integer.
// Trying to take the absolute value of the most negative long integer is not defined.
//
long int labs(long int j);


// Returns the integer part of the square root of the given 16-bit number.
//
unsigned char sqrt16(unsigned short n);


// Returns the integer part of the square root of the given 32-bit number.
//
unsigned short sqrt32(unsigned long n);


// Returns the quotient and remainder of a 16-bit unsigned division
// in a single operation.
//
void divmod16(unsigned dividend, unsigned divisor,
              unsigned *quotient, unsigned *remainder);


// Returns the quotient and remainder of an 8-bit unsigned division
// in a single operation.
//
void divmod8(unsigned char dividend, unsigned char divisor,
             unsigned char *quotient, unsigned char *remainder);


// Divides an unsigned 32-bit integer by an unsigned 8-bit integer.
// The two words designated by 'dividendInQuotientOut' are the input dividend.
// The 32-bit quotient is left in those two words.
//
void divdwb(unsigned dividendInQuotientOut[2], unsigned char divisor);


// Previous name of divdwb().
//
#define div328 divdwb


// Divides an unsigned 32-bit integer by an unsigned 16-bit integer.
// The two words designated by 'dividendInQuotientOut' are the input dividend.
// The 32-bit quotient is left in those two words.
//
void divdww(unsigned dividendInQuotientOut[2], unsigned divisor);


// Multiply a word by a byte.
// Stores the high word of the product in *hi and returns the low word.
//
unsigned mulwb(unsigned char *hi, unsigned wordFactor, unsigned char byteFactor);


// Similar to mulwb().
unsigned mulww(unsigned *hi, unsigned factor0, unsigned factor1);


// Stores 0 in twoWords[0], twoWords[1].
//
void zerodw(unsigned *twoWords);


// Adds the 16-bit integer 'term' to the 32-bit integer designated by
// twoWords[0] and twoWords[1].
//
void adddww(unsigned *twoWords, unsigned term);


// Subtracts the 16-bit integer 'term' from the 32-bit integer designated by
// twoWords[0] and twoWords[1].
//
void subdww(unsigned *twoWords, unsigned term);


// Returns 0 if the 32-bit unsigned word composed of left[0] and left[1]
// (where left[0] is the high word) is equal to 'right';
// returns +1 if left > right; -1 if left < right.
//
signed char cmpdww(unsigned left[2], unsigned right);


// Must be called by a program that uses --mc6839, preferrably
// at the beginning of main(), to enable the use of %f in a
// printf() call.
//
void enable_printf_float(void);


#if defined(_CMOC_MC6839_)


// Set the given function pointer as the one to be invoked upon
// a MC6839 floating point operation failure.
// Only usable if --mc6839 is passed to CMOC.
// The default trap function in a CMOC-generated program does nothing.
//
void setMC6839Trap(void (*trapFunctionPointer)(void));


// Binary (base 2) logarithm.
//
float log2f(float x);  // x > 0


#endif


#if defined(_COCO_BASIC_) || defined(DRAGON) || defined(_CMOC_MC6839_)


// Natural (base e) logarithm.
//
float logf(float x);  // x > 0


#endif


#if defined(_COCO_BASIC_) || defined(DRAGON) || defined(_CMOC_NATIVE_FLOAT_) || defined(_CMOC_MC6839_)

// Converts an ASCII decimal floating point number to a float.
// The string is allowed to contain a suffix (e.g., "1.2E6XYZ");
// endptr: Receives the address where the parsing stopped.
// Caution: Passing a string whose value does not fit in a float
//          may have undefined behavior.
// An 'E' used in exponential notation must be in upper-case.
//
float strtof(_CMOC_CONST_ char *nptr, char **endptr);


// Like strtof(), but does not return the end pointer.
//
float atoff(_CMOC_CONST_ char *nptr);


// Writes an ASCII decimal representation of 'f' in the buffer
// at 'out' which must contain at least 38 bytes.
// Returns 'out' upon success, or null upon failure.
//
char *ftoa(char out[38], float f);

#endif


// CAUTION: base is ignored, only base 10 is supported.
//
unsigned long strtoul(_CMOC_CONST_ char *nptr, char **endptr, int base);

unsigned long atoul(_CMOC_CONST_ char *nptr);

// CAUTION: base is ignored, only base 10 is supported.
//
long strtol(_CMOC_CONST_ char *nptr, char **endptr, int base);

long atol(_CMOC_CONST_ char *nptr);

int tolower(int c);
int toupper(int c);


// Returns to the environment that executed the current program.
// When the program was started by the CoCo's EXEC command under Disk Basic,
// this function should only be called if the Basic environment is still present
// and has not been replaced or paged out.
//
void exit(int status);


#define RAND_MAX 0x7FFF
void srand(unsigned seed);
int rand(void);


#ifndef OS9


// Changes the location of the program break by 'increment' bytes.
// Returns (void *) -1 upon failure.
// Calling sbrk() with an increment of 0 can be used to find the current location
// of the program break.
//
void *sbrk(size_t increment);


// Returns the maximum number of bytes still allocatable via sbrk().
//
size_t sbrkmax(void);


#endif


// See the CMOC manual.
void set_null_ptr_handler(void (*newHandler)(void *));
void set_stack_overflow_handler(void (*newHandler)(void *, void *));


// Function pointer type used by setConsoleOutHook().
//
typedef void (*ConsoleOutHook)(void);


// Redirect printf() et al. to the function at 'routine', which will
// receive each character to be printed in register A.
//
// The 'routine' pointer must not be null.
//
// That routine MUST preserve registers B, X, Y and U.
//
// If this function is never called, printf() et al. write to the
// system's standard character output routine.
//
// Returns the original output routine address.
// To uninstall the new routine, call this function again with
// the original routine address.
//
ConsoleOutHook setConsoleOutHook(ConsoleOutHook routine);


#ifndef VECTREX

// Blocks the execution for the specified time in 60ths of a second.
//
void delay(size_t sixtiethsOfASecond);


// Reads a line from standard input, converts an expected 16-bit decimal
// number and returns it. Not thread-safe.
//
unsigned readword(void);


// Reads a line from standard input and returns it.
// Not thread-safe.
// Returns a null pointer if the operation failed (e.g., end of file
// encountered).
//
char *readline(void);

#endif  /* ndef VECTREX */


// Sorts an array that starts at 'base', that has 'nmemb' elements, whose
// elements are 'size' bytes each.
// compar: Pointer to a function that receives pointers to two array elements
//         and that returns -1, 0 or +1 depending on whether the first element
//         comes before, is equal to, or comes after the second element.
// This function is recursive and will thus use stack space.
//
void qsort(void *base, size_t nmemb, size_t size, int (*compar)(_CMOC_CONST_ void *, _CMOC_CONST_ void *));


// Searches for the value pointed to by 'key' in the array starting at 'base',
// that has 'nmemb' elements, whose elements are 'size' bytes each.
// compar: Pointer to a function that receives pointers to the targeted key and
//         to an array element. It must return -1, 0 or +1 depending on whether
//         the targeted key comes before, is equal to, or comes after the second element.
// Returns a pointer to the element of the array that matches the targeted key,
// or NULL if none is found.
// The time taken by this function is proportional to the logarithm of the array size.
// This function is recursive and will thus use stack space.
//
void *bsearch(_CMOC_CONST_ void *key, _CMOC_CONST_ void *base, size_t nmemb, size_t size,
              int (*compar)(_CMOC_CONST_ void *key, _CMOC_CONST_ void *element));


#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif  /* __GNUC__ */

#endif  /* _H_CMOC */
