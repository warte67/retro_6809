# Standard Display Resolution Study:

## Standard Text Modes

| MODE | SCREEN | TIMING | H.OVER | V.OVER | RESOLUTION | COLORS | BUFFER |
|:----:|:------:|:------:|:------:|:------:|:----------:|:------:|:------:|
| 0x00 | text |   512x320 | 1x | 1x | 64x40   |      monochrome text        |2560   |
| 0x01 | text |   512x320 | 1x | 1x | 64x40   |        bg/fg + text         |5120   |
| 0x02 | text |   512x320 | 1x | 1x | 64x40   |  clear-bgnd 256-color text  |5120   |
| 0x03 | text |   512x320 | 1x | 1x | 64x40   | color_0-bgnd 256-color text |5120   |
| 0x04 | text |   512x320 | 1x | 2x | 64x20   |      monochrome text        |1280   |
| 0x05 | text |   512x320 | 1x | 2x | 64x20   |        bg/fg + text         |2560   |
| 0x06 | text |   512x320 | 1x | 2x | 64x20   |  clear-bgnd 256-color text  |2560   |
| 0x07 | text |   512x320 | 1x | 2x | 64x20   | color_0-bgnd 256-color text |2560   |
| 0x08 | text |   512x320 | 1x | 3x | 64x13   |      monochrome text        |853    |
| 0x09 | text |   512x320 | 1x | 3x | 64x13   |        bg/fg + text         |1706   |
| 0x0A | text |   512x320 | 1x | 3x | 64x13   |  clear-bgnd 256-color text  |1706   |
| 0x0B | text |   512x320 | 1x | 3x | 64x13   | color_0-bgnd 256-color text |1706   |
| 0x0C | text |   512x320 | 1x | 4x | 64x10   |      monochrome text        |640    |
| 0x0D | text |   512x320 | 1x | 4x | 64x10   |        bg/fg + text         |1280   |
| 0x0E | text |   512x320 | 1x | 4x | 64x10   |  clear-bgnd 256-color text  |1280   |
| 0x0F | text |   512x320 | 1x | 4x | 64x10   | color_0-bgnd 256-color text |1280   |
| 0x10 | text |   512x320 | 2x | 1x | 32x40   |      monochrome text        |1280   |
| 0x11 | text |   512x320 | 2x | 1x | 32x40   |        bg/fg + text         |2560   |
| 0x12 | text |   512x320 | 2x | 1x | 32x40   |  clear-bgnd 256-color text  |2560   |
| 0x13 | text |   512x320 | 2x | 1x | 32x40   | color_0-bgnd 256-color text |2560   |
| 0x14 | text |   512x320 | 2x | 2x | 32x20   |      monochrome text        |640    |
| 0x15 | text |   512x320 | 2x | 2x | 32x20   |        bg/fg + text         |1280   |
| 0x16 | text |   512x320 | 2x | 2x | 32x20   |  clear-bgnd 256-color text  |1280   |
| 0x17 | text |   512x320 | 2x | 2x | 32x20   | color_0-bgnd 256-color text |1280   |
| 0x18 | text |   512x320 | 2x | 3x | 32x13   |      monochrome text        |426    |
| 0x19 | text |   512x320 | 2x | 3x | 32x13   |        bg/fg + text         |853    |
| 0x1A | text |   512x320 | 2x | 3x | 32x13   |  clear-bgnd 256-color text  |853    |
| 0x1B | text |   512x320 | 2x | 3x | 32x13   | color_0-bgnd 256-color text |853    |
| 0x1C | text |   512x320 | 2x | 4x | 32x10   |      monochrome text        |320    |
| 0x1D | text |   512x320 | 2x | 4x | 32x10   |        bg/fg + text         |640    |
| 0x1E | text |   512x320 | 2x | 4x | 32x10   |  clear-bgnd 256-color text  |640    |
| 0x1F | text |   512x320 | 2x | 4x | 32x10   | color_0-bgnd 256-color text |640    |
| 0x20 | text |   512x320 | 3x | 1x | 21x40   |      monochrome text        |853    |
| 0x21 | text |   512x320 | 3x | 1x | 21x40   |        bg/fg + text         |1706   |
| 0x22 | text |   512x320 | 3x | 1x | 21x40   |  clear-bgnd 256-color text  |1706   |
| 0x23 | text |   512x320 | 3x | 1x | 21x40   | color_0-bgnd 256-color text |1706   |
| 0x24 | text |   512x320 | 3x | 2x | 21x20   |      monochrome text        |426    |
| 0x25 | text |   512x320 | 3x | 2x | 21x20   |        bg/fg + text         |853    |
| 0x26 | text |   512x320 | 3x | 2x | 21x20   |  clear-bgnd 256-color text  |853    |
| 0x27 | text |   512x320 | 3x | 2x | 21x20   | color_0-bgnd 256-color text |853    |
| 0x28 | text |   512x320 | 3x | 3x | 21x13   |      monochrome text        |284    |
| 0x29 | text |   512x320 | 3x | 3x | 21x13   |        bg/fg + text         |568    |
| 0x2A | text |   512x320 | 3x | 3x | 21x13   |  clear-bgnd 256-color text  |568    |
| 0x2B | text |   512x320 | 3x | 3x | 21x13   | color_0-bgnd 256-color text |568    |
| 0x2C | text |   512x320 | 3x | 4x | 21x10   |      monochrome text        |213    |
| 0x2D | text |   512x320 | 3x | 4x | 21x10   |        bg/fg + text         |426    |
| 0x2E | text |   512x320 | 3x | 4x | 21x10   |  clear-bgnd 256-color text  |426    |
| 0x2F | text |   512x320 | 3x | 4x | 21x10   | color_0-bgnd 256-color text |426    |
| 0x30 | text |   512x320 | 4x | 1x | 16x40   |      monochrome text        |640    |
| 0x31 | text |   512x320 | 4x | 1x | 16x40   |        bg/fg + text         |1280   |
| 0x32 | text |   512x320 | 4x | 1x | 16x40   |  clear-bgnd 256-color text  |1280   |
| 0x33 | text |   512x320 | 4x | 1x | 16x40   | color_0-bgnd 256-color text |1280   |
| 0x34 | text |   512x320 | 4x | 2x | 16x20   |      monochrome text        |320    |
| 0x35 | text |   512x320 | 4x | 2x | 16x20   |        bg/fg + text         |640    |
| 0x36 | text |   512x320 | 4x | 2x | 16x20   |  clear-bgnd 256-color text  |640    |
| 0x37 | text |   512x320 | 4x | 2x | 16x20   | color_0-bgnd 256-color text |640    |
| 0x38 | text |   512x320 | 4x | 3x | 16x13   |      monochrome text        |213    |
| 0x39 | text |   512x320 | 4x | 3x | 16x13   |        bg/fg + text         |426    |
| 0x3A | text |   512x320 | 4x | 3x | 16x13   |  clear-bgnd 256-color text  |426    |
| 0x3B | text |   512x320 | 4x | 3x | 16x13   | color_0-bgnd 256-color text |426    |
| 0x3C | text |   512x320 | 4x | 4x | 16x10   |      monochrome text        |160    |
| 0x3D | text |   512x320 | 4x | 4x | 16x10   |        bg/fg + text         |320    |
| 0x3E | text |   512x320 | 4x | 4x | 16x10   |  clear-bgnd 256-color text  |320    |
| 0x3F | text |   512x320 | 4x | 4x | 16x10   | color_0-bgnd 256-color text |320    |
| 0x40 | text |   640x400 | 1x | 1x | 80x50   |      monochrome text        |4000   |
| 0x41 | text |   640x400 | 1x | 1x | 80x50   |        bg/fg + text         |8000   |
| 0x42 | text |   640x400 | 1x | 1x | 80x50   |  clear-bgnd 256-color text  |8000   |
| 0x43 | text |   640x400 | 1x | 1x | 80x50   | color_0-bgnd 256-color text |8000   |
| 0x44 | text |   640x400 | 1x | 2x | 80x25   |      monochrome text        |2000   |
| 0x45 | text |   640x400 | 1x | 2x | 80x25   |        bg/fg + text         |4000   |
| 0x46 | text |   640x400 | 1x | 2x | 80x25   |  clear-bgnd 256-color text  |4000   |
| 0x47 | text |   640x400 | 1x | 2x | 80x25   | color_0-bgnd 256-color text |4000   |
| 0x48 | text |   640x400 | 1x | 3x | 80x16   |      monochrome text        |1333   |
| 0x49 | text |   640x400 | 1x | 3x | 80x16   |        bg/fg + text         |2666   |
| 0x4A | text |   640x400 | 1x | 3x | 80x16   |  clear-bgnd 256-color text  |2666   |
| 0x4B | text |   640x400 | 1x | 3x | 80x16   | color_0-bgnd 256-color text |2666   |
| 0x4C | text |   640x400 | 1x | 4x | 80x12   |      monochrome text        |1000   |
| 0x4D | text |   640x400 | 1x | 4x | 80x12   |        bg/fg + text         |2000   |
| 0x4E | text |   640x400 | 1x | 4x | 80x12   |  clear-bgnd 256-color text  |2000   |
| 0x4F | text |   640x400 | 1x | 4x | 80x12   | color_0-bgnd 256-color text |2000   |
| 0x50 | text |   640x400 | 2x | 1x | 40x50   |      monochrome text        |2000   |
| 0x51 | text |   640x400 | 2x | 1x | 40x50   |        bg/fg + text         |4000   |
| 0x52 | text |   640x400 | 2x | 1x | 40x50   |  clear-bgnd 256-color text  |4000   |
| 0x53 | text |   640x400 | 2x | 1x | 40x50   | color_0-bgnd 256-color text |4000   |
| 0x54 | text |   640x400 | 2x | 2x | 40x25   |      monochrome text        |1000   |
| 0x55 | text |   640x400 | 2x | 2x | 40x25   |        bg/fg + text         |2000   |
| 0x56 | text |   640x400 | 2x | 2x | 40x25   |  clear-bgnd 256-color text  |2000   |
| 0x57 | text |   640x400 | 2x | 2x | 40x25   | color_0-bgnd 256-color text |2000   |
| 0x58 | text |   640x400 | 2x | 3x | 40x16   |      monochrome text        |666    |
| 0x59 | text |   640x400 | 2x | 3x | 40x16   |        bg/fg + text         |1333   |
| 0x5A | text |   640x400 | 2x | 3x | 40x16   |  clear-bgnd 256-color text  |1333   |
| 0x5B | text |   640x400 | 2x | 3x | 40x16   | color_0-bgnd 256-color text |1333   |
| 0x5C | text |   640x400 | 2x | 4x | 40x12   |      monochrome text        |500    |
| 0x5D | text |   640x400 | 2x | 4x | 40x12   |        bg/fg + text         |1000   |
| 0x5E | text |   640x400 | 2x | 4x | 40x12   |  clear-bgnd 256-color text  |1000   |
| 0x5F | text |   640x400 | 2x | 4x | 40x12   | color_0-bgnd 256-color text |1000   |
| 0x60 | text |   640x400 | 3x | 1x | 26x50   |      monochrome text        |1333   |
| 0x61 | text |   640x400 | 3x | 1x | 26x50   |        bg/fg + text         |2666   |
| 0x62 | text |   640x400 | 3x | 1x | 26x50   |  clear-bgnd 256-color text  |2666   |
| 0x63 | text |   640x400 | 3x | 1x | 26x50   | color_0-bgnd 256-color text |2666   |
| 0x64 | text |   640x400 | 3x | 2x | 26x25   |      monochrome text        |666    |
| 0x65 | text |   640x400 | 3x | 2x | 26x25   |        bg/fg + text         |1333   |
| 0x66 | text |   640x400 | 3x | 2x | 26x25   |  clear-bgnd 256-color text  |1333   |
| 0x67 | text |   640x400 | 3x | 2x | 26x25   | color_0-bgnd 256-color text |1333   |
| 0x68 | text |   640x400 | 3x | 3x | 26x16   |      monochrome text        |444    |
| 0x69 | text |   640x400 | 3x | 3x | 26x16   |        bg/fg + text         |888    |
| 0x6A | text |   640x400 | 3x | 3x | 26x16   |  clear-bgnd 256-color text  |888    |
| 0x6B | text |   640x400 | 3x | 3x | 26x16   | color_0-bgnd 256-color text |888    |
| 0x6C | text |   640x400 | 3x | 4x | 26x12   |      monochrome text        |333    |
| 0x6D | text |   640x400 | 3x | 4x | 26x12   |        bg/fg + text         |666    |
| 0x6E | text |   640x400 | 3x | 4x | 26x12   |  clear-bgnd 256-color text  |666    |
| 0x6F | text |   640x400 | 3x | 4x | 26x12   | color_0-bgnd 256-color text |666    |
| 0x70 | text |   640x400 | 4x | 1x | 20x50   |      monochrome text        |1000   |
| 0x71 | text |   640x400 | 4x | 1x | 20x50   |        bg/fg + text         |2000   |
| 0x72 | text |   640x400 | 4x | 1x | 20x50   |  clear-bgnd 256-color text  |2000   |
| 0x73 | text |   640x400 | 4x | 1x | 20x50   | color_0-bgnd 256-color text |2000   |
| 0x74 | text |   640x400 | 4x | 2x | 20x25   |      monochrome text        |500    |
| 0x75 | text |   640x400 | 4x | 2x | 20x25   |        bg/fg + text         |1000   |
| 0x76 | text |   640x400 | 4x | 2x | 20x25   |  clear-bgnd 256-color text  |1000   |
| 0x77 | text |   640x400 | 4x | 2x | 20x25   | color_0-bgnd 256-color text |1000   |
| 0x78 | text |   640x400 | 4x | 3x | 20x16   |      monochrome text        |333    |
| 0x79 | text |   640x400 | 4x | 3x | 20x16   |        bg/fg + text         |666    |
| 0x7A | text |   640x400 | 4x | 3x | 20x16   |  clear-bgnd 256-color text  |666    |
| 0x7B | text |   640x400 | 4x | 3x | 20x16   | color_0-bgnd 256-color text |666    |
| 0x7C | text |   640x400 | 4x | 4x | 20x12   |      monochrome text        |250    |
| 0x7D | text |   640x400 | 4x | 4x | 20x12   |        bg/fg + text         |500    |
| 0x7E | text |   640x400 | 4x | 4x | 20x12   |  clear-bgnd 256-color text  |500    |
| 0x7F | text |   640x400 | 4x | 4x | 20x12   | color_0-bgnd 256-color text |500    |

## Standard Bitmap Modes

| MODE | SCREEN | TIMING | H.OVER | V.OVER | RESOLUTION | COLORS | BUFFER |
|:----:|:------:|:------:|:------:|:------:|:----------:|:------:|:------:|
| 0x88 | bitmap | 512x320 | 1x | 3x | 512x106 |    2 colors |    6826   |
| 0x8C | bitmap | 512x320 | 1x | 4x | 512x80  |    2 colors |    5120   |
| 0x94 | bitmap | 512x320 | 2x | 2x | 256x160 |    2 colors |    5120   |
| 0x98 | bitmap | 512x320 | 2x | 3x | 256x106 |    2 colors |    3413   |
| 0x99 | bitmap | 512x320 | 2x | 3x | 256x106 |    4 colors |    6826   |
| 0x9C | bitmap | 512x320 | 2x | 4x | 256x80  |    2 colors |    2560   |
| 0x9D | bitmap | 512x320 | 2x | 4x | 256x80  |    4 colors |    5120   |
| 0xA0 | bitmap | 512x320 | 3x | 1x | 170x320 |    2 colors |    6826   |
| 0xA4 | bitmap | 512x320 | 3x | 2x | 170x160 |    2 colors |    3413   |
| 0xA5 | bitmap | 512x320 | 3x | 2x | 170x160 |    4 colors |    6826   |
| 0xA8 | bitmap | 512x320 | 3x | 3x | 170x106 |    2 colors |    2275   |
| 0xA9 | bitmap | 512x320 | 3x | 3x | 170x106 |    4 colors |    4551   |
| 0xAC | bitmap | 512x320 | 3x | 4x | 170x80  |    2 colors |    1706   |
| 0xAD | bitmap | 512x320 | 3x | 4x | 170x80  |    4 colors |    3413   |
| 0xAE | bitmap | 512x320 | 3x | 4x | 170x80  |   16 colors |    6826   |
| 0xB0 | bitmap | 512x320 | 4x | 1x | 128x320 |    2 colors |    5120   |
| 0xB4 | bitmap | 512x320 | 4x | 2x | 128x160 |    2 colors |    2560   |
| 0xB5 | bitmap | 512x320 | 4x | 2x | 128x160 |    4 colors |    5120   |
| 0xB8 | bitmap | 512x320 | 4x | 3x | 128x106 |    2 colors |    1706   |
| 0xB9 | bitmap | 512x320 | 4x | 3x | 128x106 |    4 colors |    3413   |
| 0xBA | bitmap | 512x320 | 4x | 3x | 128x106 |   16 colors |    6826   |
| 0xBC | bitmap | 512x320 | 4x | 4x | 128x80  |    2 colors |    1280   |
| 0xBD | bitmap | 512x320 | 4x | 4x | 128x80  |    4 colors |    2560   |
| 0xBE | bitmap | 512x320 | 4x | 4x | 128x80  |   16 colors |    5120   |
| 0xCC | bitmap | 640x400 | 1x | 4x | 640x100 |    2 colors |    8000   |
| 0xD4 | bitmap | 640x400 | 2x | 2x | 320x200 |    2 colors |    8000   |
| 0xD8 | bitmap | 640x400 | 2x | 3x | 320x133 |    2 colors |    5333   |
| 0xDC | bitmap | 640x400 | 2x | 4x | 320x100 |    2 colors |    4000   |
| 0xDD | bitmap | 640x400 | 2x | 4x | 320x100 |    4 colors |    8000   |
| 0xE4 | bitmap | 640x400 | 3x | 2x | 213x200 |    2 colors |    5333   |
| 0xE8 | bitmap | 640x400 | 3x | 3x | 213x133 |    2 colors |    3555   |
| 0xE9 | bitmap | 640x400 | 3x | 3x | 213x133 |    4 colors |    7111   |
| 0xEC | bitmap | 640x400 | 3x | 4x | 213x100 |    2 colors |    2666   |
| 0xED | bitmap | 640x400 | 3x | 4x | 213x100 |    4 colors |    5333   |
| 0xF0 | bitmap | 640x400 | 4x | 1x | 160x400 |    2 colors |    8000   |
| 0xF4 | bitmap | 640x400 | 4x | 2x | 160x200 |    2 colors |    4000   |
| 0xF5 | bitmap | 640x400 | 4x | 2x | 160x200 |    4 colors |    8000   |
| 0xF8 | bitmap | 640x400 | 4x | 3x | 160x133 |    2 colors |    2666   |
| 0xF9 | bitmap | 640x400 | 4x | 3x | 160x133 |    4 colors |    5333   |
| 0xFC | bitmap | 640x400 | 4x | 4x | 160x100 |    2 colors |    2000   |
| 0xFD | bitmap | 640x400 | 4x | 4x | 160x100 |    4 colors |    4000   |
| 0xFE | bitmap | 640x400 | 4x | 4x | 160x100 |   16 colors |    8000   |
