/***  types.hpp  ****************************
 *      _                                      _                     
 *     | |                                    | |                    
 *     | |_   _   _   _ __     ___   ___      | |__    _ __    _ __  
 *     | __| | | | | | '_ \   / _ \ / __|     | '_ \  | '_ \  | '_ \ 
 *     | |_  | |_| | | |_) | |  __/ \__ \  _  | | | | | |_) | | |_) |
 *      \__|  \__, | | .__/   \___| |___/ (_) |_| |_| | .__/  | .__/ 
 *             __/ | | |                              | |     | |    
 *            |___/  |_|                              |_|     |_|    
 * 
 ***************************************/ 

// #pragma once    // (Easy Mode Header Guard)
#ifndef __TYPES_HPP__
    #define __TYPES_HPP__

    // GENERATE_MEMORY_MAP: Generate a memory map definition file?
    //      true:  generate Memory_Map.hpp and use the local unordered map
    //      false: to use enums from Memory_Map.hpp
    #define GENERATE_MEMORY_MAP     true         

    #define MEMORY_MAP_OUTPUT_FILE_HPP  "./include/Memory_Map.hpp"
    #define MEMORY_MAP_OUTPUT_FILE_ASM  "./include/Memory_Map.asm"

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

    // Create an appropriate MAP() macro
    #if GENERATE_MEMORY_MAP == true
        // use the unordered map
        #define MAP_IMPL(key) Memory::Map(#key)                     
        #define MAP(key) MAP_IMPL(key)
    #else // use the enumeration
        #include "Memory_Map.hpp"
        #define MAP(key) key                
    #endif  // END: GENERATE_MEMORY_MAP == true

#endif  // __TYPES_HPP__
