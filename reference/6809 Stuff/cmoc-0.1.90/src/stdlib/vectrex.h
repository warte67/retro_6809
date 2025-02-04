// This file is in the public domain.

#ifndef __vectrex_h__
#define __vectrex_h__

#ifndef _CMOC_HAVE_FALSE_TRUE_
enum { FALSE, TRUE };
#define _CMOC_HAVE_FALSE_TRUE_
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

#endif
