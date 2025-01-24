/*** MMU.cpp *******************************************
 *      __  __ __  __ _    _                        
 *     |  \/  |  \/  | |  | |                       
 *     | \  / | \  / | |  | |       ___ _ __  _ __  
 *     | |\/| | |\/| | |  | |      / __| '_ \| '_ \ 
 *     | |  | | |  | | |__| |  _  | (__| |_) | |_) |
 *     |_|  |_|_|  |_|\____/  (_)  \___| .__/| .__/ 
 *                                     | |   | |    
 *                                     |_|   |_|  
 * 
 * Memory Management Unit (MMU) Device
 *
 * The Memory Management Unit (MMU) controls memory access and mapping 
 * within the system, managing a 2MB PSRAM memory pool. It organizes 
 * memory nodes, defining regions such as RAM and ROM, and supports 
 * 32-byte windowed direct memory access for efficient memory operations. 
 * The MMU handles paged memory with 8KB pages, enabling flexible memory 
 * allocation and protection. It ensures that the CPU and other devices 
 * access valid memory regions with appropriate read/write permissions, 
 * optimizing system performance. After "formatting" with METADATA there 
 * will be 1'677'696 bytes of free space for dynamic memory allocation.
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#include "Bus.hpp"
#include "MMU.hpp"


/***************************
* Constructor / Destructor *
***************************/

MMU::MMU() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Memory Management Unit Device Created" << clr::RETURN;

    static MMU* bWasInitialized = this;    
    if (bWasInitialized != this) { 
        Bus::Error("MMU already initialized!", __FILE__, __LINE__);
    }

    _device_name = "MMU_DEVICE"; 
} // END: MMU()

MMU::~MMU() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Memory Management Unit Device Destroyed" << clr::RETURN; 
} // END: ~MMU()



/******************
* Virtual Methods *
******************/



int  MMU::OnAttach(int nextAddr)
{
    SetBaseAddress(nextAddr);
    Word old_address=nextAddr;
    this->heading = "Memory Management Unit Hardware Registers";
    
    std::cout << clr::indent() << clr::LT_BLUE << "MMU::OnAttach() Entry" << clr::RETURN;    

    ////////////////////////////////////////////////
    // (Byte) MMU_1_SELECT
    //      Page Select for 8K Memory Bank 1 (0-255)
    /////
    mapped_register.push_back({ "MMU_1_SELECT", nextAddr, 
        nullptr, 
        nullptr,  
        { "Page Select for 8K Memory Bank 1 (0-255)"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MMU_1_FLAGS
    //      Memory Bank 1 Condition Flags
    /////
    mapped_register.push_back({ "MMU_1_FLAGS", nextAddr, 
        nullptr, 
        nullptr,  
        { "Memory Bank 1 Condition Flags"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MMU_2_SELECT
    //      Page Select for 8K Memory Bank 2 (0-255)
    /////
    mapped_register.push_back({ "MMU_2_SELECT", nextAddr, 
        nullptr, 
        nullptr,  
        { "Page Select for 8K Memory Bank 2 (0-255)"} });
    nextAddr++;


     ////////////////////////////////////////////////
    // (Byte) MMU_2_FLAGS
    //      Memory Bank 2 Condition Flags
    /////
    mapped_register.push_back({ "MMU_2_FLAGS", nextAddr, 
        nullptr, 
        nullptr,  
        { "Memory Bank 2 Condition Flags"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // MMU_TYPE enumeration
    //      MMU_TYPE 0 = RAM
    //      MMU_TYPE 1 = PERSISTENT RAM
    //      MMU_TYPE 2 = ROM
    /////
    Word _MMU_type = 0;
    mapped_register.push_back({ "MMU_TYPE_RAM", _MMU_type, 
        nullptr, nullptr, { "    Random Access Memory (RAM)"} });
    _MMU_type++; // nextAddr++;    
    mapped_register.push_back({ "MMU_TYPE_ROM", _MMU_type, 
        nullptr, nullptr, { "    Read Only Memory (ROM)"} });
    _MMU_type++; // nextAddr++;    


    ////////////////////////////////////////////////
    // (Word) MMU_FAST_INDEX
    //      16-bit Index into Array of 32-byte Fast Memory Elements
    /////
    mapped_register.push_back({ "MMU_FAST_INDEX", nextAddr, 
        nullptr, 
        nullptr,  
        { "(Word) Index into Array (32-byte Fast Memory)"} });
    nextAddr++;    
    mapped_register.push_back( { "", nextAddr, nullptr, nullptr, {""}}); nextAddr+=1;    


    ////////////////////////////////////////////////
    // (Word) MMU_FAST_WINDOW
    //      32-byte Memory Window For Fast Memory Access
    /////
    mapped_register.push_back({ "MMU_FAST_WINDOW", nextAddr, 
        nullptr, 
        nullptr,  
        { "(32-Bytes) 32-byte Memory Window For Fast Memory Access"} });
    // nextAddr++;    
    for (int index = 0; index < 32; index++) {
        mapped_register.push_back( { "", nextAddr, nullptr, nullptr, {""}}); nextAddr+=1;    
    }




    ////////////////////////////////////////////////
    // (Constant) MMU_END
    //      End of Banked Memory Register Space
    /////
    nextAddr--;
    mapped_register.push_back({ "MMU_END", nextAddr, 
        nullptr, nullptr,  { "End of Banked Memory Register Space"} });
    nextAddr++;
    
    
    ////////////////////////////////////////////////
    // (Constant) MMU_TOP
    //      Top of Banked Register Space
    //      (start of the next device)
    /////
    mapped_register.push_back({ "MMU_TOP", nextAddr, 
    nullptr, nullptr,  { "Top of Banked Memory Register Space", "---"}});
    std::cout << clr::indent() << clr::LT_BLUE << "MMU::OnAttach() Exit" << clr::RETURN;


    _size = nextAddr - old_address;
    return _size;
}


void MMU::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "MMU::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "MMU::OnInit() Exit" << clr::RETURN;
}


void MMU::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "MMU::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "MMU::OnQuit() Exit" << clr::RETURN;
}


// END: MMU.cpp
