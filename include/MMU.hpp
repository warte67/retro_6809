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
 * optimizing system performance.
 * 
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#pragma once

// #include <vector>
#include "IDevice.hpp"

class MMU : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    MMU();
    virtual ~MMU();

public: 

    // VIRTUAL METHODS

    virtual int  OnAttach(int nextAddr) override;   // attach to the memory map
    virtual void OnInit() override;                 // initialize
    virtual void OnQuit() override;                 // shutdown

    // NOT USED:

    virtual void OnActivate() override {}
    virtual void OnDeactivate() override {}
    virtual void OnEvent(SDL_Event*) override {}
    virtual void OnUpdate(float) override {}
    virtual void OnRender() override {}
 
public: 

    // CUSTOM TYPES USED BY THIS DEVICE:
    // ...

private: 

    struct MEMORY_NODE {
        Byte block_count;   // how many 32-byte blocks are included in this allocation
        Byte status;        // status flags
                            // bit  0	Allocated:  0 = Free,   1 = Allocated
                            // bit  1   8k Paged?:  0 = No,     1 = Yes
                            // bit  2	Type:       0 = RAM,    1 = ROM
                            // bit  3   Fragmented? 0 = No,     1 = Yes
                            // bit  4   ...
                            // bit  5   ...
                            // bit  6   ...
                            // bit  7   Error:      0 = No,     1 = Yes
        Word start_block;   // index of the first block in this allocation
        Word parent_node;   // index of the parent node in the memory pool
        Word next_node;     // index of the child node in the memory pool
        Byte data[32]={0};  // 32-byte data block, preinitialized to zero
    };  

    // allocate a 2MB memory pool (Each node represents 32-bytes)
    std::vector<MEMORY_NODE> _memory_pool=std::vector<MEMORY_NODE>(65536);  
};

/**************************************

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





 */


// END: MMU.hpp