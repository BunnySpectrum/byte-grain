#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <assert.h>


#define DISPLAY_MAX_COUNT 2
#define SCREEN_MAX_COUNT DISPLAY_MAX_COUNT //for now, 1:1 between screen and display

_Static_assert(!(DISPLAY_MAX_COUNT > 255) || (DISPLAY_MAX_COUNT <= 0), "DISPLAY_MAX_COUNT must be in range [1, 255].");
_Static_assert(!(SCREEN_MAX_COUNT > 255) || (SCREEN_MAX_COUNT <= 0), "SCREEN_MAX_COUNT must be in range [1, 255].");

// #if (DISPLAY_MAX_COUNT > 255) || (DISPLAY_MAX_COUNT <= 0)
// #error "DISPLAY_MAX_COUNT must be in range [1, 255]."
// #endif

// #if (SCREEN_MAX_COUNT > 255) || (SCREEN_MAX_COUNT <= 0)
// #error "SCREEN_MAX_COUNT must be in range [1, 255]."
// #endif

#endif