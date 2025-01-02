/****************************************************************
 *                          _                                    
 *                         (_)                                   
 *      _ __ ___     __ _   _   _ __         ___   _ __    _ __  
 *     | '_ ` _ \   / _` | | | | '_ \       / __| | '_ \  | '_ \ 
 *     | | | | | | | (_| | | | | | | |  _  | (__  | |_) | | |_) |
 *     |_| |_| |_|  \__,_| |_| |_| |_| (_)  \___| | .__/  | .__/ 
 *                                                | |     | |    
 *                                                |_|     |_|   
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
 *          Codium: AI Coding Autocompletion & Chat or Continue - Codestral, Claude, and more
 *
 *      Debian Based Linux Dependencies:
            sudo apt install libsdl2-dev libsdl2-gfx-dev libsdl2-image-dev
            sudo apt install libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev
            sudo apt install build-essential git bison
 *      Arch Linux Dependencies:
            sudo pacman -S sdl2 sdl2_gfx sdl2_image sdl2_mixer sdl2_ttf
            sudo pacman -S base-devel git gcc bison             
 *
 *      Run with:
 *          ./run
 *
 *      Build with:
 *          make                # builds with debug symbols
 *          make release        # builds for performance but without debug
 *          make clean          # cleans the build folder   
 *          make run            # builds and runs 
 * 
 *      Fancy Text:
 *          Headers     - https://patorjk.com/software/taag/#p=display&h=0&v=0&c=c&f=Big
 *          Functions   - https://patorjk.com/software/taag/#p=display&h=0&v=0&c=c%2B%2B&f=Small
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
 ******************************/

#ifndef DEBUG
    #ifndef RELEASE
        #define RELEASE
    #endif
#endif


#include "Bus.hpp"
#include "clr.hpp"

std::string gfx_mode(unsigned char mode) {
    std::string result = "| ";

    float width = 640.0f;
    float height = 400.0f;
    float div = 1.0f;

    result += clr::pad("0x" + clr::hex(mode,2) + " | ", 6);
    // screen mode
    if (mode & 0b10000000) {
        result += clr::pad("bitmap | ", 9);
    } else {
        result += clr::pad("text | ", 9);
    }
    // video timing
    if (mode & 0b01000000) {
        result += clr::pad("640x400 | ", 8);
        width = 640;
        height = 400;
    } else {
        result += clr::pad("512x320 | ", 8);
        width = 512;
        height = 320;
    }
    // horizontal overscan
    switch ((mode & 0b00110000) >> 4) {
        case 0b00: result += clr::pad("1x | ", 4); width /= 1.0f; break;
        case 0b01: result += clr::pad("2x | ", 4); width /= 2.0f; break;
        case 0b10: result += clr::pad("4x | ", 4); width /= 4.0f; break;
        case 0b11: result += clr::pad("8x | ", 4); width /= 8.0f; break;
    }
    // vertical overscan
    switch ((mode & 0b00001100) >> 2) {
        case 0b00: result += clr::pad("1x | ",4); height /= 1.0f; break;
        case 0b01: result += clr::pad("2x | ",4); height /= 2.0f; break;
        case 0b10: result += clr::pad("4x | ",4); height /= 4.0f; break;
        case 0b11: result += clr::pad("8x | ",4); height /= 8.0f; break;
    }
    // adjust for text mode
    if (!(mode & 0b10000000)) {
        width /= 8.0f;
        height /=8.0f;
    }
    // overall resolution     
    result += clr::pad(std::to_string((int)width) + "x" + std::to_string((int)height), 7) + " | ";

    // buffer size
    int buffer = (int)((width * height) / div);
    result += clr::pad(std::to_string(buffer), 6);

    if (buffer <= 8000)
        result += clr::pad("|   std    |", 8);
    else if (buffer <= 64000)   
        result += clr::pad("| extended |", 8);
    else
        result += clr::pad("|   none   |", 8);        

    // color depth
    if (!(mode & 0b10000000)) {
        // result += clr::pad("text | ", 7); div = 0.5f;
        switch (mode & 0b00000011) {
            case 0b00: result += clr::pad("     monochrome text       ",17); div = 1.0f; break;
            case 0b01: result += clr::pad("       bg/fg + text        ",17); div = 0.5f; break;
            case 0b10: result += clr::pad(" clear-bgnd 256-color text ",17); div = 0.5f; break;
            case 0b11: result += clr::pad("color_0-bgnd 256-color text",17); div = 0.5f; break;
        }
    } else {
        switch (mode & 0b00000011) {
            case 0b00: result += clr::pad("   2 colors",17); div = 8.0f; break;
            case 0b01: result += clr::pad("   4 colors",17); div = 4.0f; break;
            case 0b10: result += clr::pad("  16 colors",17); div = 2.0f; break;
            case 0b11: result += clr::pad(" 256 colors",17); div = 1.0f; break;
        }
    }
    
    return result;

}


int main() {
    // home the cursor
    std::cout << clr::csr_pos();

    // clear the screen and home the cursor
    std::cout << clr::erase_in_display(3);
    std::cout << clr::erase_in_display(2);

    // get the compiler mode
    std::string comp_mode = "";
    #ifdef DEBUG
        #undef RELEASE
        comp_mode = "DEBUG";
    #endif
    #ifdef RELEASE
        #undef DEBUG
        comp_mode = "RELEASE";
    #endif

    // print the header
    std::cout << clr::RED << "----";
    std::cout << clr::ORANGE << "====";
    std::cout << clr::YELLOW << "[ ";
    std::cout << clr::WHITE << comp_mode << " MODE";
    std::cout << clr::YELLOW << " ]";
    std::cout << clr::ORANGE << "====";
    std::cout << clr::RED << "----";
    std::cout << clr::RETURN;

    // get the exit mode
    std::string exit_mode = "ABNORMAL";
    Bus& bus = Bus::GetInstance();
    if (bus.Run())
        exit_mode = "NORMAL";

    // print the footer
    std::cout << clr::RED << "----";
    std::cout << clr::ORANGE << "====";
    std::cout << clr::YELLOW << "[ ";
    std::cout << clr::WHITE << exit_mode << " TERMINATION";
    std::cout << clr::YELLOW << " ]";
    std::cout << clr::ORANGE << "====";
    std::cout << clr::RED << "----";
    std::cout << clr::RETURN;  

    // Bus::Error("Something like a simulated error happened!");
    
    #define DISPLAY_MODE_LIST false
    #if DISPLAY_MODE_LIST
        std::cout << "| MODE | SCREEN | TIMING | H.OVER | V.OVER | RESOLUTION | BUFFER | VALID | COLORS |\n";
        std::cout << "|:----:|:------:|:------:|:------:|:------:|:----------:|:------:|:-----:|:------:|\n";
        for (int i = 0; i < 256; i++) {
            std::cout << gfx_mode(i) << " |\n";
        }
    #endif


    return 0;
}


