#include "Mouse.h"


Byte Mouse::read(Word offset, bool debug) { return 0xCC; }
void Mouse::write(Word offset, Byte data, bool debug) {}

Word Mouse::OnAttach(Word nextAddr) { return 0; }



void Mouse::OnInit() {}
void Mouse::OnQuit() {}
void Mouse::OnActivate() {}
void Mouse::OnDeactivate() {}
void Mouse::OnEvent(SDL_Event* evnt) {}
void Mouse::OnUpdate(float fElapsedTime) {}
void Mouse::OnRender() {}