# retro_6809
A complete refactoring of the alpha_6809 project. This is a work in progress. Please check back later for updates.

### Project Summary: **The Dream 6809 Computer Reimagined**

## Overview

The **retro_6809** project is a fully open-source retro computing project designed to replicate the vision of a 6809-based computer with modern components. Using two Raspberry Pi Pico 2 boards, this project brings together the power of the **Motorola 6809** CPU emulation with modern peripherals, graphics, audio, and general-purpose input/output capabilities. It provides an environment for **students** and **developers** to learn about hardware design, embedded systems, operating systems, and video game development.

### Historical Context and Motivation

The project draws inspiration from the 1980s era, specifically from my experience programming on the TRS-80 Color Computer and the Commodore 64. I started out programming in **BASIC**, which was too slow for my needs. I soon switched to assembly for the **Motorola 6809** processor, but I couldn't help but notice that my nephew’s **Commodore 64** had better graphics and sound capabilities. While I did learn assembly for the **C64**, I still longed for a machine that combined the 6809's power with better graphics and sound.

The **retro_6809** project aims to fulfill that dream. By using modern Raspberry Pi Pico 2 boards, we have a platform that provides the **6809 emulation** along with rich hardware features.

---

## Hardware Architecture

Each **Raspberry Pi Pico 2** board has **two processor cores**. The system uses two **Pico 2** boards, each playing a specific role:

- **Core 1 on Board 1:** Cycle-accurate **6809 emulation**, system clock generation, and memory management.
- **Core 2 on Board 1:** **GPU emulation** that drives VGA output, manages pixel streaming, and generates **HSYNC** and **VSYNC** signals.
- **Core 1 on Board 2:** **Commodore SID emulation** for FM audio synthesis.
- **Core 2 on Board 2:** Manages **USB peripherals** (keyboard, mouse, joysticks, storage) and communication with a host PC for debugging.

Additionally, **32 GPIO pins** are exposed for external projects.

### Graphics Capabilities

The **GPU** on Board 1 (Core 2) uses **12 GPIO pins** to generate VGA output through resistor ladders, supporting three color channels (0V to 0.7V). The system features:

- **256-color palette** (A4R4G4B4 format) with up to **4096 colors**.
- **64KB GPU-addressable background buffer** for a **320x200 display** with 256 colors.
- **8KB CPU-addressable foreground layer** for low-resolution text and graphics.
- **Sprites:** Supports up to **64 sprites** with collision detection. The first **16 sprites** support pixel-level collision, and **bounding-box** collision is supported for the first **32 sprites**. These sprites use a **4x4 collision mask**.
- **Tiles:** A fuly featured 16x16 tilemap system is planned, but not yet designed.

### Memory Architecture

The system uses a **2MB paged memory system**:

- **8KB of addressable memory** pages through the entire **2MB memory**.
- Each page can be **RAM, ROM, or Persistent memory**. Persistent memory is used to save application states when the page is switched.
  
Additionally, there are **512 bytes of hardware registers** available, which can be programmatically addressed for devices such as the GPU, Keyboard, Joysticks, USB Storage, Floating Point Math, and more.

---

## Software Features

This project includes a **cycle-accurate emulator** of the **Motorola 6809**, providing a platform for both retro and modern software development. The emulator includes a **VGA graphics output**, a **FM audio synthesizer**, and a **fully functional I/O system** with USB support for peripherals.

### Use Cases for Education

The **retro_6809** project serves as a hands-on platform for teaching:

- **Microcontroller design** and embedded systems.
- **Operating systems** and **kernel development** for low-level programming.
- **Video game design**, hardware interfacing, and graphics programming.
- Understanding the **basics of assembly language** and retro computing.

---

## License

This project is licensed under the **GNU General Public License v3.0** (GPL v3). You are free to use, modify, and distribute the code and designs under the terms of the [GPL-3.0](LICENSE.md), as long as you follow the license's requirements. 

### GPL v3 Highlights

- **Freedom to use, modify, and distribute**: You are free to use the software for any purpose, modify it, and distribute your changes under the same terms.
- **Share-alike**: Any derivative works of this project, whether hardware or software, must also be released under the GPL v3 license. Any modifications to the code or design must be shared with the community.
- **Commercial Use**: You may sell the pre-built hardware and provide commercial services, but any modified code or design must remain open-source and licensed under GPL v3.
- **Reverse Engineering**: You have the right to reverse-engineer the software, as long as any modified code is shared under the GPL v3.

### Closed-Source Software Note

Software developed to **utilize the 2MB banked memory format** can be closed-source, as long as it does not directly incorporate or link to GPL-licensed code. However, **reverse-engineering** or **disassembly** of the code is allowed, and any modifications must still adhere to the GPL v3 if they are derived from this project.

###### For the full GPL v3 license text, please visit the official page: [https://www.gnu.org/licenses/gpl-3.0.html](https://www.gnu.org/licenses/gpl-3.0.html)

---

## Contributing

Contributions to the project are welcome! If you would like to contribute to the development of the retro_6809 project, please follow these steps:

1. Fork the repository.
2. Create a new branch.
3. Make your changes.
4. Submit a pull request.

Please ensure that all contributions are well-documented and comply with the GPL v3 license.

---

## Acknowledgments

- The **Raspberry Pi Pico 2** boards for their powerful and affordable microcontroller capabilities.
- The community of **retro computing enthusiasts** who inspired and contributed to the original ideas behind this project.
- The **open-source community** for their ongoing support and contribution to free software.

---

## Contact

For more information, or to get involved, feel free to open an issue on GitHub or contact the project maintainers via email.


--- 


### Closed-Source Software Note

Software developed to **utilize the 2MB banked memory format** can be closed-source, as long as it does not directly incorporate or link to GPL-licensed code. However, **reverse-engineering** or **disassembly** of the code is allowed, and any modifications must still adhere to the GPL v3 if they are derived from this project.

It is also allowed for software to access and interact with the system's **hardware registers** (located at memory addresses **$FE00-$FFEF**), as these registers are part of the open system hardware. This access does not impose any restrictions on the software being open or closed source.


---

## Contributing

Contributions to the project are welcome! If you would like to contribute to the development of the retro_6809 project, please follow these steps:

1. Fork the repository.
2. Create a new branch.
3. Make your changes.
4. Submit a pull request.

Please ensure that all contributions are well-documented and comply with the GPL v3 license.

---

## Acknowledgments

- The **Raspberry Pi Pico 2** boards for their powerful and affordable microcontroller capabilities.
- The community of **retro computing enthusiasts** who inspired and contributed to the original ideas behind this project.
- The **open-source community** for their ongoing support and contribution to free software.

---

## Contact

For more information, or to get involved, feel free to open an issue on GitHub or contact the project maintainers via email.
