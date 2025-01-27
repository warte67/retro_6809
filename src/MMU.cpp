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
#include "UnitTest.hpp"
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
    _mmu = this;

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
    // (Word) MMU_1_SELECT
    //      Page Select for 8K Memory Bank 1 (MMU_HANDLE)
    /////
    mapped_register.push_back({ "MMU_PAGE_1_SELECT", nextAddr, 
        [this](Word) { return (_mmu_1_select>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_1_select = (_mmu_1_select & 0x00FF) | (data << 8); },  
        { "(Word) Page Select for 8K Memory Bank 1"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_1_select & 0xFF; }, 
        [this](Word, Byte data) { _mmu_1_select = (_mmu_1_select & 0xFF00) | data; }, 
    {""}}); nextAddr+=1;       


    ////////////////////////////////////////////////
    // (Word) MMU_2_SELECT
    //      Page Select for 8K Memory Bank 2 (MMU_HANDLE)
    /////
    mapped_register.push_back({ "MMU_PAGE_2_SELECT", nextAddr, 
        [this](Word) { return (_mmu_2_select>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_2_select = (_mmu_2_select & 0x00FF) | (data << 8); },  
        { "(Word) Page Select for 8K Memory Bank 2"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_2_select & 0xFF; }, 
        [this](Word, Byte data) { _mmu_2_select = (_mmu_2_select & 0xFF00) | data; }, 
    {""}}); nextAddr+=1;     


    ////////////////////////////////////////////////
    // (Word) MMU_BLOCKS_FREE
    //      Number of 32-Byte Blocks Available for Allocation  (Read Only)
    /////
    mapped_register.push_back({ "MMU_BLOCKS_FREE", nextAddr, 
        [this](Word) { return (_mmu_blocks_free>>8) & 0xFF; }, 
        nullptr,    //[this](Word, Byte data) { _mmu_blocks_free = (_mmu_blocks_free & 0x00FF) | (data << 8); }, 
        { "(Word) Number of 32-Byte Blocks Available for Allocation (Read Only)"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_blocks_free & 0xFF; }, 
        nullptr,    // [this](Word, Byte data) { _mmu_blocks_free = (_mmu_blocks_free & 0xFF00) | data; }, 
    {""}}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Word) MMU_BLOCKS_ALLOCATED
    //      Number of 32-Byte Blocks Available for Allocation  (Read Only)
    /////
    mapped_register.push_back({ "MMU_BLOCKS_ALLOCATED", nextAddr, 
        [this](Word) { return (_mmu_blocks_allocated>>8) & 0xFF; }, 
        nullptr, // [this](Word, Byte data) { _mmu_blocks_allocated = (_mmu_blocks_allocated & 0x00FF) | (data << 8); }, 
        { "(Word) Number of 32-Byte Blocks Currently Allocated  (Read Only)"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_blocks_allocated & 0xFF; },  
        nullptr, // [this](Word, Byte data) { _mmu_blocks_allocated = (_mmu_blocks_allocated & 0xFF00) | data; }, 
    {""}}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Word) MMU_BLOCKS_FRAGGED
    //      Number of 32-Byte Blocks Available for Allocation  (Read Only)
    /////
    mapped_register.push_back({ "MMU_BLOCKS_FRAGGED", nextAddr, 
        [this](Word) { return (_mmu_blocks_fragged>>8) & 0xFF; }, 
        nullptr, // [this](Word, Byte data) { _mmu_blocks_fragged = (_mmu_blocks_fragged & 0x00FF) | (data << 8); },   
        { "(Word) Number of 32-Byte Blocks Currently Fragmented  (Read Only)"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_blocks_fragged & 0xFF; },  
        nullptr, // [this](Word, Byte data) { _mmu_blocks_fragged = (_mmu_blocks_fragged & 0xFF00) | data; }, 
    {""}}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Word)  MMU_ARG_1
    //      Argument 1 for MMU Command
    /////
    mapped_register.push_back({ "MMU_ARG_1", nextAddr, 
        [this](Word) { return (_mmu_arg_1>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_1 = (_mmu_arg_1 & 0x00FF) | (data << 8); },   
        { "(Word) Argument 1 for MMU Command"} });
    mapped_register.push_back( { "", nextAddr+1, 
        [this](Word) { return _mmu_arg_1 & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_1 = (_mmu_arg_1 & 0xFF00) | data; }, 
    {""}});

    mapped_register.push_back({ "MMU_ARG_1_MSB", nextAddr, 
        [this](Word) { return (_mmu_arg_1>>8) & 0xFF; },  
        [this](Word, Byte data) { _mmu_arg_1 = (_mmu_arg_1 & 0x00FF) | (data << 8); },   
        { "(Byte) Argument 1 Most Significant Byte for MMU Command"} }); 
        nextAddr++;

    mapped_register.push_back({ "MMU_ARG_1_LSB", nextAddr, 
        [this](Word) { return _mmu_arg_1 & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_1 = (_mmu_arg_1 & 0xFF00) | data; }, 
        { "(Byte) Argument 1 Least Significant Byte for MMU Command"} }); 
        nextAddr++;


    ////////////////////////////////////////////////
    // (Word)  MMU_ARG_2
    //      Argument 2 for MMU Command
    /////
    mapped_register.push_back({ "MMU_ARG_2", nextAddr, 
        [this](Word) { return (_mmu_arg_2>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_2 = (_mmu_arg_2 & 0x00FF) | (data << 8); },   
        { "(Word) Argument 2 for MMU Command"} });
    mapped_register.push_back( { "", nextAddr+1, 
        [this](Word) { return _mmu_arg_2 & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_2 = (_mmu_arg_2 & 0xFF00) | data; }, 
        {""}});
    mapped_register.push_back({ "MMU_ARG_2_MSB", nextAddr, 
        [this](Word) { return (_mmu_arg_2>>8) & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_2 = (_mmu_arg_2 & 0x00FF) | (data << 8); },   
        { "(Byte) Argument 2 Most Significant Byte for MMU Command"} }); 
        nextAddr++;
    mapped_register.push_back({ "MMU_ARG_2_LSB", nextAddr, 
        [this](Word) { return _mmu_arg_2 & 0xFF; }, 
        [this](Word, Byte data) { _mmu_arg_2 = (_mmu_arg_2 & 0xFF00) | data; }, 
        { "(Byte) Argument 2 Least Significant Byte for MMU Command",""} }); 
        nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MMU_COMMAND
    //      Memory Management Unit Command
    /////
    mapped_register.push_back({ "MMU_COMMAND", nextAddr, 
        [this](Word) { return _mmu_command; },  
        [this](Word, Byte data) { 
            // dispatch the MMU Command
            _mmu_command = data; 
            do_command(data);
        },   
        { "(Byte) Memory Management Unit Command:" }}); nextAddr++;
    // ADD COMMAND ENUMERATION:
    Byte cmd = 0;
    for (const auto& command : _mmu_command_list) 
    {
        const auto& key = command.key;
        const auto& description = command.description;
        std::vector<std::string> comment = { "   $" + clr::hex(cmd, 2) + " = " + description };
        // Check for the special case of "MMU_CMD_SIZE" and add an extra empty string
        if (key == "MMU_CMD_SIZE")
        {
            comment.push_back("");  // Adding an extra empty string to add a blank line
        }
        mapped_register.push_back({
            key,                    // Command name (e.g., "MMU_CMD_NOP")
            cmd++,                  // Increment the cmd value for each entry
            nullptr,                // Placeholder for the function pointer
            nullptr,                // Placeholder for another value
            comment                 // Formatted description
        });
    }


    ////////////////////////////////////////////////
    // (Byte) MMU_ERROR             
    //      Memory Management Unit Error Code:    (Read Only)
    /////
    mapped_register.push_back({ "MMU_ERROR", nextAddr, 
        [this](Word) { return _mmu_error; },  
        nullptr,  
        { "(Byte) Memory Management Unit Error Code:     (Read Only)" }}); nextAddr++;
    // ADD ERROR CODE ENUMERATION:
    Byte err = 0;
    for (const auto& error : _mmu_error_list)
    {
        std::vector<std::string> comments = { "   $" + clr::hex(err, 2) + " = " + error.second };
        if (error.first == "MMU_ERR_SIZE")
        {
            comments.push_back("");  // Adding an extra empty string to add a blank line
        }
        mapped_register.push_back({
            error.first,            // The constant name (e.g., "MMU_ERR_NONE")
            err++,                  // Increment the error code value for each entry
            nullptr,                // Placeholder for function pointer (not used here)
            nullptr,                // Placeholder for another value (not used here)
            { comments }            // Formatted description (error message)
        });
    }


    ////////////////////////////////////////////////
    // (Word) MMU_META_HANDLE
    //      Handle for the current allocation chain
    /////
    mapped_register.push_back({ "MMU_META_HANDLE", nextAddr, 
        [this](Word) { return (_mmu_handle>>8) & 0xFF; },  
        [this](Word, Byte data) 
        { 
            _mmu_handle = (_mmu_handle & 0x00FF) | (data << 8); 
            if (_mmu_handle >= MMU_MEMORY_SIZE) _mmu_handle = MMU_MEMORY_SIZE - 1;
            _mmu_raw_index = _mmu_handle; 
        }, 
        { "(Word) Handle for the current allocation chain",""} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_handle & 0xFF; },  
        [this](Word, Byte data) {
            _mmu_handle = (_mmu_handle & 0xFF00) | data; 
            if (_mmu_handle >= MMU_MEMORY_SIZE) _mmu_handle = MMU_MEMORY_SIZE - 1;
            _mmu_raw_index = _mmu_handle; 
        }, 
        {""}});
    nextAddr++;


    // ////////////////////////////////////////////////
    // // (Byte)  MMU_PAGE_INDEX
    // //      Page Index: 0xFF if not part of an 8K page
    // /////
    // mapped_register.push_back({ "MMU_PAGE_INDEX", nextAddr, 
    //     [this](Word) { return _metadata_pool[_mmu_raw_index].page_index; }, 
    //     [this](Word, Byte data) { _metadata_pool[_mmu_raw_index].page_index = data; },   
    //     { "(Byte) Page Index: 0xFF if not part of an 8K page"} });
    // nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MMU_META_STATUS
    //      Status flags for the current allocation node
    /////
    mapped_register.push_back({ "MMU_META_STATUS", nextAddr, 
        [this](Word) { return _metadata_pool[_mmu_raw_index].status; },  
        [this](Word, Byte data) { _metadata_pool[_mmu_raw_index].status = data; },
        // [this](Word, Byte data) { _metadata_pool[_mmu_raw_index].status = data & 0b1111'1110; },   // do not allow the user to change the allocated bit       
        { "(Byte) Status Flags:" }}); nextAddr++;
    // ADD STATUS FLAGS ENUMERATION:
    Byte stat = 0;
    mapped_register.push_back({ "MMU_STFLG_ALLOC"   , (1<<stat++),nullptr,nullptr, { "   0000'0001: Is Allocated: 0 = Free, 1 = Allocated"}}); 
    mapped_register.push_back({ "MMU_STFLG_PAGED"   , (1<<stat++),nullptr,nullptr, { "   0000'0010: Paged Memory: 0 = No,   1 = Yes"}});
    mapped_register.push_back({ "MMU_STFLG_READONLY", (1<<stat++),nullptr,nullptr, { "   0000'0100: Memory Type:  0 = RAM,  1 = ROM"}});
    mapped_register.push_back({ "MMU_STFLG_FRAGD"   , (1<<stat++),nullptr,nullptr, { "   0000'1000: Fragmented:   0 = No,   1 = Yes"}});
    mapped_register.push_back({ "MMU_STFLG_LOCKED"  , (1<<stat++),nullptr,nullptr, { "   0001'0000: Locked:       0 = No,   1 = Yes"}});
    mapped_register.push_back({ "MMU_STFLG_RES_1"   , (1<<stat++),nullptr,nullptr, { "   0010'0000:   (reserved)"}});
    mapped_register.push_back({ "MMU_STFLG_RES_2"   , (1<<stat++),nullptr,nullptr, { "   0100'0000:   (reserved)"}});
    mapped_register.push_back({ "MMU_STFLG_ERROR"   , (1<<stat++),nullptr,nullptr, { "   1000'0000: Error:        0 = No,   1 = Yes",""}});
    

    ////////////////////////////////////////////////
    // (32-Bytes) MMU_META_DATA
    //      32-Bytes of data for the current allocation
    /////
    for (int i=0; i<32; i++) 
    {
        if (i==0)
        {
            mapped_register.push_back({ "MMU_META_DATA", nextAddr, 
            [this,i](Word) { return _metadata_pool[_mmu_raw_index].data[i]; }, 
            [this,i](Word, Byte data) 
            { 
                // Read Only if the root node or the current node is locked or Read Only
                Word root_node = _metadata_pool[_mmu_raw_index].root_node;
                if (!(_metadata_pool[root_node].status & 0x04) &&
                    !(_metadata_pool[_mmu_raw_index].status & 0x04) &&
                    !(_metadata_pool[root_node].status & 0x10) &&
                    !(_metadata_pool[_mmu_raw_index].status & 0x10))
                {
                    _metadata_pool[_mmu_raw_index].data[i] = data; 
                }
            },{ "(32-Bytes) Data Window for the Current Allocation"} }); nextAddr++;
        }
        else
        {
            mapped_register.push_back( { "", nextAddr, 
            [this,i](Word) { return _metadata_pool[_mmu_raw_index].data[i]; },   
            [this,i](Word, Byte data) { 
                // Read Only if the root node or the current node is locked or Read Only
                Word root_node = _metadata_pool[_mmu_raw_index].root_node;
                if (!(_metadata_pool[root_node].status & 0x04) &&
                    !(_metadata_pool[_mmu_raw_index].status & 0x04) &&
                    !(_metadata_pool[root_node].status & 0x10) &&
                    !(_metadata_pool[_mmu_raw_index].status & 0x10))
                {
                    _metadata_pool[_mmu_raw_index].data[i] = data; 
                }            
            },{""}}); nextAddr++;
        }
    }


    ////////////////////////////////////////////////
    // (Word) MMU_META_ROOT
    //      Handle for the current allocation chain
    /////
    mapped_register.push_back({ "MMU_META_ROOT", nextAddr, 
        [this](Word) { return (_metadata_pool[_mmu_raw_index].root_node>>8) & 0xFF; },  
        [this](Word, Byte data) { _metadata_pool[_mmu_raw_index].root_node = (_metadata_pool[_mmu_raw_index].root_node & 0x00FF) | (data << 8); }, 
        { "(Word) Root node of the current allocation       (Read Only)"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _metadata_pool[_mmu_raw_index].root_node & 0xFF; },  
        [this](Word, Byte data) { _metadata_pool[_mmu_raw_index].root_node = (_metadata_pool[_mmu_raw_index].root_node & 0xFF00) | data; },
    {""}}); nextAddr++;


    ////////////////////////////////////////////////
    // (Word) MMU_META_PREV
    //      Handle for the current allocation chain
    /////
    mapped_register.push_back({ "MMU_META_PREV", nextAddr, 
        [this](Word) { return (_metadata_pool[_mmu_raw_index].prev_node>>8) & 0xFF; },  
        [this](Word, Byte data) { _metadata_pool[_mmu_raw_index].prev_node = (_metadata_pool[_mmu_raw_index].prev_node & 0x00FF) | (data << 8); },   
        { "(Word) Previous node of the current allocation   (Read Only)"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _metadata_pool[_mmu_raw_index].prev_node & 0xFF; },   
        [this](Word, Byte data) { _metadata_pool[_mmu_raw_index].prev_node = (_metadata_pool[_mmu_raw_index].prev_node & 0xFF00) | data; },
    {""}}); nextAddr++;


    ////////////////////////////////////////////////
    // (Word) MMU_META_NEXT
    //      Handle for the current allocation chain
    /////
    mapped_register.push_back({ "MMU_META_NEXT", nextAddr, 
        [this](Word) { return (_metadata_pool[_mmu_raw_index].next_node>>8) & 0xFF; },
        [this](Word, Byte data) { _metadata_pool[_mmu_raw_index].next_node = (_metadata_pool[_mmu_raw_index].next_node & 0x00FF) | (data << 8); }, 
        { "(Word) Next node of the current allocation       (Read Only)"} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _metadata_pool[_mmu_raw_index].next_node & 0xFF; }, 
        [this](Word, Byte data) { _metadata_pool[_mmu_raw_index].next_node = (_metadata_pool[_mmu_raw_index].next_node & 0xFF00) | data; },
    {""}}); nextAddr++;


    ////////////////////////////////////////////////
    // (Word) MMU_RAW_INDEX
    //      Handle for the current allocation chain
    /////
    mapped_register.push_back({ "MMU_RAW_INDEX", nextAddr, 
        [this](Word) { return (_mmu_raw_index>>8) & 0xFF; }, 
        [this](Word, Byte data) 
        { 
            _mmu_raw_index = (_mmu_raw_index & 0x00FF) | (data << 8); 
            if (_mmu_raw_index >= MMU_MEMORY_SIZE) { _mmu_raw_index = MMU_MEMORY_SIZE - 1; }
            _mmu_handle = _metadata_pool[_mmu_raw_index].root_node;
        },
        { "(Word) Raw Index of the current memory node  (For Debugging)",""} });
    nextAddr++;
    mapped_register.push_back( { "", nextAddr, 
        [this](Word) { return _mmu_raw_index & 0xFF; },
        [this](Word, Byte data) 
        { 
            _mmu_raw_index = (_mmu_raw_index & 0xFF00) | data; 
            if (_mmu_raw_index >= MMU_MEMORY_SIZE) { _mmu_raw_index = MMU_MEMORY_SIZE - 1; }
            _mmu_handle = _metadata_pool[_mmu_raw_index].root_node;
        },
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

    // initialize the MMU command functions:
    for (const auto& command : _mmu_command_list) 
    {
        register_command(Memory::Map(command.key, __FILE__, __LINE__), [this, action = command.action]() -> Byte {
            return action();
        });
    }

    // Initialize the metadata pool
    for (size_t i = 0; i < _metadata_pool.size(); ++i) {
        METADATA_NODE& node = _metadata_pool[i];

        // Initialize each node to a clean state
        node.status = 0x00;  // Mark the node as free (status bit 0 = 0)
        node.root_node = MMU_BAD_HANDLE;
        node.prev_node = MMU_BAD_HANDLE;
        node.next_node = MMU_BAD_HANDLE;       
        std::fill(node.data.begin(), node.data.end(), 0);  // Clear data block
    }    
    std::cout << clr::indent() << clr::LT_BLUE << "MMU::OnInit() Exit" << clr::RETURN;
}


void MMU::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "MMU::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "MMU::OnQuit() Exit" << clr::RETURN;
}




/***
 * @brief Sets the MMU error code and logs a helpful error message.
 *
 * Clears the current command and sets the error code to the given value.
 * If the error code is valid (i.e., it is less than the size of the
 * `_mmu_error_list` array), logs a helpful error message with the
 * comment associated with the error code. Otherwise, logs a generic
 * "Unknown Error" message.
 */
void MMU::error(Byte error_code) 
{
    _mmu_error = error_code;    // Set the error code
    _mmu_command = 0;           // Clear the command (indicates an error)
    // Log the error with a helpful message
    if (error_code < _mmu_error_list.size())
    {
        std::string comment = _mmu_error_list[error_code].second;
        UnitTest::Log(clr::RED + "MMU Error: " + comment + " $" + clr::hex(error_code, 2) + clr::RESET);
    }
    else
    {
        UnitTest::Log(clr::RED + "MMU Error: Unknown Error $" + clr::hex(error_code, 2) + clr::RESET);
    }
}


/**
 * @brief Executes a memory management unit command.
 * 
 * This function sets the current command and attempts to find
 * and execute the corresponding handler function from the
 * registered MMU commands. If the command is invalid, it
 * sets an error code indicating the invalid command.
 * 
 * @param command The command code to execute (one of the MMU_CMD_* constants).
 */
void MMU::do_command(Byte command)
{
    // std::cout << clr::indent() << clr::PURPLE << "MMU::do_command() Entry" << clr::RETURN;
    _mmu_command = command;
    // find the command and run it
    auto it = _mmu_commands.find(command);
    if (it != _mmu_commands.end()) {
        it->second();  // Call the associated function
    } else {
        error(MAP(MMU_ERR_INVALID));  // Handle invalid commands
    }    
    // std::cout << clr::indent() << clr::PURPLE << "MMU::do_command() Exit" << clr::RETURN;
}

/**
 * No operation. This function does absolutely nothing.
 * It is useful for testing the MMU's error handling.
 */
Byte MMU::do_nop()
{
    // This function literally does nothing
    // (as in NO OPERATION)
    return MAP(MMU_CMD_NOP);
}
bool MMU::_test_nop()
{
    bool test_results = true;
    return test_results;
}


/**
 * Allocates an 8KB memory page and updates the metadata.
 * 
 * This function allocates a full 8KB memory page starting from the bottom
 * of the memory pool and moving toward the top. The allocation is based on 
 * the status flag template provided in `MMU_ARG_1_MSB`, ensuring that bit 1
 * is set to indicate the allocation of a full 8KB page. The user can specify 
 * the memory type (RAM or ROM) using bit 2 of the status flag. After successfully 
 * allocating the page, the function updates the associated metadata and returns 
 * the command code for page allocation (`MMU_CMD_PG_ALLOC`).
 * 
 * The following relevant bits in the status flag template are considered:
 * 
 *              0000'0010: Paged Memory: 0 = No, 1 = Yes (always 1 for this function)
 *              0000'0100: Memory Type: 0 = RAM, 1 = ROM
 *              0001'0000: Locked: 0 = No, 1 = Yes
 *              0010'0000: Reserved or User Data
 *              0100'0000: Reserved or User Data  
 * 
 * @return The command code for page allocation (`MMU_CMD_PG_ALLOC`).
 */
Byte MMU::do_pg_alloc()
{
    // Set the status flag to indicate a full 8k page
    Byte status = Memory::Read(MAP(MMU_ARG_1_MSB));
    status &= 0b0110'0110;      // Mask out the error, fragmented, locked, and allocated bits
    status |= MAP(MMU_STFLG_PAGED);  // Set bit 1 to indicate full 8k page
    Memory::Write(MAP(MMU_ARG_1_MSB), (Byte)status);
    Memory::Write(MAP(MMU_ARG_1_LSB), (Byte)255); // allocate 256 nodes

    // Allocate the memory page
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_ALLOC));
    Word handle = Memory::Read_Word(MAP(MMU_META_HANDLE));

    // Check if the allocation was successful
    if (handle == MMU_BAD_HANDLE) {
        UnitTest::Log(clr::RED + "Error: Unable to allocate 8KB page!" + clr::RESET);
        error(MAP(MMU_ERR_ALLOC));
        return MAP(MMU_CMD_PG_ALLOC);
    }

    // Return the mapped command for page allocation
    return MAP(MMU_CMD_PG_ALLOC);
}


bool MMU::_test_pg_alloc()
{
    bool test_results = true;

    // // since this runs immediately after initialization, we are expecting
    // // _mmu_handle, _mmu_1_select, and _mmu_2_select to be initialized to MMU_BAD_HANDLE.
    // if (_mmu_handle != MMU_BAD_HANDLE) {
    //     UnitTest::Log(clr::RED + "Error: _mmu_handle Not Initialized! $" + clr::hex(_mmu_handle, 4) + clr::RESET);
    //     test_results = false;
    // }   
    // if (_mmu_1_select != MMU_BAD_HANDLE) {
    //     UnitTest::Log(clr::RED + "Error: _mmu_1_select Not Initialized $" + clr::hex(_mmu_1_select, 4) + clr::RESET);
    //     test_results = false;
    // }   
    // if (_mmu_2_select != MMU_BAD_HANDLE) {
    //     UnitTest::Log(clr::RED + "Error: _mmu_2_select Not Initialized $" + clr::hex(_mmu_2_select, 4) + clr::RESET);
    //     test_results = false;
    // }   

    // // Create a page and check if it was allocated
    // Memory::Write(MAP(MMU_ARG_1_MSB), (Byte)0);
    // Memory::Write(MAP(MMU_ARG_1_LSB), (Byte)0);
    // Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_PG_ALLOC));
    // _test_page_handle = Memory::Read_Word(MAP(MMU_META_HANDLE));
    // if (_test_page_handle == MMU_BAD_HANDLE) {
    //     UnitTest::Log(clr::RED + "Error: Unable to allocate 8KB page!" + clr::RESET);
    //     test_results = false;
    // }
    // if (_test_page_handle != 0x0000) {
    //     UnitTest::Log(clr::RED + "(RAM) Memory Handle Allocation Failed! ($" + clr::hex(_test_page_handle, 4) + ")" + clr::RESET);
    //     test_results = false;
    // }
    // // verify the remaining memory after a single allocation
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_FREE)) != 0xCBCC) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_BLOCKS_FREE] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_BLOCKS_FREE)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }
    // // verify the blocks allocated after a single allocation
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)) != 0x0100) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_BLOCKS_ALLOCATED] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }    
    // // verify the MMU_META_ROOT after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_ROOT)) != 0x0000) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_META_ROOT] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_ROOT)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }    
    // // verify the MMU_META_PREV after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_PREV)) != 0xFFFF) { // 0x0000) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_META_PREV] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_PREV)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }    
    // // verify the MMU_META_NEXT after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_NEXT)) != 0x0001) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_META_NEXT] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_NEXT)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }
    // // verify MMU_RAW_INDEX is same as the handle after a single allocation
    // if (Memory::Read_Word(MAP(MMU_RAW_INDEX)) != _test_page_handle) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_RAW_INDEX] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_RAW_INDEX)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }
    // Byte data = 0x01;   
    // for (Word addr = 0; addr < 32; ++addr) {
    //     Memory::Write(MAP(MMU_META_DATA) + addr, data);
    //     if (Memory::Read(MAP(MMU_META_DATA) + addr) != data) {
    //         UnitTest::Log(clr::RED + "(RAM) [MMU_META_DATA + $" + clr::hex(addr, 2) + "] Failed Read/Write Test!" + clr::RESET);
    //         test_results = false;            
    //     }
    //     data++;
    // }

    // // Verify the parsing of the page allocation
    // int allocated_bytes = 8 * 1024;
    // Word current_node = _test_page_handle;
    // while (current_node != MMU_BAD_HANDLE) {
    //     _mmu_raw_index = current_node;
    //     for (Word addr = 0; addr < 32; ++addr) {
    //         Memory::Write(MAP(MMU_META_DATA) + addr, data);
    //         if (Memory::Read(MAP(MMU_META_DATA) + addr) != data) {
    //             UnitTest::Log(clr::RED + "(RAM) [MMU_META_DATA + $" + clr::hex(addr, 2) + "] Failed Read/Write Test!" + clr::RESET);
    //             test_results = false;            
    //         }
    //         data++;
    //         allocated_bytes--;
    //     }        
    //     current_node = Memory::Read_Word(MAP(MMU_META_NEXT));
    //     if (allocated_bytes < 0) {
    //         UnitTest::Log(clr::RED + "(RAM) Page Parse Size Exceeded! " + std::to_string(allocated_bytes) + " bytes!" + clr::RESET);
    //         test_results = false; 
    //         break;            
    //     }
    // }
    return test_results;
}


Byte MMU::do_pg_free()
{
    // Read the handle of the node to be freed from memory
    Word handle = Memory::Read_Word(MAP(MMU_META_HANDLE));


    // Check if the root node is allocated and is an 8k page
    if ((_metadata_pool[handle].status & 0x03) == 0x03) 
    {
        // loop through all of the nodes in this chain and set the status to 0x01
        while (handle != MMU_BAD_HANDLE) 
        {
            _metadata_pool[handle].status = 0x01;
            handle = _metadata_pool[handle].next_node;
        }

        // Perform the actual memory free operation
        do_free();
        return MAP(MMU_CMD_PG_FREE);
    }
    else 
    {
        // Handle the case where the page cannot be freed (invalid state)
        error(MAP(MMU_ERR_PG_FREE));
        UnitTest::Log(clr::RED + "MMU::do_pg_free() Page Free Failed!" + clr::RESET);
        return MAP(MMU_CMD_PG_FREE);
    }
    return MAP(MMU_CMD_PG_FREE);
}


bool MMU::_test_pg_free()
{
    bool test_results = true;

    // // free the test page
    // Memory::Write_Word(MAP(MMU_META_HANDLE), _test_page_handle);
    // Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_FREE));

    // // verify the remaining memory after a single allocation
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_FREE)) != 0xCCCC) {
    //     UnitTest::Log(clr::RED + "[MMU_BLOCKS_FREE] 8K Page Free Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_BLOCKS_FREE)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }
    // // verify the blocks allocated after a single allocation
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)) != 0x0000) {
    //     UnitTest::Log(clr::RED + "[MMU_BLOCKS_ALLOCATED] 8K Page Free Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }    
    return test_results;
}


/**
 * Allocate a chain of metadata nodes until (MMU_ARG_1_LSB)+1 amount is reached.
 * 
 * This function allocates a chain of metadata nodes until the total allocation
 * size reaches the value of MMU_ARG_1_LSB+1. It returns the mapped command for
 * page allocation (MMU_CMD_PG_ALLOC).
 * 
 * The function first creates a new handle for the root node of the allocation
 * chain. It then sets the handle for the first node in the allocation chain
 * and initializes the root node's root_node and prev_node to point to itself. 
 * It also sets the nodes status flag to the value found within MMU_ARG_1_MSB.
 * 
 * In the case where bit 2 of the status flag stored within MMU_ARG_1_MSB is set
 * to 1 the value of 255 will be stored into MMU_ARG_1_LSB regardless of the
 * value currently stored in MMU_ARG_1_LSB. In this case a full sized 8k page
 * should be allocated.
 * 
 * The function then starts creating the chain of metadata nodes. For each node,
 * it sets the previous node of the new node to the current node index, sets the
 * next node of the current node to the new nodes index, and updates the status 
 * flag to the value in MMU_ARG_1_MSB. This process repeats until the number of 
 * nodes in the chain is equal to the value stored in MMU_ARG_1_LSB+1 or 1-256.
 * 
 * If there is more space to allocate, the function will handle the allocation
 * error by calling error(MMU_ERR_ALLOC) to indicate a memory allocation error.
 * When this happens the next_node of the current node is set to MMU_BAD_HANDLE.
 * The function then breaks out of the loop and returns normally.
 * 
 * If there is no more space available, the function breaks out of the loop and
 * logs an error message. It then calls error on allocation failure and returns
 * the failure code (MMU_CMD_PG_ALLOC).
 * 
 * Finally, the function logs the allocation result and returns the mapped
 * command for page allocation (MMU_CMD_PG_ALLOC).     
 * 
 * @param (MMU_ARG_1_MSB) status flag template for allocation
 *         relevant bits:
 *              0000'0010: Paged Memory: 0 = No,   1 = Yes
 *              0000'0100: Memory Type:  0 = RAM,  1 = ROM
 *              0001'0000: Locked:       0 = No,   1 = Yes
 *              0010'0000:   (reserved) or User Data
 *              0100'0000:   (reserved) or User Data
 * 
 * @param (MMU_ARG_1_LSB)+1 or 1-256 The number of 32-byte nodes to allocate.
 * 
 * 
 * @return The value in MMU_RAW_INDEX should be set to the handle of the root node.
 */
Byte MMU::do_alloc()
{
    // Read the status flag and allocation size
    Byte status_flag = Memory::Read(MAP(MMU_ARG_1_MSB)) & 0b0111'0110;  // Mask out the error, fragmented, 
    Memory::Write(MAP(MMU_ARG_1_MSB), (Byte)status_flag);                     // and allocated bits.

    Word allocation_size = Memory::Read(MAP(MMU_ARG_1_LSB)); // Nodes to allocate
    // Check if full-page allocation is requested
    if (status_flag & MAP(MMU_STFLG_PAGED)) {
        Memory::Write(MAP(MMU_ARG_1_LSB), (Byte)255); // Update allocation size
        allocation_size = 255;
    }

    // Create the root node handle
    Word handle = create_handle();
    if (handle == MMU_BAD_HANDLE) {
        error(MAP(MMU_ERR_HANDLE));
        return MAP(MMU_CMD_ALLOC);
    }
    _mmu_handle = handle;

    // Set the root node
    _mmu_raw_index = handle;
    _metadata_pool[_mmu_raw_index].root_node = handle;
    _metadata_pool[_mmu_raw_index].prev_node = MMU_BAD_HANDLE;
    _metadata_pool[_mmu_raw_index].next_node = MMU_BAD_HANDLE;
    _metadata_pool[_mmu_raw_index].status = (status_flag | 0b0000'0001);  // Set status flag as allocated

    Word current_node = handle;

    // Allocate a Chain of Nodes
    for (Word i = 0; i < allocation_size; ++i) 
    {
        // Find a free node
        Word new_node = 0;
        for (new_node = 0; new_node < _metadata_pool.size(); ++new_node) 
        {
            if (!(_metadata_pool[new_node].status & 0b0000'0001)) { break; }            
        }   
        if (new_node == _metadata_pool.size())
        {
            error(MAP(MMU_ERR_ALLOC));
            _metadata_pool[current_node].next_node = MMU_BAD_HANDLE;            
            return MAP(MMU_CMD_ALLOC);
        }

        // Link the current node to the new node
        _metadata_pool[new_node].prev_node = current_node;
        _metadata_pool[new_node].root_node = handle;
        _metadata_pool[current_node].next_node = new_node;
        _metadata_pool[new_node].status = (status_flag & 0b0111'1111);  // Mask Out Errors
        _metadata_pool[new_node].status = (status_flag | 0b0000'0001);  // Set status flag as allocated

        // Check for fragmentation (gap between nodes)
        if (new_node != current_node + 1) {
            // Set the fragmented flag if the nodes are not sequential
            _metadata_pool[current_node].status |= MAP(MMU_STFLG_FRAGD);
            _mmu_blocks_fragged++;
            _metadata_pool[new_node].status |= MAP(MMU_STFLG_FRAGD);
            _mmu_blocks_fragged++;
        }

        // Update block counters
        _mmu_blocks_free--;           // One less block available
        _mmu_blocks_allocated++;      // One more block allocated

        current_node = new_node; // Move to the new node
    }    

    // Log success
    // UnitTest::Log("Allocation complete. Root handle: " + std::to_string(handle));
    return MAP(MMU_CMD_ALLOC);
}


bool MMU::_test_alloc()
{
    bool test_results = true;

    // // test RAM allocation
    // // first allocate a single block and verify it
    // Memory::Write(MAP(MMU_ARG_1_MSB), (Byte)0);         // status flag
    // Memory::Write(MAP(MMU_ARG_1_LSB), (Byte)0x00);                // single 32-byte node
    // Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_ALLOC));    // execute the allocation command
    // _test_rw_handle = Memory::Read_Word(MAP(MMU_META_HANDLE));    // save the handle
    // _mmu_raw_index = _test_rw_handle;
    // // verify the handle after a single allocation
    // if (_test_rw_handle != 0x0000) {
    //     UnitTest::Log(clr::RED + "(RAM) Memory Handle Allocation Failed! ($" + clr::hex(_test_rw_handle, 4) + ")" + clr::RESET);
    //     test_results = false;
    // }
    // // verify the remaining memory after a single allocation
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_FREE)) != 0xCCCB) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_BLOCKS_FREE] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_BLOCKS_FREE)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }
    // // verify the blocks allocated after a single allocation
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)) != 0x0001) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_BLOCKS_ALLOCATED] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }    
    // // verify the MMU_META_ROOT after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_ROOT)) != 0x0000) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_META_ROOT] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_ROOT)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }    
    // // verify the MMU_META_PREV after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_PREV)) != 0xFFFF) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_META_PREV] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_PREV)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }    
    // // verify the MMU_META_NEXT after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_NEXT)) != MMU_BAD_HANDLE) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_META_NEXT] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_NEXT)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }
    // // verify MMU_RAW_INDEX is same as the handle after a single allocation
    // if (Memory::Read_Word(MAP(MMU_RAW_INDEX)) != _test_rw_handle) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_RAW_INDEX] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_RAW_INDEX)), 4) + ")" + clr::RESET);
    //     test_results = false;
    // }
    // Byte data = 0x01;   
    // for (Word addr = 0; addr < 32; ++addr) {
    //     Memory::Write(MAP(MMU_META_DATA) + addr, data);
    //     if (Memory::Read(MAP(MMU_META_DATA) + addr) != data) {
    //         UnitTest::Log(clr::RED + "(RAM) [MMU_META_DATA + $" + clr::hex(addr, 2) + "] Failed Read/Write Test!" + clr::RESET);
    //         test_results = false;            
    //     }
    //     data++;
    // }

    // // test ROM allocation
    // // first allocate a single block and verify it
    // Memory::Write(MAP(MMU_ARG_1_MSB), (Byte)MAP(MMU_STFLG_READONLY));         // status flag
    // Memory::Write(MAP(MMU_ARG_1_LSB), (Byte)0x00);                // single 32-byte node
    // Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_ALLOC));    // execute the allocation command
    // _test_ro_handle = Memory::Read_Word(MAP(MMU_META_HANDLE));    // fetch the handle
    // // verify the handle after a single allocation
    // if (_test_ro_handle != 0x0001) {
    //     UnitTest::Log(clr::RED + "(ROM) Memory Handle Allocation Failed! ($" + clr::hex(_test_ro_handle, 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }
    // // verify the remaining memory after a single allocation
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_FREE)) != 0xCCCA) {
    //     UnitTest::Log(clr::RED + "(ROM) [MMU_BLOCKS_FREE] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_BLOCKS_FREE)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }
    // // verify the blocks allocated after a single allocation
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)) != 0x0002) {
    //     UnitTest::Log(clr::RED + "(ROM) [MMU_BLOCKS_ALLOCATED] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }    
    // // verify the MMU_META_ROOT after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_ROOT)) != 0x0001) {
    //     UnitTest::Log(clr::RED + "(ROM) [MMU_META_ROOT] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_ROOT)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }    
    // // verify the MMU_META_PREV after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_PREV)) != 0xFFFF) { //0x0001) {
    //     UnitTest::Log(clr::RED + "(ROM) [MMU_META_PREV] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_PREV)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }    
    // // verify the MMU_META_NEXT after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_NEXT)) != MMU_BAD_HANDLE) {
    //     UnitTest::Log(clr::RED + "(ROM) [MMU_META_NEXT] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_NEXT)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }
    // // verify MMU_RAW_INDEX is same as the handle after a single allocation
    // if (Memory::Read_Word(MAP(MMU_RAW_INDEX)) != _test_ro_handle) {
    //     UnitTest::Log(clr::RED + "(ROM) [MMU_RAW_INDEX] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_RAW_INDEX)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }
    // data = 0xFF;   
    // _mmu_raw_index = _test_ro_handle;
    // for (Word addr = 0; addr < 32; ++addr) {
    //     Memory::Write(MAP(MMU_META_DATA) + addr, data);
    //     if (Memory::Read(MAP(MMU_META_DATA) + addr) == data) {
    //         UnitTest::Log(clr::RED + "(ROM) [MMU_META_DATA + $" + clr::hex(addr, 2) + "] Failed Read Only Test!" + clr::RESET);
    //         test_results = false;            
    //     }
    //     --data;
    // }

    // // test LOCKED allocation
    // // first allocate a single block and verify it
    // Memory::Write(MAP(MMU_ARG_1_MSB), (Byte)MAP(MMU_STFLG_LOCKED));         // status flag
    // Memory::Write(MAP(MMU_ARG_1_LSB), (Byte)0x00);                // single 32-byte node
    // Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_ALLOC));    // execute the allocation command
    // _test_lock_handle = Memory::Read_Word(MAP(MMU_META_HANDLE));    // fetch the handle
    // // verify the handle after a single allocation
    // if (_test_lock_handle != 0x0002) {
    //     UnitTest::Log(clr::RED + "(locked) Memory Handle Allocation Failed! ($" + clr::hex(_test_lock_handle, 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }
    // // verify the remaining memory after a single allocation
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_FREE)) != 0xCCC9) {
    //     UnitTest::Log(clr::RED + "(locked) [MMU_BLOCKS_FREE] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_BLOCKS_FREE)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }
    // // verify the blocks allocated after a single allocation
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)) != 0x0003) {
    //     UnitTest::Log(clr::RED + "(locked) [MMU_BLOCKS_ALLOCATED] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }    
    // // verify the MMU_META_ROOT after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_ROOT)) != 0x0002) {
    //     UnitTest::Log(clr::RED + "(locked) [MMU_META_ROOT] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_ROOT)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }    
    // // verify the MMU_META_PREV after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_PREV)) != 0xFFFF) { //0x0002) {
    //     UnitTest::Log(clr::RED + "(locked) [MMU_META_PREV] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_PREV)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }    
    // // verify the MMU_META_NEXT after a single allocation
    // if (Memory::Read_Word(MAP(MMU_META_NEXT)) != MMU_BAD_HANDLE) {
    //     UnitTest::Log(clr::RED + "(locked) [MMU_META_NEXT] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_META_NEXT)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }
    // // verify MMU_RAW_INDEX is same as the handle after a single allocation
    // if (Memory::Read_Word(MAP(MMU_RAW_INDEX)) != _test_lock_handle) {
    //     UnitTest::Log(clr::RED + "(locked) [MMU_RAW_INDEX] Failed! ($" + clr::hex(Memory::Read_Word(MAP(MMU_RAW_INDEX)), 4) + ")" + clr::RESET); 
    //     test_results = false;
    // }
    // data = 0x40;   
    // _mmu_raw_index = _test_lock_handle;
    // for (Word addr = 0; addr < 32; ++addr) {
    //     Memory::Write(MAP(MMU_META_DATA) + addr, data);
    //     if (Memory::Read(MAP(MMU_META_DATA) + addr) == data) {
    //         UnitTest::Log(clr::RED + "(locked) [MMU_META_DATA + $" + clr::hex(addr, 2) + "] Failed Locked Memory Test!" + clr::RESET);
    //         test_results = false;            
    //     }
    //     data++;
    // }    

    return test_results;
}

/**
 * Frees a chain of metadata nodes starting from the current handle.
 *
 * This function identifies the root node from the current handle
 * and checks if the handle is valid and allocated. If not, it reports
 * an error and returns the mapped command for page deallocation.
 * 
 * It traverses through the chain of nodes starting from the root node,
 * clearing metadata and marking each node as free. Once all nodes in the
 * chain are freed, it deallocates the handle, resets relevant indices, 
 * and logs the successful deallocation.
 * 
 * @param (MMU_META_HANDLE) is loaded with the handle to be freed.
 * 
 * @return The mapped command for page deallocation (MMU_CMD_FREE).
 */

Byte MMU::do_free()
{
    // Get the root node from the current handle
    Word root_node = _mmu_handle;

    // If the handle is invalid, report an error
    if (root_node == MMU_BAD_HANDLE || 
        !(_metadata_pool[root_node].status & 0b0000'0001)) 
    {
        error(MAP(MMU_ERR_FREE)); // Invalid handle or not allocated
        return MAP(MMU_CMD_FREE);
    }

    // Traverse and free all nodes in the chain
    Word current_node = root_node;
    while (current_node != MMU_BAD_HANDLE) 
    {
        Word next_node = _metadata_pool[current_node].next_node; // Save next node

        // Clear metadata for the current node
        _metadata_pool[current_node].root_node = MMU_BAD_HANDLE;
        _metadata_pool[current_node].prev_node = MMU_BAD_HANDLE;
        _metadata_pool[current_node].next_node = MMU_BAD_HANDLE;
        _metadata_pool[current_node].status = 0; // Clear Status
        // _metadata_pool[current_node].status  &= ~0x01; // Mark as free

        // Update the block counters
        _mmu_blocks_free++;           // One more block is free
        _mmu_blocks_allocated--;      // One less block is allocated 

        // clear the data too
        for (int i=0; i<32; ++i) { _metadata_pool[current_node].data[i] = 0; }

        // Move to the next node
        current_node = next_node; 
    }

    // Clear the handle
    deallocate_handle(root_node);
    _mmu_handle = MMU_BAD_HANDLE;
    // _mmu_raw_index = MMU_BAD_HANDLE;

    // Log success
    // UnitTest::Log("Deallocation complete for root handle: " + std::to_string(root_node));
    return MAP(MMU_CMD_FREE);
}

bool MMU::_test_free()
{
    bool test_results = true;

    // // free the RAM node
    // Memory::Write_Word(MAP(MMU_META_HANDLE), _test_rw_handle);
    // Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_FREE));
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_FREE)) != 0xCCCA) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_BLOCKS_FREE] MMU_CMD_FREE Command Failed!" + clr::RESET);
    //     test_results = false;        
    // }
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)) != 2) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_BLOCKS_ALLOCATED] MMU_CMD_FREE Command Failed!" + clr::RESET);
    //     test_results = false;        
    // }
    // if (Memory::Read_Word(MAP(MMU_META_HANDLE)) != MMU_BAD_HANDLE) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_META_HANDLE] MMU_CMD_FREE Command Failed!" + clr::RESET);
    //     test_results = false;        
    // }    

    // // free the ROM node
    // Memory::Write_Word(MAP(MMU_META_HANDLE), _test_ro_handle);
    // Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_FREE));
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_FREE)) != 0xCCCB) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_BLOCKS_FREE] MMU_CMD_FREE Command Failed!" + clr::RESET);
    //     test_results = false;        
    // }
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)) != 1) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_BLOCKS_ALLOCATED] MMU_CMD_FREE Command Failed!" + clr::RESET);
    //     test_results = false;        
    // }
    // if (Memory::Read_Word(MAP(MMU_META_HANDLE)) != MMU_BAD_HANDLE) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_META_HANDLE] MMU_CMD_FREE Command Failed!" + clr::RESET);
    //     test_results = false;        
    // }  

    // // free the Locked node
    // Memory::Write_Word(MAP(MMU_META_HANDLE), _test_lock_handle);
    // Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_FREE));
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_FREE)) != 0xCCCC) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_BLOCKS_FREE] MMU_CMD_FREE Command Failed!" + clr::RESET);
    //     test_results = false;        
    // }
    // if (Memory::Read_Word(MAP(MMU_BLOCKS_ALLOCATED)) != 0) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_BLOCKS_ALLOCATED] MMU_CMD_FREE Command Failed!" + clr::RESET);
    //     test_results = false;        
    // }
    // if (Memory::Read_Word(MAP(MMU_META_HANDLE)) != MMU_BAD_HANDLE) {
    //     UnitTest::Log(clr::RED + "(RAM) [MMU_META_HANDLE] MMU_CMD_FREE Command Failed!" + clr::RESET);
    //     test_results = false;        
    // }  

    return test_results;
}

Byte MMU::do_load_root()
{    
    // // do nothing if _mmu_raw_index points to an invalid handle
    // auto it = std::find(_handles.begin(), _handles.end(), _mmu_raw_index);
    // if (it == _handles.end()) { return MAP(MMU_CMD_LOAD_ROOT); }

    // if the rootnode is invalid, do nothing
    METADATA_NODE& node = _metadata_pool[_mmu_raw_index];
    if (node.root_node != MMU_BAD_HANDLE) 
    {
        _mmu_raw_index = node.root_node;
    }
    else
    {
        UnitTest::Log(clr::RED + "[MMU_CMD_LOAD_ROOT] Command Failed!" + clr::RESET);
        error(MAP(MMU_ERR_NODE));
    }
    return MAP(MMU_CMD_LOAD_ROOT);
}
bool MMU::_test_load_root()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_load_next()
{
    METADATA_NODE& node = _metadata_pool[_mmu_raw_index];
    if (node.next_node != MMU_BAD_HANDLE) 
    {
        _mmu_raw_index = node.next_node;
    }
    else
    {
        UnitTest::Log(clr::RED + "[MMU_CMD_LOAD_NEXT] Command Failed!" + clr::RESET);
        error(MAP(MMU_ERR_NODE));
    }
    return MAP(MMU_CMD_LOAD_NEXT);
}
bool MMU::_test_load_next()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_load_prev()
{
    METADATA_NODE& node = _metadata_pool[_mmu_raw_index];
    if (node.prev_node != MMU_BAD_HANDLE) 
    {
        _mmu_raw_index = node.prev_node;
    }
    else
    {
        UnitTest::Log(clr::RED + "[MMU_CMD_LOAD_PREV] Command Failed!" + clr::RESET);
        error(MAP(MMU_ERR_NODE));
    }
    return MAP(MMU_CMD_LOAD_PREV);
}
bool MMU::_test_load_prev()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_load_last()
{    
    while (_metadata_pool[_mmu_raw_index].next_node != MMU_BAD_HANDLE)
    {
        _mmu_raw_index = _metadata_pool[_mmu_raw_index].next_node;
    }
    return MAP(MMU_CMD_LOAD_LAST);
}
bool MMU::_test_load_last()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_del_node()
{
    // 1. Retrieve the handle of the node to delete
    Word handle = Memory::Read_Word(MAP(MMU_META_HANDLE));

    // 2. Validate that the node exists and is allocated
    if (handle == MMU_BAD_HANDLE || (_metadata_pool[handle].status & 0x01) != 0x01) {
        UnitTest::Log(clr::RED + "Error: Invalid node or node not allocated!" + clr::RESET);
        error(MAP(MMU_ERR_NODE));
        return MAP(MMU_CMD_DEL_NODE);
    }

    // 3. Check if the node is part of an active page allocation
    Word page_handle = _metadata_pool[handle].root_node;
    if (page_handle != MMU_BAD_HANDLE) {
        // Node is part of an active page allocation, cannot delete it directly
        UnitTest::Log(clr::RED + "Error: Node is part of an active page and cannot be deleted!" + clr::RESET);
        error(MAP(MMU_ERR_NODE));
        return MAP(MMU_CMD_DEL_NODE);
    }

    // 4. Check if it's a root node
    Word prev_node = _metadata_pool[handle].prev_node;
    Word next_node = _metadata_pool[handle].next_node;

    // If it's a root node, perform root-specific operations
    if (_metadata_pool[handle].root_node == handle) {
        if (next_node != MMU_BAD_HANDLE) {
            // If there is a next node, set the next node as the new root
            _metadata_pool[next_node].root_node = next_node;
            _metadata_pool[next_node].prev_node = MMU_BAD_HANDLE; // The next node becomes the root node

            // Update the list to reflect the new root node
            _metadata_pool[handle].root_node = MMU_BAD_HANDLE;
            _metadata_pool[handle].next_node = MMU_BAD_HANDLE;
        } else {
            // If there is no next node, we're deleting the last root node
            _metadata_pool[handle].root_node = MMU_BAD_HANDLE;
        }
    } else {
        // For non-root nodes, update previous/next node pointers
        if (prev_node != MMU_BAD_HANDLE) {
            _metadata_pool[prev_node].next_node = next_node;
        }
        if (next_node != MMU_BAD_HANDLE) {
            _metadata_pool[next_node].prev_node = prev_node;
        }
    }

    // 5. Remove the node's handle from the _handles vector
    auto it = std::find(_handles.begin(), _handles.end(), handle);
    if (it != _handles.end()) {
        _handles.erase(it);  // Remove the handle from the vector
    }

    // 6. Mark node as freed/unallocated by clearing the status
    _metadata_pool[handle].status &= ~0x03;  // Reset the allocated/locked flags

    // 7. Clean up the node's data and pointers
    std::fill(std::begin(_metadata_pool[handle].data), std::end(_metadata_pool[handle].data), 0);  // Clear the node's data
    _metadata_pool[handle].prev_node = MMU_BAD_HANDLE;
    _metadata_pool[handle].next_node = MMU_BAD_HANDLE;

    // 8. Call MMU_CMD_FREE to free the node's memory (if necessary)
    Memory::Write_Word(MAP(MMU_META_HANDLE), handle);
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_FREE));

    // 9. Return the command code for deleting the node
    return MAP(MMU_CMD_DEL_NODE);
}
bool MMU::_test_del_node()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_ins_before() {
    // 1. Get the target node's handle from the registers (this should come from MMU_META_HANDLE)
    Word target_handle = Memory::Read_Word(MAP(MMU_META_HANDLE));

    // 2. Validate that the target node exists and is allocated
    if (target_handle == MMU_BAD_HANDLE || (_metadata_pool[target_handle].status & 0x01) == 0) {
        UnitTest::Log(clr::RED + "Error: Invalid target node or node not allocated!" + clr::RESET);
        error(MAP(MMU_ERR_NODE));
        return MAP(MMU_CMD_INS_BEFORE);
    }

    // 3. Allocate the new node by calling do_alloc()
    Memory::Write(MAP(MMU_ARG_1_LSB), (Byte)1); // Requesting allocation of one 32-byte node
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_ALLOC));
    Word new_handle = Memory::Read_Word(MAP(MMU_META_HANDLE));

    // 4. Check if the allocation was successful
    if (new_handle == MMU_BAD_HANDLE) {
        UnitTest::Log(clr::RED + "Error: Unable to allocate new node!" + clr::RESET);
        error(MAP(MMU_ERR_ALLOC));
        return MAP(MMU_CMD_INS_BEFORE);
    }

    // 5. Get the target node from the metadata pool
    METADATA_NODE& target_node = _metadata_pool[target_handle];

    // 6. Update pointers to insert the new node before the target node
    Word prev_handle = target_node.prev_node;

    // Set the previous node of the target to the new node
    target_node.prev_node = new_handle;

    // Set the next node of the new node to the target node
    _metadata_pool[new_handle].next_node = target_handle;

    // If there was a node before the target, update its next_node to the new node
    if (prev_handle != MMU_BAD_HANDLE) {
        _metadata_pool[prev_handle].next_node = new_handle;
        _metadata_pool[new_handle].prev_node = prev_handle;
    } else {
        _metadata_pool[new_handle].prev_node = MMU_BAD_HANDLE; // New node is the first node
    }

    // 7. Mark the new node as allocated
    _metadata_pool[new_handle].status |= 0x01; // Set allocated bit

    // 8. Update the root_node pointer if necessary
    if (target_handle == _metadata_pool[target_handle].root_node) {
        // The new node becomes the root node, and it should be the start of a chain
        // Traverse from the new node and update each node's root_node pointer
        Word current_node = new_handle;
        while (current_node != MMU_BAD_HANDLE) {
            _metadata_pool[current_node].root_node = new_handle;  // Update root_node for each node in the chain
            current_node = _metadata_pool[current_node].next_node;  // Move to the next node in the chain
        }
    }

    // 9. Optionally add the new handle to the handles list if necessary
    _handles.push_back(new_handle);

    // 10. Return the appropriate command for the operation
    return MAP(MMU_CMD_INS_BEFORE);
}
bool MMU::_test_ins_before()
{
    bool test_results = true;
    return test_results;
}


// Helper function to allocate a new node with the provided flags
Word MMU::allocate_new_node()
{
    // 1. Write the allocation flags (status template)
    Byte status_flags = 0;  // Populate with appropriate flags for allocation
    Memory::Write(MAP(MMU_ARG_1_MSB), status_flags);

    // 2. Write the number of nodes to allocate (1 node in this case)
    Memory::Write(MAP(MMU_ARG_1_LSB), (Byte)0); // Allocate 1 node (0+1)

    // 3. Perform the allocation command
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_ALLOC));

    // 4. Read the handle of the newly allocated node
    Word handle = Memory::Read_Word(MAP(MMU_META_HANDLE));

    // 5. Add the handle to _handles vector (keep track of allocated handles)
    _handles.push_back(handle);

    return handle;
}



Byte MMU::do_ins_after() {
    // 1. Get the target node's handle from the registers (this should come from MMU_META_HANDLE)
    Word target_handle = Memory::Read_Word(MAP(MMU_META_HANDLE));

    // 2. Validate that the target node exists and is allocated
    if (target_handle == MMU_BAD_HANDLE || (_metadata_pool[target_handle].status & 0x01) == 0) {
        UnitTest::Log(clr::RED + "Error: Invalid target node or node not allocated!" + clr::RESET);
        error(MAP(MMU_ERR_NODE));
        return MAP(MMU_CMD_INS_AFTER);
    }

    // 3. Allocate the new node by calling do_alloc()
    Memory::Write(MAP(MMU_ARG_1_LSB), (Byte)1); // Requesting allocation of one 32-byte node
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_ALLOC));
    Word new_handle = Memory::Read_Word(MAP(MMU_META_HANDLE));

    // 4. Check if the allocation was successful
    if (new_handle == MMU_BAD_HANDLE) {
        UnitTest::Log(clr::RED + "Error: Unable to allocate new node!" + clr::RESET);
        error(MAP(MMU_ERR_ALLOC));
        return MAP(MMU_CMD_INS_AFTER);
    }

    // 5. Get the target node from the metadata pool
    METADATA_NODE& target_node = _metadata_pool[target_handle];

    // 6. Update pointers to insert the new node after the target node
    Word next_handle = target_node.next_node;

    // Set the next node of the target to the new node
    target_node.next_node = new_handle;

    // Set the previous node of the new node to the target node
    _metadata_pool[new_handle].prev_node = target_handle;

    // If there was a node after the target, update its prev_node to the new node
    if (next_handle != MMU_BAD_HANDLE) {
        _metadata_pool[next_handle].prev_node = new_handle;
        _metadata_pool[new_handle].next_node = next_handle;
    } else {
        _metadata_pool[new_handle].next_node = MMU_BAD_HANDLE; // New node is the last node
    }

    // 7. Mark the new node as allocated
    _metadata_pool[new_handle].status |= 0x01; // Set allocated bit

    // 8. Update the root_node pointer for the new node
    _metadata_pool[new_handle].root_node = _metadata_pool[target_handle].root_node;

    // 9. Optionally add the new handle to the handles list if necessary
    _handles.push_back(new_handle);

    // 10. Return the appropriate command for the operation
    return MAP(MMU_CMD_INS_AFTER);
}
bool MMU::_test_ins_after()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_push_back()
{
    // go to the last node
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_LOAD_LAST));
    // insert after
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_INS_AFTER));    
    return MAP(MMU_CMD_PUSH_BACK);
}
bool MMU::_test_push_back()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_push_front()
{
    // go to the root node
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_LOAD_ROOT));
    // insert before
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_INS_BEFORE));
    return MAP(MMU_CMD_PUSH_FRONT);
}
bool MMU::_test_push_front()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_pop_back() {
    // Go to the last node
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_LOAD_LAST));
    // Delete the current node
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_DEL_NODE));
    return MAP(MMU_CMD_POP_BACK);
}
bool MMU::_test_pop_back()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_pop_front() {
    // Go to the root node
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_LOAD_ROOT));
    // Delete the current node
    Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_DEL_NODE));
    return MAP(MMU_CMD_POP_FRONT);
}
bool MMU::_test_pop_front()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_lock_node()
{
    _metadata_pool[_mmu_raw_index].status |= MAP(MMU_STFLG_LOCKED);
    return MAP(MMU_CMD_LOCK_NODE);
}
bool MMU::_test_lock_node()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_unlock_node()
{
    _metadata_pool[_mmu_raw_index].status &= ~MAP(MMU_STFLG_LOCKED);
    return MAP(MMU_CMD_UNLOCK_NODE);
}
bool MMU::_test_unlock_node()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_defrag() {
    // Check if there are any fragmented blocks to defragment
    if (_mmu_blocks_fragged == 0) {
        UnitTest::Log(clr::YELLOW + "No fragmentation detected. Defrag skipped." + clr::RESET);
        return MAP(MMU_CMD_DEFRAG);
    }

    UnitTest::Log("Defragmentation started...");

    // // Initialize necessary variables for tracking the defrag process
    // Word current_node = _mmu_raw_index;  // Start from the root node
    // Word prev_node = MMU_BAD_HANDLE;
    // Word first_fragmented_node = MMU_BAD_HANDLE;
    // Word last_fragmented_node = MMU_BAD_HANDLE;

    // // Iterate through the pool and find fragmented blocks
    // while (current_node != MMU_BAD_HANDLE) 
    // {
    //     if (_metadata_pool[current_node].status & MAP(MMU_STFLG_FRAGD)) 
    //     {
    //         // If the current node is fragmented, we need to move it
    //         if (first_fragmented_node == MMU_BAD_HANDLE) 
    //         {
    //             first_fragmented_node = current_node;  // Mark the first fragmented node
    //         }
    //         last_fragmented_node = current_node;  // Update the last fragmented node

    //         // Remove the node from the chain
    //         Word next_node = _metadata_pool[current_node].next_node;
    //         if (prev_node != MMU_BAD_HANDLE) 
    //         {
    //             _metadata_pool[prev_node].next_node = next_node;
    //         }
    //         if (next_node != MMU_BAD_HANDLE) 
    //         {
    //             _metadata_pool[next_node].prev_node = prev_node;
    //         }

    //         // Now move the fragmented node to the end of the pool (to compact memory)
    //         // Word new_location = _mmu_raw_index;  // Use this as a placeholder for the compaction location
    //         _metadata_pool[current_node].next_node = MMU_BAD_HANDLE;
    //         _metadata_pool[current_node].prev_node = MMU_BAD_HANDLE;
    //         _metadata_pool[current_node].root_node = MMU_BAD_HANDLE;
    //         _metadata_pool[current_node].status &= ~MAP(MMU_STFLG_FRAGD);  // Clear the fragmented flag

    //         // Put the node back in the chain at the end (or wherever needed)
    //         if (last_fragmented_node != MMU_BAD_HANDLE) 
    //         {
    //             _metadata_pool[last_fragmented_node].next_node = current_node;
    //             _metadata_pool[current_node].prev_node = last_fragmented_node;
    //         }
    //         _mmu_blocks_fragged--;  // One less fragmented block

    //         prev_node = current_node;  // Move to the next node in the chain
    //     }
    //     current_node = _metadata_pool[current_node].next_node;
    // }

    // // Finalize defragging and update root node
    // if (first_fragmented_node != MMU_BAD_HANDLE) 
    // {
    //     _metadata_pool[first_fragmented_node].root_node = _mmu_raw_index;
    // }

    UnitTest::Log(clr::GREEN + "Defragmentation completed successfully!" + clr::RESET);
    return MAP(MMU_CMD_DEFRAG);
}

bool MMU::_test_defrag()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_reset() {
    // 1. Reset the page bit for all nodes
    for (auto& metadata : _metadata_pool) 
    {
        metadata.status &= ~0x02;  // Clear the "page" bit
    }

    // 2. Deallocate all allocated nodes
    for (auto& handle : _handles) 
    {
        if ((_metadata_pool[handle].status & 0x01) == 0x01) // Check if allocated
        {  
            Memory::Write_Word(MAP(MMU_META_HANDLE), handle);
            if ((_metadata_pool[handle].status & 0x02) == 0x02) // check if paged
            {
                Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_PG_FREE));
            }
            else
            {
                Memory::Write(MAP(MMU_COMMAND), (Byte)MAP(MMU_CMD_FREE));
            }
        }
    }

    // 3. Clear the metadata pool
    for (auto& metadata : _metadata_pool) 
    {
        metadata.status = 0;                              // Mark as unallocated
        metadata.root_node = MMU_BAD_HANDLE;              // Clear root pointer
        metadata.prev_node = MMU_BAD_HANDLE;              // Clear previous pointer
        metadata.next_node = MMU_BAD_HANDLE;              // Clear next pointer
        std::fill(std::begin(metadata.data), std::end(metadata.data), 0);  // Clear data
    }

    // 4. Clear the handles list
    _handles.clear();

    // 5. Reset MMU state variables
    _mmu_raw_index = MMU_BAD_HANDLE;  // Invalidate current node pointer
    _mmu_blocks_free = _metadata_pool.size();  // All blocks are now free
    _mmu_blocks_allocated = 0;                 // No blocks are allocated
    // Reset other state variables if applicable

    // 6. Reset MMU-related hardware registers (example, adjust as needed)
    Memory::Write_Word(MAP(MMU_PAGE_1_SELECT), (Word)0xFFFF);  // Clear page 1 mapping
    Memory::Write_Word(MAP(MMU_PAGE_2_SELECT), (Word)0xFFFF);  // Clear page 2 mapping
    Memory::Write_Word(MAP(MMU_META_HANDLE), (Word)MMU_BAD_HANDLE);

    // 7. Brute Force Version
    if (false)
    {
        for (size_t i = 0; i < _metadata_pool.size(); ++i) {
            METADATA_NODE& node = _metadata_pool[i];

            // Initialize each node to a clean state
            node.status = 0x00;  // Mark the node as free (status bit 0 = 0)
            node.root_node = MMU_BAD_HANDLE;
            node.prev_node = MMU_BAD_HANDLE;
            node.next_node = MMU_BAD_HANDLE;       
            std::fill(node.data.begin(), node.data.end(), 0);  // Clear data block
        }  
        _handles.clear();
        _mmu_raw_index = MMU_BAD_HANDLE;
        _mmu_blocks_free = 0xCCCC;
        _mmu_blocks_allocated = 0;
    }


    UnitTest::Log(clr::GREEN + "MMU has been reset successfully!" + clr::RESET);
    return MAP(MMU_CMD_RESET);
}

bool MMU::_test_reset()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_size()
{
    return MAP(MMU_CMD_SIZE);
}
bool MMU::_test_size()
{
    bool test_results = true;
    return test_results;
}

bool MMU::OnTest()
{
    bool all_tests_passed = true;

    // Perform Unit Tests for each command
    for (const auto& command : _mmu_command_list) 
    {
        if (!command.test()) 
        {
            UnitTest::Log(clr::RED + "Test failed for command: " + command.key);
            all_tests_passed = false;
        }
    }

    // Display the result of the tests
    if (all_tests_passed)
        UnitTest::Log(clr::WHITE + _device_name + clr::GREEN + " Unit Tests PASSED" + clr::RESET);
    else
        UnitTest::Log(clr::WHITE + _device_name + clr::RED + " Unit Tests FAILED" + clr::RESET);

    return all_tests_passed;
}

Word MMU::create_handle()
{
    // Look for an available node in the metadata pool
    for (size_t i = 0; i < _metadata_pool.size(); ++i) {
        METADATA_NODE& node = _metadata_pool[i];
        
        // Check if the node is free (assuming status flag bit 0 indicates allocation status)
        if ((node.status & 0x01) == 0) {  // If the node is not allocated
            node.status |= 0x01;  // Mark it as allocated
            node.status &= 0x80;  // Mask out Errors

            node.page_index = 0xff;     // 0xFF = invalid page index
            node.root_node = i;
            node.prev_node = MMU_BAD_HANDLE;
            node.next_node = MMU_BAD_HANDLE;

            // Update block counters
            _mmu_blocks_free--;           // One less block available
            _mmu_blocks_allocated++;      // One more block allocated

            // Add the node's index to the handles list as the root of the allocated memory
            _handles.push_back(i);  // `i` is the root node index (handle)

            return i;  // Return the root node index as the handle
        }
    }

    // If no free node was found, return a "null" handle or error
    return MMU_BAD_HANDLE;  
}

void MMU::deallocate_handle(Word handle)
{
    // Ensure the handle is valid
    if (handle >= _metadata_pool.size()) {
        // Handle invalid index
        UnitTest::Log(clr::RED + "Error: Invalid handle!" + clr::RESET);
        return;
    }

    Word current_node_index = handle;

    // Traverse the chain of nodes and mark them as free
    while (current_node_index != MMU_BAD_HANDLE) {
        METADATA_NODE& node = _metadata_pool[current_node_index];

        // If the node is allocated
        if (node.status & 0x01)
        {  
            // Mark the node as free (clear the allocated bit)
            node.status &= ~0x01;  // Reset the "allocated" bit to 0

            // Update block counters        
            _mmu_blocks_free++;           // One more block is free
            _mmu_blocks_allocated--;      // One less block is allocated       

            // clear the data too
            for (int i=0; i<32; ++i) { node.data[i] = 0; }
        }

        // Move to the next node in the chain
        current_node_index = node.next_node;  // Assuming next_node points to the next block
    }

    // Remove the handle from the handle list
    auto it = std::find(_handles.begin(), _handles.end(), handle);
    if (it != _handles.end()) {
        _handles.erase(it);
    }
}










/*** class BANKED_MEM *******************************************************
 * 
 *      ____    _    _   _ _  _______ ____            __  __ _____ __  __ 
 *     | __ )  / \  | \ | | |/ / ____|  _ \          |  \/  | ____|  \/  |
 *     |  _ \ / _ \ |  \| | ' /|  _| | | | |         | |\/| |  _| | |\/| |
 *     | |_) / ___ \| |\  | . \| |___| |_| |         | |  | | |___| |  | |
 *     |____/_/   \_\_| \_|_|\_\_____|____/   _____  |_|  |_|_____|_|  |_|
 *                                           |_____|       
 * 
 ****************************************************************/


Byte BANKED_MEM::bank_read(Word address)
{
    MMU* mmu = MMU::instance();

    Byte bank_num = 0;
    Word offset = address - MAP(BANKMEM_ONE);
    if (address >= MAP(BANKMEM_TWO))
    {
        bank_num = 1;
        offset = address - MAP(BANKMEM_TWO);
    }
    if ( ((Memory::Read_Word(MAP(MMU_PAGE_1_SELECT)) != 0xFFFF) && bank_num == 0) || 
         ((Memory::Read_Word(MAP(MMU_PAGE_2_SELECT)) != 0xFFFF) && bank_num == 1) )
    {
        Byte node_num = (offset / 32);
        Byte data_index = (offset % 32);
        Word bank_handle;
        if (bank_num == 0)
        {
            bank_handle = Memory::Read_Word(MAP(MMU_PAGE_1_SELECT));
            if (bank_handle >= 0xCCCC) { bank_handle = 0xCCCB; }
        }
        else
        {
            bank_handle = Memory::Read_Word(MAP(MMU_PAGE_2_SELECT));
            if (bank_handle >= 0xCCCC) { bank_handle = 0xCCCB; }
        }
        // verify that the handle is valid
        if ( (mmu->_metadata_pool[bank_handle].status & 0x03) == 0x03)
        {
            Word current_node = bank_handle;
            // if this is a valid handle
            if  ( (mmu->_metadata_pool[bank_handle].status & 0x01) && 
                  (mmu->_metadata_pool[bank_handle].root_node == bank_handle) )
            {
                // brute force search
                while (node_num > 0)
                {
                    current_node = mmu->_metadata_pool[current_node].next_node;
                    --node_num;
                    if (current_node == MMU::MMU_BAD_HANDLE) 
                    { 
                        UnitTest::Log(clr::RED + "BANKED_MEM Page Read Error!" + clr::RESET);
                        Bus::Error("BANKED_MEM Page Read Error!", __FILE__, __LINE__);
                        return 0; 
                    }
                }
                return mmu->_metadata_pool[current_node].data[data_index];
            }
        }
    }

    // default (handle 0xFFFF) read
    return Memory::memory(address); 
}

void BANKED_MEM::bank_write(Word address, Byte data)
{
    MMU* mmu = MMU::instance();

    Byte bank_num = 0;
    Word offset = address - MAP(BANKMEM_ONE);
    if (address >= MAP(BANKMEM_TWO))
    {
        bank_num = 1;
        offset = address - MAP(BANKMEM_TWO);
    }
    if ( ((Memory::Read_Word(MAP(MMU_PAGE_1_SELECT)) != 0xFFFF) && bank_num == 0) || 
         ((Memory::Read_Word(MAP(MMU_PAGE_2_SELECT)) != 0xFFFF) && bank_num == 1) )
    {
        Byte node_num = (offset / 32);
        Byte data_index = (offset % 32);
        Word bank_handle;
        if (bank_num == 0)
        {
            bank_handle = Memory::Read_Word(MAP(MMU_PAGE_1_SELECT));
            if (bank_handle >= 0xCCCC) { bank_handle = 0xCCCB; }
        }
        else
        {
            bank_handle = Memory::Read_Word(MAP(MMU_PAGE_2_SELECT));
            if (bank_handle >= 0xCCCC) { bank_handle = 0xCCCB; }
        }
        // verify that the handle is valid
        if ( (mmu->_metadata_pool[bank_handle].status & 0x03) == 0x03)
        {
            Word current_node = bank_handle;
            // if this is a valid handle
            if  ( (mmu->_metadata_pool[bank_handle].status & 0x01) && 
                  (mmu->_metadata_pool[bank_handle].root_node == bank_handle) )
            {
                while (node_num > 0)
                {
                    current_node = mmu->_metadata_pool[current_node].next_node;
                    --node_num;
                    if (current_node == MMU::MMU_BAD_HANDLE) 
                    { 
                        UnitTest::Log(clr::RED + "BANKED_MEM Page Write Error!" + clr::RESET);
                        Bus::Error("BANKED_MEM Page Write Error!", __FILE__, __LINE__);
                        return; 
                    }
                }
                mmu->_metadata_pool[current_node].data[data_index] = data;
                return;
            }
        }
    }
    // default (handle 0xFFFF) write
    Memory::memory(address, data);
}


