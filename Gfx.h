/////
// Gfx.h
// 
// Graphics Device
////////

#ifndef __GFX_H__
#define __GFX_H__

#include "types.h"
#include "Device.h"

class Gfx : public Device
{
    public:
        Gfx() {  Name("Gfx"); }
        Gfx(std::string sName) { Name(sName); }
        virtual ~Gfx() {}    

        virtual Word OnAttach(Word nextAddr);
		virtual void OnInit();
		virtual void OnQuit();
		virtual void OnActivate();
		virtual void OnDeactivate();
		virtual void OnEvent(SDL_Event* evnt);
		virtual void OnUpdate(float fElapsedTime);
		virtual void OnRender();

        virtual Byte read(Word offset, bool debug = false);
        virtual void write(Word offset, Byte data, bool debug = false);

        // read-only accessors:
        SDL_Renderer* GetRenderer() { return _renderer; }
        SDL_Window* GetWindow() { return _window; }
        SDL_Texture* GetBGTexture() { return _bg_texture; }

    private:

        // helper functions:
        void _decode_gmode();

        // internals:
        Uint32 _window_flags = SDL_WINDOW_OPENGL | 
                            SDL_WINDOW_RESIZABLE;
        Uint32 _render_flags = SDL_RENDERER_ACCELERATED |
                            SDL_RENDERER_TARGETTEXTURE;
        SDL_Window* _window = nullptr;
        SDL_Renderer* _renderer = nullptr;
        SDL_Texture* _bg_texture;      

        const float _aspect = 1.6f;     //16x10
        int _timing_width = 512;
        int _timing_height = _timing_width / _aspect;   //384;
        int _window_width = _timing_width * 2;
        int _window_height = _timing_height * 2;
        int _bpp = 1;
        int _pixel_width = 1;
        int _pixel_height = 1;
        int _texture_width = _timing_width / _pixel_width;
        int _texture_height = _timing_height / _pixel_height;
        bool _fullscreen = false;
        bool _vsync = false;

        // base graphics device registers
        Byte _dsp_gmode = 0x00;     // DSP_GMODE
        Byte _dsp_emuflags = 0;     // DSP_EMUFLAGS
        Byte _dsp_pxlofs = 0;       // DSP_PXLOFS
        Word _dsp_tbase = 0x0400;   // DSP_TBASE
        Word _dsp_gbase = 0x0000;   // DSP_GBASE
        Word _dsp_gaddr = 0x0000;   // DSP_GADDR
        Byte _dsp_data = 0x00;      // DSP_GDATA
        // display pitch registers (not yet implemented)
        Word _dsp_tpitch = _texture_width / 8;  // DSP_TPITCH
        Word _dsp_gpitch = _texture_width;      // DSP_GPITCH

        Word _g_top_addr = 0xffff;  // top of graphics memory +1 (start sprites here)

        // color palette
        Byte _dsp_pal_idx = 0x00;   // DSP_PAL_IDX
        Word _dsp_pal_clr = 0x0000; // DSP_PAL_CLR
        // text glyph definition array  (move to glyph mode device)
        Byte _dsp_glyph_idx = 0x00; // DSP_GLYPH_IDX
        // Byte _dsp_glhph_data[8]{0}; // DSP_GLYPH_DATA 


        // 64k graphics display buffer
        std::vector<Byte> _gfxDisplayBuffer;

        // helper functions:
        void _updateGraphics(float fElapsedTime);
        void _updateTiles(float fElapsedTime);
        void _updateTextScreen(float fElapsedTime);
        
        void _setPixel(int x, int y, Byte color_index, bool bIgnoreAlpha = false);
        void _setPixel_unlocked(void* pixels, int pitch, int x, int y, Byte color_index, bool bIgnoreAlpha = false);


        // Palette Stuff
        union PALETTE {
            Word color;
            struct {
                Uint8 a : 4;
                Uint8 b : 4;
                Uint8 g : 4;
                Uint8 r : 4;
            };
        };
        std::vector<PALETTE> _palette;
    public:
        Uint8 red(Uint8 index) { Uint8 c = _palette[index].r;  return c | (c << 4) | (c << 8) | (c << 12); }
        Uint8 grn(Uint8 index) { Uint8 c = _palette[index].g;  return c | (c << 4) | (c << 8) | (c << 12); }
        Uint8 blu(Uint8 index) { Uint8 c = _palette[index].b;  return c | (c << 4) | (c << 8) | (c << 12); }
        Uint8 alf(Uint8 index) { Uint8 c = _palette[index].a;  return c | (c << 4) | (c << 8) | (c << 12); }        
};

class Sprite : public Device
{
    public:

        Sint16 x_pos;
        Sint16 y_pos;
        Word flags;
        Byte collision_data[8];
        Byte color_data[256];
};


#endif  // __GFX_H__