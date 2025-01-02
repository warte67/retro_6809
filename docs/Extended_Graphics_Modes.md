# Extended Graphics Modes

Notes: ...

## Extended Tile Modes
| MODE | SCREEN | COLORS | H.OVER | V.OVER | TIMING | RESOLUTION | BUFFER |
|:----:|:------:|:------:|:------:|:------:|:------:|:----------:|:------:|
| 0x00 | tiled  | 2 colors | W/1 | H/1 | 512x320 | 32x20 | 640 bytes |  
| 0x01 | tiled  | 2 colors | W/1 | H/1 | 640x400 | 40x25 | 1000 bytes |  
| 0x02 | tiled  | 2 colors | W/1 | H/2 | 512x320 | 32x10 | 320 bytes |  
| 0x03 | tiled  | 2 colors | W/1 | H/2 | 640x400 | 40x12 | 500 bytes |  
| 0x04 | tiled  | 2 colors | W/1 | H/3 | 512x320 | 32x6 | 213 bytes |  
| 0x05 | tiled  | 2 colors | W/1 | H/3 | 640x400 | 40x8 | 333 bytes |  
| 0x06 | tiled  | 2 colors | W/1 | H/4 | 512x320 | 32x5 | 160 bytes |  
| 0x07 | tiled  | 2 colors | W/1 | H/4 | 640x400 | 40x6 | 250 bytes |  
| 0x08 | tiled  | 2 colors | W/2 | H/1 | 512x320 | 16x20 | 320 bytes |  
| 0x09 | tiled  | 2 colors | W/2 | H/1 | 640x400 | 20x25 | 500 bytes |  
| 0x0A | tiled  | 2 colors | W/2 | H/2 | 512x320 | 16x10 | 160 bytes |  
| 0x0B | tiled  | 2 colors | W/2 | H/2 | 640x400 | 20x12 | 250 bytes |  
| 0x0C | tiled  | 2 colors | W/2 | H/3 | 512x320 | 16x6 | 106 bytes |  
| 0x0D | tiled  | 2 colors | W/2 | H/3 | 640x400 | 20x8 | 166 bytes |  
| 0x0E | tiled  | 2 colors | W/2 | H/4 | 512x320 | 16x5 | 80 bytes |  
| 0x0F | tiled  | 2 colors | W/2 | H/4 | 640x400 | 20x6 | 125 bytes |  
| 0x10 | tiled  | 2 colors | W/3 | H/1 | 512x320 | 10x20 | 213 bytes |  
| 0x11 | tiled  | 2 colors | W/3 | H/1 | 640x400 | 13x25 | 333 bytes |  
| 0x12 | tiled  | 2 colors | W/3 | H/2 | 512x320 | 10x10 | 106 bytes |  
| 0x13 | tiled  | 2 colors | W/3 | H/2 | 640x400 | 13x12 | 166 bytes |  
| 0x14 | tiled  | 2 colors | W/3 | H/3 | 512x320 | 10x6 | 71 bytes |  
| 0x15 | tiled  | 2 colors | W/3 | H/3 | 640x400 | 13x8 | 111 bytes |  
| 0x16 | tiled  | 2 colors | W/3 | H/4 | 512x320 | 10x5 | 53 bytes |  
| 0x17 | tiled  | 2 colors | W/3 | H/4 | 640x400 | 13x6 | 83 bytes |  
| 0x18 | tiled  | 2 colors | W/4 | H/1 | 512x320 | 8x20 | 160 bytes |  
| 0x19 | tiled  | 2 colors | W/4 | H/1 | 640x400 | 10x25 | 250 bytes |  
| 0x1A | tiled  | 2 colors | W/4 | H/2 | 512x320 | 8x10 | 80 bytes |  
| 0x1B | tiled  | 2 colors | W/4 | H/2 | 640x400 | 10x12 | 125 bytes |  
| 0x1C | tiled  | 2 colors | W/4 | H/3 | 512x320 | 8x6 | 53 bytes |  
| 0x1D | tiled  | 2 colors | W/4 | H/3 | 640x400 | 10x8 | 83 bytes |  
| 0x1E | tiled  | 2 colors | W/4 | H/4 | 512x320 | 8x5 | 40 bytes |  
| 0x1F | tiled  | 2 colors | W/4 | H/4 | 640x400 | 10x6 | 62 bytes |  
| 0x20 | tiled  | 4 colors | W/1 | H/1 | 512x320 | 32x20 | 640 bytes |  
| 0x21 | tiled  | 4 colors | W/1 | H/1 | 640x400 | 40x25 | 1000 bytes |  
| 0x22 | tiled  | 4 colors | W/1 | H/2 | 512x320 | 32x10 | 320 bytes |  
| 0x23 | tiled  | 4 colors | W/1 | H/2 | 640x400 | 40x12 | 500 bytes |  
| 0x24 | tiled  | 4 colors | W/1 | H/3 | 512x320 | 32x6 | 213 bytes |  
| 0x25 | tiled  | 4 colors | W/1 | H/3 | 640x400 | 40x8 | 333 bytes |  
| 0x26 | tiled  | 4 colors | W/1 | H/4 | 512x320 | 32x5 | 160 bytes |  
| 0x27 | tiled  | 4 colors | W/1 | H/4 | 640x400 | 40x6 | 250 bytes |  
| 0x28 | tiled  | 4 colors | W/2 | H/1 | 512x320 | 16x20 | 320 bytes |  
| 0x29 | tiled  | 4 colors | W/2 | H/1 | 640x400 | 20x25 | 500 bytes |  
| 0x2A | tiled  | 4 colors | W/2 | H/2 | 512x320 | 16x10 | 160 bytes |  
| 0x2B | tiled  | 4 colors | W/2 | H/2 | 640x400 | 20x12 | 250 bytes |  
| 0x2C | tiled  | 4 colors | W/2 | H/3 | 512x320 | 16x6 | 106 bytes |  
| 0x2D | tiled  | 4 colors | W/2 | H/3 | 640x400 | 20x8 | 166 bytes |  
| 0x2E | tiled  | 4 colors | W/2 | H/4 | 512x320 | 16x5 | 80 bytes |  
| 0x2F | tiled  | 4 colors | W/2 | H/4 | 640x400 | 20x6 | 125 bytes |  
| 0x30 | tiled  | 4 colors | W/3 | H/1 | 512x320 | 10x20 | 213 bytes |  
| 0x31 | tiled  | 4 colors | W/3 | H/1 | 640x400 | 13x25 | 333 bytes |  
| 0x32 | tiled  | 4 colors | W/3 | H/2 | 512x320 | 10x10 | 106 bytes |  
| 0x33 | tiled  | 4 colors | W/3 | H/2 | 640x400 | 13x12 | 166 bytes |  
| 0x34 | tiled  | 4 colors | W/3 | H/3 | 512x320 | 10x6 | 71 bytes |  
| 0x35 | tiled  | 4 colors | W/3 | H/3 | 640x400 | 13x8 | 111 bytes |  
| 0x36 | tiled  | 4 colors | W/3 | H/4 | 512x320 | 10x5 | 53 bytes |  
| 0x37 | tiled  | 4 colors | W/3 | H/4 | 640x400 | 13x6 | 83 bytes |  
| 0x38 | tiled  | 4 colors | W/4 | H/1 | 512x320 | 8x20 | 160 bytes |  
| 0x39 | tiled  | 4 colors | W/4 | H/1 | 640x400 | 10x25 | 250 bytes |  
| 0x3A | tiled  | 4 colors | W/4 | H/2 | 512x320 | 8x10 | 80 bytes |  
| 0x3B | tiled  | 4 colors | W/4 | H/2 | 640x400 | 10x12 | 125 bytes |  
| 0x3C | tiled  | 4 colors | W/4 | H/3 | 512x320 | 8x6 | 53 bytes |  
| 0x3D | tiled  | 4 colors | W/4 | H/3 | 640x400 | 10x8 | 83 bytes |  
| 0x3E | tiled  | 4 colors | W/4 | H/4 | 512x320 | 8x5 | 40 bytes |  
| 0x3F | tiled  | 4 colors | W/4 | H/4 | 640x400 | 10x6 | 62 bytes |  
| 0x40 | tiled  | 16 colors | W/1 | H/1 | 512x320 | 32x20 | 640 bytes |  
| 0x41 | tiled  | 16 colors | W/1 | H/1 | 640x400 | 40x25 | 1000 bytes |  
| 0x42 | tiled  | 16 colors | W/1 | H/2 | 512x320 | 32x10 | 320 bytes |  
| 0x43 | tiled  | 16 colors | W/1 | H/2 | 640x400 | 40x12 | 500 bytes |  
| 0x44 | tiled  | 16 colors | W/1 | H/3 | 512x320 | 32x6 | 213 bytes |  
| 0x45 | tiled  | 16 colors | W/1 | H/3 | 640x400 | 40x8 | 333 bytes |  
| 0x46 | tiled  | 16 colors | W/1 | H/4 | 512x320 | 32x5 | 160 bytes |  
| 0x47 | tiled  | 16 colors | W/1 | H/4 | 640x400 | 40x6 | 250 bytes |  
| 0x48 | tiled  | 16 colors | W/2 | H/1 | 512x320 | 16x20 | 320 bytes |  
| 0x49 | tiled  | 16 colors | W/2 | H/1 | 640x400 | 20x25 | 500 bytes |  
| 0x4A | tiled  | 16 colors | W/2 | H/2 | 512x320 | 16x10 | 160 bytes |  
| 0x4B | tiled  | 16 colors | W/2 | H/2 | 640x400 | 20x12 | 250 bytes |  
| 0x4C | tiled  | 16 colors | W/2 | H/3 | 512x320 | 16x6 | 106 bytes |  
| 0x4D | tiled  | 16 colors | W/2 | H/3 | 640x400 | 20x8 | 166 bytes |  
| 0x4E | tiled  | 16 colors | W/2 | H/4 | 512x320 | 16x5 | 80 bytes |  
| 0x4F | tiled  | 16 colors | W/2 | H/4 | 640x400 | 20x6 | 125 bytes |  
| 0x50 | tiled  | 16 colors | W/3 | H/1 | 512x320 | 10x20 | 213 bytes |  
| 0x51 | tiled  | 16 colors | W/3 | H/1 | 640x400 | 13x25 | 333 bytes |  
| 0x52 | tiled  | 16 colors | W/3 | H/2 | 512x320 | 10x10 | 106 bytes |  
| 0x53 | tiled  | 16 colors | W/3 | H/2 | 640x400 | 13x12 | 166 bytes |  
| 0x54 | tiled  | 16 colors | W/3 | H/3 | 512x320 | 10x6 | 71 bytes |  
| 0x55 | tiled  | 16 colors | W/3 | H/3 | 640x400 | 13x8 | 111 bytes |  
| 0x56 | tiled  | 16 colors | W/3 | H/4 | 512x320 | 10x5 | 53 bytes |  
| 0x57 | tiled  | 16 colors | W/3 | H/4 | 640x400 | 13x6 | 83 bytes |  
| 0x58 | tiled  | 16 colors | W/4 | H/1 | 512x320 | 8x20 | 160 bytes |  
| 0x59 | tiled  | 16 colors | W/4 | H/1 | 640x400 | 10x25 | 250 bytes |  
| 0x5A | tiled  | 16 colors | W/4 | H/2 | 512x320 | 8x10 | 80 bytes |  
| 0x5B | tiled  | 16 colors | W/4 | H/2 | 640x400 | 10x12 | 125 bytes |  
| 0x5C | tiled  | 16 colors | W/4 | H/3 | 512x320 | 8x6 | 53 bytes |  
| 0x5D | tiled  | 16 colors | W/4 | H/3 | 640x400 | 10x8 | 83 bytes |  
| 0x5E | tiled  | 16 colors | W/4 | H/4 | 512x320 | 8x5 | 40 bytes |  
| 0x5F | tiled  | 16 colors | W/4 | H/4 | 640x400 | 10x6 | 62 bytes |  
| 0x60 | tiled  | 256 colors | W/1 | H/1 | 512x320 | 32x20 | 640 bytes |  
| 0x61 | tiled  | 256 colors | W/1 | H/1 | 640x400 | 40x25 | 1000 bytes |  
| 0x62 | tiled  | 256 colors | W/1 | H/2 | 512x320 | 32x10 | 320 bytes |  
| 0x63 | tiled  | 256 colors | W/1 | H/2 | 640x400 | 40x12 | 500 bytes |  
| 0x64 | tiled  | 256 colors | W/1 | H/3 | 512x320 | 32x6 | 213 bytes |  
| 0x65 | tiled  | 256 colors | W/1 | H/3 | 640x400 | 40x8 | 333 bytes |  
| 0x66 | tiled  | 256 colors | W/1 | H/4 | 512x320 | 32x5 | 160 bytes |  
| 0x67 | tiled  | 256 colors | W/1 | H/4 | 640x400 | 40x6 | 250 bytes |  
| 0x68 | tiled  | 256 colors | W/2 | H/1 | 512x320 | 16x20 | 320 bytes |  
| 0x69 | tiled  | 256 colors | W/2 | H/1 | 640x400 | 20x25 | 500 bytes |  
| 0x6A | tiled  | 256 colors | W/2 | H/2 | 512x320 | 16x10 | 160 bytes |  
| 0x6B | tiled  | 256 colors | W/2 | H/2 | 640x400 | 20x12 | 250 bytes |  
| 0x6C | tiled  | 256 colors | W/2 | H/3 | 512x320 | 16x6 | 106 bytes |  
| 0x6D | tiled  | 256 colors | W/2 | H/3 | 640x400 | 20x8 | 166 bytes |  
| 0x6E | tiled  | 256 colors | W/2 | H/4 | 512x320 | 16x5 | 80 bytes |  
| 0x6F | tiled  | 256 colors | W/2 | H/4 | 640x400 | 20x6 | 125 bytes |  
| 0x70 | tiled  | 256 colors | W/3 | H/1 | 512x320 | 10x20 | 213 bytes |  
| 0x71 | tiled  | 256 colors | W/3 | H/1 | 640x400 | 13x25 | 333 bytes |  
| 0x72 | tiled  | 256 colors | W/3 | H/2 | 512x320 | 10x10 | 106 bytes |  
| 0x73 | tiled  | 256 colors | W/3 | H/2 | 640x400 | 13x12 | 166 bytes |  
| 0x74 | tiled  | 256 colors | W/3 | H/3 | 512x320 | 10x6 | 71 bytes |  
| 0x75 | tiled  | 256 colors | W/3 | H/3 | 640x400 | 13x8 | 111 bytes |  
| 0x76 | tiled  | 256 colors | W/3 | H/4 | 512x320 | 10x5 | 53 bytes |  
| 0x77 | tiled  | 256 colors | W/3 | H/4 | 640x400 | 13x6 | 83 bytes |  
| 0x78 | tiled  | 256 colors | W/4 | H/1 | 512x320 | 8x20 | 160 bytes |  
| 0x79 | tiled  | 256 colors | W/4 | H/1 | 640x400 | 10x25 | 250 bytes |  
| 0x7A | tiled  | 256 colors | W/4 | H/2 | 512x320 | 8x10 | 80 bytes |  
| 0x7B | tiled  | 256 colors | W/4 | H/2 | 640x400 | 10x12 | 125 bytes |  
| 0x7C | tiled  | 256 colors | W/4 | H/3 | 512x320 | 8x6 | 53 bytes |  
| 0x7D | tiled  | 256 colors | W/4 | H/3 | 640x400 | 10x8 | 83 bytes |  
| 0x7E | tiled  | 256 colors | W/4 | H/4 | 512x320 | 8x5 | 40 bytes |  
| 0x7F | tiled  | 256 colors | W/4 | H/4 | 640x400 | 10x6 | 62 bytes | 

---

## Extended Bitmap Modes
| MODE | SCREEN | COLORS | H.OVER | V.OVER | TIMING | RESOLUTION | BUFFER |
|:----:|:------:|:------:|:------:|:------:|:------:|:----------:|:------:|
| 0x80 | bitmap | 2 colors | W/1 | H/1 | 512x320 | 512x320 | 20480 bytes |  
| 0x81 | bitmap | 2 colors | W/1 | H/1 | 640x400 | 640x400 | 32000 bytes |  
| 0x82 | bitmap | 2 colors | W/1 | H/2 | 512x320 | 512x160 | 10240 bytes |  
| 0x83 | bitmap | 2 colors | W/1 | H/2 | 640x400 | 640x200 | 16000 bytes |  
| 0x84 | bitmap | 2 colors | W/1 | H/3 | 512x320 | 512x106 | 6826 bytes |  
| 0x85 | bitmap | 2 colors | W/1 | H/3 | 640x400 | 640x133 | 10666 bytes |  
| 0x86 | bitmap | 2 colors | W/1 | H/4 | 512x320 | 512x80 | 5120 bytes |  
| 0x87 | bitmap | 2 colors | W/1 | H/4 | 640x400 | 640x100 | 8000 bytes |  
| 0x88 | bitmap | 2 colors | W/2 | H/1 | 512x320 | 256x320 | 10240 bytes |  
| 0x89 | bitmap | 2 colors | W/2 | H/1 | 640x400 | 320x400 | 16000 bytes |  
| 0x8A | bitmap | 2 colors | W/2 | H/2 | 512x320 | 256x160 | 5120 bytes |  
| 0x8B | bitmap | 2 colors | W/2 | H/2 | 640x400 | 320x200 | 8000 bytes |  
| 0x8C | bitmap | 2 colors | W/2 | H/3 | 512x320 | 256x106 | 3413 bytes |  
| 0x8D | bitmap | 2 colors | W/2 | H/3 | 640x400 | 320x133 | 5333 bytes |  
| 0x8E | bitmap | 2 colors | W/2 | H/4 | 512x320 | 256x80 | 2560 bytes |  
| 0x8F | bitmap | 2 colors | W/2 | H/4 | 640x400 | 320x100 | 4000 bytes |  
| 0x90 | bitmap | 2 colors | W/3 | H/1 | 512x320 | 170x320 | 6826 bytes |  
| 0x91 | bitmap | 2 colors | W/3 | H/1 | 640x400 | 213x400 | 10666 bytes |  
| 0x92 | bitmap | 2 colors | W/3 | H/2 | 512x320 | 170x160 | 3413 bytes |  
| 0x93 | bitmap | 2 colors | W/3 | H/2 | 640x400 | 213x200 | 5333 bytes |  
| 0x94 | bitmap | 2 colors | W/3 | H/3 | 512x320 | 170x106 | 2275 bytes |  
| 0x95 | bitmap | 2 colors | W/3 | H/3 | 640x400 | 213x133 | 3555 bytes |  
| 0x96 | bitmap | 2 colors | W/3 | H/4 | 512x320 | 170x80 | 1706 bytes |  
| 0x97 | bitmap | 2 colors | W/3 | H/4 | 640x400 | 213x100 | 2666 bytes |  
| 0x98 | bitmap | 2 colors | W/4 | H/1 | 512x320 | 128x320 | 5120 bytes |  
| 0x99 | bitmap | 2 colors | W/4 | H/1 | 640x400 | 160x400 | 8000 bytes |  
| 0x9A | bitmap | 2 colors | W/4 | H/2 | 512x320 | 128x160 | 2560 bytes |  
| 0x9B | bitmap | 2 colors | W/4 | H/2 | 640x400 | 160x200 | 4000 bytes |  
| 0x9C | bitmap | 2 colors | W/4 | H/3 | 512x320 | 128x106 | 1706 bytes |  
| 0x9D | bitmap | 2 colors | W/4 | H/3 | 640x400 | 160x133 | 2666 bytes |  
| 0x9E | bitmap | 2 colors | W/4 | H/4 | 512x320 | 128x80 | 1280 bytes |  
| 0x9F | bitmap | 2 colors | W/4 | H/4 | 640x400 | 160x100 | 2000 bytes |  
| 0xA0 | bitmap | 4 colors | W/1 | H/1 | 512x320 | 512x320 | 40960 bytes |  
| 0xA1 | bitmap | 4 colors | W/1 | H/1 | 640x400 | 640x400 | 64000 bytes |  
| 0xA2 | bitmap | 4 colors | W/1 | H/2 | 512x320 | 512x160 | 20480 bytes |  
| 0xA3 | bitmap | 4 colors | W/1 | H/2 | 640x400 | 640x200 | 32000 bytes |  
| 0xA4 | bitmap | 4 colors | W/1 | H/3 | 512x320 | 512x106 | 13653 bytes |  
| 0xA5 | bitmap | 4 colors | W/1 | H/3 | 640x400 | 640x133 | 21333 bytes |  
| 0xA6 | bitmap | 4 colors | W/1 | H/4 | 512x320 | 512x80 | 10240 bytes |  
| 0xA7 | bitmap | 4 colors | W/1 | H/4 | 640x400 | 640x100 | 16000 bytes |  
| 0xA8 | bitmap | 4 colors | W/2 | H/1 | 512x320 | 256x320 | 20480 bytes |  
| 0xA9 | bitmap | 4 colors | W/2 | H/1 | 640x400 | 320x400 | 32000 bytes |  
| 0xAA | bitmap | 4 colors | W/2 | H/2 | 512x320 | 256x160 | 10240 bytes |  
| 0xAB | bitmap | 4 colors | W/2 | H/2 | 640x400 | 320x200 | 16000 bytes |  
| 0xAC | bitmap | 4 colors | W/2 | H/3 | 512x320 | 256x106 | 6826 bytes |  
| 0xAD | bitmap | 4 colors | W/2 | H/3 | 640x400 | 320x133 | 10666 bytes |  
| 0xAE | bitmap | 4 colors | W/2 | H/4 | 512x320 | 256x80 | 5120 bytes |  
| 0xAF | bitmap | 4 colors | W/2 | H/4 | 640x400 | 320x100 | 8000 bytes |  
| 0xB0 | bitmap | 4 colors | W/3 | H/1 | 512x320 | 170x320 | 13653 bytes |  
| 0xB1 | bitmap | 4 colors | W/3 | H/1 | 640x400 | 213x400 | 21333 bytes |  
| 0xB2 | bitmap | 4 colors | W/3 | H/2 | 512x320 | 170x160 | 6826 bytes |  
| 0xB3 | bitmap | 4 colors | W/3 | H/2 | 640x400 | 213x200 | 10666 bytes |  
| 0xB4 | bitmap | 4 colors | W/3 | H/3 | 512x320 | 170x106 | 4551 bytes |  
| 0xB5 | bitmap | 4 colors | W/3 | H/3 | 640x400 | 213x133 | 7111 bytes |  
| 0xB6 | bitmap | 4 colors | W/3 | H/4 | 512x320 | 170x80 | 3413 bytes |  
| 0xB7 | bitmap | 4 colors | W/3 | H/4 | 640x400 | 213x100 | 5333 bytes |  
| 0xB8 | bitmap | 4 colors | W/4 | H/1 | 512x320 | 128x320 | 10240 bytes |  
| 0xB9 | bitmap | 4 colors | W/4 | H/1 | 640x400 | 160x400 | 16000 bytes |  
| 0xBA | bitmap | 4 colors | W/4 | H/2 | 512x320 | 128x160 | 5120 bytes |  
| 0xBB | bitmap | 4 colors | W/4 | H/2 | 640x400 | 160x200 | 8000 bytes |  
| 0xBC | bitmap | 4 colors | W/4 | H/3 | 512x320 | 128x106 | 3413 bytes |  
| 0xBD | bitmap | 4 colors | W/4 | H/3 | 640x400 | 160x133 | 5333 bytes |  
| 0xBE | bitmap | 4 colors | W/4 | H/4 | 512x320 | 128x80 | 2560 bytes |  
| 0xBF | bitmap | 4 colors | W/4 | H/4 | 640x400 | 160x100 | 4000 bytes |  
| 0xA3 | bitmap | 4 colors | W/1 | H/2 | 640x400 | 640x200 | 32000 bytes |  
| 0xA4 | bitmap | 4 colors | W/1 | H/3 | 512x320 | 512x106 | 13653 bytes |  
| 0xA5 | bitmap | 4 colors | W/1 | H/3 | 640x400 | 640x133 | 21333 bytes |  
| 0xA6 | bitmap | 4 colors | W/1 | H/4 | 512x320 | 512x80 | 10240 bytes |  
| 0xA7 | bitmap | 4 colors | W/1 | H/4 | 640x400 | 640x100 | 16000 bytes |  
| 0xA8 | bitmap | 4 colors | W/2 | H/1 | 512x320 | 256x320 | 20480 bytes |  
| 0xA9 | bitmap | 4 colors | W/2 | H/1 | 640x400 | 320x400 | 32000 bytes |  
| 0xAA | bitmap | 4 colors | W/2 | H/2 | 512x320 | 256x160 | 10240 bytes |  
| 0xAB | bitmap | 4 colors | W/2 | H/2 | 640x400 | 320x200 | 16000 bytes |  
| 0xAC | bitmap | 4 colors | W/2 | H/3 | 512x320 | 256x106 | 6826 bytes |  
| 0xC2 | bitmap | 16 colors | W/1 | H/2 | 512x320 | 512x160 | 40960 bytes |  
| 0xC3 | bitmap | 16 colors | W/1 | H/2 | 640x400 | 640x200 | 64000 bytes |  
| 0xC4 | bitmap | 16 colors | W/1 | H/3 | 512x320 | 512x106 | 27306 bytes |  
| 0xC5 | bitmap | 16 colors | W/1 | H/3 | 640x400 | 640x133 | 42666 bytes |  
| 0xC6 | bitmap | 16 colors | W/1 | H/4 | 512x320 | 512x80 | 20480 bytes |  
| 0xC7 | bitmap | 16 colors | W/1 | H/4 | 640x400 | 640x100 | 32000 bytes |  
| 0xC8 | bitmap | 16 colors | W/2 | H/1 | 512x320 | 256x320 | 40960 bytes |  
| 0xC9 | bitmap | 16 colors | W/2 | H/1 | 640x400 | 320x400 | 64000 bytes |  
| 0xCA | bitmap | 16 colors | W/2 | H/2 | 512x320 | 256x160 | 20480 bytes |  
| 0xCB | bitmap | 16 colors | W/2 | H/2 | 640x400 | 320x200 | 32000 bytes |  
| 0xCC | bitmap | 16 colors | W/2 | H/3 | 512x320 | 256x106 | 13653 bytes |  
| 0xCD | bitmap | 16 colors | W/2 | H/3 | 640x400 | 320x133 | 21333 bytes |  
| 0xCE | bitmap | 16 colors | W/2 | H/4 | 512x320 | 256x80 | 10240 bytes |  
| 0xCF | bitmap | 16 colors | W/2 | H/4 | 640x400 | 320x100 | 16000 bytes |  
| 0xD0 | bitmap | 16 colors | W/3 | H/1 | 512x320 | 170x320 | 27306 bytes |  
| 0xD1 | bitmap | 16 colors | W/3 | H/1 | 640x400 | 213x400 | 42666 bytes |  
| 0xD2 | bitmap | 16 colors | W/3 | H/2 | 512x320 | 170x160 | 13653 bytes |  
| 0xD3 | bitmap | 16 colors | W/3 | H/2 | 640x400 | 213x200 | 21333 bytes |  
| 0xD4 | bitmap | 16 colors | W/3 | H/3 | 512x320 | 170x106 | 9102 bytes |  
| 0xD5 | bitmap | 16 colors | W/3 | H/3 | 640x400 | 213x133 | 14222 bytes |  
| 0xD6 | bitmap | 16 colors | W/3 | H/4 | 512x320 | 170x80 | 6826 bytes |  
| 0xD7 | bitmap | 16 colors | W/3 | H/4 | 640x400 | 213x100 | 10666 bytes |  
| 0xD8 | bitmap | 16 colors | W/4 | H/1 | 512x320 | 128x320 | 20480 bytes |  
| 0xD9 | bitmap | 16 colors | W/4 | H/1 | 640x400 | 160x400 | 32000 bytes |  
| 0xDA | bitmap | 16 colors | W/4 | H/2 | 512x320 | 128x160 | 10240 bytes |  
| 0xDB | bitmap | 16 colors | W/4 | H/2 | 640x400 | 160x200 | 16000 bytes |  
| 0xDC | bitmap | 16 colors | W/4 | H/3 | 512x320 | 128x106 | 6826 bytes |  
| 0xDD | bitmap | 16 colors | W/4 | H/3 | 640x400 | 160x133 | 10666 bytes |  
| 0xDE | bitmap | 16 colors | W/4 | H/4 | 512x320 | 128x80 | 5120 bytes |  
| 0xDF | bitmap | 16 colors | W/4 | H/4 | 640x400 | 160x100 | 8000 bytes |  
| 0xE4 | bitmap | 256 colors | W/1 | H/3 | 512x320 | 512x106 | 54613 bytes |  
| 0xE6 | bitmap | 256 colors | W/1 | H/4 | 512x320 | 512x80 | 40960 bytes |  
| 0xE7 | bitmap | 256 colors | W/1 | H/4 | 640x400 | 640x100 | 64000 bytes |  
| 0xEA | bitmap | 256 colors | W/2 | H/2 | 512x320 | 256x160 | 40960 bytes |  
| 0xEB | bitmap | 256 colors | W/2 | H/2 | 640x400 | 320x200 | 64000 bytes |  
| 0xEC | bitmap | 256 colors | W/2 | H/3 | 512x320 | 256x106 | 27306 bytes |  
| 0xED | bitmap | 256 colors | W/2 | H/3 | 640x400 | 320x133 | 42666 bytes |  
| 0xEE | bitmap | 256 colors | W/2 | H/4 | 512x320 | 256x80 | 20480 bytes |  
| 0xEF | bitmap | 256 colors | W/2 | H/4 | 640x400 | 320x100 | 32000 bytes |  
| 0xF0 | bitmap | 256 colors | W/3 | H/1 | 512x320 | 170x320 | 54613 bytes |  
| 0xF2 | bitmap | 256 colors | W/3 | H/2 | 512x320 | 170x160 | 27306 bytes |  
| 0xF3 | bitmap | 256 colors | W/3 | H/2 | 640x400 | 213x200 | 42666 bytes |  
| 0xF4 | bitmap | 256 colors | W/3 | H/3 | 512x320 | 170x106 | 18204 bytes |  
| 0xF5 | bitmap | 256 colors | W/3 | H/3 | 640x400 | 213x133 | 28444 bytes |  
| 0xF6 | bitmap | 256 colors | W/3 | H/4 | 512x320 | 170x80 | 13653 bytes |  
| 0xF7 | bitmap | 256 colors | W/3 | H/4 | 640x400 | 213x100 | 21333 bytes |  
| 0xF8 | bitmap | 256 colors | W/4 | H/1 | 512x320 | 128x320 | 40960 bytes |  
| 0xF9 | bitmap | 256 colors | W/4 | H/1 | 640x400 | 160x400 | 64000 bytes |  
| 0xFA | bitmap | 256 colors | W/4 | H/2 | 512x320 | 128x160 | 20480 bytes |  
| 0xFB | bitmap | 256 colors | W/4 | H/2 | 640x400 | 160x200 | 32000 bytes |  
| 0xFC | bitmap | 256 colors | W/4 | H/3 | 512x320 | 128x106 | 13653 bytes |  
| 0xFD | bitmap | 256 colors | W/4 | H/3 | 640x400 | 160x133 | 21333 bytes |  
| 0xFE | bitmap | 256 colors | W/4 | H/4 | 512x320 | 128x80 | 10240 bytes |  
| 0xFF | bitmap | 256 colors | W/4 | H/4 | 640x400 | 160x100 | 16000 bytes |  

