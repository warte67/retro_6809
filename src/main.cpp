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
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
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
 *      - public should be in CamelCase (w/leading= cap)
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

/**
 * Displays a list of available display modes in a tabular format.
 * The function categorizes display modes into three sections:
 * 
 * 1. Standard Graphics Modes:
 *    - Includes standard text modes and standard graphics modes.
 *    - Displays mode, width, height, columns, rows, buffer size, and color information.
 * 
 * 2. Standard Text Modes:
 *    - Displays mode, width, height, columns, rows, and buffer size for standard text.
 * 
 * 3. Extended Graphics Modes:
 *    - Includes extended bitmap modes.
 *    - Displays mode, width, height, bits per pixel, colors, and buffer size.
 * 
 * Each section is printed with a header and a table format, with modes 
 * iteratively calculated and displayed. Modes with buffer sizes above
 * a certain threshold have their color depth reduced to fit the buffer.
 */

void DisplayModeList() 
{
    // display the standard text modes:
    std::cout << clr::RETURN;
    std::cout << "\n# Standard Graphics modes:\n";
    std::cout << "\n## Standard Text Modes:\n\n";
    std::cout << "| MODE | WIDTH | HEIGHT | COLUMNS |  ROWS  | BUFFER |\n";
    std::cout << "|:----:|:-----:|:------:|:-------:|:------:|:------:|\n";        
    for (int i=0; i < 8; i++) {
        int width = 320, height = 200;
        if (i & 1) { width = 256; height = 160; }
        std::cout << "$" << clr::hex(i,2) << " | ";
        // Over-Scan
        int HS = 8, VS = 8;
        switch (i&7) {
            case 0:  { HS = 1; VS = 1; break; }
            case 1:  { HS = 1; VS = 1; break; }
            case 2:  { HS = 1; VS = 2; break; }
            case 3:  { HS = 1; VS = 2; break; }
            case 4:  { HS = 2; VS = 1; break; }
            case 5:  { HS = 2; VS = 1; break; }               
            case 6:  { HS = 2; VS = 2; break; }
            case 7:  { HS = 2; VS = 2; break; }
        }
        std::cout << (width/HS) << " | ";
        std::cout << (height/VS) << " | ";
        std::cout << (width/HS)/8 << " | ";
        std::cout << (height/VS)/8 << " | ";
        std::cout << (((width/HS)/8) * ((height/VS)/8)) * 2 << " |\n";
    }
    std::cout << "--- \n";

    // display the standard graphics modes:
    std::cout << "\n## Standard Graphics Modes:\n\n";
    std::cout << "| MODE | WIDTH | HEIGHT |  1-bpp | COLORS | BUFFER |\n";
    std::cout << "|:----:|:-----:|:------:|:------:|:------:|:------:|\n";
    for (int i=0; i < 32; i++) {
        int width = 320, height = 200;
        //if (i%8==0) std::cout << std::endl;
        if (i & 1) { width = 256; height = 160; }
        std::cout << "| $" << clr::hex(i,2) << " | ";
        // Over-Scan
        int HS = 8, VS = 8;
        switch (i&7) {
            case 0:  { HS = 1; VS = 1; break; }
            case 1:  { HS = 1; VS = 1; break; }
            case 2:  { HS = 1; VS = 2; break; }
            case 3:  { HS = 1; VS = 2; break; }
            case 4:  { HS = 2; VS = 1; break; }
            case 5:  { HS = 2; VS = 1; break; }               
            case 6:  { HS = 2; VS = 2; break; }
            case 7:  { HS = 2; VS = 2; break; }
        }
        std::cout << (width/HS) << " | ";
        std::cout << (height/VS) << " | ";

        int b = 1<<((i>>3) & 7);
        int buffer = ((width/HS) * (height/VS))/8;
        std::cout << buffer << " | ";

        switch (b) {
            case 1:  { std::cout << "2-colors | ";      buffer *= 1; break; }
            case 2:  { std::cout << "4-colors | ";      buffer *= 2; break; }
            case 4:  { std::cout << "16-colors | ";     buffer *= 4; break; }
            case 8:  { std::cout << "256-colors | ";    buffer *= 8; break; }
        }
        if (buffer > 8000)
            std::cout << "(" << buffer << ")*" << " |\n";
        else
            std::cout << buffer << " |\n";                
    }
    std::cout << "* Modes with buffers > 8000 will have their color depth reduced until the buffer fits.\n";
    std::cout << "--- \n";

    // display the extended graphics modes:
    std::cout << "\n# Extended Graphics Modes:\n";
    std::cout << "\n## Extended Bitmap Modes:\n";
    std::cout << "| MODE | WIDTH | HEIGHT |  1-bpp | COLORS | BUFFER |\n";
    std::cout << "|:----:|:-----:|:------:|:------:|:------:|:------:|\n";
    for (int i=0; i < 32; i++) {
        int width = 320, height = 200;
        //if (i%8==0) std::cout << std::endl;
        if (i & 1) { width = 256; height = 160; }
        std::cout << "| $" << clr::hex(i,2) << " | ";
        // Over-Scan
        int HS = 8, VS = 8;
        switch (i&7) {
            case 0:  { HS = 1; VS = 1; break; }
            case 1:  { HS = 1; VS = 1; break; }
            case 2:  { HS = 1; VS = 2; break; }
            case 3:  { HS = 1; VS = 2; break; }
            case 4:  { HS = 2; VS = 1; break; }
            case 5:  { HS = 2; VS = 1; break; }               
            case 6:  { HS = 2; VS = 2; break; }
            case 7:  { HS = 2; VS = 2; break; }
        }
        std::cout << (width/HS) << " | ";
        std::cout << (height/VS) << " | ";

        int b = 1<<((i>>3) & 7);
        int buffer = ((width/HS) * (height/VS))/8;
        std::cout << buffer << " | ";

        switch (b) {
            case 1:  { std::cout << "2-colors | ";      buffer *= 1; break; }
            case 2:  { std::cout << "4-colors | ";      buffer *= 2; break; }
            case 4:  { std::cout << "16-colors | ";     buffer *= 4; break; }
            case 8:  { std::cout << "256-colors | ";    buffer *= 8; break; }
        }
        if (buffer > 64000)
            std::cout << "(" << buffer << ")*" << " |\n";
        else
            std::cout << buffer << " |\n";                
    }
    std::cout << "--- \n";
}

/**
 * @brief The main entry point of the program.
 *
 * This function initializes the bus and runs it. The bus is the main
 * interface to the program. It provides all the necessary methods to
 * interact with the program.
 *
 * @return 0 if the program terminated normally, 1 otherwise.
 */
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
        DisplayModeList();
    #endif


    return 0;
}

