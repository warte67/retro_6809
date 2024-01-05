// GfxText.cpp

#include "GfxText.h"

Byte GfxText::read(Word offset, bool debug)
{
//	printf("%s::read()\n", Name().c_str());

	return 0xCC;
}
void GfxText::write(Word offset, Byte data, bool debug)
{
//	printf("%s::write()\n", Name().c_str());
}

Word GfxText::OnAttach(Word nextAddr)
{
//	printf("%s::OnAttach()\n", Name().c_str());

	Word old_addr = nextAddr;
	// ...
	return nextAddr - old_addr;
}

void GfxText::OnInit()
{
//	printf("%s::OnInit()\n", Name().c_str());
}

void GfxText::OnQuit()
{
//	printf("%s::OnQuit()\n", Name().c_str());
}

void GfxText::OnActivate()
{
//	printf("%s::OnActivate()\n", Name().c_str());
}

void GfxText::OnDeactivate()
{
//	printf("%s::OnDeactivate()\n", Name().c_str());
}

void GfxText::OnEvent(SDL_Event* evnt)
{
//	printf("%s::OnEvent()\n", Name().c_str());
}

void GfxText::OnUpdate(float fElapsedTime)
{
//	printf("%s::OnUpdate()\n", Name().c_str());
}

void GfxText::OnRender()
{
//	printf("%s::OnRender()\n", Name().c_str());
}
