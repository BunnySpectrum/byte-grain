#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdint.h>
#include "utils/bg_codes.h"

// We have one screen context per screen in the application
// the context contains the handle for the screen and the display
typedef struct ScreenContext{
    uint8_t hScreen;
    uint8_t hDisplay;
}ScreenContext_s;

BG_CODES_e screen_register(uint8_t *hScreen);

BG_CODES_e screen_init(ScreenContext_s screenCtx);

void screen_update();

#endif