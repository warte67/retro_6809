// Gfx.h

#ifndef __GFX_H__
#define __GFX_H__

#include "types.h"
#include "Device.h"

class Gfx : public Device
{
	friend class Bus;
	
    public:
        Gfx() {  _deviceName = "GFX_DEVICE"; }
        // Gfx(std::string sName) { _deviceName = sName; }
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

	protected:
        // SDL window stuff	
		const float _base_texture_width = 128.0f;	// 1024x768 basic screen timing
		float _window_width = 1280.0f;;	
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
		Byte _h_scan = 0;			// invalid default
		Byte _v_scan = 0;			// invalid default
		bool _vsync = false;
		bool _fullscreen = false;
		bool _extended_graphics_enable = false;
		bool _extended_tilemode_enable = false;
		bool _standard_graphics = false;	// i.e. the standard buffer
		bool _standard_bitmap = false;		// 0:text, 1:graphics

		// hardware registers
		Byte _dsp_gres	= 0b11011111;	// default
		Byte _dsp_gmode = 0b01001010;	// default
	
	private:
		// helpers
		void _decode_gfx();
};


#endif //__GFX_H__


    // DSP_GRES: BBRR.HHVV
    //     BB:00 = Extended Graphics 1-bpp (2-color mode)
    //     BB:01 = Extended Graphics 2-bpp (4-color mode)
    //     BB:10 = Extended Graphics 4-bpp (16-color mode)
    //     BB:11 = Extended Graphics 8-bpp (256-color mode)
    //     RR:00 = 16:9  aspect (1.777778)
    //     RR:01 = 16:10 aspect (1.600000)
    //     RR:10 = 16:11 aspect (1.454545)
    //     RR:11 = 16:12 aspect (1.333333)
    //     HH:00 = 1x Horizontal Multiplier
    //     HH:01 = 2x Horizontal Multiplier
    //     HH:10 = 3x Horizontal Multiplier
    //     HH:11 = 4x Horizontal Multiplier
    //     VV:00 = 1x Vertical Multiplier
    //     VV:01 = 2x Vertical Multiplier
    //     VV:10 = 3x Vertical Multiplier
    //     VV:11 = 4x Vertical Multiplier
    
    // DSP_GMODE: ABCD.EFGG
    //     A:0 = VSYNC OFF
    //     A:1 = VSYNC ON
    //     B:0 = Fullscreen Enabled (emulator only)
    //     B:1 = Windowed Enabled (emulator only)
    //     C:0 = Extended Graphics DISABLED
    //     C:1 = Extended Graphics ENABLED
    //     D:0 = Extended Bitmap Mode Active
    //     D:1 = Extended Tile Graphics Active
    //     E:0 = Standard Graphics DISABLED
    //     E:1 = Standard Graphics ENABLED
    //     F:0 = Standard Text Mode ENABLED
    //     F:1 = Standard Bitmap Mode ENABLED
    //     GG:00 = Extended Graphics 1-bpp (2-colors)
    //     GG:01 = Extended Graphics 2-bpp (4-colors)
    //     GG:10 = Extended Graphics 4-bpp (16-colors)
    //     GG:11 = Extended Graphics 8-bpp (256-colors)