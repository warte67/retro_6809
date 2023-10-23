// GfxDebug.cpp
//
#include "Bus.h"
#include "GfxDebug.h"
#include "font8x8_system.h"


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

    DisplayEnum("", 0, "");
    DisplayEnum("", 0, "Debug Hardware Registers:");
    DisplayEnum("DBG_BEGIN", nextAddr, " start of mouse cursor hardware registers");
    DisplayEnum("DBG_TEMP", nextAddr, " (Byte) Simple Debug test register");
    nextAddr += 1;

    // add more IGfxDevices 
    // ...

    DisplayEnum("DBG_END", nextAddr, "End Debug Registers");

    return nextAddr - old_addr;
}

void GfxDebug::OnInit() {}
void GfxDebug::OnQuit() {}

void GfxDebug::OnActivate()
{
    // debug layer texture size
    _tex_width = m_gfx->_base_texture_width * 4;
    _tex_height = _tex_width / m_gfx->_aspect;

    // create the glyph textures 
    printf("glyph_textures.size(): %d\n", (int)glyph_textures.size());
    OnWindowResize();

    // create the working debug layer texture
    _debug_texture = SDL_CreateTexture(m_gfx->_renderer, SDL_PIXELFORMAT_ARGB4444,
        SDL_TEXTUREACCESS_TARGET, _tex_width, _tex_height);
    SDL_SetTextureBlendMode(_debug_texture, SDL_BLENDMODE_BLEND);
}
void GfxDebug::OnDeactivate()
{
    // printf("GfxDebug::OnDeactivate()\n");
    // destroy the debug layer texture
    if (_debug_texture)
    {
        SDL_DestroyTexture(_debug_texture);
        _debug_texture = nullptr;
    }
    // destroy the glyph textures
    for (auto& a : glyph_textures)
        SDL_DestroyTexture(a);
    glyph_textures.clear();

    // printf("glyph_textures.size(): %d\n", (int)glyph_textures.size());
}

void GfxDebug::OnWindowResize()
{
    // For whatever reason, when the viewing window gets resized, 
    //  the glyphs get corrupted. A hacky workaround solution is to recreate them 
    //  when this happens. Need a better understanding of whats really happening.

    if (!_bIsDebugActive)
        return;

    // destroy the glyph textures
    for (auto& a : glyph_textures)
        SDL_DestroyTexture(a);
    glyph_textures.clear();

    // recreate the glyphs
    if (glyph_textures.size() == 0)
    {
        for (int t = 0; t < 256; t++)
        {
            SDL_Texture* glyph = SDL_CreateTexture(m_gfx->_renderer, SDL_PIXELFORMAT_RGBA4444,
                SDL_TEXTUREACCESS_TARGET, 8, 8);
            SDL_SetRenderTarget(m_gfx->_renderer, glyph);
            SDL_SetTextureBlendMode(glyph, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_gfx->_renderer, 0, 0, 0, 0);
            SDL_RenderClear(m_gfx->_renderer);
            for (int y = 0; y < 8; y++)
            {
                for (int x = 0; x < 8; x++)
                {
                    Byte bitMask = 1 << (7 - x);
                    if (font8x8_system[t][y] & bitMask)
                    {
                        SDL_SetRenderDrawColor(m_gfx->_renderer, 255, 255, 255, 255);
                        SDL_RenderDrawPoint(m_gfx->_renderer, x, y);
                    }
                }
            }
            glyph_textures.push_back(glyph);
        }
    }
}

void GfxDebug::OnEvent(SDL_Event* evnt)
{
    switch (evnt->type)
    {
        case SDL_KEYDOWN:
        {
            // toggle fullscreen/windowed
            if (evnt->key.keysym.sym == SDLK_RETURN)
            {
                if (SDL_GetModState() & KMOD_ALT)
                {
                    Byte data = Bus::Inst().read(DSP_EXT);
                    if (m_gfx->_windowed)
                        data &= ~0x01;
                    else
                        data |= 0x01;
                    Bus::Inst().write(DSP_EXT, data);
                }
            }
            if (evnt->key.keysym.sym == SDLK_EQUALS)
            {
                Byte data = Bus::Inst().read(DSP_GRES);
                Byte asp = (data & 0x30) >> 4;
                asp++;
                asp = (asp << 4) | (data & 0xCF);
                Bus::Inst().write(DSP_GRES, asp);
            }
            if (evnt->key.keysym.sym == SDLK_MINUS)
            {
                Byte data = Bus::Inst().read(DSP_GRES);
                Byte asp = (data & 0x30) >> 4;
                asp--;
                asp = (asp << 4) | (data & 0xCF);
                Bus::Inst().write(DSP_GRES, asp);
            }
            if (evnt->key.keysym.sym == SDLK_LEFTBRACKET)
            {
                Byte data = Bus::Inst().read(DSP_GRES);
                Byte asp = (data & 0xC0) >> 6;
                asp--;
                asp = (asp << 6) | (data & 0x3F);
                Bus::Inst().write(DSP_GRES, asp);
            }
            if (evnt->key.keysym.sym == SDLK_RIGHTBRACKET)
            {
                Byte data = Bus::Inst().read(DSP_GRES);
                Byte asp = (data & 0xC0) >> 6;
                asp++;
                asp = (asp << 6) | (data & 0x3F);
                Bus::Inst().write(DSP_GRES, asp);
            }
            // resolution keys
            if (evnt->key.keysym.sym == SDLK_RIGHT)
            {
                Byte data = Bus::Inst().read(DSP_GRES);
                Byte asp = (data & 0x0C) >> 2;
                asp--;
                asp = (asp << 2) | (data & 0xF3);
                Bus::Inst().write(DSP_GRES, asp);
            }
            if (evnt->key.keysym.sym == SDLK_LEFT)
            {
                Byte data = Bus::Inst().read(DSP_GRES);
                Byte asp = (data & 0x0C) >> 2;
                asp++;
                asp = (asp << 2) | (data & 0xF3);
                Bus::Inst().write(DSP_GRES, asp);
            }
            if (evnt->key.keysym.sym == SDLK_UP)
            {
                Byte data = Bus::Inst().read(DSP_GRES);
                Byte asp = (data & 0x03) >> 0;
                asp--;
                asp = (asp << 0) | (data & 0xFC);
                Bus::Inst().write(DSP_GRES, asp);
            }
            if (evnt->key.keysym.sym == SDLK_DOWN)
            {
                Byte data = Bus::Inst().read(DSP_GRES);
                Byte asp = (data & 0x03) >> 0;
                asp++;
                asp = (asp << 0) | (data & 0xFC);
                Bus::Inst().write(DSP_GRES, asp);
            }
            // standard graphics mode (SDLK_BACKSLASH)
            if (evnt->key.keysym.sym == SDLK_BACKSLASH)
            {
                Byte data = Bus::Inst().read(DSP_EXT);
                if (data & 0x04)
                    data &= ~0x04;
                else
                    data |= 0x04;
                Bus::Inst().write(DSP_EXT, data);

            }

        } // SDL_KEYDOWN
    }
}

void GfxDebug::OnUpdate(float fElapsedTime)
{
    // wait until the cpu thread is up and running
    if (Bus::m_cpu == nullptr)
        return;
    // do nothing if the debugger is not active
    if (!_bIsDebugActive)
        return;

    // clear the debug layer texture
    SDL_SetRenderTarget(m_gfx->_renderer, _debug_texture);
    SDL_SetRenderDrawColor(m_gfx->_renderer, 0, 0, 0, 128);   // mouse layer background color
    SDL_RenderClear(m_gfx->_renderer);

    // render to the debug layer texture
    OutText(1, 1, "Debug Platform Under Construction...");
    OutText(1, 2, "Hello World");

    // clean up
    SDL_SetRenderTarget(m_gfx->_renderer, NULL);
}

void GfxDebug::OnRender()
{
    // do nothing if the debugger is not active
    if (!_bIsDebugActive)
        return;

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
}

void GfxDebug::OutGlyph(int col, int row, Byte glyph, Byte red, Byte grn, Byte blu, bool bDropShadow)
{
    SDL_Rect dst = { col * 8, row * 8, 8, 8 };
    SDL_SetRenderTarget(m_gfx->_renderer, _debug_texture);
    if (bDropShadow)
    {
        SDL_Rect drop = { dst.x + 1, dst.y + 1, dst.w, dst.h };
        SDL_SetTextureColorMod(glyph_textures[glyph], 0, 0, 0);
        SDL_RenderCopy(m_gfx->_renderer, glyph_textures[glyph], NULL, &drop);

    }
    SDL_SetTextureColorMod(glyph_textures[glyph], red, grn, blu);
    SDL_RenderCopy(m_gfx->_renderer, glyph_textures[glyph], NULL, &dst);
}

int GfxDebug::OutText(int col, int row, std::string text, Byte red, Byte grn, Byte blu, bool bDropshadow)
{
    int pos = 0;
    for (auto& a : text)
        OutGlyph(col++, row, text[pos++], red, grn, blu, bDropshadow);
    return pos;
}


