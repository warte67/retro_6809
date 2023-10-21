// GfxDebug.cpp
//
#include "Bus.h"
#include "GfxDebug.h"


Byte GfxDebug::read(Word offset, bool debug)
{
    switch (offset)
    {
    //case CSR_XPOS + 0:      return mouse_x >> 8;
    //case CSR_XPOS + 1:      return mouse_x & 0xFF;
    //case CSR_YPOS + 0:      return mouse_y >> 8;
    //case CSR_YPOS + 1:      return mouse_y & 0xFF;
    }
    // return default bit pattern
    return 0xCC;
}
void GfxDebug::write(Word offset, Byte data, bool debug)
{
    switch (offset)
    {
    //case CSR_XPOS + 0:    mouse_x = (mouse_x & 0x00ff) | (data << 8);    break;
    //case CSR_XPOS + 1:    mouse_x = (mouse_x & 0xff00) | (data << 0);    break;
    //case CSR_YPOS + 0:    mouse_y = (mouse_y & 0x00ff) | (data << 8);    break;
    //case CSR_YPOS + 1:    mouse_y = (mouse_y & 0xff00) | (data << 0);    break;
    }
    // write statically
    Bus::Inst().write(offset, data, true);
}

Word GfxDebug::OnAttach(Word nextAddr)
{
    int size = 0;
    Word old_addr = nextAddr;

 /******

    DisplayEnum("", 0, "");
    DisplayEnum("", 0, "Mouse Cursor Hardware Registers:");
    DisplayEnum("CSR_BEGIN", nextAddr, " start of mouse cursor hardware registers");
    DisplayEnum("CSR_XPOS", nextAddr, " (Word) horizontal mouse cursor coordinate");
    nextAddr += 2;
    DisplayEnum("CSR_YPOS", nextAddr, " (Word) vertical mouse cursor coordinate");
    nextAddr += 2;

    // add more IGfxDevices 
    // ...

    DisplayEnum("CSR_END", nextAddr, "End Mouse Registers");
******/

    return nextAddr - old_addr;
}



void GfxDebug::OnInit() {}
void GfxDebug::OnQuit() {}

void GfxDebug::OnActivate()
{
    // debug layer texture size
    _tex_width = m_gfx->_base_texture_width * 4;
    _tex_height = _tex_width / m_gfx->_aspect;

    // printf("_tex_width: %d  t_height: %d\n", _tex_width, _tex_height);

    // create the working mouse layer texture
    _debug_texture = SDL_CreateTexture(m_gfx->_renderer, SDL_PIXELFORMAT_ARGB4444,
        SDL_TEXTUREACCESS_TARGET, _tex_width, _tex_height);
    SDL_SetTextureBlendMode(_debug_texture, SDL_BLENDMODE_BLEND);
}
void GfxDebug::OnDeactivate()
{
    // destroy the debug layer texture
    if (_debug_texture)
    {
        SDL_DestroyTexture(_debug_texture);
        _debug_texture = nullptr;
    }
}

void GfxDebug::OnEvent(SDL_Event* evnt)
{
    //switch (evnt->type)
    //{
    //case SDL_MOUSEMOTION:
    //{
    //    // CSR_XPOS
    //    // CSR_YPOS
    //    int mx = evnt->button.x;
    //    int my = evnt->button.y;
    //    write_word(CSR_XPOS, mx);
    //    write_word(CSR_YPOS, my);
    //    mx = read_word(CSR_XPOS);
    //    my = read_word(CSR_YPOS);
    //    printf("MX: %5d    MY:%5d\n", mx, my);
    //    break;
    //}
    //}
}

void GfxDebug::OnUpdate(float fElapsedTime)
{
    // clear the debug layer texture
    SDL_SetRenderTarget(m_gfx->_renderer, _debug_texture);
    SDL_SetRenderDrawColor(m_gfx->_renderer, 0, 0, 0, 96);   // mouse layer background color
    SDL_RenderClear(m_gfx->_renderer);

    // clean up
    SDL_SetRenderTarget(m_gfx->_renderer, NULL);
}

void GfxDebug::OnRender()
{
    SDL_SetRenderTarget(m_gfx->_renderer, NULL);

    // build the destination rectangle according to current aspect ratio
    int ww = m_gfx->_window_width;
    int wh = m_gfx->_window_height;
    float fh = (float)m_gfx->_window_height;
    float fw = fh * m_gfx->_aspect;

    if (fw > ww)
    {
        fw = ww;
        fh = fw / m_gfx->_aspect;
    }
    SDL_Rect dest =
    {
        int(m_gfx->_window_width / 2 - (int)fw / 2),
        int(wh / 2 - (int)fh / 2),
        (int)fw,
        (int)fh
    };
    // render the mouse layer texture
    SDL_RenderCopy(m_gfx->_renderer, _debug_texture, NULL, &dest);


    printf("OnRender\n");

}