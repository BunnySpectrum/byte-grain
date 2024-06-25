#ifndef _BG_COLORS_H_
#define _BG_COLORS_H_

// limited to values [0, 0x3F]
typedef enum BG_COLOR{
    BG_COLOR_WHITE = 0,
    BG_COLOR_BLACK = 1,
    BG_COLOR_RED = 2,
    BG_COLOR_BLUE = 3,
    BG_COLOR_MAX = 0x3F
}BG_COLOR_e;

// https://chrishewett.com/blog/true-rgb565-colour-picker/
typedef enum BG_RGB565{
    BG_RGB565_BLACK = 0x0000,
    BG_RGB565_WHITE = 0xFFFF,
    BG_RGB565_RED = 0xF800,
    BG_RGB565_YELLOW = 0xFFE0,
    BG_RGB565_MAGENTA = 0xF81F,
    BG_RGB565_GREEN = 0x07E0,
    BG_RGB565_CYAN = 0x07FF,
    BG_RGB565_BLUE = 0x001F,

}BG_RGB565_e;

#endif