#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>
#include "config.h"
#include "utils/bg_codes.h"
#include "utils/bg_colors.h"

typedef struct DisplayContext DisplayContext_s;

typedef BG_CODES_e (*fp_display_register)(uint8_t hDisplay);
typedef BG_CODES_e (*fp_display_init)(DisplayContext_s *displayCtx);
typedef BG_CODES_e (*fp_display_deinit)(DisplayContext_s *displayCtx);
typedef BG_CODES_e (*fp_display_draw_pixel)(DisplayContext_s *displayCtx, uint16_t x, uint16_t y, BG_COLOR_e color);
typedef BG_CODES_e (*fp_display_draw_buffer)(DisplayContext_s *displayCtx, uint16_t x, uint16_t y, uint16_t width, uint16_t height, BG_COLOR_e *pColor);

typedef struct DisplayContext{
    uint8_t hDisplay;
    fp_display_register display_register;
    fp_display_init display_init;
    fp_display_init display_deinit;
    fp_display_draw_pixel display_draw_pixel;
    fp_display_draw_buffer display_draw_buffer;
}DisplayContext_s;

// typedef struct SocketDisplayDriver{
//     uint8_t foo;
// } SocketDisplayDriver_s;

// struct SocketDisplayContext{
//     DisplayContext_s dispCtx;
//     SocketDisplayDriver_s dispDrv;
// };


BG_CODES_e display_new_handle(uint8_t *handle);
BG_CODES_e display_context_for_handle(uint8_t handle, DisplayContext_s **ctx);



#endif