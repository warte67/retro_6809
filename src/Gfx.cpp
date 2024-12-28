/*** Gfx.cpp *******************************************
 *       _____    __                                   
 *      / ____|  / _|                                  
 *     | |  __  | |_  __  __       ___   _ __    _ __  
 *     | | |_ | |  _| \ \/ /      / __| | '_ \  | '_ \ 
 *     | |__| | | |    >  <   _  | (__  | |_) | | |_) |
 *      \_____| |_|   /_/\_\ (_)  \___| | .__/  | .__/ 
 *                                      | |     | |    
 *                                      |_|     |_|    
 *
 * This is the base class for all graphics devices. It provides the
 * basic methods for reading and writing to the device's memory.
 * 
 ************************************/



#include "Gfx.hpp"
#include "Memory.hpp"


/****************
 * Read / Write *
 ***************/ 

Byte Gfx::OnRead(Word offset) 
{ 
    Byte data = IDevice::OnRead(offset);
    // std::cout << clr::indent() << clr::CYAN << "Gfx::OnRead($"<< clr::hex(offset,4) << ") = $" << clr::hex(data,2) << "\n" << clr::RESET;

    if (offset == MAP(GFX_MODE))    { data = _gfx_mode; }
    if (offset == MAP(GFX_EMU))     { data = _gfx_emu; }

    return data;
} // END: Gfx::OnRead()

void Gfx::OnWrite(Word offset, Byte data) 
{ 
    // std::cout << clr::indent() << clr::CYAN << "Gfx::OnWrite($" << clr::hex(offset,4) << ", $" << clr::hex(data,2) << ")\n" << clr::RESET;

    if (offset == MAP(GFX_MODE))    
    { 
        if (_change_gfx_mode(data)) 
        { 
            _gfx_mode = data; 
        }
    }
    if (offset == MAP(GFX_EMU))     
    {
        if (_change_emu_mode(data)) 
        { 
            _gfx_emu = data; 
        }
    }

    IDevice::OnWrite( offset, data);
} // END: Gfx::OnWrite()


/***************************
* Constructor / Destructor *
***************************/

Gfx::Gfx() 
{ 
    std::cout << clr::indent_push() << clr::CYAN << "Gfx Created" << clr::RETURN;
    _device_name = "Gfx"; 
} // END: Gfx()

Gfx::~Gfx() 
{ 
    std::cout << clr::indent_pop() << clr::CYAN << "Gfx Destroyed" << clr::RETURN; 
} // END: ~Gfx()



/******************
* Virtual Methods *
******************/


/*****************************************************************
 * virtual int OnAttach(int nextAddr):
 * 
 * This is a virtual method that is called by the Memory class.
 * It is called when the device is attached to the memory map.
 * The device is responsible for allocating its own memory space
 * and returning the size of the allocation.
 * 
 * @param nextAddr The current address in the memory map.
 * @return The size of the allocation.
 ********************/
int  Gfx::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnAttach() Entry" << clr::RETURN;
    if (nextAddr == 0) { ; } // stop the compiler from complaining

    Word old_address=nextAddr;
    this->heading = "Start of Gfx Device Hardware Registers";

    register_node new_node;
    new_node = { "GFX_MODE", nextAddr,  {   "(Byte) Graphics Mode",
                                            "   - bit  7   = reserved" ,
                                            "   - bit  6   = 0:screen is text, ",
                                            "               1:screen is bitmap",
                                            "   - bits 4-5 = horizontal overscan: ",
                                            "               00=1x, 01=2x, 10=4x",
                                            "   - bits 2-3 = vertical overscan: ",
                                            "               00=1x, 01=2x, 10=4x",
                                            "   - bits 0-1 = Color Mode: 00=2-clr, ",
                                            "               01=4-clr, 10=16-clr, ",
                                            "               11=256-clr",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "GFX_EMU", nextAddr,  {    "(Byte) Emulation Flags",
                                            "   - bit  7    = vsync: 0=off, 1=on",
                                            "   - bit  6    = main: 0=windowed,",
                                            "                 1=fullscreen",
                                            "   - bit  5    = debug: 0=off, 1=on",
                                            "   - bit  4    = debug: 0=windowed, ",
                                            "                 1=fullscreen",                                                                                        
                                            "   - bits 2-3  = Debug Monitor 0-3",
                                            "   - bits 0-1  = Active Monitor 0-3",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    _size = nextAddr - old_address;
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnAttach() Exit" << clr::RETURN;
    return _size;   // return the size of the allocation
}


/*****************************************************************
 * bool OnInit():
 * 
 * This is a virtual method that is called by the Memory class.
 * It is called when the device is initialized.
 * The device is responsible for initializing its internal state
 * and returning true if the initialization was successful.
 * 
 * @return True if the initialization was successful.
 *********************/
bool Gfx::OnInit()
{
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnInit() Exit" << clr::RETURN;
    return true;
}


/******************************************************************
 * virtual bool OnQuit(): 
 * 
 * This is a virtual method that is called by the Memory class.
 * It is called when the device is being shut down.
 * The device is responsible for releasing any allocated memory
 * and any other resources.
 *
 * @return True if the shutdown was successful.
 **********************/
bool Gfx::OnQuit()
{
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnQuit() Exit" << clr::RETURN;
    return true;
}


/******************************************************************
 * virtual bool OnActivate():
 * 
 * This is a virtual method that is called to activate the device.
 * It is responsible for preparing the device to be operational,
 * ensuring that all necessary conditions are met for the device to
 * function.
 * 
 * @return True if the device was successfully activated, false
 *          otherwise.
 ***********************/
bool Gfx::OnActivate()
{
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnActivate() Exit" << clr::RETURN;
    return true;
}


/******************************************************************
 * virtual bool OnDeactivate():
 *
 * This is a virtual method that is called to deactivate the device.
 * It is responsible for releasing any resources that were allocated
 * when the device was activated, and for ensuring that all necessary
 * conditions are met for the device to be safely deactivated.
 * 
 * @return True if the device was successfully deactivated, false 
 *          otherwise.
 ***********************/
bool Gfx::OnDeactivate()
{
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::CYAN << "Gfx::OnDeactivate() Exit" << clr::RETURN;
    return true;
}


/********************************************************************
 * virtual bool OnEvent(SDL_Event* evnt):
 * 
 * This is a virtual method that handles events sent to the device.
 * It processes the given SDL_Event and performs the necessary actions
 * based on the event type. 
 *
 * @param evnt A pointer to an SDL_Event structure that contains 
 * the event data to be processed.
 *
 * @return True if the event was handled successfully, false otherwise.
 ********************************************************************/

bool Gfx::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnEvent() Entry" << clr::RETURN;
    if (evnt) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnEvent() Exit" << clr::RETURN;
    return true;
}


/****************************************************************
 * virtual bool OnUpdate(float fElapsedTime):
 *
 * This is a virtual method that is called periodically to allow the
 * device to update its internal state. The frequency of the calls
 * is determined by the Memory class, and the time since the last
 * call is given in the fElapsedTime parameter.
 *
 * @param fElapsedTime The time, in seconds, that has elapsed since
 *  the last call to OnUpdate.
 *
 * @return True if the update was successful, false otherwise.
 ************************************************************************/
bool Gfx::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnUpdate() Entry" << clr::RETURN;
    if (fElapsedTime==0.0f) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnUpdate() Exit" << clr::RETURN;
    return true;
}


/*************************************************************************
 * virtual bool OnRender():
 *
 * This is a virtual method that is called to draw the device's user
 * interface. It is responsible for rendering all necessary graphics
 * and for updating the display.
 * 
 * @return True if the render was successful, false otherwise.
 *************************************************************************/
bool Gfx::OnRender()
{
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::CYAN << "Gfx::OnRender() Exit" << clr::RETURN;
    return true;
}

/*************************************************************************
 * bool _change_gfx_mode(Byte mode):
 * 
 * This function processes the given graphics mode and changes the
 * internal state of the device accordingly. It returns true if the
 * mode change was successful, false otherwise.
 ************************************************************************/
bool Gfx::_change_gfx_mode(Byte mode) 
{ 
// GFX_MODE          equ   0xFE00  ; (Byte) Graphics Mode
//                                 ;    - bit  7   = reserved
//                                 ;    - bit  6   = 0:screen is text, 
//                                 ;                1:screen is bitmap
//                                 ;    - bits 4-5 = horizontal overscan: 
//                                 ;                00=1x, 01=2x, 10=4x
//                                 ;    - bits 2-3 = vertical overscan: 
//                                 ;                00=1x, 01=2x, 10=4x
//                                 ;    - bits 0-1 = Color Mode: 00=2-clr, 
//                                 ;                01=4-clr, 10=16-clr, 
//                                 ;                11=256-clr

    // horizontal overscan pattern 11 is also 4x (instead of error)
    if ((mode & 0b00110000) == 0b00110000) 
    { 
        mode &= 0b11001111;
        mode |= 0b00100000;
    }
    // vertical overscan pattern 11 is also 4x (instead of error)
    if ((mode & 0b00001100) == 0b00001100) 
    { 
        mode &= 0b11110011;
        mode |= 0b00001000;
    }

    return true; 
}

/*************************************************************************
 * bool _change_emu_mode(Byte emu):
 * 
 * This function processes the given emulation flags and changes the
 * internal state of the device accordingly. It returns true if the
 * flag change was successful, false otherwise.
 ************************************************************************/
bool Gfx::_change_emu_mode(Byte emu) 
{ 
// GFX_EMU           equ   0xFE01  ; (Byte) Emulation Flags
//                             ;    - bit  7    = vsync: 0=off, 1=on
//                             ;    - bit  6    = main: 0=windowed,
//                             ;                  1=fullscreen
//                             ;    - bit  5    = debug: 0=off, 1=on
//                             ;    - bit  4    = debug: 0=windowed, 
//                             ;                  1=fullscreen
//                             ;    - bits 2-3  = Debug Monitor 0-3
//                             ;    - bits 0-1  = Active Monitor 0-3

    if (emu)  {;}     // stop the compiler from complaining

    return true; 
}    



/***** NOTES: ******


# Video Timing Calculations for 320x200 @ 60Hz

## Horizontal Timing (pixels)
- Active Display: 320
- Front Porch: 16
- Sync Pulse: 32  
- Back Porch: 32
- Total Horizontal: 400 pixels

## Vertical Timing (lines)
- Active Display: 200
- Front Porch: 10
- Sync Pulse: 2
- Back Porch: 33
- Total Vertical: 245 lines

## Clock Calculations
- Total Pixels per Frame = 400 * 245 = 98,000 pixels
- Frames per Second = 60
- Pixel Clock = 98,000 * 60 = 5.88 MHz

## Resolution Modes via Overscan Flags
Base Resolution (no overscan):    320x200
Horizontal Overscan:             160x200 (doubled horizontal pixels)
Vertical Overscan:               320x100 (doubled vertical pixels)
Both Overscan:                   160x100 (doubled in both directions)

# Display Buffer Memory Requirements

## 320x200 Base Resolution
- Monochrome (1bpp):   8,000 bytes  (320x200 ÷ 8)
- 4-Color (2bpp):     16,000 bytes  (320x200 ÷ 4)
- 16-Color (4bpp):    32,000 bytes  (320x200 ÷ 2)
- 256-Color (8bpp):   64,000 bytes  (320x200 ÷ 1)

## 320x100 Resolution
- Monochrome (1bpp):   4,000 bytes  (320x100 ÷ 8)
- 4-Color (2bpp):      8,000 bytes  (320x100 ÷ 4)
- 16-Color (4bpp):    16,000 bytes  (320x100 ÷ 2)
- 256-Color (8bpp):   32,000 bytes  (320x100 ÷ 1)

## 320x50 Resolution
- Monochrome (1bpp):   2,000 bytes  (320x50 ÷ 8)
- 4-Color (2bpp):      4,000 bytes  (320x50 ÷ 4)
- 16-Color (4bpp):     8,000 bytes  (320x50 ÷ 2)
- 256-Color (8bpp):   16,000 bytes  (320x50 ÷ 1)

## 160x200 Resolution
- Monochrome (1bpp):   4,000 bytes  (160x200 ÷ 8)
- 4-Color (2bpp):      8,000 bytes  (160x200 ÷ 4)
- 16-Color (4bpp):    16,000 bytes  (160x200 ÷ 2)
- 256-Color (8bpp):   32,000 bytes  (160x200 ÷ 1)

## 160x100 Resolution
- Monochrome (1bpp):   2,000 bytes  (160x100 ÷ 8)
- 4-Color (2bpp):      4,000 bytes  (160x100 ÷ 4)
- 16-Color (4bpp):     8,000 bytes  (160x100 ÷ 2)
- 256-Color (8bpp):   16,000 bytes  (160x100 ÷ 1)

## 160x50 Resolution
- Monochrome (1bpp):   1,000 bytes  (160x50 ÷ 8)
- 4-Color (2bpp):      2,000 bytes  (160x50 ÷ 4)
- 16-Color (4bpp):     4,000 bytes  (160x50 ÷ 2)
- 256-Color (8bpp):    8,000 bytes  (160x50 ÷ 1)

## 80x200 Resolution
- Monochrome (1bpp):   2,000 bytes  (80x200 ÷ 8)
- 4-Color (2bpp):      4,000 bytes  (80x200 ÷ 4)
- 16-Color (4bpp):     8,000 bytes  (80x200 ÷ 2)
- 256-Color (8bpp):   16,000 bytes  (80x200 ÷ 1)

## 80x100 Resolution
- Monochrome (1bpp):   1,000 bytes  (80x100 ÷ 8)
- 4-Color (2bpp):      2,000 bytes  (80x100 ÷ 4)
- 16-Color (4bpp):     4,000 bytes  (80x100 ÷ 2)
- 256-Color (8bpp):    8,000 bytes  (80x100 ÷ 1)

## 80x50 Resolution
- Monochrome (1bpp):     500 bytes  (80x50 ÷ 8)
- 4-Color (2bpp):      1,000 bytes  (80x50 ÷ 4)
- 16-Color (4bpp):     2,000 bytes  (80x50 ÷ 2)
- 256-Color (8bpp):    4,000 bytes  (80x50 ÷ 1)

## Notes
- Border color taken from palette index 0
- Visible area varies by overscan mode
- Timing compatible with 6MHz target pixel clock
- Overscan flags modify pixel/line doubling during active display only
- Sync timing parameters remain constant across all modes

# Text Mode Specifications

## Character Format
- 8x8 pixel characters
- 8 bytes per character definition (1 byte per row)
- 256 possible characters (8-bit character code)
- Character ROM size: 2048 bytes (256 chars * 8 bytes)

## Attribute Byte Format
- Byte 1: Character code (0-255)
- Byte 2: Color attributes
  - Bits 7-4: Foreground color (0-15)
  - Bits 3-0: Background color (0-15)

## Available Text Resolutions
1. 40x25 characters (320x200 base)
   - 1000 character positions
   - 2000 bytes total (2 bytes per position)

2. 40x12 characters (320x100 V2x)
   - 480 character positions
   - 960 bytes total

3. 40x6 characters (320x50 V4x)
   - 240 character positions
   - 480 bytes total

4. 20x25 characters (160x200 H2x)
   - 500 character positions
   - 1000 bytes total

5. 20x12 characters (160x100 H2x+V2x)
   - 240 character positions
   - 480 bytes total

6. 20x6 characters (160x50 H2x+V4x)
   - 120 character positions
   - 240 bytes total

7. 10x25 characters (80x200 H4x)
   - 250 character positions
   - 500 bytes total

8. 10x12 characters (80x100 H4x+V2x)
   - 120 character positions
   - 240 bytes total

9. 10x6 characters (80x50 H4x+V4x)
   - 60 character positions
   - 120 bytes total



# Sprite System Specifications

## Complete Sprite Data Structure (80 bytes per sprite)
### Image Data (64 bytes)
- 16x16 pixels
- 4 colors per sprite (2 bits per pixel)
- 64 bytes total for image data

### Palette Data (8 bytes)
- 4 colors per sprite
- 2 bytes per color (A4R4G4B4 format)
- 8 bytes total for palette data

### Attribute Block (8 bytes)
#### Byte 0: Control Flags
- Bit 7: Enable/Disable sprite (1=visible, 0=hidden)
- Bits 6-5: Priority (0x11 front, 0x10 mid-fore, 0x01 mid-back, 0x00 back)
- Bit 4: Enable 4x4 collision detection (sprites 0-15)
- Bit 3: Enable bounding box collision (sprites 16-31)
- Bits 2-0: User definable bits (custom per-sprite flags)

#### Bytes 1-2: X Position
- 16-bit signed integer for horizontal position

#### Bytes 3-4: Y Position
- 16-bit signed integer for vertical position

#### Bytes 5-6: Collision Mask
- Byte 5: [TTTT][TTMM] (Top and Top-Middle rows)
- Byte 6: [BBMM][BBBB] (Bottom-Middle and Bottom rows)

#### Byte 7: User Definable Data
- Available for custom programmer use on a per sprite basis

## System Capacity
- 64 sprites total (5120 bytes)
- Memory usage: 5120 bytes total

## Collision Detection Tiers
1. Sprites 0-15: Enhanced collision detection
   - Bounding box detection
   - 4x4 collision mask for near pixel-perfect detection
2. Sprites 16-31: Basic collision
   - Bounding box detection only
3. Sprites 32-63: No collision detection
   - Visual elements only

## Priority System (2-bit)
- 0x11: Front layer (UI, projectiles)
- 0x10: Middle foreground (players, enemies)
- 0x01: Middle background (platforms, collectibles)
- 0x00: Background (decorative elements)

## Notes:
- Within each priority level, sprite index determines draw order
- 4x4 collision mask provides efficient near pixel-perfect collision detection
- Individual palettes allow unique colors per sprite




************/ 




// END: Gfx.cpp
