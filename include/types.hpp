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

// // Generate a memory map definition file?
constexpr bool DUMP_MEMORY_MAP          = true;
constexpr bool MEMORY_MAP_OUTPUT_CPP    = true;

// Tests To Perform
constexpr bool MEM_TESTS                = true;  


// simple types for 8-bit archetecture 
#ifndef Byte
    #define Byte Uint8
#endif
#ifndef Word
    #define Word Uint16
#endif
#ifndef DWord
    #define DWord Uint32
#endif

// common includes
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <iostream>

#include "clr.hpp"





#endif  // __TYPES_HPP__
