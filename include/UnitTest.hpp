/*** UnitTest.hpp ****************************
 *      _    _       _ _ _______        _         _                 
 *     | |  | |     (_) |__   __|      | |       | |                
 *     | |  | |_ __  _| |_ | | ___  ___| |_      | |__  _ __  _ __  
 *     | |  | | '_ \| | __|| |/ _ \/ __| __|     | '_ \| '_ \| '_ \ 
 *     | |__| | | | | | |_ | |  __/\__ \ |_   _  | | | | |_) | |_) |
 *      \____/|_| |_|_|\__||_|\___||___/\__| (_) |_| |_| .__/| .__/ 
 *                                                     | |   | |    
 *                                                     |_|   |_|    
 *  
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 * 
 *************************************/

#pragma once

#include <cstdio>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include "types.hpp"


// Unit Test Constants:
#define ASSERT(condition, message) \
    UnitTest::Assert(condition, message, __FILE__, __LINE__)    

class UnitTest {
private:
    UnitTest();
    ~UnitTest();

public: // PUBLIC SINGLETON STUFF

    UnitTest(const UnitTest&) = delete;				    // delete the copy constructor
    UnitTest(UnitTest&&) = delete;					    // delete the move constructor
    UnitTest& operator=(const UnitTest&) = delete;	    // delete the copy assignment operator
    UnitTest& operator=(UnitTest&&) = delete;			// delete the move assignment operator
	static UnitTest& GetInstance() { static UnitTest inst; return inst; }

    // Test Log Functions
    static void Init(const std::string& filename = "test.log");
    static void Quit();
    static void Log(const std::string& message);
    static void Assert(bool condition, const std::string& message, const char* file, int line);
    static void print_log_to_console();

    static bool RangeTest_RW(std::string name, Uint16 start, Uint16 end);

private:

    // Time format helper
    static std::string GetCurrentTime();

    // Log file stream
    inline static std::ofstream testLog;
};

