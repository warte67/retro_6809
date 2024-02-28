// GfxText.cpp

#include "Bus.h"
#include "Gfx.h"
#include "GfxText.h"
#include <iostream>

Byte GfxText::read(Word offset, bool debug)
{
	Byte data = Bus::Read(offset, true);
	//printf("GfxText Read: $%02x\n", data);
	return data;
}
void GfxText::write(Word offset, Byte data, bool debug)
{
	//printf("GfxText Write: $%04x ($%02x)\n", offset, data);
}

Word GfxText::OnAttach(Word nextAddr)
{
//	printf("%s::OnAttach()\n", Name().c_str());

	Word old_addr = nextAddr;
	// ...
	return nextAddr - old_addr;
}

void GfxText::OnInit()
{
//	printf("%s::OnInit()\n", Name().c_str());
}

void GfxText::OnQuit()
{
//	printf("%s::OnQuit()\n", Name().c_str());
}

void GfxText::OnActivate()
{
//	printf("%s::OnActivate()\n", Name().c_str());
}

void GfxText::OnDeactivate()
{
//	printf("%s::OnDeactivate()\n", Name().c_str());
}

void GfxText::OnEvent(SDL_Event* evnt)
{
//	printf("%s::OnEvent()\n", Name().c_str());
}

void GfxText::OnUpdate(float fElapsedTime)
{
//	printf("%s::OnUpdate()\n", Name().c_str());

	if (!m_gfx->_standard_graphics_enable)
		return;
	if (m_gfx->_standard_display_mode)
		return;
	const float _delay = 0.01666667f;
	static float _acc = fElapsedTime;
	_acc += fElapsedTime;
	if (_acc > fElapsedTime + _delay)
	{
		_acc -= _delay;
		_updateTextScreen();
	}
}

void GfxText::OnRender()
{
//	printf("%s::OnRender()\n", Name().c_str());
}


void GfxText::_updateTextScreen() 
{
    void *pixels;
    int pitch;

    if (SDL_LockTexture(m_gfx->_std_texture, NULL, &pixels, &pitch) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
        Bus::Error("");
    }
    else
    {		
		Word end = Bus::Read_Word(STD_VID_MAX);
		Word addr = STD_VID_MIN;
		for (; addr <= end; addr += 2)
		{
			Byte ch = Bus::Read(addr, true);
			Byte at = Bus::Read(addr + 1, true);
			Byte fg = at >> 4;
			Byte bg = at & 0x0f;
			Word index = addr - STD_VID_MIN;
			Byte width = m_gfx->_texture_width / 8;
			int x = ((index / 2) % width) * 8;
			int y = ((index / 2) / width) * 8;
			for (int v = 0; v < 8; v++)
			{
				for (int h = 0; h < 8; h++)
				{
					int color = bg;
					if (m_gfx->_dsp_glyph_data[ch][v] & (1 << 7 - h))
						color = fg;
					m_gfx->_setPixel_unlocked(pixels, pitch, x + h, y + v, color);
				}
			}
		}
        SDL_UnlockTexture(m_gfx->_std_texture); 
    }
	// render to the screen texture
	SDL_SetRenderTarget(m_gfx->_renderer, m_gfx->_render_target);
	SDL_RenderCopy(m_gfx->_renderer, m_gfx->_std_texture, NULL, NULL);			
} 

