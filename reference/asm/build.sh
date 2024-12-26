#! /bin/bash
#
# bash build.sh kernel_f000
#
#lwasm -9 test.asm --format=ihex -otest.hex -ltest.lst
#echo "lwasm -9 $1.asm --format=ihex -o$1.hex -l$1.lst"
#lwasm -9 $1.asm --format=ihex -o$1.hex -l$1.lst
echo "asm6809 -H -9 $1.asm -o $1.hex -l $1.lst -s $1.sym"
asm6809 -H -9 $1.asm -o $1.hex -l $1.lst -s $1.sym