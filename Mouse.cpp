////
// Mouse.cpp
////////

#include "Bus.h"
#include "Mouse.h"

Byte Mouse::read(Word offset, bool debug) 
{   
    switch (offset)
    {
        case CSR_XPOS+0:		return mouse_x >> 8;
        case CSR_XPOS+1:		return mouse_x & 0xFF;
        case CSR_YPOS+0:		return mouse_y >> 8;
        case CSR_YPOS+1:		return mouse_y & 0xFF;
        case CSR_XOFS:			return mouse_x_offset;
        case CSR_YOFS:			return mouse_y_offset;
        case CSR_SIZE:			return m_size;
        case CSR_FLAGS:			return button_flags;
        case CSR_PAL_INDX:		return m_palette_index;
        case CSR_PAL_DATA+0:    return (mouse_palette[m_palette_index].color) >> 8;
        case CSR_PAL_DATA+1:    return (mouse_palette[m_palette_index].color) & 0xFF;
        case CSR_BMP_INDX:		return bmp_offset;
        case CSR_BMP_DATA:		return cursor_buffer[bmp_offset / 16][bmp_offset % 16];
    }
    return 0xCC;
}

void Mouse::write(Word offset, Byte data, bool debug) 
{
	// for CSR_XPOS && CSR_YPOS see: SDL_WarpMouseInWindow and SDL_WarpMouseGlobal mouse_x = something;
	switch(offset)
	{
		case CSR_XOFS:		mouse_x_offset = data;	break;
		case CSR_YOFS:		mouse_y_offset = data;	break;
		case CSR_SIZE:		m_size = data;
			if (m_size >= 0x20)
				m_size = 0x20;
			break;
		case CSR_SCROLL: 	mouse_wheel = data;		break;
		case CSR_PAL_INDX:	m_palette_index = data;	break;
		case CSR_PAL_DATA+0:	
			mouse_palette[m_palette_index].color = 
				(mouse_palette[m_palette_index].color & 0x00FF) | (data << 8);
			bIsDirty = true;	
			break;
		case CSR_PAL_DATA+1:
			mouse_palette[m_palette_index].color =
				(mouse_palette[m_palette_index].color & 0xFF00) | (data << 0);
			bIsDirty = true;
			break;				
		case CSR_BMP_INDX:	bmp_offset = data;		break;
		case CSR_BMP_DATA:
			cursor_buffer[bmp_offset / 16][bmp_offset % 16] = data;
			bIsDirty = true;
			break;
	}
}

Word Mouse::OnAttach(Word nextAddr) 
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
    DisplayEnum("CSR_SIZE", nextAddr, " (Byte) cursor size (0-15) 0:off");
    nextAddr += 1;
    DisplayEnum("CSR_SCROLL", nextAddr, " (Signed) MouseWheel Scroll: -1, 0, 1");
    nextAddr += 1;
    DisplayEnum("CSR_FLAGS", nextAddr, " (Byte) mouse button flags:");
    DisplayEnum("", 0, " CSR_FLAGS:");
    DisplayEnum("", 0, "      bits 0-5: button states");
    DisplayEnum("", 0, "      bits 6-7: number of clicks");
    nextAddr += 1;
    DisplayEnum("CSR_PAL_INDX", nextAddr, " (Byte) mouse cursor color palette index (0-15)");
    nextAddr += 1;
    DisplayEnum("CSR_PAL_DATA", nextAddr, " (Word) mouse cursor color palette data RGBA4444");
    nextAddr += 2;
    DisplayEnum("CSR_BMP_INDX", nextAddr, " (Byte) mouse cursor bitmap pixel offset");
    nextAddr += 1;
    DisplayEnum("CSR_BMP_DATA", nextAddr, " (Byte) mouse cursor bitmap pixel index color");
    nextAddr += 2;
    DisplayEnum("CSR_END", nextAddr, " end of mouse cursor hardware registers");
    nextAddr += 1;
        
    return nextAddr - old_addr;
} 

void Mouse::OnInit()
{
	gfx = Bus::gfx();
	// Initialaize the palette
	if (mouse_palette.size() == 0)
	{
		std::vector<PALETTE> ref = {
			{ 0x0000 },		// 0000 0000.0000 0000	0
			{ 0x0005 },		// 0000 0000.0000 0101	1
			{ 0x000A },		// 0000 0000.0000 1010	2
			{ 0x000F },		// 0000 0000.0000 1111	3
			{ 0xFFFF },		// 1111 1111.1111 1111	4
			{ 0xCCCF },		// 1010 1010.1010 1111	5
			{ 0x555F },		// 0101 0101.0101 1111	6
			{ 0x00CF },		// 0000 0000.1010 1111	7
			{ 0x0C0F },		// 0000 1010.0000 1111	8
			{ 0xC00F },		// 1010 0000.0000 1111	9
			{ 0x00FF },		// 0000 0000.1111 1111	a
			{ 0x0F0F },		// 0000 1111.0000 1111	b
			{ 0x0FFF },		// 0000 1111.1111 1111	c
			{ 0xF0FF },		// 1111 0000.1111 1111	d
			{ 0xFF0F },		// 1111 1111.0000 1111	e
			{ 0xFFFF },		// 1111 1111.1111 1111	f
		};
		for (int t = 0; t < 16; t++)
			mouse_palette.push_back(ref[t]);
		// add blank entries for the rest of the 256 color palette entries
		PALETTE blank{ 0 };
		while (mouse_palette.size() < 256)
			mouse_palette.push_back(blank);
	}
	// Bus::write(CSR_SIZE, m_size);
}
void Mouse::OnQuit() {}

void Mouse::OnActivate() 
{
    SDL_ShowCursor(SDL_DISABLE);

	if (mouse_texture == nullptr)
	{
		mouse_texture = SDL_CreateTexture(gfx->GetRenderer(), SDL_PIXELFORMAT_RGBA4444,
			SDL_TEXTUREACCESS_TARGET, 16, 16);
		SDL_SetTextureBlendMode(mouse_texture, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(gfx->GetRenderer(), mouse_texture);
		SDL_SetRenderDrawColor(gfx->GetRenderer(), 0, 0, 0, 0x00);
		SDL_RenderClear(gfx->GetRenderer());
		// build the mouse cursor texture
		for (int h = 0; h < 16; h++)
		{
			for (int v = 0; v < 16; v++)
			{
				Byte i = cursor_buffer[v][h] & 0x0f;
				Byte r = _red(i);
				Byte g = _grn(i);
				Byte b = _blu(i);
				Byte a = _alf(i);
				SDL_SetRenderDrawColor(gfx->GetRenderer(), r, g, b, a);
				SDL_RenderDrawPoint(gfx->GetRenderer(), h, v);
			}
		}
		SDL_SetRenderTarget(gfx->GetRenderer(), NULL);
	}

	//		// set up clipping
	//		if (false)
	//		{
	//			int ww, wh;
	//			SDL_GetWindowSize(gfx->GetWindow(), &ww, &wh);
	//			float fh = (float)wh;
	//			float fw = fh * gfx->Aspect();
	//			if (fw > ww)
	//			{
	//				fw = (float)ww;
	//				fh = fw / gfx->Aspect();
	//			}
	//			SDL_Rect dest = { int(ww / 2 - (int)fw / 2), int(wh / 2 - (int)fh / 2), (int)fw, (int)fh };
	//			SDL_RenderSetClipRect(gfx->GetRenderer(), &dest);
	//		}
}

void Mouse::OnDeactivate() 
{
    SDL_ShowCursor(SDL_ENABLE);
	if (mouse_texture)
	{
		SDL_DestroyTexture(mouse_texture);
		mouse_texture = nullptr;
	}	
}

void Mouse::OnEvent(SDL_Event* evnt) 
{
	int ww, wh;
	SDL_GetWindowSize(gfx->GetWindow(), &ww, &wh);
	float fh = (float)wh;
	float fw = fh * gfx->Aspect();
	if (fw > ww)
	{
		fw = (float)ww;
		fh = fw / gfx->Aspect();
	}
	SDL_Rect dest = { int(ww / 2 - (int)fw / 2), int(wh / 2 - (int)fh / 2), (int)fw, (int)fh };

	switch (evnt->type)
	{
		case SDL_MOUSEMOTION:
		{
			SDL_Surface* surf = SDL_GetWindowSurface(gfx->GetWindow());
			float sw = (float)surf->w;
			float sh = (float)surf->h;

			// float rw = (float)gfx->ResWidth();
			// float rh = (float)gfx->ResHeight();

			float rw = (float)Bus::read(DSP_SCN_WIDTH);
			float rh = (float)Bus::read(DSP_SCN_HEIGHT);

			float w_aspect = sw / rw;
			float h_aspect = sh / rh;
			mouse_x_screen = evnt->button.x;
			mouse_y_screen = evnt->button.y;
			mouse_x = int((float)mouse_x_screen / w_aspect);
			mouse_y = int((float)mouse_y_screen / h_aspect);
			
			//if (gfx->Fullscreen())
			{
				 // trim to clipping region
				int ww, wh;
				SDL_GetWindowSize(gfx->GetWindow(), &ww, &wh);
				float fh = (float)wh;
				float fw = fh * gfx->Aspect();
				if (fw > ww)
				{
					fw = (float)ww;
					fh = fw / gfx->Aspect();
				}
				SDL_Rect dest = { int(ww / 2 - (int)fw / 2), int(wh / 2 - (int)fh / 2), (int)fw, (int)fh };
				w_aspect = (float)dest.w / rw;
				h_aspect = (float)dest.h / rh;
				int mx = int((mouse_x_screen/w_aspect) - (dest.x/w_aspect));
				if (mx < 0)  mx = 0;
				if (mx >= rw) mx = (int)rw - 1;
				int my = int((mouse_y_screen / h_aspect) - (dest.y / h_aspect));
				if (my < 0)  my = 0;
				if (my >= rh) my = (int)rh - 1;				
				mouse_x = mx;
				mouse_y = my;
			}
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
			Bus::write(CSR_FLAGS, button_flags);
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			// printf("MOUSEWHEEL: %d\n", evnt->wheel.y);
			mouse_wheel = evnt->wheel.y;
			//bus->debug_write(CSR_SCROLL, mouse_wheel);
			Bus::write(CSR_SCROLL, mouse_wheel);
			break;
		}
	}
}

void Mouse::OnUpdate(float fElapsedTime) 
{
	if (m_size && bIsDirty)
	{
		// update the mouse cursor colors
		SDL_SetRenderTarget(gfx->GetRenderer(), mouse_texture);
		SDL_SetRenderDrawColor(gfx->GetRenderer(), 0, 0, 0, 0x00);
		SDL_RenderClear(gfx->GetRenderer());
		for (int h = 0; h < 16; h++)
		{
			for (int v = 0; v < 16; v++)
			{
				Byte i = cursor_buffer[v][h] & 0x0f;
				Byte r = _red(i);
				Byte g = _grn(i);
				Byte b = _blu(i);
				Byte a = _alf(i);
				SDL_SetRenderDrawColor(gfx->GetRenderer(), r, g, b, a);
				SDL_RenderDrawPoint(gfx->GetRenderer(), h, v);
			}
		}
		bIsDirty = false;
	}		
}

void Mouse::OnRender() 
{
	// set up clipping
	int ww, wh;
	SDL_GetWindowSize(gfx->GetWindow(), &ww, &wh);
	float fh = (float)wh;
	float fw = fh * gfx->Aspect();
	if (fw > ww)
	{
		fw = (float)ww;
		fh = fw / gfx->Aspect();
	}
	SDL_Rect dest = { int(ww / 2 - (int)fw / 2), int(wh / 2 - (int)fh / 2), (int)fw, (int)fh };
	SDL_RenderSetClipRect(gfx->GetRenderer(), &dest);

	// render the textures	
	SDL_SetRenderTarget(gfx->GetRenderer(), NULL);

	// render the texture
	SDL_Rect dst = { mouse_x_screen, mouse_y_screen, m_size * 8, m_size * 8 };
	//if (gfx->Fullscreen())
	{
		SDL_Rect clip;
		SDL_RenderGetClipRect(gfx->GetRenderer(), &clip);
		if (dst.x < clip.x)	dst.x = clip.x;
		if (dst.x > clip.x + clip.w) dst.x = clip.x + clip.w;
		if (dst.y < clip.y)	dst.y = clip.y;
		if (dst.y > clip.y + clip.h) dst.y = clip.y + clip.h;
	}
	dst.x -= mouse_x_offset;
	dst.y -= mouse_y_offset;

	// render the mouse cursor
	SDL_RenderCopy(gfx->GetRenderer(), mouse_texture, NULL, &dst);	

	// reset the clipping rectangle
	SDL_RenderSetClipRect(gfx->GetRenderer(), NULL);
}