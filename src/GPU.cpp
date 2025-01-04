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

    if (offset == MAP(GPU_OPTIONS))     { data = _gpu_options;  }
    else if (offset == MAP(GPU_MODE))   { data = _gpu_mode;     }

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

    if (offset == MAP(GPU_OPTIONS))         
    { 
        data = _verify_gpu_mode_change(data, offset);   
        // _verify_gpu_mode_change(_gpu_mode, MAP(GPU_MODE));
    }
    else if (offset == MAP(GPU_MODE))   
    { 
        data = _verify_gpu_mode_change(data, offset);  
        // _verify_gpu_mode_change(_gpu_options, MAP(GPU_OPTIONS));
    }

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
    
     new_node = { "GPU_OPTIONS", nextAddr,  {   "(Byte) Bitflag Enables",
                                "   - bit 7    = Extended Bitmap:",
                                "                 0: Tilemap Display",
                                "                 1: Bitmap Display",
                                "   - bits 5-6 = Extended Color Mode:",
                                "                 00: 2-Colors",
                                "                 01: 4-Colors",
                                "                 10: 16-Colors",
                                "                 11: 256-Colors",
                                "   - bits 4   = Extended Display Enable",
                                "                0: Disabled",
                                "                1: Enabled",
                                "   - bits 3   = Application Screen Mode",
                                "                0: Windowed",
                                "                1: Fullscreen",
                                "   - bits 2   = Debug Enable",
                                "                0: Disabled",
                                "                1: Enabled",
                                "   - bits 1   = Sprite Enable",
                                "                0: Disabled",
                                "                1: Enabled",
                                "   - bit  0   = Standard Display Enable",
                                "                0: Disabled",
                                "                1: Enabled",
                                "" } }; nextAddr+=1;
    mapped_register.push_back(new_node);    

     new_node = { "GPU_MODE", nextAddr,  {   "(Byte) Bitflag Enables",
                                "   - bit 7    = Standard Bitmap:",
                                "                 0: Text Display",
                                "                 1: Bitmap Display",
                                "   - bits 5-6 = Standard Color Mode:",
                                "                 00: 2-Colors",
                                "                 01: 4-Colors",
                                "                 10: 16-Colors",
                                "                 11: 256-Colors",
                                "   - bits 0-4 = Display Mode (0-31)",
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

        // create the renderer
        pRenderer = SDL_CreateRenderer(pWindow, NULL);

        // the extended video mode should determine the overall renderer logical presentation area
        SDL_SetRenderLogicalPresentation(pRenderer, (int)_screen_width, (int)_screen_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);


        // build the textures
        pExt_Texture = SDL_CreateTexture(pRenderer, 
                SDL_PIXELFORMAT_ARGB4444, 
                SDL_TEXTUREACCESS_STREAMING, 
                (int)_screen_width/2, (int)_screen_height/2);

        pStd_Texture = SDL_CreateTexture(pRenderer, 
                SDL_PIXELFORMAT_ARGB4444, 
                SDL_TEXTUREACCESS_STREAMING, 
                (int)_screen_width/2, (int)_screen_height/2);

        pMain_Texture = SDL_CreateTexture(pRenderer, 
                SDL_PIXELFORMAT_ARGB4444, 
                SDL_TEXTUREACCESS_TARGET, 
                (int)_screen_width, (int)_screen_height);
        
    } // END OF SDL3 Initialization

    // Build The Color Palette
    _build_palette();

    // Reserve 64k for the extended video buffer
    int bfr_size = 64*1024;
    _ext_video_buffer.reserve(bfr_size);
    
    // for (int i=0; i<bfr_size; i++) _ext_video_buffer[i] = rand() % 256; // fill with random data

    // initialize the font glyph buffer
    for (int i=0; i<256; i++)
        for (int r=0; r<8; r++)
            _gfx_glyph_data[i][r] = font8x8_system[i][r];    

    // // initialize the initial default display mode   
    Memory::Write(MAP(GPU_OPTIONS), _gpu_options);
    Memory::Write(MAP(GPU_MODE), _gpu_mode);
    // _gpu_options = _verify_gpu_mode_change(_gpu_options, MAP(GPU_OPTIONS));
    // _gpu_mode = _verify_gpu_mode_change(_gpu_mode, MAP(GPU_MODE));

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
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Entry" << clr::RETURN;
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Exit" << clr::RETURN;
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
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnDeactivate() Entry" << clr::RETURN;       
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnDeactivate() Exit" << clr::RETURN;
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
            // if (evnt->key.key == SDLK_V)
            // {   // [V] Toggle VSYNC
            //     Byte data = Memory::Read( MAP(GPU_EMULATION) );
            //     if (data &  0b10000000) {
            //         data &= 0b01111111;
            //     } else {
            //         data |= 0b10000000;
            //     }
            //     Memory::Write(MAP(GPU_EMULATION), data);
            // }
            
            if (evnt->key.key == SDLK_F)
            {   // [F] Toggle Fullscreen
                Byte data = Memory::Read( MAP(GPU_OPTIONS) );
                if (data &  0b0000'1000) {
                    data &= 0b1111'0111;
                } else {
                    data |= 0b0000'1000;
                }
                Memory::Write(MAP(GPU_OPTIONS), data);
            } // END: if (evnt->key.key == SDLK_F)

            if (evnt->key.key == SDLK_RIGHT)
            {
                Byte data = Memory::Read( MAP(GPU_MODE) );
                data++;
                Memory::Write(MAP(GPU_MODE), data);
            }
            if (evnt->key.key == SDLK_LEFT)
            {
                Byte data = Memory::Read( MAP(GPU_MODE) );
                data--;
                Memory::Write(MAP(GPU_MODE), data);
            }

            // [E] Extended Display Enable Toggle
            if (evnt->key.key == SDLK_E)
            {
                SDL_Keymod mod = SDL_GetModState();
                if (mod & SDL_KMOD_SHIFT)
                { // [SHIFT] + [E]  // toggle between extended tilemap and bitmap mode
                    Byte data = Memory::Read( MAP(GPU_OPTIONS) );
                    if (data &  0b1000'0000) {
                        data &= 0b0111'1111;
                    } else {
                        data |= 0b1000'0000;
                    }
                    Memory::Write(MAP(GPU_OPTIONS), data);
                } else { // [E] Extended Display Enable
                    Byte data = Memory::Read( MAP(GPU_OPTIONS) );
                    if (data &  0b0001'0000) {
                        data &= 0b1110'1111;
                    } else {
                        data |= 0b0001'0000;
                    }
                    Memory::Write(MAP(GPU_OPTIONS), data);
                }
            }
            // [S] Standard Display Enable Toggle
            if (evnt->key.key == SDLK_S)
            {
                SDL_Keymod mod = SDL_GetModState();
                if (mod & SDL_KMOD_SHIFT)
                { // [SHIFT] + [S]  // toggle between text and graphics mode
                    Byte data = Memory::Read( MAP(GPU_MODE) );
                    if (data &  0b1000'0000) {
                        data &= 0b0111'1111;
                    } else {
                        data |= 0b1000'0000;
                    }
                    // Memory::Write(MAP(GPU_OPTIONS), Memory::Read( MAP(GPU_OPTIONS) ));
                    Memory::Write(MAP(GPU_MODE), data);
                } else { // [S] Standard Display Enable
                    Byte data = Memory::Read( MAP(GPU_OPTIONS) );
                    if (data &  0b0000'0001) {
                        data &= 0b1111'1110;
                    } else {
                        data |= 0b0000'0001;
                    }
                    Memory::Write(MAP(GPU_OPTIONS), data);
                    // Memory::Write(MAP(GPU_MODE), Memory::Read( MAP(GPU_MODE) ));
                }
            }

            break;       
        } // END: case SDL_EVENT_KEY_DOWN

        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        {            
            _gpu_options |= 0b0000'1000; // Set bit 6 = ENTER FULLSCREEN
            
            // std::cout << "SDL_EVENT_WINDOW_ENTER_FULLSCREEN (0x" << clr::hex(_gpu_options,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        {
            _gpu_options &= 0b1111'0111; // Clear bit 6 = LEAVE FULLSCREEN
            // std::cout << "SDL_EVENT_WINDOW_LEAVE_FULLSCREEN (0x" << clr::hex(_gpu_options,2) << ")\n";
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
        for (int i=MAP(VIDEO_START); i<MAP(VIDEO_TOP); i+=2) { 
            Memory::Write(i+0, color++);  
            Memory::Write(i+1, data++);  
        }
    }
    runningTime += fElapsedTime;
    // std::cout << fElapsedTime << std::endl;
    

    // mode changes
    if (true)
    {   static Word video_mode = 0x0180;    
        static float delta_change = fElapsedTime;
        static float video_running_time = fElapsedTime;
        if (video_running_time > delta_change + 0.500f) 
        {
            delta_change = fElapsedTime;
            video_running_time = fElapsedTime;
            video_mode++;
            if (video_mode & 0b0001'1110'0000'0000) video_mode += 0b0001'1110'0000'0000;
            Word mode = video_mode | 0b0001'0001'0000'0000;
            mode |= 0b1000'1000'0000'0000;   // force full screen
            Memory::Write_Word(MAP(GPU_OPTIONS), mode);
    std::cout << "(0x" << clr::hex(video_mode,4) << ") Video Mode: " << clr::hex(mode,4) << std::endl;
        }
        video_running_time += fElapsedTime;
    }   
    // is extended graphics enabled?
    if (_gpu_options & 0b0001'0000)
    {
        _render_extended_graphics(); 
        // std::cout << "_render_extended_graphics()\n";
    }

    // is standard graphics enabled?
    if (_gpu_options & 0b0000'0001)
    {
        _render_standard_graphics();
        // std::cout << "_render_standard_graphics()\n";
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
    SDL_FRect r{0.0f, 0.0f, _ext_width, _ext_height};

    // clear the primary texture
    SDL_SetRenderTarget(pRenderer, pMain_Texture);
    SDL_SetRenderDrawColor(pRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(pRenderer);

    // render Extended Graphics
    if (Memory::Read(MAP(GPU_OPTIONS)) & 0b0001'0000)
    {
        // SDL_SetTextureScaleMode(pExt_Texture, SDL_SCALEMODE_LINEAR);   // blurry
        SDL_SetTextureScaleMode(pExt_Texture, SDL_SCALEMODE_NEAREST);     // clear
        SDL_RenderTexture(pRenderer, pExt_Texture, &r, NULL);
    }

    // render Standard Graphics
    if (Memory::Read(MAP(GPU_OPTIONS)) & 0b0000'0001)
    {
        // SDL_SetTextureScaleMode(pStd_Texture, SDL_SCALEMODE_LINEAR);   // blurry
        SDL_SetTextureScaleMode(pStd_Texture, SDL_SCALEMODE_NEAREST);     // clear
        SDL_RenderTexture(pRenderer, pStd_Texture, &r, NULL);
    }

    // Set the render target to the window
    SDL_SetRenderTarget(pRenderer, NULL);

    // SDL_SetTextureScaleMode(pMain_Texture, SDL_SCALEMODE_LINEAR);   // blurry
    SDL_SetTextureScaleMode(pMain_Texture, SDL_SCALEMODE_NEAREST);     // clear
    SDL_RenderTexture(pRenderer, pMain_Texture, NULL, NULL);

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnRender() Exit" << clr::RETURN;
    return true;
} // END: GPU::OnRender()


void GPU::_render_extended_graphics()
{    
    //    GPU_EXT_MODE          = 0xFE02, // (Byte) Extended Graphics Mode
    int _width = _ext_width;
    int _height = _ext_height;

    // clear the extended texture
    _clear_texture(pExt_Texture, 0, 0, 0, 0);

    // is the extended display enabled
    if ((_gpu_options & 0b0001'0000)==0)
        return; // nope, just return

    if ( (_gpu_options & 0b1000'0000) == 0)
    {   // IS Extended Display In Tiled Mode?

        // TODO: render tiled graphics
        //std::cout << "GPU::_render_extended_graphics() ---> Displaying extended tilemap buffer" << std::endl;

        _update_tile_buffer();
    } else {

        int bpp = 0;
        switch((_gpu_options & 0b0110'0000)>>5)    // extended color mode bits 5-
        {
            case 0x00: bpp = 1; break;
            case 0x01: bpp = 2; break;
            case 0x02: bpp = 4; break;
            case 0x03: bpp = 8; break;
        }
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
                        // std::string err = "Pixel index out of range: " + std::to_string(pixel_index);
                        // Bus::Error(err.c_str());
                        SDL_UnlockTexture(pExt_Texture);
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
    //int _width = _std_width;
    //int _height = _std_height;

    // clear the standard texture
    _clear_texture(pStd_Texture, 0, 0, 0, 0);

    // is the standard display enabled
    if ((_gpu_options & 0b0000'0001)==0)
        return;

    // Render the Standard Display Buffer
    if ( (_gpu_mode & 0b1000'0000) == 0) 
    { // IS Standard Display Rendering Text?
        _update_text_buffer();        
    } 
    else
    { // Standard Display Rendering Graphics
        int bpp = 0;
        int div = 0;
        int std_color_mode = (_gpu_mode & 0b0110'0000) >> 5;
        int buffer_size;

        // Reduce the standard color mode if necessary. This
        // should already fit, but this is a safety check.
        do
        {
            switch(std_color_mode)
            {
                case 0x00: div = 8; bpp = 1; break;
                case 0x01: div = 4; bpp = 2; break;
                case 0x02: div = 2; bpp = 4; break;
                case 0x03: div = 1; bpp = 8; break;
            }            
            buffer_size = (_std_width * _std_height)/div;
            std_color_mode--;
        } while (buffer_size > 8000);

        // display the standard bitmap buffer
        Word pixel_index = MAP(VIDEO_START);
        void *pixels;
        int pitch;
        if (!SDL_LockTexture(pStd_Texture, NULL, (void **)&pixels, &pitch))
            Bus::Error(SDL_GetError());	
        else
        {
            for (int y = 0; y < _std_height; y++)
            {
                for (int x = 0; x < _std_width; )
                {
                    // 256 color mode
                    if (bpp == 8)
                    {
                        Byte index = Memory::Read(pixel_index++);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 16 color mode
                    else if (bpp == 4)
                    {
                        Byte data = Memory::Read(pixel_index++);
                        Byte index = (data >> 4);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                        index = (data & 0x0f);
                        _setPixel_unlocked(pixels, pitch, x++, y, index, true);   
                    }
                    // 4 color mode
                    else if (bpp == 2)
                    {
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
                        Bus::Error(err.c_str(), __FILE__, __LINE__);
                        SDL_UnlockTexture(pStd_Texture); 
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
        Word end = ((col*row)*2) + MAP(VIDEO_START);
        // Word end  = VIDEO_START+128;
		Word addr = MAP(VIDEO_START);
		for (; addr < end; addr += 2)
		{
			Byte at = Memory::Read(addr+0, true);
			Byte ch = Memory::Read(addr+1, true);
			Byte bg = at >> 4;
			Byte fg = at & 0x0f;
			Word index = addr - MAP(VIDEO_START);
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


void GPU::_update_tile_buffer()
{
    // for now just color cycle the extended video buffer
    static Byte clr = 1;
    static int delta = 1;
    static Byte r = 0;
    static Byte g = 0;
    static Byte b = 0;
    Byte a = 15;

    static Uint64 _last_time = SDL_GetTicks();
    if (SDL_GetTicks() - _last_time > 25) {
        _last_time = SDL_GetTicks();
    
        if (clr & 0b0000'0100)  r+=delta;
        if (clr & 0b0000'0010)  g+=delta;
        if (clr & 0b0000'0001)  b+=delta;

        // reverse delta
        if (r == 8 || g == 8 || b == 8) { delta *= -1; }
        if (r == 0 || g == 0 || b == 0) {
            delta *= -1;
            clr ++;
            if (clr > 15)
                clr = 1;
        }
    }
    _clear_texture(pExt_Texture, r, g, b, a);
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
			{ 0x0000 },		// 0: black    0xF000
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

void GPU::_display_mode_helper(Byte mode, int &width, int &height)
{
    switch(mode & 0x1F)
    {
        case 0x00: width = 320; height = 200; break;
        case 0x01: width = 256; height = 160; break;
        case 0x02: width = 320; height = 100; break;
        case 0x03: width = 256; height = 80 ; break;
        case 0x04: width = 160; height = 200; break;
        case 0x05: width = 128; height = 160; break;
        case 0x06: width = 160; height = 100; break;
        case 0x07: width = 128; height = 80 ; break;
        case 0x08: width = 320; height = 200; break;
        case 0x09: width = 256; height = 160; break;
        case 0x0A: width = 320; height = 100; break;
        case 0x0B: width = 256; height = 80 ; break;
        case 0x0C: width = 160; height = 200; break;
        case 0x0D: width = 128; height = 160; break;
        case 0x0E: width = 160; height = 100; break;
        case 0x0F: width = 128; height = 80 ; break;
        case 0x10: width = 320; height = 200; break;
        case 0x11: width = 256; height = 160; break;
        case 0x12: width = 320; height = 100; break;
        case 0x13: width = 256; height = 80 ; break;
        case 0x14: width = 160; height = 200; break;
        case 0x15: width = 128; height = 160; break;
        case 0x16: width = 160; height = 100; break;
        case 0x17: width = 128; height = 80 ; break;
        case 0x18: width = 320; height = 200; break;
        case 0x19: width = 256; height = 160; break;
        case 0x1A: width = 320; height = 100; break;
        case 0x1B: width = 256; height = 80 ; break;
        case 0x1C: width = 160; height = 200; break;
        case 0x1D: width = 128; height = 160; break;
        case 0x1E: width = 160; height = 100; break;
        case 0x1F: width = 128; height = 80 ; break;
    }    
}

Byte GPU::_verify_gpu_mode_change(Byte data, Word map_register)
{
    // Byte saved_options = _gpu_options;
    // Byte saved_mode = _gpu_mode;

    // toggle fullscreen / windowed
    if (map_register == MAP(GPU_OPTIONS)) 
    {
        // if nothing else but the application fullscreen state has changed
        if ( (data & 0b0000'1000) != (_gpu_options & 0b0000'1000) )
        {
            if (data & 0b0000'1000) {
                SDL_SetWindowFullscreen(pWindow, true);
            } else {
                SDL_SetWindowFullscreen(pWindow, false);
            }             
        }     
        _gpu_options = data;   
    }

    if (map_register == MAP(GPU_MODE))
    {
        int width, height;
        _display_mode_helper( (data & 0x1f) , width, height);

        _ext_width  = (float)width;
        _ext_height = (float)height;
        _std_width  = (float)width;
        _std_height = (float)height;

        int buffer_size = 99999;
        while (buffer_size > 8000)
        {
            int div = 0;
            int std_color_mode = (data & 0b0110'0000) >> 5;
            switch(std_color_mode)
            {
                case 0x00: div = 8; break;
                case 0x01: div = 4; break;
                case 0x02: div = 2; break;
                case 0x03: div = 1; break;
            }            
            buffer_size = (_std_width * _std_height)/div;
            if (buffer_size > 8000) 
            { 
                // reduce the standard bitmap color depth
                data = (data & 0b1001'1111) | ((std_color_mode-1)<<5);
                //std::cout << "GPU: Reducing Standard Bitmap Color Depth: " << buffer_size << "\n";
                _gpu_mode = data;
            }
        }
    }
    return data;
}


// END: GPU.cpp

