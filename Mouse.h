////
// Mouse.h
////////

#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <vector>

#include "Device.h"
#include "Gfx.h"

class Mouse : public Device
{
    public:
        Mouse() {  Name("Mouse"); }
        Mouse(std::string sName) { Name(sName); }
        virtual ~Mouse() {}    

        virtual Byte read(Word offset, bool debug = false);
        virtual void write(Word offset, Byte data, bool debug = false);

        virtual Word OnAttach(Word nextAddr);
		virtual void OnInit();
		virtual void OnQuit();
		virtual void OnActivate();
		virtual void OnDeactivate();
		virtual void OnEvent(SDL_Event* evnt);
		virtual void OnUpdate(float fElapsedTime);
		virtual void OnRender();

        // accessors
        int Mouse_Xpos()           { return mouse_x_screen; }
        int Mouse_Ypos()           { return mouse_y_screen; }
        int Mouse_Xofs()           { return mouse_x_offset; }
        int Mouse_Yofs()           { return mouse_y_offset; }
        Byte Mouse_Size()          { return m_size; }
        void Mouse_Size(Byte size) { m_size = size; }

    private:
        int mouse_x_screen = 0;
        int mouse_y_screen = 0;
        int mouse_x = 0;
        int mouse_y = 0;
        char mouse_wheel = 0;
        Uint8 mouse_x_offset = 0;		// mouse cursor offset x
        Uint8 mouse_y_offset = 0;		// mouse cursor offset y
        Byte m_size = DEFAULT_MOUSE_SIZE;
        Uint8 button_flags = 0;
        SDL_Texture* mouse_texture = nullptr;	

        // cursor stuff
        Byte bmp_offset = 0;
        Byte cursor_buffer[16][16] = {		// indexed 16-color bitmap data for the mouse cursor
            {3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {3,4,3,3,2,1,0,0,0,0,0,0,0,0,0,0},
            {2,3,4,4,3,3,2,1,0,0,0,0,0,0,0,0},
            {1,3,4,4,4,4,3,3,2,1,0,0,0,0,0,0},
            {0,2,3,4,4,4,4,4,3,3,2,1,0,0,0,0},
            {0,1,3,4,4,4,4,4,4,4,3,3,2,1,0,0},
            {0,0,2,3,4,4,4,4,4,4,4,4,3,2,1,0},
            {0,0,1,3,4,4,4,4,4,3,3,3,2,1,0,0},
            {0,0,0,2,3,4,4,4,3,2,1,1,0,0,0,0},
            {0,0,0,1,3,4,4,3,2,1,0,0,0,0,0,0},
            {0,0,0,0,2,3,4,3,1,0,0,0,0,0,0,0},
            {0,0,0,0,1,3,4,3,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,2,3,2,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,1,2,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        };

        // Palette Stuff
        Uint8 m_palette_index = 0;
        union PALETTE {
            Word color;
            struct {
                Uint8 a : 4;
                Uint8 b : 4;
                Uint8 g : 4;
                Uint8 r : 4;
            };
        };    
        std::vector<PALETTE> default_palette;
};


#endif // __MOUSE_H__