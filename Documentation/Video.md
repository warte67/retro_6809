# Design Document: Video System for Raspberry Pi Pico 2-based Emulator

## 1. Introduction
This document outlines the design of the **video system** for an 8-bit emulator running on a **Raspberry Pi Pico 2** platform, specifically emulating a **Motorola 6809-based machine**. The emulator’s video system will include support for different resolution modes, sprite management, collision detection, and memory organization for the display. The design aims to be both memory-efficient and fast enough to run on a microcontroller-based platform with limited resources.

## 2. System Overview
The video system is divided into several key components:
- **Resolution Management:** Different display resolutions with pixel scaling.
- **Frame Buffer:** Two banks of video memory dedicated to different tasks.
- **Sprite Management:** A sprite layer with collision detection for interactive elements.
- **Collision Detection:** Bounding box and pixel-perfect collision detection for sprites.

## 3. Video Memory Organization
The video memory is split into two distinct regions:
1. **CPU-Only Bank (8 KB)**: Dedicated to small foreground buffers for text and low-resolution graphics.
2. **GPU-Only Bank (64 KB)**: Dedicated to the main frame buffer for high-resolution video rendering.

### CPU-Only Bank (8 KB)
- **Purpose**: Primarily used for low-resolution graphics (e.g., 320x200 or text-based modes) where the CPU has direct access to the video memory.
- **Usage**:
  - **Text Mode**: A small text buffer for console or status bar.
  - **Low-Resolution Graphics**: Simple graphical objects (e.g., small sprites or tiles).

### GPU-Only Bank (64 KB)
- **Purpose**: Holds the main frame buffer for high-resolution video, with access restricted to the GPU (or controlled via hardware registers).
- **Usage**:
  - **High-Resolution Graphics**: Modes such as **640x400** and **512x384** at **2 bits per pixel** (4 colors).
  - **Memory Efficiency**: By using a 2-bit color depth, the video memory footprint remains small but supports multiple video modes.

## 4. Resolution and Color Modes
The system will support various video resolutions and color depths, each optimized for efficiency while still delivering a retro-style display.

- **320x200 @ 2 bpp (4 colors)**: Fits in the **CPU-only bank** (8 KB).
- **640x400 @ 2 bpp (4 colors)**: Fits in the **GPU-only bank** (64 KB).
- **512x384 @ 2 bpp (4 colors)**: Fits in the **GPU-only bank** (64 KB).
- **Pixel Doubling/Scaling**: The system can support **scaling** for retro-style pixel doubling, where lower resolutions can be scaled up to higher resolutions like **640x400** or **512x384**.

## 5. Sprite Layer
The sprite layer consists of up to **64 sprites** that can be drawn on the screen. Each sprite can have its own pixel data, position, and properties.

### Sprite Properties:
- **Position**: X and Y coordinates on the screen.
- **Size**: Width and height of the sprite.
- **Priority**: Used for determining render order (higher priority sprites are drawn over lower priority ones).
- **Collision Detection Flags**: Determines if the sprite participates in collision detection.

### Collision Detection
Collision detection is applied to the **first 16 sprites** only. These sprites can be interactable and will support:
- **Bounding Box Collision**: A quick check to determine if two sprites’ bounding boxes overlap.
- **Pixel-Perfect Collision**: If bounding boxes overlap, the system will check the exact pixels in the overlapping area to determine if they collide.

- **The remaining 48 sprites** will **not have collision detection**, typically used for background or decorative elements.

### Spatial Indexing for Collision Detection
To optimize collision detection, we use a **quad-tree** to efficiently manage the sprites based on their positions:
- **Quad-Tree Structure**: The 2D screen is divided into **quadrants**, and each quadrant stores references to nearby sprites.
- **Collision Checking**: When checking for collisions, only sprites within the same or adjacent quadrants are checked, reducing the number of comparisons and improving performance.

## 6. Video Rendering Pipeline
The video rendering process involves updating the frame buffer and rendering sprites:
1. **Frame Buffer Update**: The GPU updates the video memory stored in the **GPU-only bank** (64 KB).
2. **Sprite Rendering**: Each frame, the sprite layer is rendered on top of the frame buffer. The GPU fetches sprite data from memory and places it on the screen according to their X and Y positions.
3. **Collision Detection**: During rendering or as part of the logic loop, collision detection is performed on the **first 16 sprites** using the quad-tree for spatial indexing.

## 7. Video Output Interface
The video system will output video signals through a **VGA-compatible interface**:
- **VGA Signals**: Standard **HSYNC** and **VSYNC** signals will be generated to synchronize the screen.
- **Color Channels**: RGB output will be provided via a **12-bit DAC** (4 bits per channel), supporting a **4-color palette** for each active resolution mode.

## 8. Memory Map and Bank Switching
The system will utilize **bank switching** to manage memory and allow efficient access to the video buffer:
- **CPU-Only Bank (8 KB)**: Text and low-resolution modes for CPU manipulation.
- **GPU-Only Bank (64 KB)**: Frame buffer for higher resolution video, manipulated by the GPU with some CPU control via hardware registers.

## 9. Display Modes Summary

| Resolution | Color Depth | Memory Usage | Bank        | Supported Modes |
|------------|-------------|--------------|-------------|-----------------|
| 320x200    | 2 bpp (4 colors)  | 8 KB        | CPU-Only    | Text, Low-Res Graphics |
| 640x400    | 2 bpp (4 colors)  | 64 KB       | GPU-Only    | High-Res Graphics |
| 512x384    | 2 bpp (4 colors)  | 64 KB       | GPU-Only    | High-Res Graphics |
| Pixel Doubling | 2 bpp (4 colors) | 8-64 KB     | Both        | Scaled Modes    |

## 10. Performance Considerations
- The system is designed to run efficiently within the constraints of the **RP2040** microcontroller (clocked at 150 MHz).
- The **GPU-only memory** and **quad-tree** structure for collision detection will help ensure that the CPU is not overloaded with video rendering or collision logic.
- With **two banks of memory**, the system is capable of handling complex graphical tasks (e.g., high-resolution modes) while ensuring that interactive elements like sprites are fast and responsive.

## 11. Conclusion
This video system design leverages efficient memory organization, resolution flexibility, and optimized collision detection through spatial indexing. By incorporating a **two-bank memory structure**, a **quad-tree for collision detection**, and **scalable resolution modes**, the system can provide smooth performance on the **Raspberry Pi Pico 2** while supporting multiple display modes and interactive sprites.

This system is well-suited for your **Motorola 6809-based emulator** and will be able to handle both retro-style 8-bit graphics and efficient sprite management, all while fitting within the resource constraints of the **RP2040**.

---

## Next Steps
- Implement memory management for the two video memory banks.
- Develop the **quad-tree structure** and integrate it with sprite collision detection.
- Test different video modes to verify compatibility with **VGA monitors** and ensure the system performs efficiently.
