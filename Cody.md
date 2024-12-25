# Project Context: 6809 Emulator Refactor

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

## Technical Decisions
[To be populated as we make architectural choices]

## Questions to Ask
- Thread synchronization strategy
- Memory management approach 
- Event system design
- Module boundaries and interfaces

## Current Focus
Preparing for complete refactor while preserving core emulation logic
