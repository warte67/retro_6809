/***  types.hpp  ****************************
*     _                           _                 
*    | |_ _   _ _ __   ___  ___  | |__  _ __  _ __  
*    | __| | | | '_ \ / _ \/ __| | '_ \| '_ \| '_ \ 
*    | |_| |_| | |_) |  __/\__ \_| | | | |_) | |_) |
*     \__|\__, | .__/ \___||___(_)_| |_| .__/| .__/ 
*         |___/|_|                     |_|   |_|    
* 
***************************************/ 

// #pragma once    // (Easy Mode Header Guard)
#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <iostream>
    
// Generate a memory map definition file?
#define COMPILE_MEMORY_MAP      false
#define MEMORY_MAP_OUTPUT_CPP   true

// simple types

    #ifndef Byte
        #define Byte Uint8
    #endif
    #ifndef Word
        #define Word Uint16
    #endif
    #ifndef DWord
        #define DWord Uint32
    #endif

#endif  // __TYPES_HPP__
