#!/bin/bash

#lwasm -9 test.asm --format=ihex -otest.hex -ltest.lst
echo "lwasm -9 $1.asm --format=ihex -o$1.hex -l$1.lst"
lwasm -9 $1.asm --format=ihex -o$1.hex -l$1.lst