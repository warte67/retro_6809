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
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 * 
 ************************************/

#include "Bus.hpp"
#include "GPU.hpp"
#include "Memory.hpp"


/***************************
* Constructor / Destructor *
***************************/

GPU::GPU() 
{ 
    std::cout << clr::indent_push() << clr::CYAN << "GPU Created" << clr::RETURN;
    _device_name = "GPU_DEVICE"; 

    // Enforce Pseudo-Singleton Pattern
    static GPU* pGPU = this;
    if (pGPU != this)
    {
        Bus::Error("GPU already initialized!", __FILE__, __LINE__);
    }
} // END: GPU()

GPU::~GPU() 
{ 
    std::cout << clr::indent_pop() << clr::CYAN << "GPU Destroyed" << clr::RETURN; 
} // END: ~GPU()



/******************
* Virtual Methods *
******************/


int  GPU::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Entry" << clr::RETURN;
 
    SetBaseAddress(nextAddr);
    Word old_address=nextAddr;
    this->heading = "GPU Device Hardware Registers";


    ////////////////////////////////////////////////
    // (Byte) GPU_MODE
    //       Standard Display Mode
    /////
    mapped_register.push_back( { "GPU_MODE", nextAddr, nullptr, nullptr, {""}});
    mapped_register.push_back( { "GPU_MODE_MSB", nextAddr, 
        [this](Word nextAddr) 
        { 
            (void)nextAddr; 
            return (_gpu_mode>>8) & 0xFF; 
        }, 
        [this](Word nextAddr, Byte data) 
        { 
            (void)nextAddr;
            // _verify_gpu_mode_change(data, nextAddr);  // old version
            Word new_mode = (_gpu_mode & 0x00FF) | (data << 8);
            _verify_gpu_mode_change(new_mode); // new version
    }, 
    {   
        "(Byte) Graphics Display Mode",
        "- bit  7   = Extended Display Enable:",
        "              0: Disabled",
        "              1: Enabled",
        "- bit  6   = (reserved)",
        "- bits 4-5 = Extended Color Depth:",
        "              00: 2-Colors",
        "              01: 4-Colors",
        "              10: 16-Colors",
        "              11: 256-Colors",
        "- bit  3   = Extended Rendering Mode", 
        "              0: Tilemap Display",
        "              1: Bitmap Display",
        "- bit  2   = Emulation Screen Mode",
        "              0: Windowed",
        "              1: Fullscreen",
        "- bit  1   = VSync Enable",
        "              0: Disabled",
        "              1: Enabled",
        "- bit  0   = Presentation",
        "              0: Letterbox",
        "              1: Overscan / Stretch","" 
    }
    }); nextAddr+=1;
    // (Byte) GPU_MODE_LSB
    mapped_register.push_back( { "GPU_MODE_LSB", nextAddr,
        [this](Word nextAddr) 
        { 
            (void)nextAddr; 
            return _gpu_mode & 0xFF; 
        },
        [this](Word nextAddr, Byte data) 
        { 
            (void)nextAddr; 
            Word new_mode = (_gpu_mode & 0xFF00) | (data & 0xFF);
            _verify_gpu_mode_change(new_mode); // new version
        }, 
        {
            "- bit  7   = Standard Display Enable",
            "             0: Disabled",
            "             1: Enabled",
            "- bit  6    = (reserved)",
            "- bits 4-5 = Standard Bitmap Color Depth:",
            "              00: 2-Colors",
            "              01: 4-Colors",
            "              10: 16-Colors",
            "              11: 256-Colors",
            "- bit  3    = Standard Bitmap:",
            "              0: Text Display",
            "              1: Bitmap Display",
            "- bit  2    = 0: 320/256 width,  1: 160/128 width",
            "- bit  1    = 0: 200/160 height, 1: 160/80 height",
            "- bit  0    = Base Resolution: 0:320x200, 1:256x160            ",
            ""
        }
    }); nextAddr+=1;    



    ////////////////////////////////////////////////
    // (Word) GPU_VIDEO_MAX
    //       Video Buffer Maximum (Read Only)
    /////
    mapped_register.push_back( { "GPU_VIDEO_MAX", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_video_max >> 8; }, 
        nullptr, {   
            "(Word) Video Buffer Maximum (Read Only)",
            " Note: This will change to reflect",
            "       the size of the last cpu",
            "       accessible memory location",
            "       of the currently active",
            "       standard video mode.",""
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_video_max & 0xFF; }, 
        nullptr, {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_HRES
    //      Horizontal Pixel Resolution (Read Only)
    /////
    mapped_register.push_back({ "GPU_HRES", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return _gpu_hres >> 8; }, 
        nullptr,  {
            "(Word) Horizontal Pixel Resolution (Read Only)",
            "  Note: This will reflect the number of",
            "       pixel columns for bitmap modes.",""
        }}); nextAddr+=1;        
     mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_hres & 0xFF; }, 
        nullptr, {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) GPU_VRES
    //      Vertical Pixel Resolution (Read Only)
    /////
    mapped_register.push_back({ "GPU_VRES", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return _gpu_vres >> 8; }, 
        nullptr,  {
            "(Word) Vertical Pixel Resolution (Read Only)",
            "  Note: This will reflect the number of",
            "       pixel rows for bitmap modes.",""
        }}); nextAddr+=1;        
     mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_vres & 0xFF; }, 
        nullptr, {""}}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) GPU_TCOLS
    //      Text Horizontal Columns (Read Only)
    /////
    mapped_register.push_back({ "GPU_TCOLS", nextAddr,  
        [this](Word nextAddr) { (void)nextAddr; return _gpu_tcols & 0xFF; }, 
        nullptr,  {
            "(Byte) Text Horizontal Columns (Read Only)",
            "  Note: This will reflect the number of",
            "       glyph columns for text modes.",""
            ""}}); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_TROWS
    //      Text Vertical Rows (Read Only)
    /////
    mapped_register.push_back({ "GPU_TROWS", nextAddr,  
        [this](Word nextAddr) { (void)nextAddr; return _gpu_trows & 0xFF; }, 
        nullptr,  {
            "(Byte) Text Vertical Rows (Read Only)",
            "  Note: This will reflect the number of",
            "       glyph rows for text modes.",""
            ""}}); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Byte) GPU_PAL_INDEX
    //      Color Palette Index
    /////
    mapped_register.push_back({ "GPU_PAL_INDEX", nextAddr,  
        [this](Word nextAddr) { (void)nextAddr; return _gpu_pal_index; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _gpu_pal_index = data; }, 
        {
            "(Byte) Color Palette Index",
            "  Note: Use this register to set the",
            "       index into the Color Palette.",
            "       Set this value prior referencing",
            "       the color data (GPU_PAL_COLOR).",""
            ""
        }
    }); nextAddr+=1;
 

    ////////////////////////////////////////////////
    // (Word) GPU_PAL_COLOR
    //      Indexed Color Palette Data (A4R4G4B4 format)
    /////
    mapped_register.push_back({ "GPU_PAL_COLOR", nextAddr,  
        [this](Word nextAddr) { (void)nextAddr; return (_palette[_gpu_pal_index].color >> 8) & 0xFF; }, 
        [this](Word nextAddr, Byte data) { 
            (void)nextAddr; 
            Word c = _palette[_gpu_pal_index].color & 0x00ff;
			_palette[_gpu_pal_index].color = c | ((Word)data << 8);
        }, {
            "(Word) Color Palette Data (A4R4G4B4 format)",
            "  Note: This is the color data for an",
            "       individual palette entry. Write to ",
            "       DSP_PAL_IDX with the index within the",
            "       color palette prior to reading or",
            "       writing the color data in the",
            "       GFX_PAL_CLR register.",
            ""
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _palette[_gpu_pal_index].color & 0xFF; }, 
        [this](Word nextAddr, Byte data) { 
            (void)nextAddr; 
			Word c = _palette[_gpu_pal_index].color & 0xff00;
			_palette[_gpu_pal_index].color = c | data;
        }, {""}}); nextAddr+=1;      


    ////////////////////////////////////////////////
    // (Byte) GPU_GLYPH_IDX
    //     Text Glyph Index (0-255)
    /////
    mapped_register.push_back({ "GPU_GLYPH_IDX", nextAddr,  
        [this](Word nextAddr) { (void)nextAddr; return _gpu_glyph_idx; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _gpu_glyph_idx = data; }, 
        {
            "(Byte) Text Glyph Index",
            "  Note: Use this register to set the",
            "       index of a specific text glyph.",
            "       Set this value prior to updating",
            "       the glyph data (GPU_GLYPH_DATA).",
            ""
        }
    }); nextAddr+=1;


    ////////////////////////////////////////////////
    // (8-Bytes) GPU_GLYPH_DATA 
    //      Text Glyph Pixel Data Array
    /////
    mapped_register.push_back({ "GPU_GLYPH_DATA", nextAddr,  
        [this](Word nextAddr) { (void)nextAddr; return _gpu_glyph_data[_gpu_glyph_idx][0];  }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _gpu_glyph_data[_gpu_glyph_idx][0] = data; }, 
        {
            "(8-Bytes) 8 rows of binary encoded glyph pixel data",
            "  Note: This is the pixel data for a", 
            "       specific text glyph. Each 8x8",
            "       text glyph is composed of 8 bytes.",            
            "       The first byte in this array",
            "       represents the top line of 8 pixels.",            
            "       Each array entry represents a row of 8 pixels.",
            ""
        }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_glyph_data[_gpu_glyph_idx][1]; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _gpu_glyph_data[_gpu_glyph_idx][1] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_glyph_data[_gpu_glyph_idx][2]; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _gpu_glyph_data[_gpu_glyph_idx][2] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_glyph_data[_gpu_glyph_idx][3]; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _gpu_glyph_data[_gpu_glyph_idx][3] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_glyph_data[_gpu_glyph_idx][4]; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _gpu_glyph_data[_gpu_glyph_idx][4] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_glyph_data[_gpu_glyph_idx][5]; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _gpu_glyph_data[_gpu_glyph_idx][5] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_glyph_data[_gpu_glyph_idx][6]; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _gpu_glyph_data[_gpu_glyph_idx][6] = data; }, 
        {""}}); nextAddr+=1;      
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return _gpu_glyph_data[_gpu_glyph_idx][7]; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _gpu_glyph_data[_gpu_glyph_idx][7] = data; }, 
        {""}}); nextAddr+=1;      

// ADD MORE HARDWARE REGISTERS FOR THE IMAGE and the MMU Nested Devices
// LIKE THIS:
//
// nextAddr = GPU::GPU_Image::OnAttach(nextAddr);
// nextAddr = GPU::GPU_MMU::OnAttach(nextAddr);

    ////////////////////////////////////////////////
    // (Constant) GPU_END
    //      End of GPU Register Space
    /////
    nextAddr--;
    mapped_register.push_back({ "GPU_END", nextAddr, 
        nullptr, nullptr,  { "End of GPU Register Space"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Constant) GPU_TOP
    //      Top of GPU Register Space
    //      (start of the next device)
    /////
    mapped_register.push_back({ "GPU_TOP", nextAddr, 
    nullptr, nullptr,  { "Top of GPU Register Space", "---"}});
    

    std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Exit" << clr::RETURN;
    _size = nextAddr - old_address;
    return _size;
} // END: GPU::OnAttach()


void GPU::OnInit()
{
    std::cout << clr::indent() << clr::CYAN << "GPU::OnInit() Entry" << clr::RETURN;

    { // BEGIN OF SDL3 Initialization
        // initialize SDL3
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO))// | SDL_INIT_EVENTS))
        {
            std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onInit() Error" << clr::RETURN;
            Bus::Error(SDL_GetError(), __FILE__, __LINE__);
        }

        // create the main window
        pWindow = SDL_CreateWindow("SDL3 Retro_6809", initial_width, initial_height, window_flags); 
        SDL_ShowWindow(pWindow);


        // create the renderer
        pRenderer = SDL_CreateRenderer(pWindow, NULL);

        SDL_SetRenderLogicalPresentation(pRenderer, (int)_screen_width, (int)_screen_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

        // build the textures
        pExt_Texture = SDL_CreateTexture(pRenderer, 
                SDL_PIXELFORMAT_ARGB4444, 
                SDL_TEXTUREACCESS_STREAMING, 
                (int)_screen_width/2, (int)_screen_height/2);
        SDL_SetTextureScaleMode(pExt_Texture, SDL_SCALEMODE_NEAREST);            

        pStd_Texture = SDL_CreateTexture(pRenderer, 
                SDL_PIXELFORMAT_ARGB4444, 
                SDL_TEXTUREACCESS_STREAMING, 
                (int)_screen_width/2, (int)_screen_height/2);
        SDL_SetTextureScaleMode(pStd_Texture, SDL_SCALEMODE_NEAREST);            

        pForeground_Texture = SDL_CreateTexture(pRenderer, 
                SDL_PIXELFORMAT_ARGB4444, 
                SDL_TEXTUREACCESS_STREAMING, 
                (int)_screen_width/2, (int)_screen_height/2);
        SDL_SetTextureScaleMode(pForeground_Texture, SDL_SCALEMODE_NEAREST); 

    } // END OF SDL3 Initialization

    // Build The Color Palette
    _build_palette();

    // Reserve 64k for the extended video buffer
    int bfr_size = 64*1024;
    _ext_video_buffer.reserve(bfr_size);
  
    // initialize the font glyph buffer
    for (int i=0; i<256; i++)
        for (int r=0; r<8; r++)
            _gpu_glyph_data[i][r] = font8x8_system[i][r];    

    // // initialize the initial default display mode   
    // Memory::Write(MAP(GPU_OPTIONS), _gpu_options);
    Memory::Write_Word(MAP(GPU_MODE), _gpu_mode);

    std::cout << clr::indent() << clr::CYAN << "GPU::OnInit() Exit" << clr::RETURN;
} // END: GPU::OnInit()


void GPU::OnQuit()
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
        if (pForeground_Texture) { 
            SDL_DestroyTexture(pForeground_Texture);
            pForeground_Texture = nullptr;
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
} // END: GPU::OnQuit()



void GPU::OnActivate()
{
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Entry" << clr::RETURN;

    // clear out the extended video buffer
    Word d=0;
    for (int i=0; i<(64*1024); i++) { _ext_video_buffer[i] = d++; }

    // std::cout << clr::indent() << clr::CYAN << "GPU::OnActivate() Exit" << clr::RETURN;
} // END: GPU::OnActivate()



void GPU::OnDeactivate()
{
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnDeactivate() Entry" << clr::RETURN;       
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnDeactivate() Exit" << clr::RETURN;
} // END: GPU::OnDeactivate()


void GPU::OnEvent(SDL_Event* evnt)
{
    // if not a main window event, just return now
    if (SDL_GetWindowFromEvent(evnt) != pWindow) { return; }

    SDL_Keymod km = SDL_GetModState();

    switch (evnt->type) 
    {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:  
            Bus::IsRunning(false);
            break;

        case SDL_EVENT_KEY_DOWN:
        {
            if (evnt->key.key == SDLK_V)
            {   // [V] Toggle VSYNC
                Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                if (data &  0b0000'0010) {
                    data &= 0b1111'1101;
                } else {
                    data |= 0b0000'0010;
                }
                Memory::Write(MAP(GPU_MODE_MSB), data);
            }

            if (km & SDL_KMOD_CTRL)
            {            
                if (evnt->key.key == SDLK_F)
                {   // [F] Toggle Fullscreen
                    Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                    if (data &  0b0000'0100) {
                        data &= 0b1111'1011;
                    } else {
                        data |= 0b0000'0100;
                    }
                    Memory::Write(MAP(GPU_MODE_MSB), data);
                } // END: if (evnt->key.key == SDLK_F)

                // [E] Extended Display Enable Toggle
                if (evnt->key.key == SDLK_E)
                {
                    SDL_Keymod mod = SDL_GetModState();
                    if (mod & SDL_KMOD_SHIFT)
                    { // [SHIFT] + [E]  // toggle between extended tilemap and bitmap mode
                        Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                        if (data &  0b0000'1000) {
                            data &= 0b1111'0111;
                        } else {
                            data |= 0b0000'1000;
                        }
                        Memory::Write(MAP(GPU_MODE_MSB), data);
                    } else { // [E] Extended Display Enable
                        Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                        if (data &  0b1000'0000) {
                            data &= 0b0111'1111;
                        } else {
                            data |= 0b1000'0000;
                        }
                        Memory::Write(MAP(GPU_MODE_MSB), data);
                    }
                }
                // [S] Standard Display Enable Toggle
                if (evnt->key.key == SDLK_S)
                {
                    SDL_Keymod mod = SDL_GetModState();
                    if (mod & SDL_KMOD_SHIFT)
                    { // [SHIFT] + [S]  // toggle between text and bitmap mode
                        Byte data = Memory::Read( MAP(GPU_MODE_LSB) );
                        if (data &  0b0000'1000) {
                            data &= 0b1111'0111;
                        } else {
                            data |= 0b0000'1000;
                        }
                        Memory::Write(MAP(GPU_MODE_LSB), data);
                    } else { // [S] Standard Display Enable
                        Byte data = Memory::Read( MAP(GPU_MODE_LSB) );
                        if (data &  0b1000'0000) {
                            data &= 0b0111'1111;
                        } else {
                            data |= 0b1000'0000;
                        }
                        Memory::Write(MAP(GPU_MODE_LSB), data);
                    }
                }

                if (evnt->key.key == SDLK_RIGHT)
                {
                    Byte data = Memory::Read( MAP(GPU_MODE_LSB) );
                    Byte mode = data+1;
                    data = (data & 0b1111'1000) | (mode & 0b0000'0111);
                    Memory::Write(MAP(GPU_MODE_LSB), data);
                    // clear the screen
                    for (int t=MAP(VIDEO_START); t<_gpu_video_max; t+=2)     
                    {
                        Word atch = (Memory::Read(0x005C)<<8) | ' ';  // _ATTRIB
                        Memory::Write_Word(t, atch);
                    }    
                }
                if (evnt->key.key == SDLK_LEFT)
                {
                    Byte data = Memory::Read( MAP(GPU_MODE_LSB) );
                    Byte mode = data-1;
                    data = (data & 0b1111'1000) | (mode & 0b0000'0111);
                    Memory::Write(MAP(GPU_MODE_LSB), data);
                    // clear the screen
                    for (int t=MAP(VIDEO_START); t<_gpu_video_max; t+=2)     
                    {
                        Word atch = (Memory::Read(0x005C)<<8) | ' ';  // _ATTRIB
                        Memory::Write_Word(t, atch);
                    }    
				}
                if (evnt->key.key == SDLK_UP)
                {
                    // toggle VSYNC
                    Byte bits = 0b0000'0010;
                    Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                    data ^= bits;
                    Memory::Write(MAP(GPU_MODE_MSB), data);
                }
                if (evnt->key.key == SDLK_DOWN)
                {
                    // toggle between letterbox and overscan/stretch modes
                    Byte bits = 0b0000'0001;
                    Byte data = Memory::Read( MAP(GPU_MODE_MSB) );
                    data ^= bits;
                    Memory::Write(MAP(GPU_MODE_MSB), data);
                }
            }
            break;       
        } // END: case SDL_EVENT_KEY_DOWN

        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        {            
            // _gpu_options |= 0b0000'1000; // Set bit 6 = ENTER FULLSCREEN

            _gpu_mode |= 0b0000'0100'0000'0000;
            
            // std::cout << "SDL_EVENT_WINDOW_ENTER_FULLSCREEN (0x" << clr::hex(_gpu_options,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        {
            // _gpu_options &= 0b1111'0111; // Clear bit 6 = LEAVE FULLSCREEN

            _gpu_mode &= ~0b0000'0100'0000'0000;

            // std::cout << "SDL_EVENT_WINDOW_LEAVE_FULLSCREEN (0x" << clr::hex(_gpu_options,2) << ")\n";
            break;
        } // END: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN

    } // END: switch  (evnt->type) 

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnEvent() Exit" << clr::RETURN;
} // END: GPU::OnEvent()


void GPU::OnUpdate(float fElapsedTime)
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
    }
    runningTime += fElapsedTime;   

    // GPU device runs before all other devices. 
    if (true) // Mouse Cursor and/or Foreground Sprites
    {
        // Clear the foreground texture 
        _clear_texture(pForeground_Texture, 0x0, 0x0, 0x0, 0x0);
    }

    // is extended graphics enabled?
    // if (_gpu_options & 0b0001'0000)
    if (_gpu_mode & 0b10000000'0000'0000)
    {
        _render_extended_graphics(); 
//std::cout << "GPU::OnRender() ---> Rendering Extended Texture" << std::endl;
    }
    else
    {
        _clear_texture(pExt_Texture, 15, red(0), grn(0), blu(0));
// std::cout << "GPU::OnRender() ---> Clearing Extended Texture" << std::endl;
    }


    // is standard graphics enabled?
    // if (_gpu_options & 0b0000'0001)
    if (_gpu_mode & 0b0000'0000'1000'0000)
    {
        _render_standard_graphics();
// std::cout << "GPU::OnRender() ---> Rendering Standard Texture" << std::endl;
    }
    else
    {
        _clear_texture(pStd_Texture, 0, 0, 0, 0);
// std::cout << "GPU::OnRender() ---> Clearing Standard Texture" << std::endl;
    }    

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnUpdate() Exit" << clr::RETURN;
} // END: GPU::OnUpdate()


void GPU::OnRender()
{
    //std::cout << clr::indent() << clr::CYAN << "GPU::OnRender() Entry" << clr::RETURN;
    SDL_FRect r{0.0f, 0.0f, _ext_width, _ext_height};

    // render Extended Graphics
    SDL_RenderTexture(pRenderer, pExt_Texture, &r, NULL);

    // render Standard Graphics 
    SDL_RenderTexture(pRenderer, pStd_Texture, &r, NULL);

    // render Foreground Graphics 
    SDL_RenderTexture(pRenderer, pForeground_Texture, &r, NULL);

    //std::cout << clr::indent() << clr::CYAN << "GPU::OnRender() Exit" << clr::RETURN;
} // END: GPU::OnRender()



void GPU::_render_extended_graphics()
{    
    //    GPU_EXT_MODE          = 0xFE02, // (Byte) Extended Graphics Mode
    int _width = _ext_width;
    int _height = _ext_height;

    // clear the extended texture
    _clear_texture(pExt_Texture, 0, red(0), grn(0), blu(0));

    // is the extended display enabled
    if ((_gpu_mode & 0b1000'0000'0000'0000)==0)
        return; // nope, just return

    // IS Extended Display In Tiled Mode?
    if ( (_gpu_mode & 0b0000'1000'0000'0000) == 0)
    { 
        // TODO: render tiled graphics
        //std::cout << "GPU::_render_extended_graphics() ---> Displaying extended tilemap buffer" << std::endl;
        _update_tile_buffer();
    } else {
        // extended bitmap mode
        int bpp = 0;
        // switch((_gpu_options & 0b0110'0000)>>5)
        switch( ((_gpu_mode & 0b0011'0000'0000'0000)>>12)& 0x03 )    // extended color mode bits
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
    // clear the standard texture
    _clear_texture(pStd_Texture, 0, 0, 0, 0);

    // is the standard display enabled
    // if ((_gpu_options & 0b0000'0001)==0)
    if ((_gpu_mode & 0b0000'0000'1000'0000)==0)
        return;

    // Render the Standard Display Buffer
    // if ( (_gpu_mode & 0b1000'0000) == 0) 

    // IS Standard Display Rendering Text?
    if ( (_gpu_mode & 0b0000'0000'0000'1000) == 0) 
    { 
        _update_text_buffer();        
    } 
    else
    { // Standard Display Rendering Graphics
        int bpp = 0;
        int div = 0;
        // int std_color_mode = (_gpu_mode & 0b0110'0000) >> 5;
        int std_color_mode = ((_gpu_mode & 0b0000'0000'0011'0000) >> 4) & 0x03;
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
			// Byte at = Memory::Read(addr+0, true);
			// Byte ch = Memory::Read(addr+1, true);
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
    _clear_texture(pExt_Texture, a, r, g, b);
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

void GPU::_clear_texture(SDL_Texture* texture, Byte a, Byte r, Byte g, Byte b)
{
    a&=0x0f; r&=0x0f; g&=0x0f; b&=0x0f;
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
			{ 0xF000 },		// 0: black             // 0xF000
			{ 0xF555 },		// 1: dk gray
			{ 0xF007 },		// 2: dk blue
			{ 0xF600 },		// 3: dk red
			{ 0xF140 },		// 4: dk green
			{ 0xF840 },		// 5: brown
			{ 0xF406 },		// 6: purple          
			{ 0xF046 },		// 7: deep sea          // 0xF046  	
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
    //    - bit 2    = 0: normal width,  1: half width
    //    - bit 1    = 0: normal height, 1: half height
    //    - bit 0    = Base Resolution: 0:320x200, 1:256x160    
    switch(mode & 0b0000'0111)
    {
        case 0b0000'0000: width = 320; height = 200; break;
        case 0b0000'0001: width = 256; height = 160; break;
        case 0b0000'0010: width = 320; height = 100; break;
        case 0b0000'0011: width = 256; height = 80 ; break;
        case 0b0000'0100: width = 160; height = 200; break;
        case 0b0000'0101: width = 128; height = 160; break;
        case 0b0000'0110: width = 160; height = 100; break;
        case 0b0000'0111: width = 128; height = 80 ; break;
    }    
}


void GPU::_verify_gpu_mode_change(Word mode_data)
{

    int buffer_size = 99999;

    // toggle fullscreen / windowed

    // if nothing else but the application fullscreen state has changed
    if ( (mode_data & 0b0000'0100'0000'0000) != (_gpu_mode & 0b0000'0100'0000'0000) )
    {
        if (mode_data & 0b0000'0100'0000'0000) {
            SDL_SetWindowFullscreen(pWindow, true);
        } else {
            SDL_SetWindowFullscreen(pWindow, false);
        }             
    }     
    // VSync changed
    if ( (mode_data & 0b0000'0010'0000'0000) != (_gpu_mode & 0b0000'0010'0000'0000) )
    {
        if (mode_data & 0b0000'0010'0000'0000) {
            SDL_SetRenderVSync(pRenderer, 1);
            //std::cout << clr::ORANGE << "GPU::_verify_gpu_mode_change()  --  VSYNC ON" << clr::RETURN;
        } else {
            SDL_SetRenderVSync(pRenderer, 0);
            //std::cout << clr::ORANGE << "GPU::_verify_gpu_mode_change()  --  VSYNC OFF" << clr::RETURN;
        } 
    }
    _gpu_mode = mode_data;

    int width, height;
    _display_mode_helper( (_gpu_mode & 0x0007) , width, height);

    // TODO:  Remove _ext_width and _ext_height
    //    ... Remove _std_width and _std_height
    //
    _ext_width  = (float)width;
    _ext_height = (float)height;
    _std_width  = (float)width;
    _std_height = (float)height;

    // set GPU_HRES and GPU_VRES as appropriate
    _gpu_hres = width;
    _gpu_vres = height;
    _gpu_tcols = width / 8;
    _gpu_trows = height / 8;

    // adjust color depth for the standard bitmap modes
    do
    {
        int div = 0;
        int std_color_mode = ((mode_data & 0b0000'0000'0011'0000) >> 4) & 0x03;
        switch(std_color_mode & 0x03)
        {
            case 0x00: div = 8; break;
            case 0x01: div = 4; break;
            case 0x02: div = 2; break;
            case 0x03: div = 1; break;
        }            
        // buffer_size = (_std_width * _std_height)/div;
        buffer_size = (_gpu_hres * _gpu_vres)/div;
        if (buffer_size > 8000) 
        { 
            // reduce the standard bitmap color depth
            mode_data = (mode_data & 0b1111'1111'1100'1111) | ((std_color_mode-1)<<4);

            //std::cout << "GPU: Reducing Standard Bitmap Color Depth: " << buffer_size << "\n";
            _gpu_mode = mode_data;
        }
    } while (buffer_size > 8000);

    // set the GPU_VIDEO_MAX -- Standard Buffer
    if (_gpu_mode & 0b0000'0000'0000'1000)
    {   // bitmap mode
        _gpu_video_max = ( ( MAP(VIDEO_START) + buffer_size ) / 4) - 1;
    }
    else
    {   // text mode
        // buffer_size = int(((_std_width/8.0f) * (_std_height/8.0f)) * 2.0f);
        buffer_size = (_gpu_tcols * _gpu_trows) * 2;
        _gpu_video_max = ( MAP(VIDEO_START) + buffer_size ) - 1;
    }


    // bounds checking Standard Buffer is between 0x0400 and 0x23FF
    if (_gpu_video_max < MAP(VIDEO_START))  { _gpu_video_max = MAP(VIDEO_START); }
    if (_gpu_video_max > MAP(VIDEO_END))    { _gpu_video_max = MAP(VIDEO_END); }

    // adjust the renderer logical presentation to either stretch or letterbox
    if (_gpu_mode & 0b0000'0001'0000'0000) {
        SDL_SetRenderLogicalPresentation(pRenderer, (int)_ext_width, (int)_ext_height, SDL_LOGICAL_PRESENTATION_STRETCH);
		// std::cout << clr::ORANGE << "GPU::_verify_gpu_mode_change()  --  STRETCH" << clr::RETURN;
    } else {
        SDL_SetRenderLogicalPresentation(pRenderer, (int)_ext_width, (int)_ext_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
		// std::cout << clr::ORANGE << "GPU::_verify_gpu_mode_change()  --  LETTERBOX" << clr::RETURN;
    }
}


// END: GPU.cpp

