/*** BankedMem.hpp *******************************************
 *      ____              _            _ __  __                    _                 
 *     |  _ \            | |          | |  \/  |                  | |                
 *     | |_) | __ _ _ __ | | _____  __| | \  / | ___ _ __ ___     | |__  _ __  _ __  
 *     |  _ < / _` | '_ \| |/ / _ \/ _` | |\/| |/ _ \ '_ ` _ \    | '_ \| '_ \| '_ \ 
 *     | |_) | (_| | | | |   <  __/ (_| | |  | |  __/ | | | | |_  | | | | |_) | |_) |
 *     |____/ \__,_|_| |_|_|\_\___|\__,_|_|  |_|\___|_| |_| |_(_) |_| |_| .__/| .__/ 
 *                                                                      | |   | |    
 *                                                                      |_|   |_|
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

#pragma once

#include "IDevice.hpp"

class BankedMem : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    BankedMem();
    virtual ~BankedMem();

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

    enum BANK_TYPE {
        RANDOM_ACCESS=0,    // standard random access memory (RAM)
        PERSISTANT,         // persistant RAM... data is loaded and saved
        READ_ONLY,          // loaded on boot... read only memory (ROM)
        BANK_TYPE_MAX       // Count of these enumerated types
    };
    struct BANK_NODE {
        BANK_TYPE type = BANK_TYPE::PERSISTANT;  
        DWord seek_pos;     // byte position of this bank data within the file
    };
    struct BANK_HEADER {
        char id_string[7] = {"M_BANK"};
        float version = 0.0f;
        Byte bank_1_index = 0;
        Byte bank_2_index = 1;
        Word exec_vector = 0;
        BANK_NODE bank_node[256] ;
        // Byte bank_data[256][8192];
    };

    // PUBLIC METHODS:

    void set_bank_1_page(Byte page);
    void set_bank_2_page(Byte page);
    Byte get_bank_1_page()                  { return _bank_header.bank_1_index; }
    Byte get_bank_2_page()                  { return _bank_header.bank_2_index; }
    void set_bank_1_type(BANK_TYPE type)    { _bank_header.bank_node[get_bank_1_page()].type = type; }
    void set_bank_2_type(BANK_TYPE type)    { _bank_header.bank_node[get_bank_2_page()].type = type; }
    BANK_TYPE get_bank_1_type()             { return _bank_header.bank_node[get_bank_1_page()].type; }
    BANK_TYPE get_bank_2_type()             { return _bank_header.bank_node[get_bank_2_page()].type; }

private: 

    // PRIVATE MEMBERS

    BANK_HEADER _bank_header;

    bool _fileExists(const std::string& filename);  // returns true if the file exists
    bool _newDefaultFile();         // create a new 'paged.mem' bank file if not exists
    bool _loadHeader();             // load the header info from the 'paged.mem' file
    bool _saveHeader();             // save the header info to the 'paged.mem' file
    FILE* _fopen(const std::string filename);       // open a file with error handling
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


 */


// END: BankedMem.hpp