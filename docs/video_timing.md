### **Video Timing Calculations for 640x400 (Active) @ 70Hz within 640x480 Base**

## **Horizontal Timing (pixels)**:
- Active Display: 640
- Front Porch: 16
- Sync Pulse: 96  
- Back Porch: 48
- Total Horizontal: 800 pixels

## **Vertical Timing (lines)**:
- Active Display: 400  
- Front Porch: 10  
- Sync Pulse: 2  
- Back Porch: 33  
- Blank Lines: 40 (20 top, 20 bottom)  
- Total Vertical: 525 lines

## **Clock Calculations**:
- Total Pixels per Frame = 800 * 525 = 420,000 pixels
- Frames per Second = 70
- Pixel Clock = 420,000 * 70 = 29.4 MHz

## **Resolution Modes via Overscan and Border Adjustments**:
Base Resolution (no overscan): 640x400 active display within 640x480 base  
Single Overscan (2x): 320x200 active display within 320x240 base  
Double Overscan (4x): 160x100 active display within 160x120 base  
Quad Overscan (8x): 80x50 active display within 80x60 base  
512x320 (More border): 512x320 active display with extended borders

### **Video Modes with 512x320 Overscan**:
Using **512x320** as the active display, the following overscan modes are derived:

1. **Base 512x320 Resolution**:  
   - Active area is centered within the 640x480 base with increased horizontal and vertical porch sizes.
   
2. **Overscan Variants**:  
   - Single Overscan (256x160): Doubled pixels horizontally and vertically.
   - Double Overscan (128x80): Quadrupled pixels horizontally and vertically.
   - Quad Overscan (64x40): Octupled pixels horizontally and vertically.

---

### **Display Buffer Memory Requirements**

#### **640x400 Active Display**:
- Monochrome (1bpp): 32,000 bytes  (640x400 ÷ 8)
- 4-Color (2bpp): 64,000 bytes  (640x400 ÷ 4)
- 16-Color (4bpp): 128,000 bytes  (640x400 ÷ 2)
- 256-Color (8bpp): 256,000 bytes  (640x400 ÷ 1)

#### **512x320 Active Display**:
- Monochrome (1bpp): 20,480 bytes  (512x320 ÷ 8)
- 4-Color (2bpp): 40,960 bytes  (512x320 ÷ 4)
- 16-Color (4bpp): 81,920 bytes  (512x320 ÷ 2)
- 256-Color (8bpp): 163,840 bytes  (512x320 ÷ 1)

#### **320x200 Resolution**:
- Monochrome (1bpp): 8,000 bytes  (320x200 ÷ 8)
- 4-Color (2bpp): 16,000 bytes  (320x200 ÷ 4)
- 16-Color (4bpp): 32,000 bytes  (320x200 ÷ 2)
- 256-Color (8bpp): 64,000 bytes  (320x200 ÷ 1)

#### **256x160 Resolution (from 512x320 Overscan)**:
- Monochrome (1bpp): 5,120 bytes  (256x160 ÷ 8)
- 4-Color (2bpp): 10,240 bytes  (256x160 ÷ 4)
- 16-Color (4bpp): 20,480 bytes  (256x160 ÷ 2)
- 256-Color (8bpp): 40,960 bytes  (256x160 ÷ 1)

#### **160x100 Resolution**:
- Monochrome (1bpp): 2,000 bytes  (160x100 ÷ 8)
- 4-Color (2bpp): 4,000 bytes  (160x100 ÷ 4)
- 16-Color (4bpp): 8,000 bytes  (160x100 ÷ 2)
- 256-Color (8bpp): 16,000 bytes  (160x100 ÷ 1)

#### **64x40 Resolution (from 512x320 Overscan)**:
- Monochrome (1bpp): 320 bytes  (64x40 ÷ 8)
- 4-Color (2bpp): 640 bytes  (64x40 ÷ 4)
- 16-Color (4bpp): 1,280 bytes  (64x40 ÷ 2)
- 256-Color (8bpp): 2,560 bytes  (64x40 ÷ 1)

---

### **Text Mode Specifications (Adjusted for 640x400 Active)**

#### **Character Format**:
- 8x8 pixel characters
- 8 bytes per character definition (1 byte per row)
- 256 possible characters (8-bit character code)
- Character ROM size: 2048 bytes (256 chars * 8 bytes)

#### **Attribute Byte Format**:
- Byte 1: Character code (0-255)
- Byte 2: Color attributes  
  - Bits 7-4: Foreground color (0-15)  
  - Bits 3-0: Background color (0-15)

#### **Available Text Resolutions**:
1. **80x50 characters (640x400 base)**  
   - 4,000 character positions  
   - 8,000 bytes total (2 bytes per position)

2. **64x40 characters (512x320 adjusted porch)**  
   - 2,560 character positions  
   - 5,120 bytes total

3. **40x25 characters (320x200 S2x)**  
   - 1,000 character positions  
   - 2,000 bytes total

4. **32x20 characters (256x160 S2x from 512x320 overscan)**  
   - 640 character positions  
   - 1,280 bytes total

5. **20x12 characters (160x100 S4x)**  
   - 240 character positions  
   - 480 bytes total

6. **10x6 characters (64x40 S8x)**  
   - 60 character positions  
   - 120 bytes total

---

### **Notes**:
- The blank lines for the **640x400 active display** (20 lines at top and bottom) are filled with the border color.
- The **512x320 mode** uses extended porches to center the active display within the 640x480 base, allowing overscan-derived modes.
- All sync timings remain constant at 29.4 MHz pixel clock for compatibility.
- Overscan and porch adjustments are achieved without altering the sync timings, ensuring monitor compatibility.

---

### **Sprite System Specifications**

#### **Complete Sprite Data Structure (80 bytes per sprite)**

##### **Image Data (64 bytes)**:
- 16x16 pixels
- 4 colors per sprite (2 bits per pixel)
- 64 bytes total for image data

##### **Palette Data (8 bytes)**:
- 4 colors per sprite
- 2 bytes per color (A4R4G4B4 format)
- 8 bytes total for palette data

##### **Attribute Block (8 bytes)**:
- Byte 0: Control Flags  
  - Bit 7: Enable/Disable sprite (1=visible, 0=hidden)  
  - Bits 6-5: Priority (0x11 front, 0x10 mid-fore, 0x01 mid-back, 0x00 back)  
  - Bit 4: Enable 4x4 collision detection (sprites 0-15)  
  - Bit 3: Enable bounding box collision (sprites 16-31)  
  - Bits 2-0: User definable bits (custom per-sprite flags)

- Bytes 1-2: X Position  
  - 16-bit signed integer for horizontal position

- Bytes 3-4: Y Position  
  - 16-bit signed integer for vertical position

- Bytes 5-6: Collision Mask  
  - Byte 5: [TTTT][TTMM] (Top and Top-Middle rows)  
  - Byte 6: [BBMM][BBBB] (Bottom-Middle and Bottom rows)

- Byte 7: User Definable Data  
  - Available for custom programmer use on a per sprite basis

#### **System Capacity**:
- 64 sprites total (5120 bytes)
- Memory usage: 5120 bytes total

#### **Collision Detection Tiers**:
1. **Sprites 0-15**: Enhanced collision detection  
   - Bounding box detection  
   - 4x4 collision mask for near pixel-perfect detection  
2. **Sprites 16-31**: Basic collision  
   - Bounding box detection only  
3. **Sprites 32-63**: No collision detection  
   - Visual elements only

#### **Priority System (2-bit)**:
- 0x11: Front layer (UI, projectiles)
- 0x10: Middle foreground (players, enemies)
- 0x01: Middle background (platforms, collectibles)
- 0x00: Background (decorative elements)

#### **Notes**:
- Within each priority level, sprite index determines draw order
- 4x4 collision mask provides efficient near pixel-perfect collision detection
- Individual palettes allow unique colors per sprite


