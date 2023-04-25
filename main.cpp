/*****************************************************
main.cpp

build with debug symbols:
g++ -g *.cpp -o bin/main -std=c++20 -lSDL2 

build without debug symbols:
g++ *.cpp -o bin/main -std=c++20 -lSDL2 

run:
./bin/main


GIT:
https://www.youtube.com/watch?v=gJv0PcfUXE8

<this folder>
git init
git config --global user.name "Warte67"
git config --global user.email "jayfaries@yahoo.com"

git status
git add .
git commit -m "first commit"

git log --oneline
git revert <hash>

git remote add origin https://github.com/warte67/retro_6809.git

git push origin master


(boogers)

*****************************************************/

#include <stdio.h>
#include <iostream>

#include <SDL2/SDL.h>

#include "bus.h"

int main(void)
{
    printf("Apparently it works.\n");
    std::cout << "Does this work?" << std::endl;
    bus();
}



