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
    mapped_register.push_back({ "MMU_1_SELECT", nextAddr, 
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
    mapped_register.push_back({ "MMU_2_SELECT", nextAddr, 
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
        [this](Word, Byte data) { _mmu_handle = (_mmu_handle & 0x00FF) | (data << 8); }, 
        { "(Word) Handle for the current allocation chain",""} });
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
        { "(Byte) Status Flags:" }}); nextAddr++;
    // ADD STATUS FLAGS ENUMERATION:
    Byte stat = 0;
    mapped_register.push_back({ "MMU_STFLG_ALLOC" , (1<<stat++),nullptr,nullptr, { "   0000'0001: Is Allocated: 0 = Free, 1 = Allocated"}}); 
    mapped_register.push_back({ "MMU_STFLG_PAGED" , (1<<stat++),nullptr,nullptr, { "   0000'0010: Paged Memory: 0 = No,   1 = Yes"}});
    mapped_register.push_back({ "MMU_STFLG_RW_RO" , (1<<stat++),nullptr,nullptr, { "   0000'0100: Memory Type:  0 = RAM,  1 = ROM"}});
    mapped_register.push_back({ "MMU_STFLG_FRAGD" , (1<<stat++),nullptr,nullptr, { "   0000'1000: Fragmented:   0 = No,   1 = Yes"}});
    mapped_register.push_back({ "MMU_STFLG_LOCKED", (1<<stat++),nullptr,nullptr, { "   0001'0000: Locked:       0 = No,   1 = Yes"}});
    mapped_register.push_back({ "MMU_STFLG_RES_1" , (1<<stat++),nullptr,nullptr, { "   0010'0000:   (reserved)"}});
    mapped_register.push_back({ "MMU_STFLG_RES_2" , (1<<stat++),nullptr,nullptr, { "   0100'0000:   (reserved)"}});
    mapped_register.push_back({ "MMU_STFLG_ERROR" , (1<<stat++),nullptr,nullptr, { "   1000'0000: Error:        0 = No,   1 = Yes",""}});
    

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
            [this,i](Word, Byte data) { _metadata_pool[_mmu_raw_index].data[i] = data; },  
            { "(32-Bytes) Data Window for the Current Allocation"} }); nextAddr++;
        }
        else
        {
            mapped_register.push_back( { "", nextAddr, 
            [this,i](Word) { return _metadata_pool[_mmu_raw_index].data[i]; },   
            [this,i](Word, Byte data) { _metadata_pool[_mmu_raw_index].data[i] = data; },  
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
std::cout << clr::indent() << clr::PURPLE << "MMU::do_command() Entry" << clr::RETURN;
    _mmu_command = command;
    // find the command and run it
    auto it = _mmu_commands.find(command);
    if (it != _mmu_commands.end()) {
        it->second();  // Call the associated function
    } else {
        error(MAP(MMU_ERR_INVALID));  // Handle invalid commands
    }    
std::cout << clr::indent() << clr::PURPLE << "MMU::do_command() Exit" << clr::RETURN;
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


Byte MMU::do_pg_alloc()
{
    // allocate 8k memory pages from the bottom of the pool towards the top
    const size_t page_size = 8192;  // 8 KB (8192 bytes)
    const size_t node_size = 32;    // Size of a single METADATA_NODE (32 bytes)
    
    const bool IS_RAM = true;        // Set this to true for RAM, false for ROM
    const Byte STORAGE_TYPE_BIT = 0x04; // Bit 2 is for the storage type (RAM = 0, ROM = 1)

    size_t total_allocated = 0;
    size_t num_nodes = 0;

    // Create a new handle (this is the root node of the chain)
    Word handle = allocate_handle();
    if (handle == MMU_BAD_HANDLE) {
        UnitTest::Log(clr::RED + "Error: Could not allocate memory handle!" + clr::RESET);
        error(MAP(MMU_ERR_ALLOC));  // Report allocation failure
        return MAP(MMU_CMD_PG_ALLOC);  // Return failure code
    }

    // Set the handle for the first node in the allocation chain
    _mmu_handle = handle;  // Update the global MMU handle with the root node's handle

std::cout << "handle: $" << clr::hex(handle, 4) << std::endl;

    // Initialize the root node's root_node (set to itself)
    _metadata_pool[handle].root_node = handle;  // The first node's root_node should point to itself

    // Start creating the chain of metadata nodes
    Word current_node_index = handle;
    Word previous_node_index = MMU_BAD_HANDLE;  // There is no previous node for the first allocation
    while (total_allocated < page_size) {
        METADATA_NODE& node = _metadata_pool[current_node_index];

        // Set the previous node for this one
        node.prev_node = previous_node_index;

        // Set the paged memory bit (bit 1 = 1, mask = 0x02)
        node.status |= 0x02;  // Mark this node as part of a paged memory allocation

        // Set the storage type bit (bit 2 = 0 for RAM, 1 for ROM)
        if (IS_RAM) {
            node.status &= ~STORAGE_TYPE_BIT;  // Ensure RAM status (bit 2 = 0)
        } else {
            node.status |= STORAGE_TYPE_BIT;   // Set ROM status (bit 2 = 1)
        }

        // Allocate one METADATA_NODE (32 bytes)
        total_allocated += node_size;
        num_nodes++;

        // If there is more space to allocate, link to the next node
        if (total_allocated < page_size) {
            Word next_node_index = allocate_handle();
            if (next_node_index != MMU_BAD_HANDLE) {
                node.next_node = next_node_index;
                previous_node_index = current_node_index;  // Update previous_node for the next iteration
                current_node_index = next_node_index;
            }
            else {
                // If there is no more space available, break out of the loop
                UnitTest::Log(clr::RED + "Error: Not enough space in the metadata pool!" + clr::RESET);
                error(MAP(MMU_ERR_ALLOC));  // Call error on allocation failure
                return MAP(MMU_CMD_PG_ALLOC);  // Return failure code
            }
        }
        else {
            break;  // We've allocated enough blocks (8 KB)
        }
    }

    // Log the allocation result (optional)
    UnitTest::Log(clr::GREEN + "Allocated " + std::to_string(num_nodes) + " metadata nodes, " +
                  std::to_string(total_allocated) + " bytes of 8KB." + clr::RESET);

    // Return the mapped command for page allocation
    return MAP(MMU_CMD_PG_ALLOC);
}

bool MMU::_test_pg_alloc()
{
    bool test_results = true;

    // since this runs immediately after initialization, we are expecting
    // _mmu_handle, _mmu_1_select, and _mmu_2_select to be initialized to MMU_BAD_HANDLE.
    if (_mmu_handle != MMU_BAD_HANDLE) {
        UnitTest::Log(clr::RED + "Error: _mmu_handle Not Initialized! $" + clr::hex(_mmu_handle, 4) + clr::RESET);
        test_results = false;
    }   
    if (_mmu_1_select != MMU_BAD_HANDLE) {
        UnitTest::Log(clr::RED + "Error: _mmu_1_select Not Initialized $" + clr::hex(_mmu_1_select, 4) + clr::RESET);
        test_results = false;
    }   
    if (_mmu_2_select != MMU_BAD_HANDLE) {
        UnitTest::Log(clr::RED + "Error: _mmu_2_select Not Initialized $" + clr::hex(_mmu_2_select, 4) + clr::RESET);
        test_results = false;
    }   

    return test_results;
}

Byte MMU::do_pg_free()
{
    // ...
    return MAP(MMU_CMD_PG_FREE);
}
bool MMU::_test_pg_free()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_alloc()
{
    // ...
    return MAP(MMU_CMD_ALLOC);
}
bool MMU::_test_alloc()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_free()
{
    // ...
    return MAP(MMU_CMD_FREE);
}
bool MMU::_test_free()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_load_root()
{
    // ...
    return MAP(MMU_CMD_LOAD_ROOT);
}
bool MMU::_test_load_root()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_load_next()
{
    // ...
    return MAP(MMU_CMD_LOAD_NEXT);
}
bool MMU::_test_load_next()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_load_prev()
{
    // ...
    return MAP(MMU_CMD_LOAD_PREV);
}
bool MMU::_test_load_prev()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_load_last()
{
    // ...
    return MAP(MMU_CMD_LOAD_LAST);
}
bool MMU::_test_load_last()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_del_node()
{
    // ...
    return MAP(MMU_CMD_DEL_NODE);
}
bool MMU::_test_del_node()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_ins_before()
{
    // ...
    return MAP(MMU_CMD_INS_BEFORE);
}
bool MMU::_test_ins_before()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_ins_after()
{
    // ...
    return MAP(MMU_CMD_INS_AFTER);
}
bool MMU::_test_ins_after()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_push_back()
{
    // ...
    return MAP(MMU_CMD_PUSH_BACK);
}
bool MMU::_test_push_back()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_push_front()
{
    // ...
    return MAP(MMU_CMD_PUSH_FRONT);
}
bool MMU::_test_push_front()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_pop_back()
{
    // ...
    return MAP(MMU_CMD_POP_BACK);
}
bool MMU::_test_pop_back()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_pop_front()
{
    // ...
    return MAP(MMU_CMD_POP_FRONT);
}
bool MMU::_test_pop_front()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_lock_node()
{
    // ...
    return MAP(MMU_CMD_LOCK_NODE);
}
bool MMU::_test_lock_node()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_unlock_node()
{
    // ...
    return MAP(MMU_CMD_UNLOCK_NODE);
}
bool MMU::_test_unlock_node()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_defrag()
{
    // ...
    return MAP(MMU_CMD_DEFRAG);
}
bool MMU::_test_defrag()
{
    bool test_results = true;
    return test_results;
}

Byte MMU::do_reset()
{
    // ...
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

    // Unit Tests
        for (const auto& command : _mmu_command_list) {
        if (!command.test()) {
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

Word MMU::allocate_handle()
{
    // Look for an available node in the metadata pool
    for (size_t i = 0; i < _metadata_pool.size(); ++i) {
        METADATA_NODE& node = _metadata_pool[i];
        
        // Check if the node is free (assuming status flag bit 0 indicates allocation status)
        if ((node.status & 0x01) == 0) {  // If the node is not allocated
            node.status |= 0x01;  // Mark it as allocated

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

        // Mark the node as free (clear the allocated bit)
        node.status &= ~0x01;  // Reset the "allocated" bit to 0

        _mmu_blocks_free++;           // One more block is free
        _mmu_blocks_allocated--;      // One less block is allocated        

        // Move to the next node in the chain
        current_node_index = node.next_node;  // Assuming next_node points to the next block
    }

    // Remove the handle from the handle list
    auto it = std::find(_handles.begin(), _handles.end(), handle);
    if (it != _handles.end()) {
        _handles.erase(it);
    }
}