////
// types.h
//////

#ifndef __TYPES_H__
#define __TYPES_H__

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "memory_map.h"

// Generate a memory map definition file?
#define COMPILE_MEMORY_MAP      false
#define MEMORY_MAP_OUTPUT_CPP   true

// simple 6809 types
    #ifndef Byte
        #define Byte Uint8
    #endif
    #ifndef Word
        #define Word Uint16
    #endif
    #ifndef DWord
        #define DWord Uint32
    #endif

#endif //__TYPES_H__