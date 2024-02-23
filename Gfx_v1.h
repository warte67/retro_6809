// Gfx.h

#ifndef __GFX_V1_H__
#define __GFX_V1_H__

#include "types.h"
#include "Device.h"
#include "IGfxDevice.h"
#include "GfxDebug.h"
#include "GfxMouse.h"

#include "GfxExtended.h"
#include "GfxBitmap.h"
#include "GfxText.h"
#include "GfxSprite.h"

class Gfx : public Device
{
	friend class Bus;
    friend class C6809;
    friend class GfxDebug;
    friend class GfxMouse;

    friend class GfxExtended;
    friend class GfxBitmap;
    friend class GfxText;
    friend class GfxSprite;

    public:
        Gfx() { _deviceName = "GFX_DEVICE"; }
        // Gfx(Bus* _bus) { _deviceName = "GFX_DEVICE";  m_bus = _bus; }
        
        //Gfx(std::string sName) { _deviceName = sName; }
        // virtual ~Device() {}

        // virtuals
		virtual Word OnAttach(Word nextAddr);        // map memory and return the size
		virtual void OnInit();
		virtual void OnQuit();
		virtual void OnActivate();
		virtual void OnDeactivate();
		virtual void OnEvent(SDL_Event* evnt);
		virtual void OnUpdate(float fElapsedTime);
		virtual void OnRender();
		
		void OnPresent();

        virtual Byte read(Word offset, bool debug = false);
        virtual void write(Word offset, Byte data, bool debug = false);

        Word Attach(IGfxDevice* dev, Word _lastAddress, Word size = 0);

        // public accessors

	protected:
        // SDL window stuff	
		const float _base_texture_width = 128.0f;		// 1024x768 basic screen timing
       	const int std_buffer_size = 6*1024;    // standard buffer size

		float _window_width = 1280.0f;
		float _window_height = 0;			// invalid default
		float _aspect = 1.0f;				// invalid default		
        Uint32 _window_flags = 0;			// invalid default
		Uint32 _renderer_flags = 0;			// invalid default
		int _texture_width = 0;				// invalid default
		int _texture_height = 0;			// invalid default
		SDL_Texture* _render_target = nullptr;	// render target texture 
		SDL_Texture* _ext_texture 	= nullptr;	// extended texture
		SDL_Texture* _std_texture 	= nullptr;	// standard texture
		SDL_Window* _window 	= nullptr;	// invalid default
		SDL_Renderer* _renderer = nullptr;	// invalid default
		Byte _ext_bpp = 0;					// invalid default
		Byte _std_bpp = 0;					// invalid default
		Byte _h_scan = 0;			        // invalid default
		Byte _v_scan = 0;			        // invalid default
		bool _vsync = false;
		bool _windowed = false;
		bool _extended_graphics_enable = false;
		bool _extended_display_mode = false;
		bool _standard_graphics_enable = false; // invalid default
		bool _standard_display_mode = false;		// 0:text, 1:graphics

        // text glyph stuff
        Byte _dsp_glyph_idx = 0x00;         // DSP_GLYPH_IDX
        Byte _dsp_glyph_data[256][8]{0};    // DSP_GLYPH_DATA (Customizeable)

		// palette stuff
        union PALETTE {
            Word color;
            struct {
                Uint8 b : 4;
                Uint8 g : 4;
                Uint8 r : 4;
                Uint8 a : 4;
            };
        };
        Byte _dsp_pal_idx = 0x00;   // DSP_PAL_IDX
        std::vector<PALETTE> _palette;
	public:
	    Uint8 red(Uint8 index) { Uint8 c = _palette[index].r;  return c; }
        Uint8 grn(Uint8 index) { Uint8 c = _palette[index].g;  return c; }
        Uint8 blu(Uint8 index) { Uint8 c = _palette[index].b;  return c; }
        Uint8 alf(Uint8 index) { Uint8 c = _palette[index].a;  return c; }  

		// hardware registers
	protected:

        Word _std_vid_max = 0x1C00;		// [STD_VID_MAX]
		Byte _dsp_gres	= 0b11000101;	// defaults
		Byte _dsp_ext 	= 0b11001001;	// defaults
        Byte _dsp_err = 0;          

        // graphics devices based on the IGfxDevice
        GfxDebug*       m_debug = nullptr;
        GfxMouse*       m_mouse = nullptr;

        GfxExtended*    m_gfx_extended  = nullptr;
        GfxBitmap*      m_gfx_bitmap    = nullptr;
        GfxText*        m_gfx_text      = nullptr;
        GfxSprite*      m_gfx_sprite    = nullptr;


        // helpers
		void _decode_dsp_gres();
        void _decode_dsp_ext();

		void _setPixel(int x, int y, Byte color_index, 	
						SDL_Texture* _texture, bool bIgnoreAlpha = false);
        void _setPixel_unlocked(void* pixels, int pitch, int x, int y, 
								Byte color_index, bool bIgnoreAlpha = false);

		void _display_standard();
        void _updateTextScreen();       
		void _display_extended();


        std::vector< IGfxDevice*> _gfx_devices;
};

#endif //__GFX_V1_H__

/**********************************************************************************************
 


 
 **********************************************************************************************/
