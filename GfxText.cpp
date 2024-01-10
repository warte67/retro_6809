// GfxText.cpp

#include "Bus.h"
#include "Gfx.h"
#include "GfxText.h"
#include <iostream>

Byte GfxText::read(Word offset, bool debug)
{
	Byte data = Bus::Read(offset, true);
	//printf("GfxText Read: $%02x\n", data);

	// printf("_texture_width: %4d  ", m_gfx->_texture_width);
	// printf("_texture_height: %4d ", m_gfx->_texture_height);
	// printf("blinds: %f\n", float(m_gfx->_texture_height)/8.0f);
	return data;
}
void GfxText::write(Word offset, Byte data, bool debug)
{
	//printf("GfxText Write: $%04x ($%02x)\n", offset, data);

	if (m_gfx->_texture_width>0)
	{
		// Word o = offset - STD_VID_MIN;
		// Word gw = m_gfx->_texture_width / 4;	// character + attribute
		// Word h = o % gw;
		// Word v = o / gw;
		// std::cout << "h: " << h << "    v: " << v << std::endl;

		int index = (offset - STD_VID_MIN) / (m_gfx->_texture_width / 4);
		// std::cout << "index: " << index << std::endl;
		if (m_dirty_blinds.size()>0)
			m_dirty_blinds[index] = true;
	}
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

	// // create horizontal blinds
	// for (int t=0; t<m_gfx->_texture_height/8;t++)
	// {
	// 	m_texture_blinds.push_back(SDL_CreateTexture(m_gfx->_renderer, SDL_PIXELFORMAT_ARGB4444,
	//  		SDL_TEXTUREACCESS_STREAMING, m_gfx->_texture_width, 8));
	// 	m_texture_dirty.push_back(true);
	// }
	// std::cout << "OnActivate size: " << m_texture_blinds.size() << std::endl;

	for (int t=0; t<m_gfx->_texture_height/8;t++)
		m_dirty_blinds.push_back(true);
}

void GfxText::OnDeactivate()
{
//	printf("%s::OnDeactivate()\n", Name().c_str());

	// if (m_texture_blinds.size()>0)
	// {
	// 	for (auto a : m_texture_blinds)
	// 		SDL_DestroyTexture(a);
	// 	m_texture_blinds.clear();
	// 	m_texture_dirty.clear();
	// }
	// std::cout << "OnDeactivate size: " << m_texture_blinds.size() << std::endl;

	if (m_dirty_blinds.size()>0)
		for (auto a : m_dirty_blinds)
			m_dirty_blinds.clear();
}

void GfxText::OnEvent(SDL_Event* evnt)
{
//	printf("%s::OnEvent()\n", Name().c_str());
}

void GfxText::OnUpdate(float fElapsedTime)
{
//	printf("%s::OnUpdate()\n", Name().c_str());

	if (m_gfx->_standard_display_mode)
		return;
	if (!m_gfx->_standard_graphics_enable)
		return;

	const float _delay = 0.033333f;
	static float _acc = fElapsedTime;
	_acc += fElapsedTime;
	if (_acc > fElapsedTime + _delay)
	{
		_acc -= _delay;
		if (Bus::bCpuEnabled)
		{
			Bus::bCpuEnabled = false;
			 m_gfx->_display_standard();
			//_updateTextBlind(0);

			// START New Update Code

			if (m_dirty_blinds.size())
			{
				for (int i=0; i<m_dirty_blinds.size(); i++)
				{
					// dirty blind, update the screen texture
					if (m_dirty_blinds[i])
					{
						// render to this blind
						// _updateTextBlind(i);
						// m_gfx->_display_standard();

						// m_dirty_blinds[i] = false;
					}
				}
			}

			// END New Update Code

			Bus::bCpuEnabled = true;
		}
	}
}

void GfxText::OnRender()
{
//	printf("%s::OnRender()\n", Name().c_str());
}

void GfxText::_updateTextBlind(int row)
{
	// Word gw = m_gfx->_texture_width / 4;	// character + attribute
	// Word offset = STD_VID_MIN + (row * gw);
	m_gfx->_updateTextScreen();
	return;

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

}


/******
void Gfx::_updateTextScreen() 
{
    void *pixels;
    int pitch;
    if (SDL_LockTexture(_std_texture, NULL, &pixels, &pitch) < 0) {
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
			Byte width = _texture_width / 8;
			int x = ((index / 2) % width) * 8;
			int y = ((index / 2) / width) * 8;
			for (int v = 0; v < 8; v++)
			{
				for (int h = 0; h < 8; h++)
				{
					int color = bg;
					if (_dsp_glyph_data[ch][v] & (1 << 7 - h))
						color = fg;
					_setPixel_unlocked(pixels, pitch, x + h, y + v, color);
				}
			}
		}
        SDL_UnlockTexture(_std_texture); 
    }
} 
 ******/

