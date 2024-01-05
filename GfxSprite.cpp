// GfxExtended.h

#include "Gfx.h"
#include "GfxSprite.h"

Byte GfxSprite::read(Word offset, bool debug)
{
//	printf("%s::read()\n", Name().c_str());

	return 0xCC;
}
void GfxSprite::write(Word offset, Byte data, bool debug)
{
//	printf("%s::write()\n", Name().c_str());
}

Word GfxSprite::OnAttach(Word nextAddr)
{
//	printf("%s::OnAttach()\n", Name().c_str());

	Word old_addr = nextAddr;
	// ...
	return nextAddr - old_addr;
}

void GfxSprite::OnInit()
{
//	printf("%s::OnInit()\n", Name().c_str());
}

void GfxSprite::OnQuit()
{
//	printf("%s::OnQuit()\n", Name().c_str());
}

void GfxSprite::OnActivate()
{
//	printf("%s::OnActivate()\n", Name().c_str());
}

void GfxSprite::OnDeactivate()
{
//	printf("%s::OnDeactivate()\n", Name().c_str());
}

void GfxSprite::OnEvent(SDL_Event* evnt)
{
//	printf("%s::OnEvent()\n", Name().c_str());
}

void GfxSprite::OnUpdate(float fElapsedTime)
{
//	printf("%s::OnUpdate()\n", Name().c_str());
}

void GfxSprite::OnRender()
{
//	printf("%s::OnRender()\n", Name().c_str());
}
