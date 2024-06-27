#ifndef _CONFIG_H_
#define _CONFIG_H_

#define DISPLAY_MAX_COUNT 2
#define SCREEN_MAX_COUNT DISPLAY_MAX_COUNT //for now, 1:1 between screen and display

#if (DISPLAY_MAX_COUNT > 255) || (DISPLAY_MAX_COUNT <= 0)
#error "DISPLAY_MAX_COUNT must be in range [1, 255]."
#endif

#if (SCREEN_MAX_COUNT > 255) || (SCREEN_MAX_COUNT <= 0)
#error "SCREEN_MAX_COUNT must be in range [1, 255]."
#endif

#endif