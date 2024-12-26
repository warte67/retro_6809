/****************************************************************
 *                      _                         
 *                     (_)                        
 *      _ __ ___   __ _ _ _ __    ___ _ __  _ __  
 *     | '_ ` _ \ / _` | | '_ \  / __| '_ \| '_ \ 
 *     | | | | | | (_| | | | | || (__| |_) | |_) |
 *     |_| |_| |_|\__,_|_|_| |_(_)___| .__/| .__/ 
 *                                   | |   | |    
 *                                   |_|   |_|    
 *    
 *---------------------------------------------------------------
 *
 *      Build Options:
 *          make            # builds with debug symbols
 *          make release=0  # also builds with debug symbols
 *          make release=1  # builds for performance but without debug
 * 
 *          (build with -std=c++23)
 *
 *      Visual Studio Extensions:
 *          C/C++ Extension Pack
 *          Makefile Tools
 *          6x09 Assembly`
 *
 *      Linux Dependencies
            sudo apt install libsdl2-dev libsdl2-gfx-dev libsdl2-image-dev
            sudo apt install libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev
            sudo apt install build-essential git bison
 *
 *      Run with:
 *          ./run
 *
 *      Examples:
 *           #ifdef DEBUG
 *               std::cout << "(DEBUG)";
 *           #endif
 *           #ifdef RELEASE
 *               std::cout << "(RELEASE)";
 *           #endif
 *
 *      Class Identifier Convensions:
 *      - public should be in CamelCase (w/leading cap)
 *      - private underscore case with leading _'s
 *      - static should have leading s_
 *
 *		Nerd Window Border Characters:
 *			╭─────╮
 *			│     │
 *			╰─────╯
 *
 * 󰕈 jay  …/beta_6809   master   10:55 
 * 
 * Custom Text File Headers:
 * 
 *      http://patorjk.com/software/taag/#p=display&f=Big&t=
 *
 ******************************/

#ifndef DEBUG
    #ifndef RELEASE
        #define RELEASE
    #endif
#endif

#include <iostream>
#include "clr.hpp"
#include "Bus.hpp"

int main(void)
{
    // home the cursor
    std::cout << clr::csr_pos();

    // clear the screen and home the cursor
    std::cout << clr::erase_in_display(3);
    std::cout << clr::erase_in_display(2);

    std::string comp_mode = "";
    #ifdef DEBUG
        #undef RELEASE
        comp_mode = "DEBUG";
    #endif
    #ifdef RELEASE
        #undef DEBUG
        comp_mode = "RELEASE";
    #endif

    std::cout << clr::RED << "----";
    std::cout << clr::ORANGE << "====";
    std::cout << clr::YELLOW << "[ ";
    std::cout << clr::WHITE << comp_mode << " MODE";
    std::cout << clr::YELLOW << " ]";
    std::cout << clr::ORANGE << "====";
    std::cout << clr::RED << "----";
    std::cout << clr::RETURN;

    std::string exit_mode = "ABNORMAL";
    Bus& bus = Bus::GetInstance();
    if (bus.Run())
        exit_mode = "NORMAL";

    std::cout << clr::RED << "----";
    std::cout << clr::ORANGE << "====";
    std::cout << clr::YELLOW << "[ ";
    std::cout << clr::WHITE << exit_mode << " TERMINATION";
    std::cout << clr::YELLOW << " ]";
    std::cout << clr::ORANGE << "====";
    std::cout << clr::RED << "----";
    std::cout << clr::RETURN;  

    // Bus::Error("Something like a simulated error happened!");

    return 0;
}

