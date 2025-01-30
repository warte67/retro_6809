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
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
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

    #include "UnitTest.hpp"


    // GENERATE_MEMORY_MAP: Generate a memory map definition file?
    //      true:  generate Memory_Map.hpp and use the local unordered map
    //      false: to use enums from Memory_Map.hpp
    #define GENERATE_MEMORY_MAP     true
    constexpr bool MEMORY_MAP_DISPLAY_OUTPUT_FILE_HPP = false;
    constexpr bool MEMORY_MAP_DISPLAY_OUTPUT_FILE_ASM = false;

    #define MEMORY_MAP_OUTPUT_FILE_HPP  "./include/Memory_Map.hpp"
    #define MEMORY_MAP_OUTPUT_FILE_ASM  "./asm/Memory_Map.asm"    
    #define KERNEL_ROM_FILENAME         "./asm/Kernel.hex"


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
    const Word DEBUG_WIDTH = 640;
    const Word DEBUG_HEIGHT = DEBUG_WIDTH / 1.333333333333333;

    const float DEBUG_WINDOW_SCALE = 2.0f;
    const int DEBUG_WINDOW_WIDTH = DEBUG_WIDTH * DEBUG_WINDOW_SCALE;
    const int DEBUG_WINDOW_HEIGHT = DEBUG_HEIGHT * DEBUG_WINDOW_SCALE;
    const size_t DEBUG_BUFFER_SIZE = (DEBUG_WIDTH/8)*(DEBUG_HEIGHT/8);
    constexpr bool DEBUG_STARTS_ACTIVE = true;
    constexpr bool DEBUG_SINGLE_STEP = false;
    constexpr int DEBUG_HISTORY_SIZE = 16;

    #define DEBUG_THROW_ERROR_ON_WRITE_TO_READ_ONLY_MEMORY false


    // Define the MAP() macro based on the value of GENERATE_MEMORY_MAP
    // if GENERATE_MEMORY_MAP is true, use the unordered_map generated by the devices
    // if GENERATE_MEMORY_MAP is false, use the enumeration in Memory_Map.hpp
    //
    #if GENERATE_MEMORY_MAP == true
        // use the unordered map
        #define MAP_IMPL(key) Memory::Map(std::string(#key), __FILE__, __LINE__)
        #define MAP(key) MAP_IMPL(key)
    #else // use the enumeration
        #include "Memory_Map.hpp"
        #define MAP(key) static_cast<Word>(key)
    #endif  // END: GENERATE_MEMORY_MAP

    // Keyboard Constants:
    constexpr size_t EDIT_BUFFER_SIZE = 128; // FIO_LN_EDT_BUFFER through FIO_LN_EDT_END

    // Joystick/Gamepad Constants:
    #define SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS "SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS"

    // FileIO Device Constants:
    constexpr Word FILEHANDLESMAX = 256;

    // Unit Test Constants:
    #define DISPLAY_RUNTIME_UNIT_TESTS false
    #define DISPLAY_UNIT_TEST_RESULTS true





//#endif  // __TYPES_HPP__
