# Standard Graphics Modes

Notes: ...

## Standard Text Modes

| MODE | SCREEN | COLORS | H.OVER | V.OVER | TIMING | RESOLUTION | BUFFER |
|:----:|:------:|:------:|:------:|:------:|:------:|:----------:|:------:|
| 0x00 | text   | bg/fg | W/1 | H/1 | 512x320 | 64x40 | 5120 bytes |  
| 0x01 | text   | bg/fg | W/1 | H/1 | 640x400 | 80x50 | 8000 bytes |  
| 0x02 | text   | bg/fg | W/1 | H/2 | 512x160 | 64x20 | 2560 bytes |  
| 0x03 | text   | bg/fg | W/1 | H/2 | 640x200 | 80x25 | 4000 bytes |  
| 0x04 | text   | bg/fg | W/1 | H/3 | 512x106 | 64x13 | 1706 bytes |  
| 0x05 | text   | bg/fg | W/1 | H/3 | 640x133 | 80x16 | 2666 bytes |  
| 0x06 | text   | bg/fg | W/1 | H/4 | 512x80 | 64x10 | 1280 bytes |  
| 0x07 | text   | bg/fg | W/1 | H/4 | 640x100 | 80x12 | 2000 bytes |  
| 0x08 | text   | bg/fg | W/2 | H/1 | 256x320 | 32x40 | 2560 bytes |  
| 0x09 | text   | bg/fg | W/2 | H/1 | 320x400 | 40x50 | 4000 bytes |  
| 0x0A | text   | bg/fg | W/2 | H/2 | 256x160 | 32x20 | 1280 bytes |  
| 0x0B | text   | bg/fg | W/2 | H/2 | 320x200 | 40x25 | 2000 bytes |  
| 0x0C | text   | bg/fg | W/2 | H/3 | 256x106 | 32x13 | 853 bytes |  
| 0x0D | text   | bg/fg | W/2 | H/3 | 320x133 | 40x16 | 1333 bytes |  
| 0x0E | text   | bg/fg | W/2 | H/4 | 256x80 | 32x10 | 640 bytes |  
| 0x0F | text   | bg/fg | W/2 | H/4 | 320x100 | 40x12 | 1000 bytes |  
| 0x10 | text   | bg/fg | W/3 | H/1 | 170x320 | 21x40 | 1706 bytes |  
| 0x11 | text   | bg/fg | W/3 | H/1 | 213x400 | 26x50 | 2666 bytes |  
| 0x12 | text   | bg/fg | W/3 | H/2 | 170x160 | 21x20 | 853 bytes |  
| 0x13 | text   | bg/fg | W/3 | H/2 | 213x200 | 26x25 | 1333 bytes |  
| 0x14 | text   | bg/fg | W/3 | H/3 | 170x106 | 21x13 | 568 bytes |  
| 0x15 | text   | bg/fg | W/3 | H/3 | 213x133 | 26x16 | 888 bytes |  
| 0x16 | text   | bg/fg | W/3 | H/4 | 170x80 | 21x10 | 426 bytes |  
| 0x17 | text   | bg/fg | W/3 | H/4 | 213x100 | 26x12 | 666 bytes |  
| 0x18 | text   | bg/fg | W/4 | H/1 | 128x320 | 16x40 | 1280 bytes |  
| 0x19 | text   | bg/fg | W/4 | H/1 | 160x400 | 20x50 | 2000 bytes |  
| 0x1A | text   | bg/fg | W/4 | H/2 | 128x160 | 16x20 | 640 bytes |  
| 0x1B | text   | bg/fg | W/4 | H/2 | 160x200 | 20x25 | 1000 bytes |  
| 0x1C | text   | bg/fg | W/4 | H/3 | 128x106 | 16x13 | 426 bytes |  
| 0x1D | text   | bg/fg | W/4 | H/3 | 160x133 | 20x16 | 666 bytes |  
| 0x1E | text   | bg/fg | W/4 | H/4 | 128x80 | 16x10 | 320 bytes |  
| 0x1F | text   | bg/fg | W/4 | H/4 | 160x100 | 20x12 | 500 bytes |  


---

## Standard Bitmap Modes

| MODE | SCREEN | COLORS | H.OVER | V.OVER | TIMING | RESOLUTION | BUFFER |
|:----:|:------:|:------:|:------:|:------:|:----------:|:------:|:------:|
| 0x84 | bitmap | 2 colors | W/1 | H/3 | 512x106 | 512x106 | 6826 bytes |  
| 0x86 | bitmap | 2 colors | W/1 | H/4 | 512x80 | 512x80 | 5120 bytes |  
| 0x87 | bitmap | 2 colors | W/1 | H/4 | 640x100 | 640x100 | 8000 bytes |  
| 0x8A | bitmap | 2 colors | W/2 | H/2 | 256x160 | 256x160 | 5120 bytes |  
| 0x8B | bitmap | 2 colors | W/2 | H/2 | 320x200 | 320x200 | 8000 bytes |  
| 0x8C | bitmap | 2 colors | W/2 | H/3 | 256x106 | 256x106 | 3413 bytes |  
| 0x8D | bitmap | 2 colors | W/2 | H/3 | 320x133 | 320x133 | 5333 bytes |  
| 0x8E | bitmap | 2 colors | W/2 | H/4 | 256x80 | 256x80 | 2560 bytes |  
| 0x8F | bitmap | 2 colors | W/2 | H/4 | 320x100 | 320x100 | 4000 bytes |  
| 0x90 | bitmap | 2 colors | W/3 | H/1 | 170x320 | 170x320 | 6826 bytes |  
| 0x92 | bitmap | 2 colors | W/3 | H/2 | 170x160 | 170x160 | 3413 bytes |  
| 0x93 | bitmap | 2 colors | W/3 | H/2 | 213x200 | 213x200 | 5333 bytes |  
| 0x94 | bitmap | 2 colors | W/3 | H/3 | 170x106 | 170x106 | 2275 bytes |  
| 0x95 | bitmap | 2 colors | W/3 | H/3 | 213x133 | 213x133 | 3555 bytes |  
| 0x96 | bitmap | 2 colors | W/3 | H/4 | 170x80 | 170x80 | 1706 bytes |  
| 0x97 | bitmap | 2 colors | W/3 | H/4 | 213x100 | 213x100 | 2666 bytes |  
| 0x98 | bitmap | 2 colors | W/4 | H/1 | 128x320 | 128x320 | 5120 bytes |  
| 0x99 | bitmap | 2 colors | W/4 | H/1 | 160x400 | 160x400 | 8000 bytes |  
| 0x9A | bitmap | 2 colors | W/4 | H/2 | 128x160 | 128x160 | 2560 bytes |  
| 0x9B | bitmap | 2 colors | W/4 | H/2 | 160x200 | 160x200 | 4000 bytes |  
| 0x9C | bitmap | 2 colors | W/4 | H/3 | 128x106 | 128x106 | 1706 bytes |  
| 0x9D | bitmap | 2 colors | W/4 | H/3 | 160x133 | 160x133 | 2666 bytes |  
| 0x9E | bitmap | 2 colors | W/4 | H/4 | 128x80 | 128x80 | 1280 bytes |  
| 0x9F | bitmap | 2 colors | W/4 | H/4 | 160x100 | 160x100 | 2000 bytes |  
| 0xAC | bitmap | 4 colors | W/2 | H/3 | 256x106 | 256x106 | 6826 bytes |  
| 0xAE | bitmap | 4 colors | W/2 | H/4 | 256x80 | 256x80 | 5120 bytes |  
| 0xAF | bitmap | 4 colors | W/2 | H/4 | 320x100 | 320x100 | 8000 bytes |  
| 0xB2 | bitmap | 4 colors | W/3 | H/2 | 170x160 | 170x160 | 6826 bytes |  
| 0xB4 | bitmap | 4 colors | W/3 | H/3 | 170x106 | 170x106 | 4551 bytes |  
| 0xB5 | bitmap | 4 colors | W/3 | H/3 | 213x133 | 213x133 | 7111 bytes |  
| 0xB6 | bitmap | 4 colors | W/3 | H/4 | 170x80 | 170x80 | 3413 bytes |  
| 0xB7 | bitmap | 4 colors | W/3 | H/4 | 213x100 | 213x100 | 5333 bytes |  
| 0xBA | bitmap | 4 colors | W/4 | H/2 | 128x160 | 128x160 | 5120 bytes |  
| 0xBB | bitmap | 4 colors | W/4 | H/2 | 160x200 | 160x200 | 8000 bytes |  
| 0xBC | bitmap | 4 colors | W/4 | H/3 | 128x106 | 128x106 | 3413 bytes |  
| 0xBD | bitmap | 4 colors | W/4 | H/3 | 160x133 | 160x133 | 5333 bytes |  
| 0xBE | bitmap | 4 colors | W/4 | H/4 | 128x80 | 128x80 | 2560 bytes |  
| 0xBF | bitmap | 4 colors | W/4 | H/4 | 160x100 | 160x100 | 4000 bytes |  
| 0xD6 | bitmap | 16 colors | W/3 | H/4 | 170x80 | 170x80 | 6826 bytes |  
| 0xDC | bitmap | 16 colors | W/4 | H/3 | 128x106 | 128x106 | 6826 bytes |  
| 0xDE | bitmap | 16 colors | W/4 | H/4 | 128x80 | 128x80 | 5120 bytes |  
| 0xDF | bitmap | 16 colors | W/4 | H/4 | 160x100 | 160x100 | 8000 bytes |  


---

