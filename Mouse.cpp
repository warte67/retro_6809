#include "Bus.h"
#include "Mouse.h"


Byte Mouse::read(Word offset, bool debug) 
{ 
    switch (offset)
    {
    case CSR_XPOS + 0:      return mouse_x >> 8;
    case CSR_XPOS + 1:      return mouse_x & 0xFF;
    case CSR_YPOS + 0:      return mouse_y >> 8;
    case CSR_YPOS + 1:      return mouse_y & 0xFF;
    }
    // return default bit pattern
    return 0xCC; 
}
void Mouse::write(Word offset, Byte data, bool debug) 
{
    switch (offset)
    {
    case CSR_XPOS+0:        mouse_x = data >> 8;    break;
    case CSR_XPOS+1:        mouse_x = data & 0xff;  break;
    case CSR_YPOS+0:        mouse_y = data >> 8;    break;
    case CSR_YPOS+1:        mouse_y = data & 0xff;  break;
    }
    // write statically
    Bus::Inst().write(offset, data, true);
}

Word Mouse::OnAttach(Word nextAddr) 
{ 
    int size = 0;
    Word old_addr = nextAddr;

    DisplayEnum("", 0, "");
    DisplayEnum("", 0, "Mouse Cursor Hardware Registers:");
    DisplayEnum("CSR_BEGIN", nextAddr, " start of mouse cursor hardware registers");
    DisplayEnum("CSR_XPOS", nextAddr, " (Word) horizontal mouse cursor coordinate");
    nextAddr += 2;
    DisplayEnum("CSR_YPOS", nextAddr, " (Word) vertical mouse cursor coordinate");
    nextAddr += 2;
    
    // add more IGfxDevices 
    // ...

    DisplayEnum("CSR_END", nextAddr, "End Mouse Registers");

    return nextAddr - old_addr;
}



void Mouse::OnInit() {}
void Mouse::OnQuit() {}
void Mouse::OnActivate() {}
void Mouse::OnDeactivate() {}

void Mouse::OnEvent(SDL_Event* evnt) 
{
    switch (evnt->type)
    {
        case SDL_MOUSEMOTION:
        {
            // CSR_XPOS
            // CSR_YPOS
            int mx = evnt->button.x;
            int my = evnt->button.y;
            write_word(CSR_XPOS, mx);
            write_word(CSR_YPOS, my);

            mx = read_word(CSR_XPOS);
            my = read_word(CSR_YPOS);

            printf("MX: %5d    MY:%5d\n", mx, my);
            break;
        }
    }
}

void Mouse::OnUpdate(float fElapsedTime) {}
void Mouse::OnRender() {}