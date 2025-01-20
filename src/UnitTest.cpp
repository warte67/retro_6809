/*** UnitTest.cpp ****************************
 *      _    _       _ _ _______        _                          
 *     | |  | |     (_) |__   __|      | |                         
 *     | |  | |_ __  _| |_ | | ___  ___| |_        ___ _ __  _ __  
 *     | |  | | '_ \| | __|| |/ _ \/ __| __|      / __| '_ \| '_ \ 
 *     | |__| | | | | | |_ | |  __/\__ \ |_   _  | (__| |_) | |_) |
 *      \____/|_| |_|_|\__||_|\___||___/\__| (_)  \___| .__/| .__/ 
 *                                                    | |   | |    
 *                                                    |_|   |_|     
 *  
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 * 
 *************************************/

#include "Bus.hpp"

#include "UnitTest.hpp"

UnitTest::UnitTest()
{
    // ...
}

UnitTest::~UnitTest()
{
    // ...
}


void UnitTest::Init(const std::string& filename) 
{
    testLog.open(filename, std::ios::out | std::ios::trunc);
    if (!testLog.is_open()) 
    {
        std::string err_msg = "Failed to open test log file: " + filename; 
        Bus::Error(err_msg, __FILE__, __LINE__);
    }
    Log("Test Log Initialized");
}


void UnitTest::Quit() 
{
    if (testLog.is_open()) {
        Log("Test Log Closed");
        testLog.close();
    }
    // print_log_to_console();
}


void UnitTest::Log(const std::string& message) 
{
    if (testLog.is_open()) {
        testLog << GetCurrentTime() << " - " << message << std::endl;

        std::cout << clr::indent() << clr::DARK << GetCurrentTime() << " - " << message << std::endl;
    }
}


void UnitTest::Assert(bool condition, const std::string& message, const char* file, int line) 
{
    if (!condition) {
        std::ostringstream ss;
        ss << clr::RED << "ASSERT FAILED: " << message << clr::RESET; // << " (" << file << ":" << line << ")";
        Log(ss.str());
        Bus::Error(ss.str(), file, line);
        // std::cerr << ss.str() << std::endl;
        // exit(EXIT_FAILURE);  // Stop execution if assertion fails
    }
}


void UnitTest::print_log_to_console() 
{
    FILE *file = fopen("test.log", "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    } else {
        printf("Failed to open test.log\n");
    }
}

std::string UnitTest::GetCurrentTime() 
{
    std::time_t now = std::time(nullptr);
    char* dt = std::ctime(&now);
    dt[strlen(dt) - 1] = '\0'; // Remove newline character
    return std::string(dt);
}

bool UnitTest::RangeTest_RW(std::string name, Word start, Word end)
{
    bool result = true;
    std::vector<Byte> testValues = {
        0b00000000, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 
        0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b10101010, 
        0b01010101, 0b11001100, 0b00110011, 0b11110000, 0b00001111, 
        0b11111111
    };
    for (Word addr = start; addr < end; ++addr) {
        Byte originalValue = Memory::Read(addr);  // Read the current value
        bool testFailed = false;
        for (Byte pattern : testValues) {
            if (originalValue != pattern) {  // Only write if the value is different
                Memory::Write(addr, pattern);
                if (Memory::Read(addr) != pattern) {
                    result = false;
                    testFailed = true;
                    UnitTest::Log(clr::RED + "Read/Write failed at address $" + clr::hex(addr, 4) + clr::RESET );
                    break;
                }
            }
        }
        // Restore original value
        Memory::Write(addr, originalValue);
        if (testFailed) { break; }
    }
    if (result)
        UnitTest::Log(clr::GREEN + name + " Read/Write Test PASSED" + clr::RESET);
    else
        UnitTest::Log(clr::RED + name + " Read/Write Test FAILED" + clr::RESET);
    return result;
}


bool UnitTest::RangeTest_RO(std::string name, Word start, Word end)
{
    bool result = true;
    std::vector<Byte> testValues = {
        0b00000000, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 
        0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b10101010, 
        0b01010101, 0b11001100, 0b00110011, 0b11110000, 0b00001111, 
        0b11111111
    };
    for (Word addr = start; addr < end; ++addr) {
        Byte originalValue = Memory::Read(addr);  // Read the current value before writing
        bool testFailed = false;
        for (Byte pattern : testValues) {
            if (originalValue != pattern) {  // Only attempt to write if the value is different
                Memory::Write(addr, pattern);  // Try writing to the read-only memory
                // Check if write was allowed (shouldn't be for read-only)
                if (Memory::Read(addr) == pattern) {
                    result = false;
                    testFailed = true;
                    UnitTest::Log(clr::RED + "Write allowed to read-only memory at address $" + clr::hex(addr, 4) + " with pattern " + clr::hex(pattern, 2) + clr::RESET);
                    break;
                }
            }
        }
        if (testFailed) { break; }
    }
    if (result)
        UnitTest::Log(clr::GREEN +name + " Read-Only Test PASSED" + clr::RESET);
    else
        UnitTest::Log(clr::RED + name + " Read-Only Test FAILED" + clr::RESET);
    return result;
}