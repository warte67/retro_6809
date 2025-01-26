/*** MMU.hpp *******************************************
 *      __  __ __  __ _    _       _                 
 *     |  \/  |  \/  | |  | |     | |                
 *     | \  / | \  / | |  | |     | |__  _ __  _ __  
 *     | |\/| | |\/| | |  | |     | '_ \| '_ \| '_ \ 
 *     | |  | | |  | | |__| |  _  | | | | |_) | |_) |
 *     |_|  |_|_|  |_|\____/  (_) |_| |_| .__/| .__/ 
 *                                      | |   | |    
 *                                      |_|   |_|  
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

#pragma once

#include <bitset>
#include <string>
#include <map>
#include <utility>
#include "IDevice.hpp"
#include "UnitTest.hpp"

class MMU : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    MMU();
    virtual ~MMU();

public: 

    // VIRTUAL METHODS:

    virtual int  OnAttach(int nextAddr) override;   // attach to the memory map
    virtual void OnInit() override;                 // initialize
    virtual void OnQuit() override;                 // shutdown
    virtual bool OnTest() override;                 // return true for successful unit tests

    // NOT USED:

    virtual void OnActivate() override {}
    virtual void OnDeactivate() override {}
    virtual void OnEvent(SDL_Event*) override {}
    virtual void OnUpdate(float) override {}
    virtual void OnRender() override {}

private: 

    void error(Byte error_code);
    void do_command(Byte command);
    Byte do_nop();
    Byte do_pg_alloc();
    Byte do_pg_free();
    Byte do_alloc();
    Byte do_free();
    Byte do_load_root();
    Byte do_load_next();
    Byte do_load_prev();
    Byte do_load_last();
    Byte do_del_node();
    Byte do_ins_before();
    Byte do_ins_after();
    Byte do_push_back();
    Byte do_push_front();
    Byte do_pop_back();
    Byte do_pop_front();
    Byte do_lock_node();
    Byte do_unlock_node();
    Byte do_defrag();
    Byte do_reset();
    Byte do_size();

    bool _test_nop();
    bool _test_pg_alloc();  
    bool _test_pg_free();   
    bool _test_alloc();    
    bool _test_free();      
    bool _test_load_root(); 
    bool _test_load_next(); 
    bool _test_load_prev(); 
    bool _test_load_last(); 
    bool _test_del_node();  
    bool _test_ins_before(); 
    bool _test_ins_after(); 
    bool _test_push_back(); 
    bool _test_push_front(); 
    bool _test_pop_back();  
    bool _test_pop_front(); 
    bool _test_lock_node(); 
    bool _test_unlock_node(); 
    bool _test_defrag();    
    bool _test_reset();     
    bool _test_size();

    std::unordered_map<Byte, std::function<Byte()>> _mmu_commands;
    inline void register_command(Byte command, std::function<Byte()> handler)
    {
        if (_mmu_commands.find(command) != _mmu_commands.end()) {
            UnitTest::Log("Warning: Command already registered: " + command);
            Bus::Error("Command already registered", __FILE__, __LINE__);
            return;
        }        
        _mmu_commands[command] = handler;
    }


    // ****************** //
    // HARDWARE REGISTERS //
    // ****************** //


    Word MMU_BAD_HANDLE = 0xFFFF;

    Word _mmu_1_select = MMU_BAD_HANDLE;    
    // MMU_1_SELECT                 ; (Word) Page Select for 8K Memory Bank 1

    Word _mmu_2_select = MMU_BAD_HANDLE;    
    // MMU_2_SELECT                 ; (Word) Page Select for 8K Memory Bank 2

    Word _mmu_blocks_free = 0xCCCC;
    // MMU_BLOCKS_FREE              ; (Word) Number of 32-Byte Blocks Available for Allocation

    Word _mmu_blocks_allocated = 0;
    // MMU_BLOCKS_ALLOCATED         ; (Word) Number of 32-Byte Blocks Currently Allocated

    Word _mmu_blocks_fragged = 0;
    // MMU_BLOCKS_FRAGGED           ; (Word) Number of 32-Byte Blocks Currently Fragmented

    Word _mmu_arg_1 = 0;
    // MMU_ARG_1                    ; (Word)  Argument 1 for MMU Command

    Word _mmu_arg_2 = 0;
    // MMU_ARG_2                    ; (Word)  Argument 2 for MMU Command

    Byte _mmu_command = 0;  

    struct CommandInfo {
        std::string key;
        std::string description;
        std::function<Byte()> action;  // Function pointer for the command
        std::function<bool()> test;    // Function pointer for the unit test
    };

    std::vector<CommandInfo> _mmu_command_list = {
    //    CONSTANT             , DESCRIPTION                                , FUNCTION POINTER FOR THE COMMAND,             , FUNCTION POINTER FOR THE UNIT TEST
        {"MMU_CMD_NOP"        , "No Operation / Error",                     [this]() -> Byte { return do_nop(); },          [this]() -> bool { return _test_nop(); }},
        {"MMU_CMD_PG_ALLOC"   , "Page Allocate (8K Bytes)",                 [this]() -> Byte { return do_pg_alloc(); },     [this]() -> bool { return _test_pg_alloc(); }},
        {"MMU_CMD_PG_FREE"    , "Page Deallocate (8K Bytes)",               [this]() -> Byte { return do_pg_free(); },      [this]() -> bool { return _test_pg_free(); }},
        {"MMU_CMD_ALLOC"      , "Allocate Chain (< 8K Bytes)",              [this]() -> Byte { return do_alloc(); },        [this]() -> bool { return _test_alloc(); }},
        {"MMU_CMD_FREE"       , "Deallocate Chain (< 8K Bytes)",            [this]() -> Byte { return do_free(); },         [this]() -> bool { return _test_free(); }},
        {"MMU_CMD_LOAD_ROOT"  , "Load Root Node",                           [this]() -> Byte { return do_load_root(); },    [this]() -> bool { return _test_load_root(); }},
        {"MMU_CMD_LOAD_NEXT"  , "Load Next Node",                           [this]() -> Byte { return do_load_next(); },    [this]() -> bool { return _test_load_next(); }},
        {"MMU_CMD_LOAD_PREV"  , "Load Prev Node",                           [this]() -> Byte { return do_load_prev(); },    [this]() -> bool { return _test_load_prev(); }},
        {"MMU_CMD_LOAD_LAST"  , "Load Last Node",                           [this]() -> Byte { return do_load_last(); },    [this]() -> bool { return _test_load_last(); }},
        {"MMU_CMD_DEL_NODE"   , "Remove Current Node (and Adjust Links)",   [this]() -> Byte { return do_del_node(); },     [this]() -> bool { return _test_del_node(); }},
        {"MMU_CMD_INS_BEFORE" , "Insert Node Before (and activate)",        [this]() -> Byte { return do_ins_before(); },   [this]() -> bool { return _test_ins_before(); }},
        {"MMU_CMD_INS_AFTER"  , "Insert Node After (and activate)",         [this]() -> Byte { return do_ins_after(); },    [this]() -> bool { return _test_ins_after(); }},
        {"MMU_CMD_PUSH_BACK"  , "Push Back (and activate)",                 [this]() -> Byte { return do_push_back(); },    [this]() -> bool { return _test_push_back(); }},
        {"MMU_CMD_PUSH_FRONT" , "Push Front (and activate)",                [this]() -> Byte { return do_push_front(); },   [this]() -> bool { return _test_push_front(); }},
        {"MMU_CMD_POP_BACK"   , "Pop Back (and activate)",                  [this]() -> Byte { return do_pop_back(); },     [this]() -> bool { return _test_pop_back(); }},
        {"MMU_CMD_POP_FRONT"  , "Pop Front (and activate)",                 [this]() -> Byte { return do_pop_front(); },    [this]() -> bool { return _test_pop_front(); }},
        {"MMU_CMD_LOCK_NODE"  , "Lock Node",                                [this]() -> Byte { return do_lock_node(); },    [this]() -> bool { return _test_lock_node(); }},
        {"MMU_CMD_UNLOCK_NODE", "Unlock Node",                              [this]() -> Byte { return do_unlock_node(); },  [this]() -> bool { return _test_unlock_node(); }},
        {"MMU_CMD_DEFRAG"     , "Defragment / Collect Garbage",             [this]() -> Byte { return do_defrag(); },       [this]() -> bool { return _test_defrag(); }},
        {"MMU_CMD_RESET"      , "Reset Memory Management Unit",             [this]() -> Byte { return do_reset(); },        [this]() -> bool { return _test_reset(); }},
        {"MMU_CMD_SIZE"       , "Total Number of MMU Commands",             [this]() -> Byte { return do_size(); },         [this]() -> bool { return _test_size(); }}
    };

    Byte _mmu_error = 0;
    std::vector<std::pair<std::string, std::string>> _mmu_error_list = {
        { "MMU_ERR_NONE",       "No Error" },
        { "MMU_ERR_OUTOFMEM",   "Out of Memory Error" },
        { "MMU_ERR_ALLOC",      "Failed to Allocate Memory" },
        { "MMU_ERR_FREE",       "Failed to Deallocate Memory" },
        { "MMU_ERR_MAPPING",    "Memory Mapping Error" },
        { "MMU_ERR_UNMAPPING",  "Error Unmapping Memory" },
        { "MMU_ERR_INVALID",    "Invalid Command" },
        { "MMU_ERR_ARGUMENT",   "Invalid Argument" },
        { "MMU_ERR_HANDLE",     "Invalid Handle" },
        { "MMU_ERR_NODE",       "Invalid Node" },
        { "MMU_ERR_PAGE",       "Invalid Page" },
        { "MMU_ERR_BANK",       "Invalid Bank" },
        { "MMU_ERR_ADDRESS",    "Invalid Address" },
        { "MMU_ERR_OFFSET",     "Invalid Offset" },
        { "MMU_ERR_LENGTH",     "Invalid Length" },
        { "MMU_ERR_INDEX",      "Invalid Index" },
        { "MMU_ERR_UNKNOWN",    "Unknown Error" },
        { "MMU_ERR_SIZE",       "Total Number of MMU Errors" }
    };

    Word _mmu_handle = MMU_BAD_HANDLE;
    // MMU_HANDLE                   ; (Word) Handle for the current allocation chain

    // Byte _mmu_status = 0;
    // MMU_STATUS                   ; (Byte) Status flags:
    // MMU_STFLG_ALLOC              ;    0000'0001: Is Allocated: 0 = Free, 1 = Allocated
    // MMU_STFLG_PAGED              ;    0000'0010: Paged Memory: 0 = No,   1 = Yes
    // MMU_STFLG_READONLY           ;    0000'0100: Memory Type:  0 = RAM,  1 = ROM
    // MMU_STFLG_FRAGD              ;    0000'1000: Fragmented:   0 = No,   1 = Yes
    // MMU_STFLG_LOCKED             ;    0001'0000: Locked:       0 = No,   1 = Yes
    // MMU_STFLG_RES_1              ;    0010'0000:   (reserved)
    // MMU_STFLG_RES_2              ;    0100'0000:   (reserved)
    // MMU_STFLG_ERROR              ;    1000'0000: Error:        0 = No,   1 = Yes

    std::array<Byte,32> _meta_data{0};
    // MMU_META_DATA                ; (32-Bytes) 32-Byte data for the current allocation chain

    Word _mmu_raw_index = 0;
    // MMU_RAW_INDEX                ; Raw Index of the current memory node (For Debugging)

    // *************** //
    // UNIT TEST STUFF //
    // *************** //

    Word _test_rw_handle   = 0xFFFF;
    Word _test_ro_handle   = 0xFFFF;
    Word _test_lock_handle = 0xFFFF;
    Word _test_page_handle = 0xFFFF;

    // ************************* //
    // DATA TYPES AND STRUCTURES //
    // ************************* //

    Word create_handle();
    void deallocate_handle(Word handle);
    
    std::vector<Word> _handles;  // Holds the allocated memory handles (root node indices)


    struct METADATA_NODE {
        Byte reserved;              // padding to align to 8-bytes (reserved for future use)
        Byte status;                // status flags
                                    // bit  0   Is Allocated:     0 = Free,   1 = Allocated
                                    // bit  1   8k Paged Memory:  0 = No,     1 = Yes
                                    // bit  2	Storage Type:     0 = RAM,    1 = ROM
                                    // bit  3   Is Fragmented?    0 = No,     1 = Yes
                                    // bit  4   Is Locked?        0 = No,     1 = Yes
                                    // bit  5   (reserved)
                                    // bit  6   (reserved)
                                    // bit  7   Error:            0 = No,     1 = Yes
        std::array<Byte,32> data = {0};                                    
        Word root_node;             // index of the first block in this allocation
        Word prev_node;             // index of the previous node in this allocation
        Word next_node;             // index of the child node in this allocation        
    };
    // Allocate within the 2MB memory pool (Each METADATA_NODE represents 8-bytes)
    // the metadata pool will be 52'428 * 8 = 419'424 bytes.
    // MMU_MEMORY_SIZE = 52'428
    std::vector<METADATA_NODE> _metadata_pool = std::vector<METADATA_NODE>(MMU_MEMORY_SIZE);


    //          DATA pool = 1'677'696 bytes
    //      METADATA pool =   419'424 bytes
    //             equals = 2'097'120 bytes
    //                2MB = 2'097'152 bytes
    // the remaining 32 bytes left within the 2MB pool are reserved for flags
    struct PageFlags {
        std::bitset<256> flags; // 256 bits (32 bytes) to represent flags for each 32-byte block.
    };


    Word _dynamic_memory_top = MMU_MEMORY_SIZE;     // top of dynamic memory $CCCC = 52'428
    Word _dynamic_memory_bottom = 0x0000;           // bottom of dynamemic memory, 
                                                    //     preinitialized to zero for now
};






/**************************************+

Consideration for Electronic Components:
    Raspberry Pi Pico2 Board One -- Core 1:
        1) CPU Emulation
        2) 64KB CPU addressable memory
        3) SPI Controller between PICO2 Board 1 and Board 2 (aka. Interrupts)
        4) Secondary SPI Controller for 2MB Page Banked Memory (aka. DMA)

    Raspberry Pi Pico2 Board One -- Core 2:
        1) GPU Graphics Processor
        2) 64KB GPU addressable memory
        3) Dynamic Memory Management of 64KB video memory
            - Video Display Buffer
            - Tilemap Buffer
            - Sprite Buffer (and sprite list)
            - Tile Buffer
        4) Common Board One Hardware Register Processing

    Raspberry PI Pico2 Board Two -- Core 1:
        1) FM Audio Synthesis (SID Emulation)

    Raspberry PI Pico2 Board Two -- Core 2:
        1) Input / Output
        2) SPI Controller between PICO2 Board 1 and Board 2 (aka. Interrupts)
        3) Serial Communication With Host Computer for Debugging
        4) USB Devices
        5) Common Board One Hardware Register Processing


GPIO Expander Integration:
    Use SPI-compatible GPIO expanders like the MCP23S17:
        Each chip provides 16 GPIO ports, so two chips give you 32 ports.
        Use the second SPI controller for dedicated communication with these expanders.
    Address each expander via its unique chip select (CS) line.


Bank Switching Logic:

    Keep the memory access on a second SPI controller to isolate it from inter-board communication.
    Implement bank switching in software, with a dedicated register to select the active 8KB page.
    SPI SRAM such as Microchip 23LC1024 (or multiple for 2MB) works well here.

        APS1604M-SQ-SN DRAM IoT RAM 16Mb QSPI/SPI (x4, x1) SDR 144MHz, 1.8v  $1.26 each
        https://www.mouser.com/datasheet/2/1127/APM_PSRAM_QSPI_APS1604M_SQ_v2_7_PKG-1954808.pdf


SPI Lines (MOSI, MISO, SCK, and CS) -- Level Shifting Components:

    Bidirectional Level Shifters: You can use dedicated bidirectional level 
    shifters to translate the 3.3V signals from the Pico to the required 1.8V 
    for the PSRAM and vice versa. Popular ICs for this include:

        - TXB0108 (for bidirectional signaling on all SPI lines).
        - BSS138-based MOSFET level shifters (commonly used for simple low-speed 
            logic level shifting). 
        - 74LVC245 or 74LVC125 for unidirectional shifting 
            (may be used for MISO and SCK, but you'd need separate shifting for 
            bidirectional data lines like MOSI).

    * Use a bi-directional level shifter on the MOSI and MISO lines because 
        these lines need to work in both directions. 
    * Use unidirectional level shifters on SCK and CS lines, since these are 
        output-only signals from the Pico to the PSRAM.     


SPI Lines (MOSI, MISO, SCK, and CS) -- Level Shifting Components:
    Since the APS1604M-SQ operates between 1.62V and 1.98V, you'll also need to 
    supply it with the proper voltage. The most straightforward solution is to 
    use a low dropout regulator (LDO) to drop 3.3V down to the required 1.8V.
        * A suitable LDO regulator (such as the AMS1117-1.8 or TPS7A02) would 
            provide a stable 1.8V output.


Power Supply Decoupling:
    PSRAM chips like the APS1604M-SQ are sensitive to power fluctuations, so 
    make sure to place appropriate decoupling capacitors close to the PSRAM's 
    power pins (VCC and VSS) to filter out noise and ensure stable operation. 
    A typical range for these is:
        * 0.1µF (ceramic capacitor) for high-frequency decoupling.
        * 10µF (electrolytic or tantalum) for low-frequency decoupling.   


PCB Design Considerations:
    1) Signal Integrity: Ensure your SPI traces are as short and direct as 
        possible to minimize noise and signal degradation, especially given 
        that the Pico is operating at 3.3V while the PSRAM expects 1.8V.
    2) Grounding: Keep the grounds of the level shifters, Pico, and PSRAM 
        connected to a solid common ground plane to reduce potential issues 
        with noise or voltage differences.


Conclusion:
    To integrate the APS1604M-SQ with the PICO2, the main tasks are:

    1) Level shifting the SPI lines between 3.3V (Pico) and 1.8V (PSRAM).
    2) Using an LDO regulator to provide a stable 1.8V supply to the PSRAM.
    3) Implementing decoupling capacitors to ensure clean power to the PSRAM.




std::vector<std::vector<Byte>> _fast_ram(65536, std::vector<Byte>(32, 0));



To-Do: Memory Management and C++ Code Refactoring

    Implement Memory Banking Register:
        Refactor memory bank management with simple 2-state tracking: RAM vs ROM.
        Use a small array of bytes or std::bitset to track 256 x 8KB pages (RAM or ROM).
        Remove persistent storage type (flash memory) handling for simplification.
        Ensure memory type switching (RAM/ROM) is quick and handled via registers.

    Refactor to Use std::bitset for Page Management:
        Replace the manual bit flag handling with std::bitset<numPages> to track 
        memory page types (RAM vs ROM). Use std::bitset for clean bit-level manipulation 
        with page index operations. Simplify the code by leveraging set and test methods 
        for fast page type updates.

    Ensure Use of Standard C++ Libraries:
        Rely on std::bitset to reduce the likelihood of new bugs and improve code reliability.
        Keep the code efficient and maintainable by using well-tested and optimized 
        standard libraries.Enhance portability and readability, and avoid custom, 
        error-prone bit manipulation.

    Memory Management Considerations:
        Set up dynamic memory allocation in 32-byte blocks (using 16-bit pseudo-pointers).
        Allow for a "fast" RAM window for direct access to small blocks (32 bytes). Implement 
        slower, full-page (8KB) load/save operations with RAII for larger memory needs.

    Fast Dynamic RAM Setup:
        Implement "Fast RAM" as a 2D vector:

    std::vector<std::vector<Byte>> _fast_ram(65536, std::vector<Byte>(32, 0));

    Use this std::vector structure to simulate fast, 32-byte blocks that can be accessed 
    directly. This "Fast RAM" will represent a 2MB space broken into 256 x 8KB blocks, with each
    block further broken down into 32-byte sections for rapid access. Ensure that when the 
    CPU accesses these blocks, it can quickly read/write to a 32-byte window, improving speed 
    for smaller memory operations.

Optimize and Test the Memory Management Flow:
    Ensure the switch between RAM/ROM and the 2MB SPI memory is well-handled. Confirm that 
    memory mapping, bank switching, and dynamic memory allocation perform efficiently.
    Test the interaction between registers, page switching, and external memory (SPI-based SRAM 
    or PSRAM). Verify that the "Fast RAM" block system behaves as expected for rapid memory 
    operations.

    Four Bits for Error Codes and Four Bits for Commands:
    bit 7:  0=Read/Write, 1=Read Only
    bit 6:  0=allocated, 1=free
    bit 5:  0=not currently mapped, 1=mapped to the 32-byte fast mem register
    bit 4:  0=not currently mapped, 1=mapped into one of the slow 8k pages    
    bits 0-3:  16 commands: On Read = etch last command), On Write Issue a New Command:
        Command 0 = Memory Allocation Error (Read Only)
        Command 1 = Allocate Memory
        Command 2 = Deallocate Memory
        Command 3 = Load a Block of memory into the 32-byte "window" register
        Command 4 = Save a Block of memory from the 32-byte "window" register
        Command 5 = Copy a block of memory from the 32-byte "Window" to another memory node
        Command 6 = Copy a block of memory from one memory node to the 32-byte "Window"
        Command 7 = Copy a block of memory from one memory node to another memory node
        ... etc

    Byte for Per Node Memory Status:
        0	Allocated: 0 = Free, 1 = Allocated	Tracks whether the block is in use.
        1	Read/Write or Read-Only: 0 = RW, 1 = RO	Distinguishes between writable and immutable memory.
        2	Mapped to Fast Memory: 0 = No, 1 = Yes	Indicates if the block is mapped to the 32-byte fast memory register.
        3	Mapped to Paged Memory: 0 = No, 1 = Yes	Indicates if the block is mapped to the 8k page.
        4	Fragmentation Marker	Marks blocks that are fragmented or part of a split allocation for debugging or defragmentation purposes.
        5	Temporary Allocation Flag	Marks blocks as temporarily allocated (e.g., for a short-term operation, scratch memory).
        6	Cacheable Flag	Marks whether the block is cacheable (useful in performance tuning).
        7	Error State: 0 = No Error, 1 = Error	Indicates if an error has occurred with this block.        


    // (note:  Words have been defined as unsigned shorts or Uint16
    //         Bytes have been defined as unsigned chars or Uint8)
    struct MEMORY_NODE {
        Byte block_count;   // how many 32-byte blocks are included in this allocation
        Byte status;        // status flags
        Word start_block;   // index of the first block in this allocation
        Word parent_node;   // index of the parent node in the memory pool
        Word next_node;     // index of the child node in the memory pool
    };

    Exactly 8-Bytes in size. This should fit well within the 64-bit nature of the
    software emulation as well as either 16-bit or 32-bit nature of the 
    Raspberry PI PICO boards.


=================================================================================

Updated To-Do List: Memory System Implementation
1. Memory Architecture

    Fixed Memory:
        Reserve 34 KB fixed RAM below the paged memory region (for stack, text buffer, etc.).
    Paged Memory:
        Reserve 16 KB for two default 8 KB pages (Page 0 and Page 1).
        Implement mechanism for allocating additional 8 KB pages as RAM or ROM.
    Dynamic Memory:
        Design a system for allocating 32-byte blocks from the remaining memory pool (~1,934 KB after reserved areas).

2. Banked Memory Mechanisms

    Page-Based Allocation:
        Allow allocation of 8 KB pages as RAM or ROM.
        Support loading ROM data during boot or runtime from external media.
    Dynamic Allocation:
        Implement a 32-byte block allocator using:
            std::vector<MEMORY_NODE> to track allocated and free blocks.
            MEMORY_NODE Structure (8 bytes each):
                Byte block_count: Number of 32-byte blocks in the allocation.
                Byte status: Allocation, read-only, or mapping status flags.
                Word start_block: Start index of the allocation.
                Word parent_node and Word next_node: Linked list pointers for tracking allocations.

3. Register and Hardware Design

    Memory Control Registers:
        BANK_1_SELECT and BANK_2_SELECT: Select 8 KB pages for Page 1 and Page 2.
        BANK_FAST_INDEX: 16-bit register for selecting blocks in the 32-byte "Window."
        BANK_FAST_WINDOW: 32-byte register for direct memory access to dynamic blocks.
    Simplified Status and Commands:
        Use separate status and command/error registers to manage memory nodes.
        Include flags for:
            Read/Write, Read-Only, Mapped, Allocated, Dirty, or Internal Use.

4. User Interface for Programmers

    ROM Allocation:
        Enable allocation of multiple 8 KB pages as ROM.
        Provide functionality for loading ROM data at boot or runtime.
    RAM Allocation:
        Enable allocation of multiple 8 KB pages for large working data.
        Support deallocation and reuse of these pages.
    Dynamic Memory Allocation:
        Use the 32-byte block allocator for fine-grained, temporary memory needs.
        Ensure memory fragmentation is minimized using a linked list or heap structure.
    Safety Features:
        Prevent users from changing pages while dynamic memory is buffered in paged memory regions.

5. Implementation Tasks

    Page Mechanisms:
        Write functions to allocate/deallocate 8 KB pages.
        Track page allocations with bitmaps or lightweight data structures.
    Dynamic Memory System:
        Develop the 32-byte block allocator with linked list support.
        Implement functions for allocating, deallocating, and mapping memory nodes.
        Test and debug garbage collection (e.g., using the "dirty" bit and deferred deallocation).
    Performance Optimization:
        Optimize mapping to 32-byte fast memory for rapid access.
        Implement safe access patterns for paged memory to avoid programmer pitfalls.


 ************/


// END: MMU.hpp