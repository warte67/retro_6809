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
    //      Page Select for 8K Memory Bank 1 (MMU_META_HANDLE)
    /////
    mapped_register.push_back({ "MMU_1_SELECT", nextAddr, 
        [this](Word) { return _mmu_1_select; }, 
        [this](Word, Byte data) { _mmu_1_select = data; },    
        { "(Byte) Page Select for 8K Memory Bank 1"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MMU_2_SELECT
    //      Page Select for 8K Memory Bank 2 (MMU_META_HANDLE)
    /////
    mapped_register.push_back({ "MMU_2_SELECT", nextAddr, 
        [this](Word) { return _mmu_2_select; }, 
        [this](Word, Byte data) { _mmu_2_select = data; },  
        { "(Byte) Page Select for 8K Memory Bank 2 (MMU_META_HANDLE)"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Word) MMU_BLOCKS_FREE
    //      Number of 32-Byte Blocks Available for Allocation
    /////
    mapped_register.push_back({ "MMU_BLOCKS_FREE", nextAddr, 
        [this](Word) { return (_mmu_blocks_free>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_blocks_free = (_mmu_blocks_free & 0x00FF) | (data << 8); }, 
        { "(Word) Number of 32-Byte Blocks Available for Allocation"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_blocks_free & 0xFF; }, 
        [this](Word, Byte data) { _mmu_blocks_free = (_mmu_blocks_free & 0xFF00) | data; }, 
    {""}}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Word) MMU_BLOCKS_ALLOCATED
    //      Number of 32-Byte Blocks Available for Allocation
    /////
    mapped_register.push_back({ "MMU_BLOCKS_ALLOCATED", nextAddr, 
        [this](Word) { return (_mmu_blocks_allocated>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_blocks_allocated = (_mmu_blocks_allocated & 0x00FF) | (data << 8); }, 
        { "(Word) Number of 32-Byte Blocks Currently Allocated"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_blocks_allocated & 0xFF; },  
        [this](Word, Byte data) { _mmu_blocks_allocated = (_mmu_blocks_allocated & 0xFF00) | data; }, 
    {""}}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Word) MMU_BLOCKS_FRAGGED
    //      Number of 32-Byte Blocks Available for Allocation
    /////
    mapped_register.push_back({ "MMU_BLOCKS_FRAGGED", nextAddr, 
        [this](Word) { return (_mmu_blocks_fragged>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_blocks_fragged = (_mmu_blocks_fragged & 0x00FF) | (data << 8); },   
        { "(Word) Number of 32-Byte Blocks Currently Fragmented"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_blocks_fragged & 0xFF; },  
        [this](Word, Byte data) { _mmu_blocks_fragged = (_mmu_blocks_fragged & 0xFF00) | data; }, 
    {""}}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Word)  MMU_ARG_1
    //      Argument 1 for MMU Command
    /////
    mapped_register.push_back({ "MMU_ARG_1", nextAddr, 
        [this](Word) { return (_mmu_arg_1>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_1 = (_mmu_arg_1 & 0x00FF) | (data << 8); },   
        { "(Word)  Argument 1 for MMU Command"} });
    mapped_register.push_back( { "", nextAddr+1, 
        [this](Word) { return _mmu_arg_1 & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_1 = (_mmu_arg_1 & 0xFF00) | data; }, 
    {""}});

    mapped_register.push_back({ "MMU_ARG_1_MSB", nextAddr, 
        [this](Word) { return (_mmu_arg_1>>8) & 0xFF; },  
        [this](Word, Byte data) { _mmu_arg_1 = (_mmu_arg_1 & 0x00FF) | (data << 8); },   
        { "(Byte)  Argument 1 Most Significant Byte for MMU Command"} }); 
        nextAddr++;

    mapped_register.push_back({ "MMU_ARG_1_LSB", nextAddr, 
        [this](Word) { return _mmu_arg_1 & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_1 = (_mmu_arg_1 & 0xFF00) | data; }, 
        { "(Byte)  Argument 1 Least Significant Byte for MMU Command"} }); 
        nextAddr++;


    ////////////////////////////////////////////////
    // (Word)  MMU_ARG_2
    //      Argument 2 for MMU Command
    /////
    mapped_register.push_back({ "MMU_ARG_2", nextAddr, 
        [this](Word) { return (_mmu_arg_2>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_2 = (_mmu_arg_2 & 0x00FF) | (data << 8); },   
        { "(Word)  Argument 2 for MMU Command"} });
    mapped_register.push_back( { "", nextAddr+1, 
        [this](Word) { return _mmu_arg_2 & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_2 = (_mmu_arg_2 & 0xFF00) | data; }, 
        {""}});
    mapped_register.push_back({ "MMU_ARG_2_MSB", nextAddr, 
        [this](Word) { return (_mmu_arg_2>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_2 = (_mmu_arg_2 & 0x00FF) | (data << 8); },   
        { "(Byte)  Argument 2 Most Significant Byte for MMU Command"} }); 
        nextAddr++;
    mapped_register.push_back({ "MMU_ARG_2_LSB", nextAddr, 
        [this](Word) { return _mmu_arg_2 & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_2 = (_mmu_arg_2 & 0xFF00) | data; }, 
        { "(Byte)  Argument 2 Least Significant Byte for MMU Command"} }); 
        nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MMU_COMMAND
    //      Memory Management Unit Command
    /////
    mapped_register.push_back({ "MMU_COMMAND", nextAddr, 
        [this](Word) { return _mmu_command; },  
        [this](Word, Byte data) { 
            // dispatch the MMU Command
            // ...
            _mmu_command = data; 
        },   
        { "(Byte) Memory Management Unit Command:",
            "   $00 = NO OPERATION / ERROR",
            "   $01 = ALLOCATE PAGE (Always 8192 Bytes)",
            "   $02 = DEALLOCATE PAGE (Always 8192 Bytes)",
            "   $03 = ALLOCATE CHAIN (MMU_ARG_1_LSB = # Blocks)",
            "            MMU_ARG_1_LSB: 0=NONE, 1-255= (32-8160 Bytes)",
            "   $04 = DEALLOCATE CHAIN (From MMU_META_ROOT)",
            "   $05 = Load Root Node",
            "   $06 = Load Next Node",
            "   $07 = Load Prev Node",
            "   $08 = Load Last Node",
            "   $09 = Remove Current Node (and Adjust Links)",
            "   $0A = Insert Node Before (and activate)",
            "   $0B = Insert Node After (and activate)",
            "   $0C = Push Back (and activate)",
            "   $0D = Push Front (and activate)",
            "   $0E = Pop Back (and activate)",
            "   $0F = Pop Front (adjust the handle and activate next)",
            "   $10 = Lock Node",
            "   $11 = Unlock Node",
            "   $12 = Defragment / Collect Garbage",
            "   ...",
            "   $FF = RESET; Clear All Allocation Chains",""
        } 
    });
    nextAddr++;
    // ADD COMMAND ENUMERATION:
    // ...


    ////////////////////////////////////////////////
    // (Byte) MMU_ERROR
    //      Memory Management Unit Error Code:
    /////
    mapped_register.push_back({ "MMU_ERROR", nextAddr, 
        [this](Word) { return _mmu_error; },  
        [this](Word, Byte data) { _mmu_error = data; },  
        { "(Byte) Memory Management Unit Error Code:",
            "   $00 = NO ERROR",
            "   $01 = OUT OF MEMORY",
            "   $02 = MEMORY ALLOCATION ERROR",
            "   $03 = MEMORY DEALLOCATION ERROR",
            "   $04 = MEMORY MAPPING ERROR",
            "   $05 = MEMORY UNMAPPING ERROR",
            "   $06 = INVALID COMMAND",
            "   $07 = INVALID ARGUMENT",
            "   $08 = INVALID HANDLE",
            "   $09 = INVALID NODE",
            "   $0A = INVALID PAGE",
            "   $0B = INVALID BANK",
            "   $0C = INVALID ADDRESS",
            "   $0D = INVALID OFFSET",
            "   $0E = INVALID LENGTH",
            "   $0F = INVALID INDEX",
            "   $FF = UNKNOWN ERROR",""
        }
    });
    nextAddr++;
    // ADD ERROR CODE ENUMERATION:
    // ...


    ////////////////////////////////////////////////
    // (Word) MMU_META_HANDLE
    //      Handle for the current allocation chain
    /////
    mapped_register.push_back({ "MMU_META_HANDLE", nextAddr, 
        [this](Word) { return (_mmu_handle>>8) & 0xFF; },  
        [this](Word, Byte data) { _mmu_handle = (_mmu_handle & 0x00FF) | (data << 8); }, 
        { "(Word) Handle for the current allocation chain"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_handle & 0xFF; },  
        [this](Word, Byte data) { _mmu_handle = (_mmu_handle & 0xFF00) | data; }, 
        {""}});
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MMU_META_STATUS
    //      Status flags for the current allocation node
    /////
    mapped_register.push_back({ "MMU_META_STATUS", nextAddr, 
        [this](Word) { return _mmu_status; },  
        [this](Word, Byte data) { _mmu_status = data; },  
        { "(Byte) Status Flags:",
            "   bit  0 = Is Allocated:     0 = Free,   1 = Allocated",
            "   bit  1 = 8k Paged Memory:  0 = No,     1 = Yes",
            "   bit  2 = Storage Type:     0 = RAM,    1 = ROM",
            "   bit  3 = Is Fragmented?    0 = No,     1 = Yes",
            "   bit  4 = Is Locked?        0 = No,     1 = Yes",
            "   bit  5 = (reserved)",
            "   bit  6 = (reserved)",
            "   bit  7 = Error:            0 = No,     1 = Yes",""
        } 
    });
    nextAddr++;
    // ADD STATUS FLAGS ENUMERATION:
    // ...


    ////////////////////////////////////////////////
    // (32-Bytes) MMU_META_DATA
    //      32-Bytes of data for the current allocation
    /////
    for (int i=0; i<32; i++) 
    {
        if (i==0)
        {
            mapped_register.push_back({ "MMU_META_DATA", nextAddr, 
            [this,i](Word) { return _data_pool[_mmu_raw_index].data[i]; }, 
            [this,i](Word, Byte data) { _data_pool[_mmu_raw_index].data[i] = data; },  
            { "(32-Bytes) Data Window for the Current Allocation"} });
        }
        else
        {
            mapped_register.push_back( { "", nextAddr, 
            [this,i](Word) { return _data_pool[_mmu_raw_index].data[i]; },   
            [this,i](Word, Byte data) { _data_pool[_mmu_raw_index].data[i] = data; },  
            {""}}); nextAddr++;
        }
    }


    ////////////////////////////////////////////////
    // (Word) MMU_META_ROOT
    //      Handle for the current allocation chain
    /////
    mapped_register.push_back({ "MMU_META_ROOT", nextAddr, 
        [this](Word) { return (_mmu_meta_root>>8) & 0xFF; },  
        [this](Word, Byte data) { _mmu_meta_root = (_mmu_meta_root & 0x00FF) | (data << 8); }, 
        { "(Word) Root node of the current allocation       (Read Only)"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_meta_root & 0xFF; },  
        [this](Word, Byte data) { _mmu_meta_root = (_mmu_meta_root & 0xFF00) | data; },
    {""}}); nextAddr++;


    ////////////////////////////////////////////////
    // (Word) MMU_META_PREV
    //      Handle for the current allocation chain
    /////
    mapped_register.push_back({ "MMU_META_PREV", nextAddr, 
        [this](Word) { return (_mmu_meta_prev>>8) & 0xFF; },  
        [this](Word, Byte data) { _mmu_meta_prev = (_mmu_meta_prev & 0x00FF) | (data << 8); },   
        { "(Word) Previous node of the current allocation   (Read Only)"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_meta_prev & 0xFF; },   
        [this](Word, Byte data) { _mmu_meta_prev = (_mmu_meta_prev & 0xFF00) | data; },
    {""}}); nextAddr++;


    ////////////////////////////////////////////////
    // (Word) MMU_META_NEXT
    //      Handle for the current allocation chain
    /////
    mapped_register.push_back({ "MMU_META_NEXT", nextAddr, 
        [this](Word) { return (_mmu_meta_next>>8) & 0xFF; },
        [this](Word, Byte data) { _mmu_meta_next = (_mmu_meta_next & 0x00FF) | (data << 8); }, 
        { "(Word) Next node of the current allocation       (Read Only)"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_meta_next & 0xFF; }, 
        [this](Word, Byte data) { _mmu_meta_next = (_mmu_meta_next & 0xFF00) | data; },
    {""}}); nextAddr++;


    ////////////////////////////////////////////////
    // (Word) MMU_RAW_INDEX
    //      Handle for the current allocation chain
    /////
    mapped_register.push_back({ "MMU_RAW_INDEX", nextAddr, 
        [this](Word) { return (_mmu_raw_index>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_raw_index = (_mmu_raw_index & 0x00FF) | (data << 8); },
        { "(Word) Raw Index of the current memory node  (For Debugging)",""} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_raw_index & 0xFF; },
        [this](Word, Byte data) { _mmu_raw_index = (_mmu_raw_index & 0xFF00) | data; },
    {""}}); nextAddr++;


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
