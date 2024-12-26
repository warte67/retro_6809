@set arg1=%1
asm6809 -H -9 %arg1%.asm -o %arg1%.hex -l %arg1%.lst -s %arg1%.sym
