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
        struct SCR_TIMING_MODE
        {
            Word width;         // display timing width 
            Word height;        // display timing height
            float aspect;       // width / height
        };
        std::vector<SCR_TIMING_MODE> _scr_timing_modes{};

        struct SCR_DISPLAY_MODE
        {
            Word timing_index;  // display timing index
            Word res_width;     // texture width
            Word res_height;    // texture height
            Byte pixel_width;   // horizontal pixel overscan
            Byte pixel_height;  // vertical pixel overscan
            bool is_std_valid;    // is standard bitmap mode valid?
            bool is_txt_valid;    // is text mode valid?
        };
        std::vector<SCR_DISPLAY_MODE> _scr_display_modes{};
        
                const float _base_texture_width = 128.0f;		// 1024x768 basic screen timing       

        float _window_width = 1280.0f;          // screen/window raw display width
        float _window_height = 0;			    // screen/window raw display height
        float _aspect = 1.0f;				    // screen aspect ratio (not resolution aspect)
        Uint32 _window_flags = 0;			    // SDL window flags
        Uint32 _renderer_flags = 0;			    // SDL renderer flags
        int _texture_width = 0;				    // texture width, aka. display resolution width
        int _texture_height = 0;			    // texture height, aka. display resolution height
        SDL_Texture* _render_target = nullptr;	// render target texture 
        SDL_Texture* _ext_texture 	= nullptr;	// extended texture
        SDL_Texture* _std_texture 	= nullptr;	// standard texture
        SDL_Window* _window 	= nullptr;	    // SDL Window pointer
        SDL_Renderer* _renderer = nullptr;	    // SDL Renderer pointer
        Byte _ext_bpp = 0;					    // extended bits per pixel
        Byte _std_bpp = 0;					    // standard bits per pixel
        Byte _pixel_w = 0;			            // pixel width
        Byte _pixel_h = 0;			            // pixel height
        bool _vsync = true;                    // 0:unmetered, 1:use vsync
        bool _windowed = true;                  // 0:fullscreen, 1:windowed

        bool _extended_graphics_enable;
        bool _extended_display_mode;
        bool _standard_graphics_enable;     
        bool _standard_display_mode;		// 0:text, 1:graphics

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

        Byte _dsp_res = 0x6B;       // Display Resolution Timing (0x6B=320x200 default)
        Byte _dsp_mode = 0x10;      // Graphics Display Modes (0b.0000.0000)
        Byte _emu_flags = 0x01;     // Emulation flags (defaults)

        Word _std_vid_max = STD_VID_MAX;		// [STD_VID_MAX]
        //    // DEPRICATED
        //        Byte _dsp_gres	= 0b11010000;	// DEPRICATED
        //        Byte _dsp_ext 	= 0b11001001;	// DEPRICATED
        //    // DEPRICATED
        Byte _dsp_err = 0;          

        // graphics devices based on the IGfxDevice
        GfxDebug*       m_debug = nullptr;
        GfxMouse*       m_mouse = nullptr;

        GfxExtended*    m_gfx_extended  = nullptr;
        GfxBitmap*      m_gfx_bitmap    = nullptr;
        GfxText*        m_gfx_text      = nullptr;
        GfxSprite*      m_gfx_sprite    = nullptr;

        // helpers
        void _decode_display();
//            // DEPRICATED
//                void _decode_dsp_gres();
//                void _decode_dsp_ext();
//            // DEPRICATED

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
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 *                                                                                            *
 **********************************************************************************************/
