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
    print_log_to_console();
}


void UnitTest::Log(const std::string& message) 
{
    if (testLog.is_open()) {
        testLog << GetCurrentTime() << " - " << message << std::endl;
    }
}


void UnitTest::Assert(bool condition, const std::string& message, const char* file, int line) 
{
    if (!condition) {
        std::ostringstream ss;
        ss << "ASSERT FAILED: " << message; // << " (" << file << ":" << line << ")";
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
    // Test the entire memory range
    for (Word addr = start; addr < end; ++addr) {
        Byte origValue = Memory::Read(addr);
        Byte data = 1;  
        while (data != 0)
        {
            Memory::Write(addr, data);
            if (Memory::Read(addr) != data)
            {
                UnitTest::Log("Memory read/write failed for " + name + " at address $" + clr::hex(addr, 4));
                result = false;
            }
            data++;
        }
        Memory::Write(addr, origValue);
    }

    UnitTest::Log(name+" Unit Tests PASSED");
    return result;    
}