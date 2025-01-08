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

#pragma once    // (Easy Mode Header Guard)
// #ifndef __TYPES_HPP__
//     #define __TYPES_HPP__

    #include <cstddef>

    // common includes
    #include <SDL3/SDL.h>
    #include <string>
    #include <vector>
    #include <iostream>


    // GENERATE_MEMORY_MAP: Generate a memory map definition file?
    //      true:  generate Memory_Map.hpp and use the local unordered map
    //      false: to use enums from Memory_Map.hpp
    #define GENERATE_MEMORY_MAP     true

    #define MEMORY_MAP_OUTPUT_FILE_HPP  "./include/Memory_Map.hpp"
    #define MEMORY_MAP_OUTPUT_FILE_ASM  "./include/Memory_Map.asm"
    #define INITIAL_ASM_APPLICATION     "./asm/test.hex"

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

    #include "clr.hpp"

    // Debug Device Constants:
    const Word DEBUG_WIDTH = 800;
    const Word DEBUG_HEIGHT = DEBUG_WIDTH / 1.333333333333333;

    const float DEBUG_WINDOW_SCALE = 1.7f;
    // const float DEBUG_WINDOW_SCALE = 1.325f;

    const int DEBUG_WINDOW_WIDTH = DEBUG_WIDTH * DEBUG_WINDOW_SCALE;
    const int DEBUG_WINDOW_HEIGHT = DEBUG_HEIGHT * DEBUG_WINDOW_SCALE;
    const size_t DEBUG_BUFFER_SIZE = (DEBUG_WIDTH/8)*(DEBUG_HEIGHT/8);
    constexpr bool DEBUG_STARTS_ACTIVE = true;
    constexpr bool DEBUG_SINGLE_STEP = false;
    constexpr int DEBUG_HISTORY_SIZE = 16;

    // // Create an appropriate MAP() macro
    // #if GENERATE_MEMORY_MAP == true
    //     // use the unordered map
    //     #define MAP_IMPL(key) Memory::Map(#key)                     
    //     #define MAP(key) MAP_IMPL(key)
    // #else // use the enumeration
    //     #include "Memory_Map.hpp"
    //     #define MAP(key) static_cast<Word>(key)
    // #endif  // END: GENERATE_MEMORY_MAP

    // #if GENERATE_MEMORY_MAP == true
    // // use the unordered map
    // #define MAP_IMPL(key) static_cast<Word>(Memory::Map(std::string(#key)))                     
    // #define MAP(key) MAP_IMPL(key)
    // #else // use the enumeration
    //     #include "Memory_Map.hpp"
    //     #define MAP(key) static_cast<Word>(key)
    // #endif  // END: GENERATE_MEMORY_MAP

    #if GENERATE_MEMORY_MAP == true
        // use the unordered map
        #define MAP_IMPL(key) Memory::Map(std::string(#key), __FILE__, __LINE__)
        #define MAP(key) MAP_IMPL(key)
    #else // use the enumeration
        #include "Memory_Map.hpp"
        #define MAP(key) static_cast<Word>(key)
    #endif  // END: GENERATE_MEMORY_MAP




//#endif  // __TYPES_HPP__
