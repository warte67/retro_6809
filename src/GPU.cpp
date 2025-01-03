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
} // END: GPU::OnAttach()


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
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
        {
            std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onInit() Error" << clr::RETURN;
            Bus::Error(SDL_GetError(), __FILE__, __LINE__);
            return false;
        }
        // create the main window
        pWindow = SDL_CreateWindow("SDL3 Retro_6809", initial_width, initial_height, window_flags); 
        SDL_ShowWindow(pWindow);

        
    } // END OF SDL3 Initialization

    // Build The Color Palette
    _build_palette();

    // initialize the default values
    _change_gpu_enable(_gpu_enable);
    _change_std_mode(_gpu_std_mode);
    _change_ext_mode(_gpu_ext_mode);    
    _change_emu_mode(_gpu_emu_mode);

    // Reserve 64k for the extended video buffer
    int bfr_size = 64*1024;
    _ext_video_buffer.reserve(bfr_size);
    
    // for (int i=0; i<bfr_size; i++) _ext_video_buffer[i] = rand() % 256; // fill with random data

    // initialize the font glyph buffer
    for (int i=0; i<256; i++)
        for (int r=0; r<8; r++)
            _gfx_glyph_data[i][r] = font8x8_system[i][r];    

    std::cout << clr::indent() << clr::CYAN << "GPU::OnInit() Exit" << clr::RETURN;
    return true;
} // END: GPU::OnInit()


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
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    } // END OF SDL3 Shutdown

    std::cout << clr::indent() << clr::CYAN << "GPU::OnQuit() Exit" << clr::RETURN;
    return true;
} // END: GPU::OnQuit()


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

    std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Exit" << clr::RETURN;
    return true;
} // END: GPU::OnActivate()


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
} // END: GPU::OnDeactivate()


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
            if (evnt->key.key == SDLK_V)
            {   // [V] Toggle VSYNC
                Byte data = Memory::Read( MAP(GPU_EMULATION) );
                if (data &  0b10000000) {
                    data &= 0b01111111;
                } else {
                    data |= 0b10000000;
                }
                Memory::Write(MAP(GPU_EMULATION), data);
            }
            
            if (evnt->key.key == SDLK_F)
            {   // [F] Toggle Fullscreen
                Byte data = Memory::Read( MAP(GPU_EMULATION) );
                if (data &  0b0100'0000) {
                    data &= 0b1011'1111;
                } else {
                    data |= 0b0100'0000;
                }
                Memory::Write(MAP(GPU_EMULATION), data);
            } // END: if (evnt->key.key == SDLK_F)

            // [E] Extended Display Enable Toggle
            if (evnt->key.key == SDLK_E)
            {
                SDL_Keymod mod = SDL_GetModState();
                if (mod & SDL_KMOD_SHIFT)
                { // [SHIFT] + [E]  // toggle between tilemap and graphics mode
                    Byte data = Memory::Read( MAP(GPU_EXT_MODE) );
                    if (data &  0b1000'0000) {
                        data &= 0b0111'1111;
                    } else {
                        data |= 0b1000'0000;
                    }
                    Memory::Write(MAP(GPU_EXT_MODE), data);
                } else { // [E]
                    Byte data = Memory::Read( MAP(GPU_ENABLE) );
                    if (data &  0b0000'0100) {
                        data &= 0b1111'1011;
                    } else {
                        data |= 0b0000'0100;
                    }
                    Memory::Write(MAP(GPU_ENABLE), data);
                }
            }
            // [S] Standard Display Enable Toggle
            if (evnt->key.key == SDLK_S)
            {
                SDL_Keymod mod = SDL_GetModState();
                if (mod & SDL_KMOD_SHIFT)
                { // [SHIFT] + [S]  // toggle between text and graphics mode
                    Byte data = Memory::Read( MAP(GPU_STD_MODE) );
                    if (data &  0b1000'0000) {
                        data &= 0b0111'1111;
                    } else {
                        data |= 0b1000'0000;
                    }
                    Memory::Write(MAP(GPU_STD_MODE), data);
                } else { // [S]
                    Byte data = Memory::Read( MAP(GPU_ENABLE) );
                    if (data &  0b0000'1000) {
                        data &= 0b1111'0111;
                    } else {
                        data |= 0b0000'1000;
                    }
                    Memory::Write(MAP(GPU_ENABLE), data);
                }
            }

            // active monitor [1-4]
            if (evnt->key.key == SDLK_1)
            {
                Byte data = Memory::Read( MAP(GPU_EMULATION) );
                data &= 0b1111'0011;
                data |= 0b0000'0000;
                Memory::Write(MAP(GPU_EMULATION), data);
            }
            if (evnt->key.key == SDLK_2)
            {
                Byte data = Memory::Read( MAP(GPU_EMULATION) );
                data &= 0b1111'0011;
                data |= 0b0000'0100;
                Memory::Write(MAP(GPU_EMULATION), data);
            }
            if (evnt->key.key == SDLK_3)
            {
                Byte data = Memory::Read( MAP(GPU_EMULATION) );
                data &= 0b1111'0011;
                data |= 0b0000'1000;
                Memory::Write(MAP(GPU_EMULATION), data);
            }
            if (evnt->key.key == SDLK_4)
            {
                Byte data = Memory::Read( MAP(GPU_EMULATION) );
                data &= 0b1111'0011;
                data |= 0b0000'1100;
                Memory::Write(MAP(GPU_EMULATION), data);
            }

            break;       
        } // END: case SDL_EVENT_KEY_DOWN

        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        {            
            _gpu_emu_mode |= 0b01000000; // Set bit 6 = ENTER FULLSCREEN
            // std::cout << "SDL_EVENT_WINDOW_ENTER_FULLSCREEN (0x" << clr::hex(_gpu_emu_mode,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        {
            _gpu_emu_mode &= 0b10111111; // Clear bit 6 = LEAVE FULLSCREEN
            // std::cout << "SDL_EVENT_WINDOW_LEAVE_FULLSCREEN (0x" << clr::hex(_gpu_emu_mode,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

    } // END: switch  (evnt->type) 

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnEvent() Exit" << clr::RETURN;
    return true;
} // END: GPU::OnEvent()


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
    if (runningTime > deltaTime + 0.01f) {
        deltaTime = fElapsedTime;
        runningTime = fElapsedTime;
        SDL_SetWindowTitle(pWindow, Bus::GetTitle().c_str());
        // std::cout << "FPS: " << Bus::FPS() << std::endl;

        // animate some background pixels
        int bfr_size = 64*1024;
        static Byte c = 0;
        for (int i=0; i<bfr_size; i++) { _ext_video_buffer[i] = c++; } 
        c++;

        // animate some foreground data
        static Byte data = 0;
        static Byte color = 0x00;
        // for (int i=MAP(VIDEO_START); i<MAP(VIDEO_TOP); i++) { Memory::Write(i, data++);  }
        for (int i=MAP(VIDEO_START); i<MAP(VIDEO_TOP); i+=2) { 
            // if (data ==0)  color++; 
            Memory::Write(i+0, color++);  
            Memory::Write(i+1, data++);  
        }
    }
    runningTime += fElapsedTime;
    // std::cout << fElapsedTime << std::endl;
    
    // is extended graphics enabled?
    if (Memory::Read(GPU_ENABLE) & 0b00000100)
    {
        _render_extended_graphics();       
    }

    // is standard graphics enabled?
    if (Memory::Read(GPU_ENABLE) & 0b00001000)
    {
        _render_standard_graphics();
    }

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnUpdate() Exit" << clr::RETURN;
    return true;
} // END: GPU::OnUpdate()


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

    // clear the primary texture
    SDL_SetRenderTarget(pRenderer, pMain_Texture);
    SDL_SetRenderDrawColor(pRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(pRenderer);

    // render Extended Graphics
    if (Memory::Read(GPU_ENABLE) & 0b00000100)
    {
        // SDL_SetTextureScaleMode(pExt_Texture, SDL_SCALEMODE_LINEAR);   // blurry
        SDL_SetTextureScaleMode(pExt_Texture, SDL_SCALEMODE_NEAREST);     // clear
        SDL_RenderTexture(pRenderer, pExt_Texture, NULL, NULL);
    }

    // render Standard Graphics
    if (Memory::Read(GPU_ENABLE) & 0b00001000)
    {
        // SDL_SetTextureScaleMode(pStd_Texture, SDL_SCALEMODE_LINEAR);   // blurry
        SDL_SetTextureScaleMode(pStd_Texture, SDL_SCALEMODE_NEAREST);     // clear
        SDL_RenderTexture(pRenderer, pStd_Texture, NULL, NULL);
    }

    // Set the render target to the window
    SDL_SetRenderTarget(pRenderer, NULL);

    // SDL_SetTextureScaleMode(pMain_Texture, SDL_SCALEMODE_LINEAR);   // blurry
    SDL_SetTextureScaleMode(pMain_Texture, SDL_SCALEMODE_NEAREST);     // clear
    SDL_RenderTexture(pRenderer, pMain_Texture, NULL, NULL);

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnRender() Exit" << clr::RETURN;
    return true;
} // END: GPU::OnRender()


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
} // END: GPU::_change_gpu_enable


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
} // END: GPU::_change_std_mode()



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

    // return with the new extended mode
    _gpu_ext_mode = data;
    return data; 
} // END: GPU::_change_ext_mode()


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
    if (data & 0b10000000) {
        SDL_SetRenderVSync(pRenderer, 1); // VSYNC ON
    } else {
        SDL_SetRenderVSync(pRenderer, 0); // VSYNC OFF
    }   

    // bit 6: main: 0=windowed, 1=fullscreen
    if (data & 0b01000000) {
        SDL_SetWindowFullscreen(pWindow, true);
    } else {
        SDL_SetWindowFullscreen(pWindow, false);
    }   

    // bit 5: debug: 0=off, 1=on
    // ...

    // bit 4: debug: 0=windowed, 1=fullscreen
    // ...   

    // bits 2-3: Active Monitor 0-3 
    if (false) 
    { // Wayland cannot position non-popup windows
        // Fetch Active Displays
        _displays = SDL_GetDisplays(&_num_displays);
        SDL_Rect rect;
        std::cout << "Active Displays: " << _num_displays << std::endl;
        for (int i = 0; i < _num_displays; i++) {
            std::cout << "Display " << i << ": " << _displays[i] << std::endl;
            SDL_GetDisplayBounds(_displays[i], &rect);
            std::cout << "Display " << i << " bounds: " << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << std::endl;
        }
        int i = (data & 0b0000'1100) >> 2;
        if (i>=_num_displays)   i=_num_displays-1;
        SDL_GetDisplayBounds(_displays[i], &rect);
        if (!SDL_SetWindowPosition(pWindow, SDL_WINDOWPOS_CENTERED_DISPLAY(i), SDL_WINDOWPOS_CENTERED_DISPLAY(i))) {
            Bus::Error(SDL_GetError(), __FILE__, __LINE__);
        }
        std::cout << "Active Display: " << i << "  X:" << rect.x << "  Y:" << rect.y << std::endl;
        // clear active displays
        SDL_free(_displays);
    }

    // bits 0-1: Debug Monitor 0-3
    if (false)
    { // Wayland cannot position non-popup windows 
        // ...
    }

    // return with the new emulation mode
    _gpu_emu_mode = data;
    return data; 
} // END: GPU::_change_emu_mode()

void GPU::_render_extended_graphics()
{    
    //    GPU_EXT_MODE          = 0xFE02, // (Byte) Extended Graphics Mode
    int _width = _ext_width;
    int _height = _ext_height;

    //    - bit  7   = 0: screen is tiled,
    //                 1: screen is bitmap
    bool bIsTiled = !(Memory::Read(GPU_EXT_MODE) & 0b1000'0000) ? true : false;

    //    - bits 5-6 = bitmap color depth:
    //                 00: 2 colors,
    //                 01: 4 colors,
    //                 10: 16 colors, 
    //                 11: 256 colors
    int bpp = ((Memory::Read(GPU_EXT_MODE) & 0b0110'0000) >> 5) & 0b000'0011;
    bpp = 1<<bpp; // 1, 2, 4, or 8

    //    - bits 3-4 = horizontal overscan: 
    //                 00:H/1 (512 or 640)
    //                 01:H/2 (256 or 320)
    //                 10:H/3 (170 or 213)
    //                 11:H/4 (128 or 160)
    int dh = ((Memory::Read(GPU_EXT_MODE) & 0b0001'1000) >> 3) & 0b000'0011;
    switch(dh) {
        case 0: dh = 1; break;
        case 1: dh = 2; break;
        case 2: dh = 3; break;
        case 3: dh = 4; break;
    }

    //    - bits 1-2 = vertical overscan: 
    //                 00:V/1 (320 or 400)
    //                 01:V/2 (160 or 200)
    //                 10:V/3 (106 or 133)
    //                 11:V/4 (80 or 100)
    int dv = ((Memory::Read(GPU_EXT_MODE) & 0b0000'0110) >> 1) & 0b000'0011;
    switch(dv) {
        case 0: dv = 1; break;
        case 1: dv = 2; break;
        case 2: dv = 3; break;
        case 3: dv = 4; break;
    }

    //    - bit  0   = Video Timing:
    //                 0: H:512 x V:320
    //                 1: H:640 x V:400
    //                 (Overrides STD_MODE)
    int timing = (Memory::Read(GPU_EXT_MODE) & 0b0000'0001) & 0b000'0001;
    switch (timing) {
        case 0: _width = 512/dh; _height = 320/dv; break;
        case 1: _width = 640/dh; _height = 400/dv; break;
    }


    // clear the extended texture
    _clear_texture(pExt_Texture, 0, 0, 0, 0);

    if (bIsTiled) {
        // TODO: render tiled graphics
        // std::cout << "Displaying extended tilemap buffer" << std::endl;

        // _update_tile_buffer();
    } else {
        // display the extended bitmap buffer
        Word pixel_index = 0x0000;
        void *pixels;
        int pitch;
        if (!SDL_LockTexture(pExt_Texture, NULL, (void **)&pixels, &pitch))
            Bus::Error(SDL_GetError());	
        else
        {
            for (int y = 0; y < _height; y++)
            {
                for (int x = 0; x < _width; )
                {
                    // 256 color mode
                    if (bpp == 8)
                    {
                        Byte index = _ext_video_buffer[pixel_index++];
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 16 color mode
                    else if (bpp == 4)
                    {
                        Byte data = _ext_video_buffer[pixel_index++];
                        Byte index = (data >> 4);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data & 0x0f);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 4 color mode
                    else if (bpp == 2)
                    {
                        Byte data = _ext_video_buffer[pixel_index++];
                        Byte index = (data >> 6) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 4) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 2) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 0) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 2 color mode
                    else if (bpp == 1)
                    {
                        Byte data = _ext_video_buffer[pixel_index++];
                        Byte index = (data >> 7) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true); 
                        index = (data >> 6) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 5) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 4) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 3) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 2) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 1) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 0) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    if (pixel_index >  _ext_video_buffer.capacity()) {
                        std::string err = "Pixel index out of range: " + std::to_string(pixel_index);
                        Bus::Error(err.c_str());
                        return;
                    }
                }
            }
            SDL_UnlockTexture(pExt_Texture); 
        }        
    }
} // END: GPU::_render_extended_graphics()

void GPU::_render_standard_graphics()
{
    //    GPU_STD_MODE          = 0xFE01, // (Byte) Standard Graphics Mode
    int _width = _std_width;
    int _height = _std_height;

    //    - bit  7   = 0: screen is text, 
    //                 1: screen is bitmap
    bool bIsText = (Memory::Read(GPU_STD_MODE) & 0x80) == 0x80;

    //    - bits 5-6 = bitmap color depth:
    //                 00: 2 colors,
    //                 01: 4 colors,
    //                 10: 16 colors, 
    //                 11: 256 colors
    int bpp = ((Memory::Read(GPU_STD_MODE) & 0b0110'0000) >> 5) & 0b000'0011;
    bpp = 1<<bpp; // 1, 2, 4, or 8

    //    - bits 3-4 = horizontal overscan: 
    //                 00:H/1 (512 or 640)
    //                 01:H/2 (256 or 320)
    //                 10:H/3 (170 or 213)
    //                 11:H/4 (128 or 160)
    int dh = ((Memory::Read(GPU_STD_MODE) & 0b0001'1000) >> 3) & 0b000'0011;
    switch(dh) {
        case 0: dh = 1; break;
        case 1: dh = 2; break;
        case 2: dh = 3; break;
        case 3: dh = 4; break;
    }

    //    - bits 1-2 = vertical overscan: 
    //                 00:V/1 (320 or 400)
    //                 01:V/2 (160 or 200)
    //                 10:V/3 (106 or 133)
    //                 11:V/4 (80 or 100)
    int dv = ((Memory::Read(GPU_STD_MODE) & 0b0000'0110) >> 1) & 0b000'0011;
    switch(dv) {
        case 0: dv = 1; break;
        case 1: dv = 2; break;
        case 2: dv = 3; break;
        case 3: dv = 4; break;
    }

    //    - bit  0   = Video Timing:
    //                 0: H:512 x V:320
    //                 1: H:640 x V:400
    //                 (Overrides EXT_MODE)
    int timing = (Memory::Read(GPU_STD_MODE) & 0b0000'0001) & 0b000'0001;
    switch (timing) {
        case 0: _width = 512/dh; _height = 320/dv; break;
        case 1: _width = 640/dh; _height = 400/dv; break;
    }

    // clear the standard texture
    _clear_texture(pStd_Texture, 0, 0, 0, 0);

    // Render the Standard Display Buffer
    if (bIsText) {
        // display the standard text buffer
        _update_text_buffer();        
    } else {
        // display the standard bitmap buffer
        Word pixel_index = MAP(VIDEO_START);
        void *pixels;
        int pitch;
        if (!SDL_LockTexture(pStd_Texture, NULL, (void **)&pixels, &pitch))
            Bus::Error(SDL_GetError());	
        else
        {
            for (int y = 0; y < _height; y++)
            {
                for (int x = 0; x < _width; )
                {
                    // 256 color mode
                    if (bpp == 8)
                    {
                        // Byte index = _ext_video_buffer[pixel_index++];
                        Byte index = Memory::Read(pixel_index++);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 16 color mode
                    else if (bpp == 4)
                    {
                        //Byte data = _ext_video_buffer[pixel_index++];
                        Byte data = Memory::Read(pixel_index++);
                        Byte index = (data >> 4);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data & 0x0f);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 4 color mode
                    else if (bpp == 2)
                    {
                        //Byte data = _ext_video_buffer[pixel_index++];
                        Byte data = Memory::Read(pixel_index++);
                        Byte index = (data >> 6) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 4) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 2) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 0) & 0x03;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 2 color mode
                    else if (bpp == 1)
                    {
                        //Byte data = _ext_video_buffer[pixel_index++];
                        Byte data = Memory::Read(pixel_index++);
                        Byte index = (data >> 7) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true); 
                        index = (data >> 6) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 5) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 4) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 3) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 2) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 1) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data >> 0) & 1;
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    if (pixel_index > MAP(VIDEO_END)) {
                        std::string err = "Pixel index out of range: $" + clr::hex(pixel_index, 4);
                        Bus::Error(err.c_str());
                        return;
                    }
                }
            }
            SDL_UnlockTexture(pStd_Texture); 
        }        
    }
} // END: GPU::_render_standard_graphics()

void GPU::_update_text_buffer() {

    bool ignore_alpha = false;
    void *pixels;
    int pitch;

    // lock the standard texture
    if (!SDL_LockTexture(pStd_Texture, NULL, &pixels, &pitch)) {
        Bus::Error(SDL_GetError());
    }
    else
    { // Render the text
        Byte col = _std_width / 8;
        Byte row = _std_height / 8;
        Word end = ((col*row)*2) + VIDEO_START;
        // Word end  = VIDEO_START+128;
		Word addr = VIDEO_START;
		for (; addr < end; addr += 2)
		{
			Byte at = Memory::Read(addr+0, true);
			Byte ch = Memory::Read(addr+1, true);
			Byte bg = at >> 4;
			Byte fg = at & 0x0f;
			Word index = addr - VIDEO_START;
			Byte width = _std_width / 8;
			int x = ((index / 2) % width) * 8;
			int y = ((index / 2) / width) * 8;
			for (int v = 0; v < 8; v++)
			{
				for (int h = 0; h < 8; h++)
				{
					int color = bg;
                    Byte gd = GetGlyphData(ch, v);
					if (gd & (1 << (7 - h)))
						color = fg;
					_setPixel_unlocked(pixels, pitch, x + h, y + v, color, ignore_alpha);
				}
			}
		}
        SDL_UnlockTexture(pStd_Texture); 
    }
} 

/**
 * Sets a pixel in the given surface without locking the surface.
 *
 * This function combines the pixel color with the color index, using the alpha channel
 * of the color index as the alpha value for the combination. If bIgnoreAlpha is true,
 * the alpha value is ignored and the pixel color is set to the color index if the color
 * index is not transparent.
 *
 * @param pixels The surface to draw on.
 * @param pitch The pitch of the surface.
 * @param x The x coordinate of the pixel.
 * @param y The y coordinate of the pixel.
 * @param color_index The color index to use.
 * @param bIgnoreAlpha If true, the alpha value is ignored.
 */
void GPU::_setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte color_index, bool bIgnoreAlpha)
{
    Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));		// because data size is two bytes 

    bool cls = false;
    if (color_index ==0 && bIgnoreAlpha)
        cls = true;

    if (!bIgnoreAlpha)
    {       
        // int ret = ((p1 * (256-a))) + (p2 * (a+1)) >> 8;
        Uint16 pixel = *dst;	// 0xARGB
		Byte r1 = (pixel & 0x0f00) >> 8;
		Byte g1 = (pixel & 0x00f0) >> 4;
		Byte b1 = (pixel & 0x000f) >> 0;
		//
        Byte a2 = alf(color_index);
        Byte r2 = red(color_index);
        Byte g2 = grn(color_index);
        Byte b2 = blu(color_index);
        // if (bIgnoreAlpha)
        //     a2 = 15;
		//
        Byte r = (((r1 * (16-a2))) + (r2 * (a2+1))) >> 4;
        Byte g = (((g1 * (16-a2))) + (g2 * (a2+1))) >> 4;
        Byte b = (((b1 * (16-a2))) + (b2 * (a2+1))) >> 4;

        if (alf(color_index) != 0) // || bIgnoreAlpha)
        {
            *dst = (
                0xF000 | 
                (r<<8) | 
                (g<<4) | 
                (b)
            );          
            // *dst = (
            //     (a2<<12) |       // utilize the alpha channel
            //     (r<<8) | 
            //     (g<<4) | 
            //     (b)
            // );          
		}	
    }
    else
    {        
        if (cls)
        {
            *dst = 0x0000;
        }
        else
        {
            *dst = 
            (
                0xF000 |
                (red(color_index)<<8) |
                (grn(color_index)<<4) |
                 blu(color_index)
            );    
        }
    }
} // END: GPU::_setPixel_unlocked()

void GPU::_clear_texture(SDL_Texture* texture, Byte r, Byte g, Byte b, Byte a)
{
    void *pixels;
    int pitch;
    if (!SDL_LockTexture(texture, NULL, &pixels, &pitch)) {
        Bus::Error(SDL_GetError());
    } else {
        for (int y = 0; y < _std_height; y++)
        {
            for (int x = 0; x < _std_width; x++)
            {
                Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));
                *dst = (
                    (a<<12) | 
                    (r<<8)  | 
                    (g<<4)  | 
                    (b<<0) );          

            }
        }
        SDL_UnlockTexture(texture); 
    }    
}

/**
 * Initializes the palette data by filling it with the following:
 *   - CUSTOM DEFAULT COLORS (0-15)
 *   - BGR 6x6x6 CUBE COLORS (16-232)
 *   - GRAYSCALE RAMP (233-255)
 *   - 100% TRANSPARENT WHITE (256)
 *
 * The palette is only initialized once, and subsequent calls to this
 * method do not modify the palette.
 */
void GPU::_build_palette()
{
    // INITIALIZE PALETTE DATA
    if (_palette.size() == 0)
    {
        // BASIC COLORS (0-15) CUSTOM DEFAULT COLORS
        std::vector<PALETTE> ref = {    
			{ 0xF000 },		// 0: black
			{ 0xF555 },		// 1: dk gray
			{ 0xF007 },		// 2: dk blue
			{ 0xF600 },		// 3: dk red
			{ 0x0140 },		// 4: dk green
			{ 0xF840 },		// 5: brown
			{ 0xF406 },		// 6: purple          
			{ 0xF046 },		// 7: deep sea           	
			{ 0xF888 },		// 8: lt gray
			{ 0xF22F },		// 9: blue
			{ 0xFd00 },		// A: red
			{ 0xF4F6 },		// B: lt green
			{ 0xFED0 },		// C: yellow
			{ 0xF85b },		// D: Lt Purple
			{ 0xF59f },		// E: lt sky
			{ 0xFFFF },		// F: white

        // // BASIC COLORS (0-15) STANDARD CGA COLORS
        // std::vector<PALETTE> ref = {    
		// 	{ 0x0000 },	// 0: DK_BLACK
		// 	{ 0xF005 },	// 1: DK_BLUE
		// 	{ 0xF050 },	// 2: DK_GREEN
		// 	{ 0xF055 },	// 3: DK_CYAN
		// 	{ 0xF500 },	// 4: DK_RED
		// 	{ 0xF505 },	// 5: DK_MAGENTA
		// 	{ 0xF631 },	// 6: BROWN
		// 	{ 0xFAAA },	// 7: GRAY
		// 	{ 0xF555 },	// 8: DK_GRAY
		// 	{ 0xF00F },	// 9: BLUE
		// 	{ 0xF0F0 },	// a: GREEN
		// 	{ 0xF0FF },	// b: CYAN
		// 	{ 0xFF00 },	// c: RED
		// 	{ 0xFA0A },	// d: MAGENTA
		// 	{ 0xFFF0 },	// e: YELLOW
		// 	{ 0xFFFF },	// f: WHITE
        };
        for (auto &p : ref)
        {
			_palette.push_back(p);     
            // printf("%3d:$%04X\n", (int)_palette.size(), p.color);
        }
        // BGR 6x6x6 CUBE COLORS (16-232)          
        Byte b=0, g=0, r=0;
        for (float fb=0.0f; fb<=256.0f; fb+=(256.0f / 5.0f))
        {
            b = fb;
            if (fb == 256.0f) b=255;
            for (float fg=0.0f; fg<=256.0f; fg+=(256.0f / 5.0f))
            {
                g = fg;
                if (fg == 256.0f) g=255;
                for (float fr=0.0f; fr<=256.0f; fr+=(256.0f / 5.0f))
                {
                    r = fr;
                    if (fr == 256.0f) r=255;
                    PALETTE ent = { 0x0000 };
                    ent.a = 255>>4;
                    ent.b = b>>4;
                    ent.g = g>>4;
                    ent.r = r>>4;
                    _palette.push_back(ent);
                    // printf("%3d:$%04X\n", (int)_palette.size(), ent.color);
                }
            }
        }
        // GRAYSCALE RAMP (233-255) 
        for (float f=0.0f; f<=256.0f; f+=(256.0f / 24.0f))
        {
            if (f != 0.0f)
            {
                PALETTE ent = { 0x0000 };
                ent.a = 255>>4;
                ent.b = (int)f>>4;
                ent.g = (int)f>>4;
                ent.r = (int)f>>4;
                _palette.push_back(ent);
            }
        }  
        // Add one 100% transparent white entry to the end              
        _palette.push_back({0x0FFF});   // (255 = 100% transparent)
    }
} // END::_buildPalette();

// END: Gfx.cpp
