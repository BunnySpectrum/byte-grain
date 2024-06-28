#include "socket_display.h"


BG_CODES_e factory_socket_display(DisplayContext_s **displayCtx, SocketDisplayCtx_s *pDriverCtx){
    uint8_t hDisplay;

    if(BG_SUCCESS != display_new_handle(&hDisplay)){
        *displayCtx = 0;
        return BG_FAIL;
    }

    if(BG_SUCCESS != display_context_for_handle(hDisplay, displayCtx)){
        *displayCtx = 0;
        return BG_FAIL;
    }

    (*displayCtx)->hDisplay = hDisplay;
    (*displayCtx)->display_register = socket_disp_register;
    (*displayCtx)->display_init = socket_disp_init;
    (*displayCtx)->display_deinit = socket_disp_deinit;
    (*displayCtx)->display_draw_pixel = socket_disp_draw_pixel;
    (*displayCtx)->display_draw_buffer = socket_disp_draw_buffer;
    (*displayCtx)->pDriverCtx = pDriverCtx;

    return BG_SUCCESS;
}

BG_CODES_e socket_disp_register(uint8_t hDisplay){
    DisplayContext_s *pDispCtx;

    if(BG_SUCCESS != display_context_for_handle(hDisplay, &pDispCtx)){
        return BG_FAIL;
    }

    setup_socket("/tmp/test_socket2", &((SocketDisplayCtx_s*)pDispCtx->pDriverCtx)->conn);
    return BG_SUCCESS;
}

BG_CODES_e socket_disp_init(DisplayContext_s *displayCtx){
    sockServerConn_s conn;

    if(BG_SUCCESS != accept_conn(&((SocketDisplayCtx_s*)displayCtx->pDriverCtx)->conn)){
        perror("accept:");
        exit(1);
    }
    return BG_SUCCESS;
}

BG_CODES_e socket_disp_deinit(DisplayContext_s *displayCtx){
    sockServerConn_s conn;

    close_conn(&((SocketDisplayCtx_s*)displayCtx->pDriverCtx)->conn);
    return BG_SUCCESS;
}

BG_CODES_e socket_disp_draw_pixel(DisplayContext_s *displayCtx, uint16_t x, uint16_t y, BG_COLOR_e color){
    return BG_FAIL;
}

BG_CODES_e socket_disp_draw_buffer(DisplayContext_s *displayCtx, uint16_t x, uint16_t y, uint16_t width, uint16_t height, BG_COLOR_e *pColor){
    sockServerConn_s conn;

    if(BG_SUCCESS != send_buffer(&((SocketDisplayCtx_s*)displayCtx->pDriverCtx)->conn, pColor, width * height)){
        return BG_FAIL;
    }
    return BG_SUCCESS;
}