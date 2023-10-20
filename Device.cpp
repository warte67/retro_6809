/////
// Device.cpp
// 
// Base device to be attached to the memory map.
////////

#include <iostream>
#include "Device.h"


Word Device::OnAttach(Word nextAddr) 
{ 
	//printf("Device::OnAttach()\n");
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
	if (offset - m_base < m_size)
		return m_memory[(Word)(offset - m_base)];
	return 0xCC;
}

void Device::write(Word offset, Byte data, bool debug) 
{
	if (offset - m_base < m_size)
		m_memory[(Word)(offset - m_base)] = data;
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

void Device::DisplayEnum(std::string sToken, Word ofs, std::string sComment)
{
	// lambda to convert integer to hex string
	auto hex = [](uint32_t n, uint8_t digits)
	{
		std::string s(digits, '0');
		for (int i = digits - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};	

	if (COMPILE_MEMORY_MAP)
	{
		std::string sN = "$";
		std::string sCTK = "  ; ";
		std::string sCMA = "  ";
		if (MEMORY_MAP_OUTPUT_CPP)
		{
			sN = "0x";
			sCTK = "// ";
			sCMA = ", ";
		}
		if (sComment.empty())
			sCTK = "";
		// single line comment
		if (sToken.empty())
			printf("        %s%s\n", sCTK.c_str(), sComment.c_str());
		// normal line
		else
		{
			std::string sLabel = sToken;
			while (sToken.size() < 12)
				sToken += " ";
			sToken += " = ";

			std::cout << sToken << sN << hex(ofs, 4) << sCMA << sCTK << sComment << std::endl;

			// printf("%16s = 0x%04X,\t%s%s\n", 
			// 	sToken.c_str(),
			// 	ofs, 
			// 	sCTK.c_str(),
			// 	sComment.c_str()
			// );		
		}
	}
}