#include <ncurses.h>
#include <SDL2/SDL.h>
#include "Bus.hpp"

int main() {

    // initialize ncurses
    initscr();               // Initialize the ncurses mode
    // print a message
    printw("beta_6809 debug terminal:\n");    
    // update the ncurses display
    refresh();            




    std::string exit_mode = "ABNORMAL";
    Bus& bus = Bus::GetInstance();
    if (bus.Run())
        exit_mode = "NORMAL";
    printw("%s TERMINATION\n", exit_mode.c_str());




    // Pause and wait for a key
    getch();                 // Wait for user input
    // // shutdown ncurses
    endwin();                // Exit ncurses mode    
    // exit 
    return 0;
}


