// GfxBitmap.h

#include "Bus.h"
#include "Gfx.h"
#include "GfxBitmap.h"

Byte GfxBitmap::read(Word offset, bool debug)
{
	Byte data = Bus::Read(offset, true);
	//printf("GfxBitmap Read: $%02x\n", data);
	return data;
}
void GfxBitmap::write(Word offset, Byte data, bool debug)
{
	//printf("GfxBitmap Write: $%04x ($%02x)\n", offset, data);
}

Word GfxBitmap::OnAttach(Word nextAddr)
{
	//	printf("%s::OnAttach()\n", Name().c_str());

	Word old_addr = nextAddr;
	// ...
	return nextAddr - old_addr;
}

void GfxBitmap::OnInit()
{
	//	printf("%s::OnInit()\n", Name().c_str());
}

void GfxBitmap::OnQuit()
{
	//	printf("%s::OnQuit()\n", Name().c_str());
}

void GfxBitmap::OnActivate()
{
	//	printf("%s::OnActivate()\n", Name().c_str());
}

void GfxBitmap::OnDeactivate()
{
	//	printf("%s::OnDeactivate()\n", Name().c_str());
}

void GfxBitmap::OnEvent(SDL_Event* evnt)
{
	//	printf("%s::OnEvent()\n", Name().c_str());
}

void GfxBitmap::OnUpdate(float fElapsedTime)
{
	//	printf("%s::OnUpdate()\n", Name().c_str());
}

void GfxBitmap::OnRender()
{
	//	printf("%s::OnRender()\n", Name().c_str());
}
