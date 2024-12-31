# retro_6809
A complete refactoring of the alpha_6809 project. This is a work in progress. Please check back later for updates.

### Project Summary: **The Dream 6809 Computer Reimagined**

#### Background and Inspiration
The idea for this project stems from my early programming experiences in the 1980s on the TRS-80 Color Computer. While I enjoyed the challenge of programming in 6809 assembler, I envied the superior graphics and sound capabilities of contemporaneous systems like the Commodore 64. Despite learning to program for the MOS 6502, I always found the 6809 to be far more enjoyable. I often dreamed of a computer combining the elegance of the 6809 architecture with robust graphics and audio capabilities. This project, made possible by modern hardware like the Raspberry Pi Pico 2, aims to bring that dream to life.

#### Vision
The goal is to design and build a fully open-source, retro-style single-board computer that captures the spirit of an idealized 1980s computer powered by a 6809 processor. Dubbed **alpha_6809**, this system goes beyond a proof of concept, offering a practical and refactored realization of that dream. The design integrates the best of vintage computing with the versatility of modern microcontroller technology.

#### Hardware Overview
The system is built using **two Raspberry Pi Pico 2** boards, each leveraging their dual-core capabilities:  
1. **Core Allocation (Board One):**
   - **Core 1:** Cycle-accurate emulation of the Motorola 6809, memory management, and system clock generation.
   - **Core 2:** GPU functionality for VGA video output, generating video signals and rendering pixels.
2. **Core Allocation (Board Two):**
   - **Core 1:** Emulation of the Commodore SID for advanced FM audio synthesis.
   - **Core 2:** USB host functionality for peripherals like storage devices, keyboards, mice, joysticks, gamepads, and debugging interfaces.  

Additionally, **32 GPIO pins** will be available for user projects, enhancing the system’s flexibility.

#### Graphics and Video Capabilities
- **Output:** VGA-compatible video generated via 12 GPIO pins using resistor ladders for RGB channels (0-0.7V).  
- **Internal Palette:** 256 colors per frame using A4R4G4B4 format, offering up to 4096 colors with 16 transparency levels.  
- **Background Layer:** A 64KB GPU-addressable buffer, capable of rendering 320x200 resolution at 256 colors.  
- **Foreground Layer:** An 8000-byte CPU-addressable layer for text and low-resolution graphics.  

#### Sprite Engine
The sprite system supports **64 sprites**, each 16x16 pixels with 4 colors and individual palettes:
- **Collision Detection:**  
  - **Sprites 1–16:** Support both pixel-level collision detection (4x4 resolution masks) and bounding box collision.  
  - **Sprites 17–32:** Bounding box collision detection only.  
  - **Sprites 33–64:** No collision detection (visual elements only).  
- Sprites can be combined for higher color depth or larger composite shapes.

#### Memory Architecture
The system features a **2MB paged memory system**:  
- A single **8KB window** in CPU memory can map to any 8KB block in the 2MB memory space.  
- Memory types include **RAM**, **ROM**, and **Persistent** memory, allowing developers to configure blocks as needed.  
- Persistent memory is write-locked until the page is switched, ensuring efficient state preservation.

#### Upper Memory Map
The upper portion of the CPU’s addressable memory space is reserved for **512 bytes of hardware registers**. These registers enable programmatic control of the system’s integrated devices, such as:  
- **GPU** for video rendering.  
- **Keyboard and joystick input.**  
- **USB storage management.**  
- **Floating-point math operations.**  
This design allows seamless interaction with peripherals and system resources, offering a cohesive programming experience.

#### Applications
- **Programming:** A platform for retrocomputing enthusiasts to program in assembly or C, recapturing the joy of early computing.  
- **Gaming:** A robust base for creating classic-style games with advanced sound and visual capabilities.  
- **Education:** A hands-on tool for teaching:
  - **IoT Device Design:** Using GPIO and peripheral integration.  
  - **Microcontroller Architecture:** Understanding and working with dual-core microcontrollers.  
  - **Operating System Basics:** Exploring kernel and low-level software design.  
  - **Low-Level Programming:** Developing efficient, performance-critical applications.  
  - **Video Game Development:** Creating sprite-based games with hardware collision detection and layered graphics.  
- **Homebrew Projects:** A flexible system for hobbyists and tinkerers to bring creative ideas to life.

This system provides an ideal environment for students and hobbyists to learn and experiment with various aspects of modern and retro-computing technologies. The combination of open-source hardware and software encourages exploration and innovation.
