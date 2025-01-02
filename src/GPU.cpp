/*** GPU.cpp *******************************************
 *     _____ _____  _    _                        
 *    / ____|  __ \| |  | |                       
 *   | |  __| |__) | |  | |       ___ _ __  _ __  
 *   | | |_ |  ___/| |  | |      / __| '_ \| '_ \ 
 *   | |__| | |    | |__| |  _  | (__| |_) | |_) |
 *    \_____|_|     \____/  (_)  \___| .__/| .__/ 
 *                                   | |   | |    
 *                                   |_|   |_|    
 *
 * This is the base class for the primary graphics devices. It provides the
 * basic methods for reading and writing to the device's memory.
 * 
 ************************************/


#include "Bus.hpp"
#include "GPU.hpp"
#include "Memory.hpp"


/****************
 * Read / Write *
 ***************/ 

/**
 * This is the read callback for the GPU device. It is called whenever the CPU
 * reads from a location in the device's memory space.
 *
 * The read callback is responsible for returning the value stored at the
 * specified memory location.
 *
 * If the offset is out of range, the function returns a default value of 0xCC
 * to indicate an error.
 *
 * @param offset The memory offset from which to read.
 * 
 * @return The value read from the specified memory location, or 0xCC if the
 *         offset is out of range.
 */
Byte GPU::OnRead(Word offset) 
{ 
    Byte data = IDevice::OnRead(offset);    // use this for other devices
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnRead($"<< clr::hex(offset,4) << ") = $" << clr::hex(data,2) << "\n" << clr::RESET;

    if (offset == MAP(GPU_ENABLE))           { data = _gpu_enable;   }
    else if (offset == MAP(GPU_STD_MODE))    { data = _gpu_std_mode; }
    else if (offset == MAP(GPU_EXT_MODE))    { data = _gpu_ext_mode; }
    else if (offset == MAP(GPU_EMULATION))   { data = _gpu_emu_mode; }

    return data;
} // END: GPU::OnRead()


/**
 * This is the write callback for the GPU device. It is called whenever the CPU
 * writes to a location in the device's memory space.
 *
 * The write callback is responsible for modifying the device state according
 * to the value written to the specified memory location.
 *
 * If the offset is out of range, the function does nothing.
 *
 * @param offset The memory offset to which to write.
 * @param data The value to be written to the specified memory location.
 */
void GPU::OnWrite(Word offset, Byte data) 
{ 
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnWrite($" << clr::hex(offset,4) << ", $" << clr::hex(data,2) << ")\n" << clr::RESET;

    if (offset == MAP(GPU_ENABLE))          { data = _change_gpu_enable(data); } 
    else if (offset == MAP(GPU_STD_MODE))   { data = _change_std_mode(data);   }  
    else if (offset == MAP(GPU_EXT_MODE))   { data = _change_ext_mode(data);   }
    else if (offset == MAP(GPU_EMULATION))  { data = _change_emu_mode(data);   }

    IDevice::OnWrite( offset, data);
} // END: GPU::OnWrite()





/***************************
* Constructor / Destructor *
***************************/

GPU::GPU() 
{ 
    std::cout << clr::indent_push() << clr::CYAN << "GPU Created" << clr::RETURN;
    _device_name = "GPU_DEVICE"; 
} // END: GPU()

GPU::~GPU() 
{ 
    std::cout << clr::indent_pop() << clr::CYAN << "GPU Destroyed" << clr::RETURN; 
} // END: ~GPU()



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
int  GPU::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Entry" << clr::RETURN;
    if (nextAddr == 0) { ; } // stop the compiler from complaining

    Word old_address=nextAddr;
    this->heading = "GPU Device Hardware Registers";

    register_node new_node;

    new_node = { "GPU_ENABLE", nextAddr,  {   "(Byte) Bitflag Enables",
                                            "   - bits 5-7 = (reserved)",
                                            "   - bits 3   = 0: Disable Standard Mode,",
                                            "                1: Enable Standard Mode",   
                                            "   - bit  2   = 0: Disable Extended Mode,",
                                            "                1: Enable Extended Mode",
                                            "   - bit  1   = 0: Disable Sprites,",
                                            "                1: Enable Sprites",
                                            "   - bit  0   = 0: Disable Mouse Cursor,",
                                            "                1: Enable Mouse Cursor",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "GPU_STD_MODE", nextAddr,  {   "(Byte) Standard Graphics Mode",
                                            "   - bit  7   = 0: screen is text, ",
                                            "                1: screen is bitmap",
                                            "   - bits 5-6 = bitmap color depth:",
                                            "                00: 2 colors,",
                                            "                01: 4 colors,",
                                            "                10: 16 colors, ",
                                            "                11: 256 colors",
                                            "   - bits 3-4 = horizontal overscan: ",
                                            "                00:H/1 (512 or 640)",
                                            "                01:H/2 (256 or 320)",
                                            "                10:H/3 (170 or 213)",
                                            "                11:H/4 (128 or 160)",
                                            "   - bits 1-2 = vertical overscan: ",
                                            "                00:V/1 (320 or 400)",
                                            "                01:V/2 (160 or 200)",
                                            "                10:V/3 (106 or 133)",
                                            "                11:V/4 (80 or 100)",
                                            "   - bit  0   = Video Timing:",
                                            "                0: H:512 x V:320",
                                            "                1: H:640 x V:400",
                                            "                (Overrides EXT_MODE)",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "GPU_EXT_MODE", nextAddr,  {   "(Byte) Extended Graphics Mode",
                                            "   - bit  7   = 0: screen is tiled,",
                                            "                1: screen is bitmap",
                                            "   - bits 5-6 = bitmap color depth:",
                                            "                00: 2 colors,",
                                            "                01: 4 colors,",
                                            "                10: 16 colors, ",
                                            "                11: 256 colors",
                                            "   - bits 3-4 = horizontal overscan: ",
                                            "                00:H/1 (512 or 640)",
                                            "                01:H/2 (256 or 320)",
                                            "                10:H/3 (170 or 213)",
                                            "                11:H/4 (128 or 160)",
                                            "   - bits 1-2 = vertical overscan: ",
                                            "                00:V/1 (320 or 400)",
                                            "                01:V/2 (160 or 200)",
                                            "                10:V/3 (106 or 133)",
                                            "                11:V/4 (80 or 100)",
                                            "   - bit  0   = Video Timing:",
                                            "                0: H:512 x V:320",
                                            "                1: H:640 x V:400",
                                            "                (Overrides STD_MODE)",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "GPU_EMULATION", nextAddr,  {    "(Byte) Emulation Flags",
                                            "   - bit  7    = vsync: 0=off, 1=on",
                                            "   - bit  6    = main: 0=windowed,",
                                            "                 1=fullscreen",
                                            "   - bit  5    = debug: 0=windowed, ",
                                            "                 1=fullscreen",                                                                                        
                                            "   - bit  4    = debug: 0=off, 1=on",
                                            "   - bits 2-3  = Active Monitor 0-3",
                                            "   - bits 0-1  = Debug Monitor 0-3",
                                            "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);

    _size = nextAddr - old_address;
    std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Exit" << clr::RETURN;
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
bool GPU::OnInit()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnInit() Entry" << clr::RETURN;

    { // BEGIN OF SDL3 Initialization
        // initialize SDL3
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onInit() Error" << clr::RETURN;
            Bus::Error(SDL_GetError(), __FILE__, __LINE__);
            return false;
        }
        // create the main window
        pWindow = SDL_CreateWindow("SDL3 Retro_6809", initial_width, initial_height, window_flags); 
        SDL_ShowWindow(pWindow);

        
    } // END OF SDL3 Initialization

    // initialize the default values
    _change_gpu_enable(_gpu_enable);
    _change_std_mode(_gpu_std_mode);
    _change_ext_mode(_gpu_ext_mode);    
    _change_emu_mode(_gpu_emu_mode);

    std::cout << clr::indent() << clr::CYAN << "GPU::OnInit() Exit" << clr::RETURN;
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
bool GPU::OnQuit()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnQuit() Entry" << clr::RETURN;
    
    { // BEGIN OF SDL3 Shutdown
        if (pRenderer)
        { // destroy the renderer
            SDL_DestroyRenderer(pRenderer);
            pRenderer = nullptr;
        }
        if (pWindow)
        { // destroy the window
            SDL_DestroyWindow(pWindow);
            pWindow = nullptr;
        }
        // shutdown SDL
        SDL_Quit();
    } // END OF SDL3 Shutdown

    std::cout << clr::indent() << clr::CYAN << "GPU::OnQuit() Exit" << clr::RETURN;
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
bool GPU::OnActivate()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Entry" << clr::RETURN;

    // create the renderer
    pRenderer = SDL_CreateRenderer(pWindow, NULL);

    // the extended video mode should determine the overall renderer logical presentation area
    SDL_SetRenderLogicalPresentation(pRenderer, (int)_screen_width, (int)_screen_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // build the textures
    pExt_Texture = SDL_CreateTexture(pRenderer, 
            SDL_PIXELFORMAT_ARGB4444, 
            SDL_TEXTUREACCESS_STREAMING, 
            (int)_ext_width, (int)_ext_height);

    pStd_Texture = SDL_CreateTexture(pRenderer, 
            SDL_PIXELFORMAT_ARGB4444, 
            SDL_TEXTUREACCESS_STREAMING, 
            (int)_std_width, (int)_std_height);

    pMain_Texture = SDL_CreateTexture(pRenderer, 
            SDL_PIXELFORMAT_ARGB4444, 
            SDL_TEXTUREACCESS_TARGET, 
            (int)_screen_width, (int)_screen_height);
    
    std::cout << "std_width: " << std::to_string((int)_std_width) << std::endl;
    std::cout << "std_height: " << std::to_string((int)_std_height) << std::endl;

    std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Exit" << clr::RETURN;
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
bool GPU::OnDeactivate()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnDeactivate() Entry" << clr::RETURN;

    // destroy the renderer
    if (pRenderer) { 
        SDL_DestroyRenderer(pRenderer);
        pRenderer = nullptr;
    }
    // destroy the textures
    if (pExt_Texture) { 
        SDL_DestroyTexture(pExt_Texture);
        pExt_Texture = nullptr;
    }
    if (pStd_Texture) { 
        SDL_DestroyTexture(pStd_Texture);
        pStd_Texture = nullptr;
    }
    if (pMain_Texture) { 
        SDL_DestroyTexture(pMain_Texture);
        pMain_Texture = nullptr;
    }   
    
    std::cout << clr::indent() << clr::CYAN << "GPU::OnDeactivate() Exit" << clr::RETURN;
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

bool GPU::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::CYAN << "GPU::OnEvent() Entry" << clr::RETURN;
    if (evnt) { ; } // stop the compiler from complaining
    
    switch (evnt->type) 
    {
        case SDL_EVENT_KEY_DOWN:
        {
            // [ESCAPE]  KMOD_SHIFT
            SDL_Keymod mod = SDL_GetModState();

            // TESTING [F] and [F-SHIFT] to toggle fullscreen
            if (evnt->key.key == SDLK_F)
            {
                if (mod & SDL_KMOD_SHIFT) {
                    // Byte data = Memory::Read( MAP(GFX_EMU) );
                    // data &= 0b10111111;
                    // Memory::Write(MAP(GFX_EMU), data);
                    SDL_SetWindowFullscreen(pWindow, false);                    
                } else {
                    // Byte data = Memory::Read( MAP(GFX_EMU) );
                    // data |= 0b01000000  ;
                    // Memory::Write(MAP(GFX_EMU), data);
                    SDL_SetWindowFullscreen(pWindow, true);
                }                    
            } // END: if (evnt->key.key == SDLK_F)
            break;            
        } // END: case SDL_EVENT_KEY_DOWN

        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        {            
            _gpu_emu_mode |= 0b01000000; // Set bit 6 = ENTER FULLSCREEN
            std::cout << "SDL_EVENT_WINDOW_ENTER_FULLSCREEN (0x" << clr::hex(_gpu_emu_mode,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        {
            _gpu_emu_mode &= 0b10111111; // Clear bit 6 = LEAVE FULLSCREEN
            std::cout << "SDL_EVENT_WINDOW_LEAVE_FULLSCREEN (0x" << clr::hex(_gpu_emu_mode,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

    } // END: switch  (evnt->type) 

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnEvent() Exit" << clr::RETURN;
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
bool GPU::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::CYAN << "GPU::OnUpdate() Entry" << clr::RETURN;
    if (fElapsedTime==0.0f) { ; } // stop the compiler from complaining

    static float deltaTime = fElapsedTime;
    static float runningTime = fElapsedTime;
    if (runningTime > deltaTime + 0.1f) {
        deltaTime = fElapsedTime;
        runningTime = fElapsedTime;
        SDL_SetWindowTitle(pWindow, Bus::GetTitle().c_str());
        // std::cout << "FPS: " << Bus::FPS() << std::endl;
    }
    runningTime += fElapsedTime;
    // std::cout << fElapsedTime << std::endl;

    if (false)
    { // TESTING:  Something to look at while running these tests...
        SDL_SetRenderTarget(pRenderer, NULL);
        static Uint16 r=0;
        static Uint16 g=0;
        static Uint16 b=0;
        Uint16 a=255;
        SDL_SetRenderDrawColor(pRenderer, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a);
        Uint16 t=4;
        b+=t;
        if (b>255) {  b=0; g+=t;  }
        if (g>255) {  g=0; r+=t;  }
        if (r>255) {  r=0; b+=t;  }
        SDL_RenderClear(pRenderer);
    } // END TESTING ...

    Word *pixels;
    int pitch;

    if (true)
    { // Fill the Extended Display with Random Noise
        SDL_LockTexture(pExt_Texture, NULL, (void **)&pixels, &pitch); // Lock the texture for write accesspExt_Texture
        for (int y=0; y< (int)_ext_height; y++) {
            for (int x=0; x<(int)_ext_width; x++) {
                // Word gray = rand()%6;
                Word r = rand()%8;  // 16
                Word g = rand()%8;  // 16
                Word b = rand()%8;  // 16
                Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));
                *dst = ( 0xf000 | (r<<8) | (g<<4) | (b) );
            }
        }
        SDL_UnlockTexture(pExt_Texture);
    }

    if (true)
    { // Fill the Standard Display with Some Text
        SDL_LockTexture(pStd_Texture, NULL, (void **)&pixels, &pitch); // Lock the texture for write accesspExt_Texture
        // Byte glyph = '@';
        int ch = 0;
        //std::string hello = "Hello, World!";        
        std::string hello = Bus::GetTitle();
        for (auto &g : hello) {
            Byte glyph = (int)g;
            for (int y=0; y<8; y++) {
                for (int h=0; h<8; h++) {
                    int x = h + (ch*8); 
                    int bit = 1 << (7-h);
                    Word a = 4;
                    Word r = 0;
                    Word g = 0;
                    Word b = 0;
                    Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));
                    if (font8x8_system[glyph][y] & bit) {
                        a = 6;
                        r = 15;
                        g = 15;
                        b = 15;
                        *dst = ( (a<<12) | (r<<8) | (g<<4) | (b) );
                    }
                    *dst = ( (a<<12) | (r<<8) | (g<<4) | (b) );
                }  
            }
            ch++; 
        }
        SDL_UnlockTexture(pStd_Texture);
    }

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnUpdate() Exit" << clr::RETURN;
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
bool GPU::OnRender()
{
    //std::cout << clr::indent() << clr::CYAN << "GPU::OnRender() Entry" << clr::RETURN;
    // SDL_FRect r{0.0f, 0.0f, _screen_width, _screen_height};

    SDL_SetRenderVSync(pRenderer, SDL_RENDERER_VSYNC_DISABLED); // VSYNC OFF
    // SDL_SetRenderVSync(pRenderer, SDL_RENDERER_VSYNC_ADAPTIVE); // VSYNC ON

    SDL_SetRenderTarget(pRenderer, pMain_Texture);

    // SDL_SetTextureScaleMode(pExt_Texture, SDL_SCALEMODE_LINEAR);   // blurry
    SDL_SetTextureScaleMode(pExt_Texture, SDL_SCALEMODE_NEAREST);     // clear
    SDL_RenderTexture(pRenderer, pExt_Texture, NULL, NULL);

    // SDL_SetTextureScaleMode(pStd_Texture, SDL_SCALEMODE_LINEAR);   // blurry
    SDL_SetTextureScaleMode(pStd_Texture, SDL_SCALEMODE_NEAREST);     // clear
    SDL_RenderTexture(pRenderer, pStd_Texture, NULL, NULL);

    // Set the render target to the window
    SDL_SetRenderTarget(pRenderer, NULL);

    // SDL_SetTextureScaleMode(pMain_Texture, SDL_SCALEMODE_LINEAR);   // blurry
    SDL_SetTextureScaleMode(pMain_Texture, SDL_SCALEMODE_NEAREST);     // clear
    SDL_RenderTexture(pRenderer, pMain_Texture, NULL, NULL);

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnRender() Exit" << clr::RETURN;
    return true;
}


/**
 * Updates the GPU enable state (GPU_ENABLE) based on the provided data byte.
 * The data byte controls the enabling and disabling of several GPU
 * features, represented by individual bitflags within the byte.
 * 
 * The byte is structured as follows:
 *   - Bits 5-7: Reserved (masked out)
 *   - Bit 3: Standard Mode (0 = disabled, 1 = enabled)
 *   - Bit 2: Extended Mode (0 = disabled, 1 = enabled)
 *   - Bit 1: Sprites (0 = disabled, 1 = enabled)
 *   - Bit 0: Mouse Cursor (0 = disabled, 1 = enabled)
 * 
 * The following BITFLAGS are defined and can be ored together:
 *   - ENABLE_STD: 0b00001000
 *   - ENABLE_EXT: 0b00000100
 *   - ENABLE_SPRITES: 0b00000010
 *   - ENABLE_CURSOR: 0b00000001
 *
 * The function masks out the reserved bits and updates the GPU's
 * enable state based on the remaining bits.
 *
 * @param data The input byte representing the desired enable state.
 * @return The updated enable state.
 */
Byte GPU::_change_gpu_enable(Byte data)
{
    // - bits 5-7 = (reserved)
    data &= 0b00001111;     // Mask out the reserved bits

    // - bits 3   = 0: Disable Standard Mode,
    //              1: Enable Standard Mode
    // ...

    // - bit  2   = 0: Disable Extended Mode,
    //              1: Enable Extended Mode
    // ...

    // - bit  1   = 0: Disable Sprites,
    //              1: Enable Sprites
    // ...

    // - bit  0   = 0: Disable Mouse Cursor,
    //              1: Enable Mouse Cursor
    // ...
    
    // return with the new enable state
    _gpu_enable = data;
   return data;
}


/**
 * Updates the standard graphics mode of the GPU (GPU_STD_MODE).
 * This method takes a single byte as an argument, which is used
 * to set various standard graphics mode options. It validates
 * the new mode against the standard buffer size and reverts to
 * the old mode if the new mode does not fit.
 *
 * The argument byte is interpreted as follows:
 * - Bit 7: Screen mode (0 = text, 1 = bitmap)
 * - Bits 5-6: Bitmap color depth (00 = 2 colors, 01 = 4 colors, 
 *             10 = 16 colors, 11 = 256 colors)
 * - Bits 3-4: Horizontal overscan (00:H/1, 01:H/2, 10:H/3, 11:H/4)
 * - Bits 1-2: Vertical overscan (00:V/1, 01:V/2, 10:V/3, 11:V/4)
 * - Bit 0: Video Timing (0: H:512 x V:320, 1: H:640 x V:400)
 *
 * If the new mode is valid, updates the GPU's internal state
 * and marks the bus as dirty if the mode has changed.
 *
 * @param data The input byte representing the desired standard mode.
 * @return The updated standard mode if valid, otherwise the old mode.
 */

Byte GPU::_change_std_mode(Byte data) 
{ 
    Byte old_data = _gpu_std_mode;

    // save the old values
    bool _bitmap_mode = _is_std_bitmap_mode;
    Byte _color_depth = _std_color_depth;
    bool _hires = _video_hires;
    float _s_width = _std_width;
    float _s_height = _std_height;
    float _oscan_h = _std_overscan_horiz;
    float _oscan_v = _std_overscan_vert;

    // - bit  7   = 0: screen is text, 
    //              1: screen is bitmap
    if (data & 0b10000000) {
        _is_std_bitmap_mode = true;
        _is_std_text_mode = !_is_std_bitmap_mode;
    } else {
        _is_std_bitmap_mode = false;
        _is_std_text_mode = !_is_std_bitmap_mode;
    }

    // - bits 5-6 = bitmap color depth:
    //              00: 2 colors,
    //              01: 4 colors,
    //              10: 16 colors, 
    //              11: 256 colors
    _std_color_depth = (data >> 5) & 0b11;
    float div = 0.5f;
    if (_is_std_bitmap_mode) {
        switch (_std_color_depth) {
            case 0: div = 8.0f; break;
            case 1: div = 4.0f; break;
            case 2: div = 2.0f; break;
            case 3: div = 1.0f; break;
        }
    }

    // - bit  0   = Video Timing:
    //              0: H:512 x V:320
    //              1: H:640 x V:400
    //              (Overrides EXT_MODE)
    _video_hires = (data & 0b00000001);
    if (_video_hires) {
        _screen_width = 640;
        _screen_height = 400;
    } else {
        _screen_width = 512;
        _screen_height = 320;
    }

    // - bits 3-4 = horizontal overscan: 
    //              00:H/1 (512 or 640)
    //              01:H/2 (256 or 320)
    //              10:H/3 (170 or 213)
    //              11:H/4 (128 or 160)
    _std_overscan_horiz = (float)((data >> 3) & 0b11) + 1.0f;
    _std_width = _screen_width / _std_overscan_horiz;

    // - bits 1-2 = vertical overscan: 
    //              00:V/1 (320 or 400)
    //              01:V/2 (160 or 200)
    //              10:V/3 (106 or 133)
    //              11:V/4 (80 or 100)
    _std_overscan_vert = (float)((data >> 1) & 0b11) + 1.0f;
    _std_height = _screen_height / _std_overscan_vert;

    // verify the new mode will fit within the standard buffer
    int buffer_size = (_std_width * _std_height) / div;
    if (_is_std_text_mode) { 
        buffer_size = ( (_std_width/8) * (_std_height/8) );
    }
    if (buffer_size > 8000) {    
        _is_std_bitmap_mode = _bitmap_mode;
        _is_std_text_mode = !_bitmap_mode;
        _std_color_depth = _color_depth;
        _video_hires = _hires;
        _std_width = _s_width;
        _std_height = _s_height;
        _std_overscan_horiz = _oscan_h;
        _std_overscan_vert = _oscan_v;
        // ... and just return the old data
        return data;
    }

    // if the mode has changed, we're dirty!
    if (data != old_data) {
        Bus::IsDirty(true);
    }   

    // return with the new standard mode
    _gpu_std_mode = data;
    return data; 
}



/**
 * Updates the extended graphics mode of the GPU (GPU_EXT_MODE).
 * This method takes a single byte as an argument, which is used
 * to set various extended graphics mode options. It validates
 * the new mode against the extended buffer size and reverts to
 * the old mode if the new mode does not fit.
 *
 * The argument byte is interpreted as follows:
 * - Bit 7: Screen mode (0 = tilemap, 1 = bitmap)
 * - Bits 5-6: Bitmap color depth (00 = 2 colors, 01 = 4 colors, 
 *             10 = 16 colors, 11 = 256 colors)
 * - Bits 3-4: Horizontal overscan (00:H/1, 01:H/2, 10:H/3, 11:H/4)
 * - Bits 1-2: Vertical overscan (00:V/1, 01:V/2, 10:V/3, 11:V/4)
 * - Bit 0: Video Timing (0: H:512 x V:320, 1: H:640 x V:400)
 *
 * If the new mode is valid, updates the GPU's internal state
 * and marks the bus as dirty if the mode has changed.
 *
 * @param data The input byte representing the desired extended mode.
 * @return The updated extended mode if valid, otherwise the old mode.
 */
Byte GPU::_change_ext_mode(Byte data) 
{ 
    // save the old settings
    Byte old_data = _gpu_ext_mode;
    bool _bitmap_mode = _is_ext_bitmap_mode;
    Byte _color_depth = _ext_color_depth;
    bool _hires = _video_hires;
    float _s_width = _std_width;
    float _s_height = _std_height;
    float _oscan_h = _std_overscan_horiz;
    float _oscan_v = _std_overscan_vert;

    // GPU_EXT_MODE     ; (Byte) Extended Graphics Mode
    // - bit  7   = 0: screen is tiled,
    //              1: screen is bitmap
    if (data & 0b10000000) {
        _is_ext_tile_mode = true;
        _is_ext_bitmap_mode = !_is_ext_tile_mode;
    } else {
        _is_ext_tile_mode = false;
        _is_ext_bitmap_mode = !_is_ext_tile_mode; 
    }

    // - bits 5-6 = bitmap color depth:
    //              00: 2 colors,
    //              01: 4 colors,
    //              10: 16 colors, 
    //              11: 256 colors
    _ext_color_depth = (data >> 5) & 0b11;
    float div = 1.0f;
    switch (_ext_color_depth) {
        case 0: div = 8.0f; break;
        case 1: div = 4.0f; break;
        case 2: div = 2.0f; break;
        case 3: div = 1.0f; break;
    }
    // - bit  0   = Video Timing:
    //              0: H:512 x V:320
    //              1: H:640 x V:400
    //              (Overrides EXT_MODE)
    _video_hires = (data & 0b00000001);
    if (_video_hires) {
        _screen_width = 640;
        _screen_height = 400;
    } else {
        _screen_width = 512;
        _screen_height = 320;
    }

    // - bits 3-4 = horizontal overscan: 
    //              00:H/1 (512 or 640)
    //              01:H/2 (256 or 320)
    //              10:H/3 (170 or 213)
    //              11:H/4 (128 or 160)
    _ext_overscan_horiz = (float)((data >> 3) & 0b11) + 1.0f;
    _ext_width = _screen_width / _ext_overscan_horiz;

    // - bits 1-2 = vertical overscan: 
    //              00:V/1 (320 or 400)
    //              01:V/2 (160 or 200)
    //              10:V/3 (106 or 133)
    //              11:V/4 (80 or 100)
    _ext_overscan_vert = (float)((data >> 1) & 0b11) + 1.0f;
    _ext_height = _screen_height / _ext_overscan_vert;

    // verify the new mode will fit within the standard buffer
    int buffer_size = (_std_width * _std_height) / div;
    if (buffer_size > 64000) { 
        _is_ext_bitmap_mode = _bitmap_mode;
        _is_ext_tile_mode = !_bitmap_mode;
        _ext_color_depth = _color_depth;
        _video_hires = _hires;
        _ext_width = _s_width;
        _ext_height = _s_height;
        _ext_overscan_horiz = _oscan_h;
        _ext_overscan_vert = _oscan_v;
        // ... and just return the old data
        return old_data;
    }

    // return with the new standard mode
    _gpu_ext_mode = data;
    return data; 
}


/**
 * Updates the emulation mode of the GPU based on GPU_EMULATION.
 * The mode byte is structured as follows:
 *   - Bit 7: VSync (0 = off, 1 = on)
 *   - Bit 6: Main display (0 = windowed, 1 = fullscreen)
 *   - Bit 5: Debug display (0 = off, 1 = on)
 *   - Bit 4: Debug display (0 = windowed, 1 = fullscreen)
 *   - Bits 2-3: Active monitor (0-3)
 *   - Bits 0-1: Debug monitor (0-3)
 *
 * @param emu The mode byte representing the desired emulation mode.
 * @return The updated mode configuration.
 */
Byte GPU::_change_emu_mode(Byte data) 
{ 
    // Byte old_data = _gpu_emu_mode;

    // bit 7: vsync: 0=off, 1=on
    // ...

    // bit 6: main: 0=windowed, 1=fullscreen
    // ...

    // bit 5: debug: 0=off, 1=on
    // ...

    // bit 4: debug: 0=windowed, 1=fullscreen
    // ...   

    // bits 2-3: Active Monitor 0-3 
    // ...   

    // bits 0-1: Debug Monitor 0-3
    // ...  

    // return with the new emulation mode
    _gpu_emu_mode = data;
    return data; 
}    




// END: Gfx.cpp
