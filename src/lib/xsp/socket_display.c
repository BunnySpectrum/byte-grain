#include "socket_display.h"

// For now, only one socket display
static sockServerConn_s conn;

BG_CODES_e factory_socket_display(DisplayContext_s *displayCtx){
    displayCtx->display_register = socket_disp_register;
    displayCtx->display_init = socket_disp_init;
    displayCtx->display_draw_pixel = socket_disp_draw_pixel;
    displayCtx->display_draw_buffer = socket_disp_draw_buffer;

    return BG_SUCCESS;
}

BG_CODES_e socket_disp_register(uint8_t *hDisplay){
    if(BG_SUCCESS != display_new_handle(hDisplay)){
        return BG_FAIL;
    }
    setup_socket("/tmp/test_socket2", &conn);
    return BG_SUCCESS;
}

BG_CODES_e socket_disp_init(DisplayContext_s *displayCtx){
    if(BG_SUCCESS != accept_conn(&conn)){
        perror("accept:");
        exit(1);
    }
    return BG_SUCCESS;
}

BG_CODES_e socket_disp_deinit(DisplayContext_s *displayCtx){
    close_conn(&conn);
    return BG_SUCCESS;
}

BG_CODES_e socket_disp_draw_pixel(DisplayContext_s *displayCtx, uint16_t x, uint16_t y, BG_COLOR_e color){
    return BG_FAIL;
}

BG_CODES_e socket_disp_draw_buffer(DisplayContext_s *displayCtx, uint16_t x, uint16_t y, uint16_t width, uint16_t height, BG_COLOR_e *pColor){

    if(BG_SUCCESS != send_buffer(&conn, pColor, width * height)){
        return BG_FAIL;
    }
    return BG_SUCCESS;
}