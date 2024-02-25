// GfxExtended.h

#include "Bus.h"
#include "Gfx.h"
#include "GfxExtended.h"

Byte GfxExtended::read(Word offset, bool debug) 
{ 
	//printf("%s::read()\n", Name().c_str());

	return 0xCC;
}
void GfxExtended::write(Word offset, Byte data, bool debug) 
{
	//printf("%s::write()\n", Name().c_str());
}

Word GfxExtended::OnAttach(Word nextAddr) 
{ 
	//printf("%s::OnAttach()\n", Name().c_str());

	Word old_addr = nextAddr;
	// ...
	return nextAddr - old_addr;
}

void GfxExtended::OnInit()
{
	//printf("%s::OnInit()\n", Name().c_str());
}

void GfxExtended::OnQuit() 
{
	//printf("%s::OnQuit()\n", Name().c_str());
}

void GfxExtended::OnActivate() 
{
	//printf("%s::OnActivate()\n", Name().c_str());
}

void GfxExtended::OnDeactivate() 
{
	//printf("%s::OnDeactivate()\n", Name().c_str());
}

void GfxExtended::OnEvent(SDL_Event* evnt) 
{
	//printf("%s::OnEvent()\n", Name().c_str());
}

void GfxExtended::OnUpdate(float fElapsedTime) 
{
	//printf("%s::OnUpdate()\n", Name().c_str());	
}

void GfxExtended::OnRender() 
{
	//printf("%s::OnRender()\n", Name().c_str());
}
