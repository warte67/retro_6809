/***  IDevice.cpp  ****************************
 *      _____   _____                   _                                          
 *     |_   _| |  __ \                 (_)                                         
 *       | |   | |  | |   ___  __   __  _    ___    ___        ___   _ __    _ __  
 *       | |   | |  | |  / _ \ \ \ / / | |  / __|  / _ \      / __| | '_ \  | '_ \ 
 *      _| |_  | |__| | |  __/  \ V /  | | | (__  |  __/  _  | (__  | |_) | | |_) |
 *     |_____| |_____/   \___|   \_/   |_|  \___|  \___| (_)  \___| | .__/  | .__/ 
 *                                                                  | |     | |    
 *                                                                  |_|     |_|    
 ******************/

#include "IDevice.hpp"
#include "Memory.hpp"


///////////////////////////
// Public Virtual Methods
///////////////////////////

Byte IDevice::OnRead(Word offset) 
{
    if (offset - base() < size())
		return memory(offset - base() );
	return 0xCC;
}

void IDevice::OnWrite(Word offset, Byte data) 
{
    if (offset - base() < size())
        memory(offset - base(), data);
}



/////////////////////////////
// Protected Helper Methods
/////////////////////////////



Word IDevice::base() 
{ 
    // std::lock_guard<std::mutex> guard(_mutex_base); 
    return _base; 
}



void IDevice::base(Word addr) 
{ 
    // std::lock_guard<std::mutex> guard(_mutex_base); 
    _base = addr; 
}



int IDevice::size() 
{ 
    // std::lock_guard<std::mutex> guard(_mutex_size); 
    // std::lock_guard<std::mutex> mem_guard(_mutex_memory); 
    return _size = _memory.size();
}



void IDevice::size(int size) 
{ 
    // std::lock_guard<std::mutex> guard(_mutex_size); 
    // std::lock_guard<std::mutex> mem_guard(_mutex_memory); 
    _size = size; _memory.resize(size); 
}



std::string IDevice::name()  
{ 
    // std::lock_guard<std::mutex> guard(_mutex_device_name); 
    return _device_name; 
}




void IDevice::name(std::string n) 
{ 
    // std::lock_guard<std::mutex> guard(_mutex_device_name); 
    _device_name = n; 
}




Byte IDevice::memory(Word ofs) 
{ 
    // std::lock_guard<std::mutex> guard(_mutex_memory);
    return _memory[ofs]; 
}




void IDevice::memory(Word ofs, Byte data) 
{ 
    // std::lock_guard<std::mutex> guard(_mutex_memory);
    _memory[ofs] = data; 
}


/////////////////////////////
// ROM Device Helper Method
/////////////////////////////



void ROM::write_to_rom(Word offset, Byte data) 
{
    if (offset - base() < size())
        memory(offset - base(), data);
}