/////
// Device.cpp
// 
// Base device to be attached to the memory map.
////////

#include "Device.h"


Word Device::OnAttach() 
{ 
	printf("Device::OnAttach()\n");
	// return the size of this devices memory block
	// default = 0 (undefined device)
	return 0; 
}
void Device::OnInit() {}
void Device::OnQuit() {}
void Device::OnActivate() {}
void Device::OnDeactivate() {}
void Device::OnEvent(SDL_Event* evnt) {}
void Device::OnUpdate(float fElapsedTime) {}
void Device::OnRender() {}



Byte Device::read(Word offset, bool debug) 
{
	if (offset - _base < _size)
		return memory[(Word)(offset - _base)];
	return 0xCC;
}

void Device::write(Word offset, Byte data, bool debug) 
{
	if (offset - _base < _size)
		memory[(Word)(offset - _base)] = data;
}

Word Device::read_word(Word offset, bool debug) 
{
	Word ret = (read(offset) << 8) | read(offset + 1);
	return ret;
}

void Device::write_word(Word offset, Word data, bool debug)
{
	Uint8 msb = (data >> 8) & 0xFF;
	Uint8 lsb = data & 0xff;
	write(offset, msb);
	write(offset + 1, lsb);
}