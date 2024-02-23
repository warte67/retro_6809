// GfxBitmap.h

#include "Bus.h"
#include "Gfx.h"
#include "GfxBitmap.h"

Byte GfxBitmap::read(Word offset, bool debug)
{
	Byte data = Bus::Read(offset, true);
	//printf("GfxBitmap Read: $%02x\n", data);
	return data;
}
void GfxBitmap::write(Word offset, Byte data, bool debug)
{
	//printf("GfxBitmap Write: $%04x ($%02x)\n", offset, data);
}

Word GfxBitmap::OnAttach(Word nextAddr)
{
	//	printf("%s::OnAttach()\n", Name().c_str());

	Word old_addr = nextAddr;
	// ...
	return nextAddr - old_addr;
}

void GfxBitmap::OnInit()
{
	//	printf("%s::OnInit()\n", Name().c_str());
}

void GfxBitmap::OnQuit()
{
	//	printf("%s::OnQuit()\n", Name().c_str());
}

void GfxBitmap::OnActivate()
{
	//	printf("%s::OnActivate()\n", Name().c_str());
}

void GfxBitmap::OnDeactivate()
{
	//	printf("%s::OnDeactivate()\n", Name().c_str());
}

void GfxBitmap::OnEvent(SDL_Event* evnt)
{
	//	printf("%s::OnEvent()\n", Name().c_str());
}

void GfxBitmap::OnUpdate(float fElapsedTime)
{
//	printf("%s::OnUpdate()\n", Name().c_str());

	// just return if we're not displaying standard graphics modes
	if (!m_gfx->_standard_graphics_enable)
		return;
	// just return if were in standard text mode
	if (!m_gfx->_standard_display_mode)
		return;

	const float _delay = 0.01666667f;
	static float _acc = fElapsedTime;
	_acc += fElapsedTime;
	if (_acc > fElapsedTime + _delay)
	{
		_acc -= _delay;
		_updateBitmapScreen();
	}
}

void GfxBitmap::OnRender()
{
	//	printf("%s::OnRender()\n", Name().c_str());
}

void GfxBitmap::_updateBitmapScreen()
{
	Word pixel_index = STD_VID_MIN;
	void *pixels;
	int pitch;
	if (SDL_LockTexture(m_gfx->_std_texture, NULL, &pixels, &pitch) < 0)
		Bus::Error("Failed to lock texture: ");	
	else
	{
		// printf("W:%d H:%d\n", m_gfx->_texture_width, m_gfx->_texture_height);
		for (int y = 0; y < m_gfx->_texture_height; y++)
		{
			for (int x = 0; x < m_gfx->_texture_width; )
			{
				// 256 color mode
				if (m_gfx->_std_bpp == 8)
				{
					Byte index = Bus::Read(pixel_index++);
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
				}
				// 16 color mode
				else if (m_gfx->_std_bpp == 4)
				{
					Byte data = Bus::Read(pixel_index++);
					Byte index = (data >> 4);
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
					index = (data & 0x0f);
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
				}
				// 4 color mode
				else if (m_gfx->_std_bpp == 2)
				{
					Byte data = Bus::Read(pixel_index++);
					Byte index = (data >> 6) & 0x03;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
					index = (data >> 4) & 0x03;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
					index = (data >> 2) & 0x03;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
					index = (data >> 0) & 0x03;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
				}
				// 2 color mode
				else if (m_gfx->_std_bpp == 1)
				{
					Byte data = Bus::Read(pixel_index++);
					Byte index = (data >> 7) & 1;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true); 
					index = (data >> 6) & 1;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
					index = (data >> 5) & 1;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
					index = (data >> 4) & 1;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
					index = (data >> 3) & 1;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
					index = (data >> 2) & 1;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
					index = (data >> 1) & 1;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
					index = (data >> 0) & 1;
					m_gfx->_setPixel_unlocked(pixels, pitch, x++, y, index, true);   
				}
			}
		}
		SDL_UnlockTexture(m_gfx->_std_texture); 
	}	
	// render to the screen texture
	SDL_SetRenderTarget(m_gfx->_renderer, m_gfx->_render_target);
	SDL_RenderCopy(m_gfx->_renderer, m_gfx->_std_texture, NULL, NULL);			
}