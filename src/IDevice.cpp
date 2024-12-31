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

/// @brief Returns the base address of this IDevice object
/// @details This is the address offset that is used to translate
///          all read and write operations into the internal
///          memory vector.
/// @return The base address of the device
Word IDevice::base() 
{ 
    std::lock_guard<std::mutex> guard(_mutex_base); 
    return _base; 
}


/// @brief Sets the base address of this IDevice object
/// @details This is the address offset that is used to translate
///          all read and write operations into the internal
///          memory vector.
void IDevice::base(Word addr) 
{ 
    std::lock_guard<std::mutex> guard(_mutex_base); 
    _base = addr; 
}


/// @brief Retrieves the current size of the device's memory
/// @details This method locks the size and memory mutexes for thread safety
///          and updates the _size field to reflect the current size of the 
///          internal memory vector.
/// @return The current size of the device's memory
int IDevice::size() 
{ 
    std::lock_guard<std::mutex> guard(_mutex_size); 
    std::lock_guard<std::mutex> mem_guard(_mutex_memory); 
    return _size = _memory.size();
}


/// @brief Sets the size of the device's memory
/// @details This method locks the size and memory mutexes for thread safety
///          and updates the _size field to reflect the new size of the internal
///          memory vector. The internal memory vector is resized to accomodate
///          the new size.
/// @param[in] size The new size of the device's memory
void IDevice::size(int size) 
{ 
    std::lock_guard<std::mutex> guard(_mutex_size); 
    std::lock_guard<std::mutex> mem_guard(_mutex_memory); 
    _size = size; _memory.resize(size); 
}


/// @brief Retrieves the name of the device
/// @details This method locks the device name mutex for thread safety
///          and returns the name of the device as a string
/// @return The name of the device as a string
std::string IDevice::name()  
{ 
    std::lock_guard<std::mutex> guard(_mutex_device_name); 
    return _device_name; 
}


/// @brief Sets the name of the device
/// @details This method locks the device name mutex for thread safety
///          and sets the name of the device as a string
/// @param[in] n The new name of the device as a string
void IDevice::name(std::string n) 
{ 
    std::lock_guard<std::mutex> guard(_mutex_device_name); 
    _device_name = n; 
}


/// @brief Retrieves the value of the memory at the given offset
/// @details This method locks the memory mutex for thread safety
///          and returns the value of the memory at the given offset
/// @param[in] ofs The offset in the device's memory
/// @return The value at the given offset in the device's memory
Byte IDevice::memory(Word ofs) 
{ 
    std::lock_guard<std::mutex> guard(_mutex_memory);
    return _memory[ofs]; 
}


/// @brief Sets the value of the memory at the given offset
/// @details This method locks the memory mutex for thread safety
///          and sets the value of the memory at the given offset
/// @param[in] ofs The offset in the device's memory
/// @param[in] data The value to be written to the device's memory
void IDevice::memory(Word ofs, Byte data) 
{ 
    std::lock_guard<std::mutex> guard(_mutex_memory);
    _memory[ofs] = data; 
}


/////////////////////////////
// ROM Device Helper Method
/////////////////////////////

/// @brief Helper method to write a byte to the ROM
/// @details This method is used by the Memory class to write to the ROM
///          It locks the memory mutex for thread safety and
///          sets the value of the memory at the given offset
/// @param[in] offset The offset in the device's memory
/// @param[in] data The value to be written to the device's memory
void ROM::write_to_rom(Word offset, Byte data) 
{
    if (offset - base() < size())
        memory(offset - base(), data);
}