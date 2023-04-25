/*****************************************************
main.cpp

build with debug symbols:
    g++ -g *.cpp -o bin/main -std=c++20 -lSDL2 

build without debug symbols:
    g++ *.cpp -o bin/main -std=c++20 -lSDL2 

GIT Stuff:
    git status
    git add .
    git commit -m "Commit Comment"
    git push origin master

*****************************************************/

#include <stdio.h>
#include <iostream>

#include <SDL2/SDL.h>

#include "Bus.h"

int main(void)
{
    printf("Apparently it works.\n");
    std::cout << "Does this work?" << std::endl;
    Bus();
}



