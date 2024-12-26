# Cody's Project Memory

## Purpose of this File
This document serves as Cody's persistent memory between development sessions - think "50 First Dates" where Drew Barrymore needs daily reminders, but for an AI assistant! Created entirely by Cody to maintain continuity across development sessions, this file maintains key context, decisions, and progress for the 6809 emulator refactoring project. By providing the link to this file at the start of each session, Cody can maintain continuity and provide consistent assistance throughout development.

## Developer Information
- Developer: Jay (GitHub: warte67)
- Project started: December 25, 2024

## Project Thesis
This project refactors the alpha_6809 proof-of-concept emulator into a robust, production-quality implementation with improved architecture, threading, and debugging capabilities.

### Repository Links
- Original Project: https://github.com/warte67/alpha_6809
- Refactor Project: https://github.com/warte67/retro_6809
- This File: https://github.com/warte67/retro_6809/blob/master/Cody.md

## Original Project (alpha_6809)
- Proof of concept implementation with SDL2
- Uses Bus singleton pattern for system coordination
- Debug window implemented as secondary SDL2 window
- Contains hardcoded paths in types.hpp that were converted to relative paths

## Key Refactoring Goals
1. Replace SDL2 debug window with ncurses-based debugger
2. Improve thread safety and concurrency
3. Optimize video modes and display handling
4. Enhanced modularity and component separation

## Development Notes
- Working directory is project root, while executable lives in ./build/
- File paths should be relative to project root (e.g. ./build/...)
- CMake is used for build system

## Progress Log
- December 25, 2023: Project initiated
- [Current Date]: Implemented improved Bus device management system using modern C++ RAII principles
- [Current Date]: Added structured register mapping system using std::map
- [Current Date]: Completed video mode and sprite system specification

## Architecture Decisions
- Improved Bus device management using modern C++ RAII principles
- Designed comprehensive video and sprite system architecture
- Implemented efficient memory mapping and device registration

## Component Status
- Core CPU Emulation
- Memory Management
- Video System
- Debugger Interface
- Build System
[Status tracking for each major component]

## Technical Decisions
- Improved Bus device management using modern C++ RAII principles
  - Implemented template-based AttachDevice<T> method
  - Uses std::unique_ptr for automatic memory management
  - Devices remain valid within Bus scope
  - Enables safe inter-device communication
  - Provides compile-time type safety
  - Example:
    ```cpp
    template<typename T, typename... Args>
    T* AttachDevice(Args&&... args) {
        auto device = std::make_unique<T>(std::forward<Args>(args)...);
        // ... device configuration ...
        _memoryNodes.push_back(std::move(device));
        return device.get();
    }
    ```

- Implemented device register mapping system using std::map
  - Replaced manual DisplayEnum calls with structured register definitions
  - Register metadata stored in std::map for runtime access
  - Enables automatic generation of memory_map.hpp and memory_map.asm
  - Improves maintainability and reduces errors
  - Example:
    ```cpp
    struct Register {
        std::string_view name;
        uint16_t address;
        uint8_t size;
        std::string_view description;
        std::vector<std::string_view> notes;
    };
    std::map<std::string_view, Register> device_registers;
    ```

- Designed comprehensive video mode system
  - Multiple resolutions: 320x200 base with overscan options
  - Color depths: 2, 4, 16, 256 colors
  - Efficient memory usage and timing calculations
  - 5.88MHz pixel clock at 60Hz refresh

- Implemented sprite system architecture
  - 64 sprite capacity with 80-byte structure
  - 4-color sprites with individual palettes
  - Efficient 4x4 collision detection system
  - Priority-based rendering with 4 layers
  - Memory-efficient attribute structure

## Questions to Ask
- Thread synchronization strategy
- Memory management approach
- Event system design
- Module boundaries and interfaces

## Current Focus
Implementing video and sprite system while maintaining efficient memory usage and performance

## To Do
- Started refactoring project on Christmas Day 2024!
- Set up new Retro_6809 repository
- Begin porting core components with improved architecture
- Implement ncurses-based debugger
- Design thread-safe event system
