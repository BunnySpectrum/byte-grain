#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdint.h>
#include <stdio.h>
#include "config.h"
#include "xsp/socket_display.h"
#include "utils/bg_codes.h"
#include "utils/image_buf.h"

// We have one screen context per screen in the application
// the context contains the handle for the screen and the display
typedef struct ScreenContext{
    uint8_t hScreen;
    DisplayContext_s *pDispCtx;
    ImageBuf_s *pImageBuf;
}ScreenContext_s;

BG_CODES_e screen_new_handle(uint8_t *handle);
BG_CODES_e screen_context_for_handle(uint8_t handle, ScreenContext_s **ctx);

BG_CODES_e factory_screen(ScreenContext_s **screenCtx, DisplayContext_s *pDispCtx, ImageBuf_s *pImageBuf);

BG_CODES_e screen_register(uint8_t hScreen);
BG_CODES_e screen_init(ScreenContext_s *pScreenCtx);
BG_CODES_e screen_update(uint8_t hScreen);
BG_CODES_e screen_deinit(ScreenContext_s *pScreenCtx);

#endif