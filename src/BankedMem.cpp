/*** BankedMem.cpp *******************************************
 *      ____              _            _ __  __                                     
 *     |  _ \            | |          | |  \/  |                                    
 *     | |_) | __ _ _ __ | | _____  __| | \  / | ___ _ __ ___       ___ _ __  _ __  
 *     |  _ < / _` | '_ \| |/ / _ \/ _` | |\/| |/ _ \ '_ ` _ \     / __| '_ \| '_ \ 
 *     | |_) | (_| | | | |   <  __/ (_| | |  | |  __/ | | | | |_  | (__| |_) | |_) |
 *     |____/ \__,_|_| |_|_|\_\___|\__,_|_|  |_|\___|_| |_| |_(_)  \___| .__/| .__/ 
 *                                                                     | |   | |    
 *                                                                     |_|   |_| 
 * 
 * This device manages the pair of 8K memory banks defined in the 
 * BANKED_MEMORY_REGION of the memory map, where each block can be 
 * configured as RAM, ROM, or ersistent RAM, and is indexed by an 
 * 8-bit page address, enabling a total of 2MB of banked memory.
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#include "BankedMem.hpp"



/***************************
* Constructor / Destructor *
***************************/

BankedMem::BankedMem() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "BankedMem Device Created" << clr::RETURN;
    _device_name = "BANKED_MEMORY_DEVICE"; 
} // END: BankedMem()

BankedMem::~BankedMem() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "BankedMem Device Destroyed" << clr::RETURN; 
} // END: ~BankedMem()



/******************
* Virtual Methods *
******************/



int  BankedMem::OnAttach(int nextAddr)
{
    SetBaseAddress(nextAddr);
    Word old_address=nextAddr;
    this->heading = "BankedMem Device Hardware Registers";
    
    std::cout << clr::indent() << clr::LT_BLUE << "BankedMem::OnAttach() Entry" << clr::RETURN;    

    ////////////////////////////////////////////////
    // (Byte) BANK_1_SELECT
    //      Page Select for 8K Memory Bank 1 (0-255)
    /////
    mapped_register.push_back({ "BANK_1_SELECT", nextAddr, 
        nullptr, 
        nullptr,  
        { "Page Select for 8K Memory Bank 1 (0-255)"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) BANK_2_SELECT
    //      Page Select for 8K Memory Bank 2 (0-255)
    /////
    mapped_register.push_back({ "BANK_2_SELECT", nextAddr, 
        nullptr, 
        nullptr,  
        { "Page Select for 8K Memory Bank 2 (0-255)"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) BANK_1_TYPE
    //      Memory Bank 1 Storage Type
    /////
    mapped_register.push_back({ "BANK_1_TYPE", nextAddr, 
        nullptr, 
        nullptr,  
        { "Memory Bank 1 Storage Type"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) BANK_2_TYPE
    //      Memory Bank 2 Storage Type
    /////
    mapped_register.push_back({ "BANK_2_TYPE", nextAddr, 
        nullptr, 
        nullptr,  
        { "Memory Bank 2 Storage Type"} });
    nextAddr++;    


    ////////////////////////////////////////////////
    // BANK_TYPE enumeration
    //      BANK_TYPE 0 = RAM
    //      BANK_TYPE 1 = PERSISTENT RAM
    //      BANK_TYPE 2 = ROM
    /////
    Word _bank_type = 0;
    mapped_register.push_back({ "BANK_TYPE_RAM", _bank_type, 
        nullptr, nullptr, { "    Random Access Memory (RAM)"} });
    _bank_type++; // nextAddr++;    
    mapped_register.push_back({ "BANK_TYPE_ROM", _bank_type, 
        nullptr, nullptr, { "    Read Only Memory (ROM)"} });
    _bank_type++; // nextAddr++;    

    ////////////////////////////////////////////////
    // (Word) BANK_FAST_INDEX
    //      16-bit Index into Array of 32-byte Fast Memory Elements
    /////
    mapped_register.push_back({ "BANK_FAST_INDEX", nextAddr, 
        nullptr, 
        nullptr,  
        { "(Word) Index into Array (32-byte Fast Memory)"} });
    nextAddr++;    
    mapped_register.push_back( { "", nextAddr, nullptr, nullptr, {""}}); nextAddr+=1;    


    ////////////////////////////////////////////////
    // (Word) BANK_FAST_WINDOW
    //      32-byte Memory Window For Fast Memory Access
    /////
    mapped_register.push_back({ "BANK_FAST_WINDOW", nextAddr, 
        nullptr, 
        nullptr,  
        { "(32-Bytes) 32-byte Memory Window For Fast Memory Access"} });
    // nextAddr++;    
    for (int index = 0; index < 32; index++) {
        mapped_register.push_back( { "", nextAddr, nullptr, nullptr, {""}}); nextAddr+=1;    
    }




    ////////////////////////////////////////////////
    // (Constant) BANK_END
    //      End of Banked Memory Register Space
    /////
    nextAddr--;
    mapped_register.push_back({ "BANK_END", nextAddr, 
        nullptr, nullptr,  { "End of Banked Memory Register Space"} });
    nextAddr++;
    
    
    ////////////////////////////////////////////////
    // (Constant) BANK_TOP
    //      Top of Banked Register Space
    //      (start of the next device)
    /////
    mapped_register.push_back({ "BANK_TOP", nextAddr, 
    nullptr, nullptr,  { "Top of Banked Memory Register Space", "---"}});
    std::cout << clr::indent() << clr::LT_BLUE << "BankedMem::OnAttach() Exit" << clr::RETURN;


    _size = nextAddr - old_address;
    return _size;
}


void BankedMem::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "BankedMem::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "BankedMem::OnInit() Exit" << clr::RETURN;
}


void BankedMem::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "BankedMem::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "BankedMem::OnQuit() Exit" << clr::RETURN;
}


// END: BankedMem.cpp
