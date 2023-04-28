////
// Mouse.cpp
////////

#include "Mouse.h"

Byte Mouse::read(Word offset, bool debug) 
{    
    return 0xCC;
}

void Mouse::write(Word offset, Byte data, bool debug) 
{
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
    DisplayEnum("CSR_XOFS", nextAddr, " (Byte) horizontal mouse cursor offset");
    nextAddr += 1;
    DisplayEnum("CSR_YOFS", nextAddr, " (Byte) vertical mouse cursor offset");
    nextAddr += 1;
    DisplayEnum("CSR_SIZE", nextAddr, " (Byte) cursor size (0-15) 0:off");
    nextAddr += 1;
    DisplayEnum("CSR_SCROLL", nextAddr, " (Signed) MouseWheel Scroll: -1, 0, 1");
    nextAddr += 1;
    DisplayEnum("CSR_FLAGS", nextAddr, " (Byte) mouse button flags:");
    DisplayEnum("", 0, " CSR_FLAGS:");
    DisplayEnum("", 0, "      bits 0-5: button states");
    DisplayEnum("", 0, "      bits 6-7: number of clicks");
    nextAddr += 1;
    DisplayEnum("CSR_PAL_INDX", nextAddr, " (Byte) mouse cursor color palette index (0-15)");
    nextAddr += 1;
    DisplayEnum("CSR_PAL_DATA", nextAddr, " (Word) mouse cursor color palette data RGBA4444");
    nextAddr += 2;
    DisplayEnum("CSR_BMP_INDX", nextAddr, " (Byte) mouse cursor bitmap pixel offset");
    nextAddr += 1;
    DisplayEnum("CSR_BMP_DATA", nextAddr, " (Byte) mouse cursor bitmap pixel index color");
    nextAddr += 2;
    DisplayEnum("CSR_END", nextAddr, " end of mouse cursor hardware registers");
    nextAddr += 1;
        
    return nextAddr - old_addr;
} 





void Mouse::OnInit() {}
void Mouse::OnQuit() {}
void Mouse::OnActivate() {}
void Mouse::OnDeactivate() {}
void Mouse::OnEvent(SDL_Event* evnt) {}
void Mouse::OnUpdate(float fElapsedTime) {}
void Mouse::OnRender() {}