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
        Uint32 _window_flags = 0;							
		SDL_Window* _window = nullptr;
		float _aspect = 1.0f;		// invalid default

		// hardware registers
		Byte _dsp_gres	= 0b10111111;	// default
		Byte _dsp_gmode = 0b01001010;	// default
};


#endif //__GFX_H__


    // DSP_GRES: BBRR.HHVV  (0b10111111)
    //     BB:00 = 1-bpp (2-colors)
    //     BB:01 = 2-bpp (4-colors)
    //     BB:10 = 4-bpp (16-colors)
    //     BB:11 = 8-bpp (256-colors)
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
    
    // DSP_GRES: ABCD.EFGG (0b01001010)
    //     A:0 = VSYNC OFF
    //     A:1 = VSYNC ON
    //     B:0 = Fullscreen Enabled (emulator only)
    //     B:1 = Windowed Enabled (emulator only)
    //     C:0 = Extended Graphics DISABLED
    //     C:1 = Extended Graphics ENABLED
    //     D:0 = Extended Bitmap Mode Active
    //     D:1 = Extended Tile Graphics Active
    //     E:0 = Standard Text / Bitmap DISABLED
    //     E:1 = Standard Text / Bitmap ENABLED
    //     F:0 = Standard Text Mode ENABLED
    //     F:1 = Standard Bitmap Mode ENABLED
    //     GG:00 = Standard Graphics 1-bpp (2-colors)
    //     GG:01 = Standard Graphics 2-bpp (4-colors)
    //     GG:10 = Standard Graphics 4-bpp (16-colors)
    //     GG:11 = Standard Graphics 8-bpp (256-colors)