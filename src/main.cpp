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

std::string std_gfx_mode(unsigned char mode) {
    std::string result = "| ";
    float width = 512.0f;
    float height = 320.0f;    
    if (mode & 0b00000001) {
        width = 640.0f;
        height = 400.0f;
    }
    int w = (int)width;
    int h = (int)height;
    float div = 1.0f;
    // mode:
    result += clr::pad("0x" + clr::hex(mode,2) + " | ", 6);
    // screen
    if (mode & 0b10000000) {
        result += clr::pad("bitmap | ", 6);
    } else {
        result += clr::pad("text   | ", 6);
    }
    // colors
    if (mode & 0b10000000) {        
        switch ((mode & 0b01100000) >> 5) {
            case 0x00: result += "2 colors | ";     div *= 8.0f; break;
            case 0x01: result += "4 colors | ";     div *= 4.0f; break;
            case 0x02: result += "16 colors | ";    div *= 2.0f; break;
            case 0x03: result += "256 colors | ";   div *= 1.0f; break;
        }
    } else {
        result += "bg/fg | ";
    }
    // h.over
    switch ((mode & 0b00011000) >> 3) {
        case 0x00: width /= 1.0f; result += "W/1 | "; break;
        case 0x01: width /= 2.0f; result += "W/2 | "; break;
        case 0x02: width /= 3.0f; result += "W/3 | "; break;
        case 0x03: width /= 4.0f; result += "W/4 | "; break;    
    }
    // v.over
    switch ((mode & 0b00000110) >> 1) {
        case 0x00: height /= 1.0f; result += "H/1 | "; break;
        case 0x01: height /= 2.0f; result += "H/2 | "; break;
        case 0x02: height /= 3.0f; result += "H/3 | "; break;
        case 0x03: height /= 4.0f; result += "H/4 | "; break;    
    }
    // timing
    result += std::to_string(w) + "x" + std::to_string(h) + " | ";    
    // text resolution
    if ((mode & 0b10000000)==0) {
        width /= 8;
        height /= 8;
        div = 0.5f;
    }
    result += std::to_string((int)width) + "x" + std::to_string((int)height) + " | ";
    // buffer
    float buffer = (width * height) / div;
    result += std::to_string((int)buffer) + " bytes | ";
    if (buffer > 8000)  
        return "";
    return result;
}


std::string ext_gfx_mode(unsigned char mode) {
    std::string result = "| ";
    float width = 512.0f;
    float height = 320.0f;
    if (mode & 0b00000001) {
        width = 640.0f;
        height = 400.0f;
    }
    int w = (int)width;
    int h = (int)height;
    float div = 1.0f;
    // mode:
    result += clr::pad("0x" + clr::hex(mode,2) + " | ", 6);
    // screen
    if (mode & 0b10000000) {
        result += clr::pad("bitmap | ", 6);
    } else {
        result += clr::pad("tiled  | ", 6);
    }
    // colors
    switch ((mode & 0b01100000) >> 5) {
        case 0x00: result += "2 colors | ";     div *= 8.0f; break;
        case 0x01: result += "4 colors | ";     div *= 4.0f; break;
        case 0x02: result += "16 colors | ";    div *= 2.0f; break;
        case 0x03: result += "256 colors | ";   div *= 1.0f; break;
    }
    // h.over
    switch ((mode & 0b00011000) >> 3) {
        case 0x00: width /= 1.0f; result += "W/1 | "; break;
        case 0x01: width /= 2.0f; result += "W/2 | "; break;
        case 0x02: width /= 3.0f; result += "W/3 | "; break;
        case 0x03: width /= 4.0f; result += "W/4 | "; break;    
    }
    // v.over
    switch ((mode & 0b00000110) >> 1) {
        case 0x00: height /= 1.0f; result += "H/1 | "; break;
        case 0x01: height /= 2.0f; result += "H/2 | "; break;
        case 0x02: height /= 3.0f; result += "H/3 | "; break;
        case 0x03: height /= 4.0f; result += "H/4 | "; break;    
    }
    // timing
    result += std::to_string(w) + "x" + std::to_string(h) + " | ";    

    // displayed tilemap resolution
    if ((mode & 0b10000000)==0) {
        width /= 16.0f;
        height /= 16.0f;
        div = 1.0f;
    }
    result += std::to_string((int)width) + "x" + std::to_string((int)height) + " | ";
    // buffer
    float buffer = (width * height) / div;
    result += std::to_string((int)buffer) + " bytes | ";
    if (buffer > 64000.0f)  
        return "";
    return result;
}



int main() {
    // home the cursor | COLORS
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
        // display the standard text modes:
        std::cout << clr::RETURN;
        std::cout << "Standard Text Modes:\n";
        std::cout << "| MODE | SCREEN | COLORS | H.OVER | V.OVER | TIMING | RESOLUTION | BUFFER |\n";
        std::cout << "|:----:|:------:|:------:|:------:|:------:|:------:|:----------:|:------:|\n";
        for (int i = 0; i < 32; i++) {
            if (std_gfx_mode(i) != "")
                std::cout << std_gfx_mode(i) << " \n";
        }

        // display the standard graphics modes:
        std::cout << clr::RETURN;
        std::cout << "Standard Graphics Modes:\n";
        std::cout << "| MODE | SCREEN | COLORS | H.OVER | V.OVER | TIMING | RESOLUTION | BUFFER |\n";
        std::cout << "|:----:|:------:|:------:|:------:|:------:|:----------:|:------:|:------:|\n";
        for (int i = 128; i < 256; i++) {
            if (std_gfx_mode(i) != "")
                std::cout << std_gfx_mode(i) << " \n";
        }

        // display the extended tile modes:
        std::cout << clr::RETURN;
        std::cout << "Extended Tile Modes:\n";
        std::cout << "| MODE | SCREEN | COLORS | H.OVER | V.OVER | TIMING | RESOLUTION | BUFFER |\n";
        std::cout << "|:----:|:------:|:------:|:------:|:------:|:------:|:----------:|:------:|\n";
        for (int i = 0; i < 128; i++) {
            if (ext_gfx_mode(i) != "")
                std::cout << ext_gfx_mode(i) << " \n";
        }

        // display the extended graphics modes:
        std::cout << clr::RETURN;
        std::cout << "Extended Graphics Modes:\n";
        std::cout << "| MODE | SCREEN | COLORS | H.OVER | V.OVER | TIMING | RESOLUTION | BUFFER |\n";
        std::cout << "|:----:|:------:|:------:|:------:|:------:|:------:|:----------:|:------:|\n";
        for (int i = 128; i < 256; i++) {
            if (ext_gfx_mode(i) != "")
                std::cout << ext_gfx_mode(i) << " \n";
        }

    #endif


    return 0;
}

