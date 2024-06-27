#ifndef _SOCKET_DISPLAY_H_
#define _SOCKET_DISPLAY_H_

#include "xsp/display.h"
#include "xal/socket_server.h"


BG_CODES_e factory_socket_display(DisplayContext_s **displayCtx);

BG_CODES_e socket_disp_register(uint8_t hDisplay);
BG_CODES_e socket_disp_init(DisplayContext_s *displayCtx);
BG_CODES_e socket_disp_deinit(DisplayContext_s *displayCtx);
BG_CODES_e socket_disp_draw_pixel(DisplayContext_s *displayCtx, uint16_t x, uint16_t y, BG_COLOR_e color);
BG_CODES_e socket_disp_draw_buffer(DisplayContext_s *displayCtx, uint16_t x, uint16_t y, uint16_t width, uint16_t height, BG_COLOR_e *pColor);


#endif