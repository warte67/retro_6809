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
- [Future entries to track key milestones and decisions]

## Architecture Decisions
- [Section to document important architectural choices and their rationale]

## Component Status
- Core CPU Emulation
- Memory Management
- Video System
- Debugger Interface
- Build System
[Status tracking for each major component]

## Technical Decisions
[To be populated as we make architectural choices]

## Questions to Ask
- Thread synchronization strategy
- Memory management approach 
- Event system design
- Module boundaries and interfaces

## Current Focus
Preparing for complete refactor while preserving core emulation logic

## To Do
- Started refactoring project on Christmas Day 2023! 
- Set up new Retro_6809 repository
- Begin porting core components with improved architecture
- Implement ncurses-based debugger
- Design thread-safe event system
