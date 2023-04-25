/////
// Device.cpp
// 
// Base device to be attached to the memory map.
////////

#include "Device.h"

Byte Device::read(Word offset, bool debug) 
{
	if (offset - base < size)
		return memory[(Word)(offset - base)];
	return 0xCC;
}

void Device::write(Word offset, Byte data, bool debug) 
{
	if (offset - base < size)
		memory[(Word)(offset - base)] = data;
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