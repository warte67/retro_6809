// GfxMouse.cpp
//
#include "Bus.h"
#include "GfxMouse.h"


Byte GfxMouse::read(Word offset, bool debug)
{ 
    switch (offset)
    {
    case CSR_XPOS + 0:      return mouse_x >> 8;
    case CSR_XPOS + 1:      return mouse_x & 0xFF;
    case CSR_YPOS + 0:      return mouse_y >> 8;
    case CSR_YPOS + 1:      return mouse_y & 0xFF;
    case CSR_XOFS:			return mouse_x_offset;
    case CSR_YOFS:			return mouse_y_offset;
    case CSR_SCROLL:        { char d = mouse_wheel; mouse_wheel = 0; return d; }
    case CSR_FLAGS:			return button_flags;
    case CSR_BMP_INDX:		return bmp_offset;
    case CSR_BMP_DATA:		return cursor_buffer[bmp_offset / 16][bmp_offset % 16];
    }
    // return default bit pattern
    return 0xCC; 
}
void GfxMouse::write(Word offset, Byte data, bool debug)
{
    switch (offset)
    {
    case CSR_XPOS + 0:      mouse_x = (mouse_x & 0x00ff) | (data << 8); _bCsrIsDirty = true; break;
    case CSR_XPOS + 1:      mouse_x = (mouse_x & 0xff00) | (data << 0); _bCsrIsDirty = true; break;
    case CSR_YPOS + 0:      mouse_y = (mouse_y & 0x00ff) | (data << 8); _bCsrIsDirty = true; break;
    case CSR_YPOS + 1:      mouse_y = (mouse_y & 0xff00) | (data << 0); _bCsrIsDirty = true; break;
    case CSR_XOFS:		    mouse_x_offset = data; _bCsrIsDirty = true;	break;
    case CSR_YOFS:		    mouse_y_offset = data; _bCsrIsDirty = true;	break;
    case CSR_SCROLL:        mouse_wheel = data;     break;
    case CSR_BMP_INDX:	    bmp_offset = data;		break;
    case CSR_BMP_DATA:
        cursor_buffer[bmp_offset / 16][bmp_offset % 16] = data;
        _bCsrIsDirty = true;
        break;
    }
    // write statically
    Bus::Inst().write(offset, data, true);
}

Word GfxMouse::OnAttach(Word nextAddr)
{ 
    int size = 0;
    Word old_addr = nextAddr;

    DisplayEnum("", 0, "");
    DisplayEnum("", 0, "Mouse Cursor Hardware Registers:");
    DisplayEnum("CSR_BEGIN", nextAddr, " start of mouse cursor hardware registers");
    DisplayEnum("CSR_XPOS", nextAddr, " (Word) horizontal mouse cursor coordinate");
    nextAddr += 2;
    DisplayEnum("CSR_YPOS", nextAddr, " (Word) vertical mouse cursor coordinate");
    nextAddr += 2;
    DisplayEnum("CSR_XOFS", nextAddr, " (Byte) horizontal mouse cursor offset");
    nextAddr += 1;
    DisplayEnum("CSR_YOFS", nextAddr, " (Byte) vertical mouse cursor offset");
    nextAddr += 1;
    DisplayEnum("CSR_SCROLL", nextAddr, " (Signed) MouseWheel Scroll: -1, 0, 1");
    nextAddr += 1;
    DisplayEnum("CSR_FLAGS", nextAddr, " (Byte) mouse button flags:");
    DisplayEnum("", 0, " CSR_FLAGS:");
    DisplayEnum("", 0, "      bits 0-5: button states");
    DisplayEnum("", 0, "      bits 6-7: number of clicks");
    nextAddr += 1;
    DisplayEnum("CSR_BMP_INDX", nextAddr, " (Byte) mouse cursor bitmap pixel offset");
    nextAddr += 1;
    DisplayEnum("CSR_BMP_DATA", nextAddr, " (Byte) mouse cursor bitmap pixel index color");
    nextAddr += 2;



    DisplayEnum("CSR_END", nextAddr, "End Mouse Registers");

    return nextAddr - old_addr;
}



void GfxMouse::OnInit() {}
void GfxMouse::OnQuit() {}

void GfxMouse::OnActivate()
{
    SDL_ShowCursor(SDL_DISABLE);

    // mouse layer texture size
    _tex_width = m_gfx->_base_texture_width * 4;
    _tex_height = _tex_width / m_gfx->_aspect;

    // printf("_tex_width: %d  t_height: %d\n", _tex_width, _tex_height);

    // create the cursor texture
    if (_cursor_texture == nullptr)
    {
        _cursor_texture = SDL_CreateTexture(m_gfx->_renderer, SDL_PIXELFORMAT_RGBA4444,
            SDL_TEXTUREACCESS_TARGET, 16, 16);
        SDL_SetTextureBlendMode(_cursor_texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(m_gfx->_renderer, _cursor_texture);
        SDL_SetRenderDrawColor(m_gfx->_renderer, 0, 0, 0, 0x00);
        SDL_RenderClear(m_gfx->_renderer);
        // build the mouse cursor texture
        for (int h = 0; h < 16; h++)
        for (int h = 0; h < 16; h++)
        {
            for (int v = 0; v < 16; v++)
            {
                Byte i = cursor_buffer[v][h] & 0x0f;
                Byte r = m_gfx->red(i);
                Byte g = m_gfx->grn(i);
                Byte b = m_gfx->blu(i);
                Byte a = m_gfx->alf(i);
                SDL_SetRenderDrawColor(m_gfx->_renderer, r, g, b, a);
                SDL_RenderDrawPoint(m_gfx->_renderer, h, v);
            }
        }
        SDL_SetRenderTarget(m_gfx->_renderer, NULL);
    }

    // create the working mouse layer texture
    _mouse_texture = SDL_CreateTexture(m_gfx->_renderer, SDL_PIXELFORMAT_ARGB4444,
        SDL_TEXTUREACCESS_TARGET, _tex_width, _tex_height);
    SDL_SetTextureBlendMode(_mouse_texture, SDL_BLENDMODE_BLEND);
}
void GfxMouse::OnDeactivate()
{
    // destroy the mouse layer texture
    if (_mouse_texture)
    {
        SDL_DestroyTexture(_mouse_texture);
        _mouse_texture = nullptr;
    }
}

void GfxMouse::OnEvent(SDL_Event* evnt)
{
    switch (evnt->type)
    {
        case SDL_WINDOWEVENT:
        {
            switch (evnt->window.event)
            {
                case SDL_WINDOWEVENT_LEAVE:
                    _bCsrIsVisible = false;
                    break;
                case SDL_WINDOWEVENT_ENTER:
                    _bCsrIsVisible = true;
                    break;
            }
            break;
        }
        case SDL_MOUSEMOTION:
        {
            // trim mouse coordinates to clipping region
            int ww, wh;
            SDL_GetWindowSize(m_gfx->_window, &ww, &wh);
            float fh = (float)wh;
            float fw = fh * m_gfx->_aspect;
            if (fw > ww)
            {
                fw = (float)ww;
                fh = fw / m_gfx->_aspect;
            }
            SDL_Rect dest = { int(ww / 2 - (int)fw / 2), int(wh / 2 - (int)fh / 2), (int)fw, (int)fh };
            float w_aspect = (float)dest.w / m_gfx->_texture_width;
            float h_aspect = (float)dest.h / m_gfx->_texture_height;
            int mx = int((evnt->button.x / w_aspect) - (dest.x / w_aspect));
            int my = int((evnt->button.y / h_aspect) - (dest.y / h_aspect));
            write_word(CSR_XPOS, mx);
            write_word(CSR_YPOS, my);
            mx = read_word(CSR_XPOS);   // verify
            my = read_word(CSR_YPOS);   // verify

            if (mx >= m_gfx->_texture_width || my >= m_gfx->_texture_height)
                SDL_ShowCursor(true);
            else
                SDL_ShowCursor(false);
            printf("mouse_x: %d   mouse_y:%d\n", mx, my);

            break;
        }
        case SDL_MOUSEWHEEL:
        {
            write(CSR_SCROLL, evnt->wheel.y);
            char d = read(CSR_SCROLL);
            printf("WHEEL: %d\n", d);
            break;
        }
        //		CSR_FLAGS = 0x1811,        // (Byte) mouse button flags:
        //			//      bits 0-5: button states
        //			//      bits 6-7: number of clicks
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            // update the button flags
            button_flags = 0;
            int bitmask = (1 << ((evnt->button.button % 7) - 1));
            if (evnt->button.state == 0)
                button_flags &= ~bitmask;
            else
            {
                button_flags |= bitmask;
                button_flags |= (evnt->button.clicks & 0x03) << 6;
            }
            //bus->debug_write(CSR_FLAGS, button_flags);
            write(CSR_FLAGS, button_flags);

            printf("CSR_FLAGS: 0x%02X\n", read(CSR_FLAGS));
            break;
        }
    }
    
}

void GfxMouse::OnUpdate(float fElapsedTime)
{
    // draw the mouse cursor
    if (_bCsrIsDirty)
    {
        SDL_SetRenderTarget(m_gfx->_renderer, _cursor_texture);
        SDL_SetRenderDrawColor(m_gfx->_renderer, 0, 0, 0, 0x00);
        SDL_RenderClear(m_gfx->_renderer);
        // build the mouse cursor texture
        for (int h = 0; h < 16; h++)
        {
            for (int v = 0; v < 16; v++)
            {
                Byte i = cursor_buffer[v][h];// &0x0f;
                Byte r = (m_gfx->red(i) << 4) | m_gfx->red(i);
                Byte g = (m_gfx->grn(i) << 4) | m_gfx->grn(i);
                Byte b = (m_gfx->blu(i) << 4) | m_gfx->blu(i);
                Byte a = (m_gfx->alf(i) << 4) | m_gfx->alf(i);
                SDL_SetRenderDrawColor(m_gfx->_renderer, r,g,b,a);
                //SDL_SetRenderDrawColor(m_gfx->_renderer, r, g, b, a);
                SDL_RenderDrawPoint(m_gfx->_renderer, h, v);
            }
        }
        _bCsrIsDirty = false;
        // clear the mouse layer texture
        SDL_SetRenderTarget(m_gfx->_renderer, _mouse_texture);
        SDL_SetRenderDrawColor(m_gfx->_renderer, 0, 0, 0, 0);   // mouse layer background color
        
        float hs = m_gfx->_h_scan;
        float vs = m_gfx->_v_scan;
        switch (m_gfx->_h_scan)
        {
            case 2:  hs = m_gfx->_h_scan * (2.0f / 3.0f);   break;
            case 3:  hs = m_gfx->_h_scan * (2.0f / 3.0f);   break;
        }
        switch (m_gfx->_v_scan)
        {
            case 2:  vs = m_gfx->_v_scan * (2.0f / 3.0f);   break;
            case 3:  vs = m_gfx->_v_scan * (2.0f / 3.0f);   break;
        }
        //printf("_v_scan: %d\n", m_gfx->_v_scan);
        SDL_Rect dst = {
            mouse_x_offset + (int)((float)mouse_x*hs),
            mouse_y_offset + (int)((float)mouse_y*vs),
            16, 16
        };
        SDL_RenderClear(m_gfx->_renderer);
        SDL_RenderCopy(m_gfx->_renderer, _cursor_texture, NULL, &dst);
    }

    // clean up
    SDL_SetRenderTarget(m_gfx->_renderer, NULL);
}

void GfxMouse::OnRender()
{
    if (_bCsrIsVisible == false)
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
    SDL_RenderCopy(m_gfx->_renderer, _mouse_texture, NULL, &dest);

}