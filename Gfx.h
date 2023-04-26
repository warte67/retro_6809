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

    private:

    protected:
};

#endif  // __GFX_H__