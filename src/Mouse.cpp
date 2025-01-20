/*** Mouse.cpp *******************************************
 *      __  __                                        
 *     |  \/  |                                       
 *     | \  / | ___  _   _ ___  ___   ___ _ __  _ __  
 *     | |\/| |/ _ \| | | / __|/ _ \ / __| '_ \| '_ \ 
 *     | |  | | (_) | |_| \__ \  __/| (__| |_) | |_) |
 *     |_|  |_|\___/ \__,_|___/\___(_)___| .__/| .__/ 
 *                                       | |   | |    
 *                                       |_|   |_|  
 *   
 * This file serves meerly as a template as a
 * starting point for new devices.
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#include "Bus.hpp"
#include "Debug.hpp"
#include "GPU.hpp"
#include "Mouse.hpp"



/***************************
* Constructor / Destructor *
***************************/

Mouse::Mouse() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Mouse Device Created" << clr::RETURN;
    _device_name = "CSR_DEVICE"; 
} // END: Mouse()

Mouse::~Mouse() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Mouse Device Destroyed" << clr::RETURN; 
} // END: ~Mouse()



/******************
* Virtual Methods *
******************/



int  Mouse::OnAttach(int nextAddr)
{
    // std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnAttach() Entry" << clr::RETURN;    

    SetBaseAddress(nextAddr);
    Word old_address=nextAddr;
    this->heading = "Mouse Device Hardware Registers";


    ////////////////////////////////////////////////
    // (Word) CSR_XPOS
    //       Horizontal mouse cursor coordinate
    /////
    mapped_register.push_back( { "CSR_XPOS", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return mouse_x >> 8; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _bCsrIsDirty = true; mouse_x = (mouse_x & 0x00ff) | (data << 8); }, 
        { "(Word) Horizontal Mouse Cursor Coordinate" }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return mouse_x & 0xFF; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _bCsrIsDirty = true; mouse_x = (mouse_x & 0xff00) | (data << 0); }, 
        {{""}}}); nextAddr+=1;    


    ////////////////////////////////////////////////
    // (Word) CSR_YPOS
    //       Vertical mouse cursor coordinate
    /////
    mapped_register.push_back( { "CSR_YPOS", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return mouse_y >> 8; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _bCsrIsDirty = true; mouse_y = (mouse_y & 0x00ff) | (data << 8); }, 
        {"(Word) Vertical Mouse Cursor Coordinate"}}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return mouse_y & 0xFF; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _bCsrIsDirty = true; mouse_y = (mouse_y & 0xff00) | (data << 0); }, 
        {""}}); nextAddr+=1;    


    ////////////////////////////////////////////////
    // (Byte) CSR_XOFS
    //       Horizontal mouse cursor offset
    /////
    mapped_register.push_back( { "CSR_XOFS", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return mouse_x_offset; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _bCsrIsDirty = true; mouse_x_offset = data; }, 
        { "(Byte) Horizontal Mouse Cursor Offset" }}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) CSR_YOFS
    //       Vertical mouse cursor offset
    /////
    mapped_register.push_back( { "CSR_YOFS", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return mouse_y_offset; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; _bCsrIsDirty = true; mouse_y_offset = data; }, 
        { "(Byte) Vertical Mouse Cursor Offset" }}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (char) CSR_SCROLL
    //       (char) MouseWheel Scroll: -1, 0, 1
    /////
    mapped_register.push_back( { "CSR_SCROLL", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return mouse_wheel; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; mouse_wheel = data; }, 
        { "(char) MouseWheel Scroll: -1, 0, 1" }}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) CSR_FLAGS
    //       Mouse Device State Flags
    /////
    mapped_register.push_back( { "CSR_FLAGS", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return button_flags; }, 
        [this](Word nextAddr, Byte data) {
            (void)nextAddr;             
            if (data & 0x80)    {  _show_SDL_cursor(true);  }
            else                {  _show_SDL_cursor(false); }
            button_flags = data;           
        }, {   
            "(Byte) Mouse Device State Flags",
            "   bits 0-4: button states",
            "   bits 5-6: number of clicks",
            "   bits 7:   cursor enable",""
        }}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) CSR_BMP_INDX
    //       Vertical mouse cursor offset
    /////
    mapped_register.push_back( { "CSR_BMP_INDX", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return bmp_offset; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; bmp_offset = data; }, 
        { "(Byte) Mouse Cursor Bitmap Pixel Offset" }}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) CSR_BMP_DATA
    //       Mouse Cursor Bitmap Pixel Color Data ($0-$F)
    /////
    mapped_register.push_back( { "CSR_BMP_DATA", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return csr_data[bmp_offset]; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; csr_data[bmp_offset] = data; }, 
        { "(Byte) Mouse Cursor Bitmap Pixel Color Data ($0-$F)" }}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Byte) CSR_PAL_INDX
    //       Mouse Cursor Color Palette Index ($0-$F)
    /////
    mapped_register.push_back( { "CSR_PAL_INDX", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return m_palette_index; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; m_palette_index = data;	 }, 
        { "(Byte) Mouse Cursor Color Palette Index (0-15)" }}); nextAddr+=1;


    ////////////////////////////////////////////////
    // (Word) CSR_PAL_DATA
    //       Mouse Cursor Color Palette Data A4R4G4B4
    /////
    mapped_register.push_back( { "CSR_PAL_DATA", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return (_csr_palette[m_palette_index].color) >> 8; }, 
        [this](Word nextAddr, Byte data) { 
            (void)nextAddr; 
            _csr_palette[m_palette_index].color = (_csr_palette[m_palette_index].color & 0x00FF) | (data << 8); 
        }, { "(Word) Mouse Cursor Color Palette Data A4R4G4B4" }}); nextAddr+=1;
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { (void)nextAddr; return(_csr_palette[m_palette_index].color) & 0xFF; }, 
        [this](Word nextAddr, Byte data) { 
            (void)nextAddr; 
            _csr_palette[m_palette_index].color = (_csr_palette[m_palette_index].color & 0xFF00) | (data << 0); 
        }, {""}}); nextAddr+=1;    


    ////////////////////////////////////////////////
    // (Constant) CSR_END
    //      End of Mouse Device Register Space
    /////
    nextAddr--;
    mapped_register.push_back({ "CSR_END", nextAddr, 
        nullptr, nullptr,  { "End of Mouse Device Register Space"} });
    nextAddr++;
    
    
    ////////////////////////////////////////////////
    // (Constant) CSR_TOP
    //      Top of CSR Register Space
    //      (start of the next device)
    /////
    mapped_register.push_back({ "CSR_TOP", nextAddr, 
    nullptr, nullptr,  { "Top of CSR Register Space", "---"}});


    // std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnAttach() Exit" << clr::RETURN;
    _size = nextAddr - old_address;
    return _size;
}



void Mouse::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnInit() Entry" << clr::RETURN;

    _show_SDL_cursor(true);
    _render_csr_buffer();
    // copy the rest of the palette from Gfx into the local one
    GPU* gpu = Bus::GetGPU();
    _csr_palette.clear();
    for (int i=0;i<256;i++)
    {
        PALETTE clr = { 0x0000 };
        clr.a = gpu->alf(i);
        clr.r = gpu->red(i);
        clr.g = gpu->grn(i);
        clr.b = gpu->blu(i);
        _csr_palette.push_back(clr);
    }

    std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnInit() Exit" << clr::RETURN;
}



void Mouse::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnQuit() Entry" << clr::RETURN;
    std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnQuit() Exit" << clr::RETURN;
}



void Mouse::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnActivate() Entry" << clr::RETURN;
    std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnActivate() Exit" << clr::RETURN;
}



void Mouse::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnDeactivate() Entry" << clr::RETURN;
    std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnDeactivate() Exit" << clr::RETURN;
}


void Mouse::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnEvent() Entry" << clr::RETURN;

    GPU* gpu = Bus::GetGPU();
    Debug* dbg = Bus::GetDebug();
    static bool s_bCsr = (button_flags & 0x80); // cursor state
    
    switch (evnt->type)
    {

        case  SDL_EVENT_WINDOW_MOUSE_LEAVE:
        {
            if (evnt->window.windowID == gpu->GetWindowID())
            {   // leaving the main window
                // printf("EVENT: leaving the main window\n");
                s_bCsr = (button_flags & 0x80);
                _show_SDL_cursor(false);
            }
            else if (evnt->window.windowID == dbg->Get_Window_ID())
            {   // leaving the debug window
                Memory::Write(MAP(CSR_FLAGS), Memory::Read(MAP(CSR_FLAGS)));  // restore prior cursor visibility state
            }
            break;
        } // END case SDL_EVENT_WINDOW_MOUSE_LEAVE

        case SDL_EVENT_WINDOW_MOUSE_ENTER:
        {
            if (evnt->window.windowID == gpu->GetWindowID())
            {   // entering the main window
                // printf("EVENT: entering the main window\n");
                _show_SDL_cursor(s_bCsr);
            }
            else if (evnt->window.windowID == dbg->Get_Window_ID())
            {   // entering the debug window
                SDL_ShowCursor();
            }
            break;
        } // END case SDL_EVENT_WINDOW_MOUSE_ENTER

        case SDL_EVENT_MOUSE_MOTION:
        {
            if (evnt->window.windowID == gpu->GetWindowID() )
            {
                SDL_ConvertEventToRenderCoordinates(gpu->GetRenderer(), evnt);      
                mouse_x = evnt->motion.x;  
                mouse_y = evnt->motion.y; 
// std::cout << "Size: " << gpu->Get_Width() << ", " << gpu->Get_Height() << " --- ";   
// std::cout << "Mouse: " << mouse_x << ", " << mouse_y << std::endl;

                // leaving the letterbox
                if (mouse_x < 0 || mouse_x >= gpu->Get_Width() || mouse_y < 0 || mouse_y >= gpu->Get_Height()) 
                    { _show_SDL_cursor(false); }
                else 
                    { _show_SDL_cursor(true); }
            }
            break;
        } // END case SDL_MOUSEMOTION

        case SDL_EVENT_MOUSE_WHEEL:
        {
            if (evnt->window.windowID == gpu->GetWindowID() )
                mouse_wheel = evnt->wheel.y;
            break;
        } // END case SDL_MOUSEWHEEL    

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            //  CSR_FLAGS:
            //       bits 0-4: button states
            //       bits 5-6: number of clicks
            //       bits 7:   cursor enable   

            // update the button flags
            static Byte s_buttons = 0;
            Byte data = Memory::Read(MAP(CSR_FLAGS));
            data &= 0x80;       // mask out all but the cursor enable bit
            int button_mask = (1 << ((evnt->button.button % 7) - 1));
            // clear the button bit
            if (evnt->type == SDL_EVENT_MOUSE_BUTTON_UP)
                s_buttons &= ~button_mask;
            // set the button bit
            if (evnt->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                s_buttons |= button_mask;
                // update the number of clicks
                data |= (evnt->button.clicks & 0x03) << 5;
                // update the temporary register data
                data |= s_buttons;
            }
            // finally update the hardware register
            Bus::Write(MAP(CSR_FLAGS), data);
            break;
        } // END Mouse Button States
    }

    //std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnEvent() Exit" << clr::RETURN;
}


void Mouse::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnUpdate() Entry" << clr::RETURN;
    (void)fElapsedTime;     // stop the compiler from complaining about unused parameters


    // GPU *gpu = Bus::GetGPU();
    // gpu->ClearMainTexture();

    _display_SDL_cursor();


    //std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnUpdate() Exit" << clr::RETURN;
}


void Mouse::OnRender()
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnRender() Entry" << clr::RETURN;

    // renders to the standard display texture

    //std::cout << clr::indent() << clr::LT_BLUE << "Mouse::OnRender() Exit" << clr::RETURN;
}


void Mouse::_show_SDL_cursor(bool b)
{
    if (b) 
    {
        button_flags |= 0x80;   // show the application cursor
        SDL_HideCursor();       // hide the hardware cursor
    } else {
        button_flags &= ~0x80;  // hide the application cursor
        SDL_ShowCursor();       // show the hardware cursor
    }
}

void Mouse::_display_SDL_cursor()
{
    GPU* gpu = Bus::GetGPU();
    void *pixels;
    int pitch;
    if (!SDL_LockTexture(gpu->GetTexture(), NULL, &pixels, &pitch)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
        // Bus::Error("");

            // std::cout << clr::indent_pop() << clr::ORANGE << "Bus::_onInit() Error" << clr::RETURN;
        Bus::Error(SDL_GetError(), __FILE__, __LINE__);
    }
    else
    {
        // static bool b_wasCleared = false;
        // if (b_wasCleared == false)
        // {
        //     start with a clear texture (pixel streaming version)
        //     for (int ty=0; ty < Memory::Read_Word(MAP(GPU_VRES)); ty++)
        //         for (int tx=0; tx < Memory::Read_Word(MAP(GPU_HRES)); tx++)
        //             _setPixel_cursor(pixels, pitch, tx, ty, 255);    
        //     b_wasCleared = true;
        // }
        if (button_flags & 0x80)
        {
            // b_wasCleared = false;
            
            // render the cursor
            int x = mouse_x + mouse_x_offset;
            int y = mouse_y + mouse_y_offset;   

            int id = 0;
            for (int v=0; v<16; v++)
            {
                for (int h=0; h<16; h++)
                {
                    Byte c = csr_data[id++];
                    _setPixel_cursor(pixels, pitch, h+x, v+y, c);
                }
            }
        }
        SDL_UnlockTexture(gpu->GetTexture()); 
    }   
}

void Mouse::_render_csr_buffer()
{
    // renders the user ledgible data from the csr_buffer to the csr_data vector
    csr_data.clear();
    for (auto& li : csr_buffer)    
    {
        for (auto& ch : li)
        {
            Byte c = 0xff;  // space is 100% transparent, use character index 255.
            if (ch != ' ')
            {
                if (ch>='0' && ch<='9')
                    c = ch-'0';
                else if (ch>='a' && ch<='f')
                    c = (ch-'a')+10;
                else if (ch>='A' && ch<='F')
                    c = (ch-'A')+10;
            }
            csr_data.push_back(c);
        }
    }
}

void Mouse::_setPixel_cursor(void* pixels, int pitch, int x, int y, Byte color_index)
{
    // strict bounds checking
    if (x<0 || y<0)         return;
    if (x >= Memory::Read_Word(MAP(GPU_HRES)) )  return;
    if (y >= Memory::Read_Word(MAP(GPU_VRES)) ) return;

    Uint16 *dst = (Uint16*)((Uint8*)pixels + (y * pitch) + (x*sizeof(Uint16)));
    Uint16 pixel = *dst;	// 0xARGB

    constexpr bool use_alpha = true;    // testing:  this flag does work as expected.
                                        //      Update the _setPixel_unlocked() method in Gfx.

    if (use_alpha)
    {
        Byte r1 = (pixel & 0x0f00) >> 8;
        Byte g1 = (pixel & 0x00f0) >> 4;
        Byte b1 = (pixel & 0x000f) >> 0;
        
        Byte a2 = this->alf(color_index);
        Byte r2 = this->red(color_index);
        Byte g2 = this->grn(color_index);
        Byte b2 = this->blu(color_index);

        Byte r = (((r1 * (16-a2))) + (r2 * (a2+1))) >> 4;
        Byte g = (((g1 * (16-a2))) + (g2 * (a2+1))) >> 4;
        Byte b = (((b1 * (16-a2))) + (b2 * (a2+1))) >> 4;

        // if (color_index == 255)
        // {
        //     *dst = 0x00000000;
        // }
        // else 
        if (alf(color_index) != 0)
        {
            *dst = (
                (a2<<12) | 
                (r<<8) | 
                (g<<4) | 
                (b)
            );          
        }	
    }
    else
    {
        *dst = 
        (
            0xF000 |                    // full alpha, no color mixing
            (red(color_index)<<8) |
            (grn(color_index)<<4) |
            blu(color_index)
        );    
       
    }}



// END: Mouse.cpp
