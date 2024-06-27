#include "screen.h"


static uint8_t activeScreens = 0;
static ScreenContext_s screenContexts[SCREEN_MAX_COUNT] = {};


BG_CODES_e screen_new_handle(uint8_t *handle){
    activeScreens++;

    if( (SCREEN_MAX_COUNT == activeScreens) ){
        *handle = 0;
        printf("Unable to allocate more screen handles.");
        return BG_FAIL;
    }

    *handle = activeScreens;

    return BG_SUCCESS;
}

BG_CODES_e screen_context_for_handle(uint8_t handle, ScreenContext_s **ctx){
    if(handle == 0 || handle > SCREEN_MAX_COUNT){
        printf("Screen handle was %d but must be in range [0, %d].\n", handle, SCREEN_MAX_COUNT);
        ctx = 0;
        return BG_FAIL;
    }

    *ctx = &screenContexts[handle - 1];

    return BG_SUCCESS;
}

BG_CODES_e factory_screen(ScreenContext_s **screenCtx, DisplayContext_s *pDispCtx, ImageBuf_s *pImageBuf){
    uint8_t hScreen;

    if(BG_SUCCESS != screen_new_handle(&hScreen)){
        *screenCtx = 0;
        return BG_FAIL;
    }

    if(BG_SUCCESS != screen_context_for_handle(hScreen, screenCtx)){
        *screenCtx = 0;
        return BG_FAIL;
    }

    (*screenCtx)->hScreen = hScreen;
    (*screenCtx)->pDispCtx = pDispCtx;
    (*screenCtx)->pImageBuf = pImageBuf;
    printf("Screen factory: hScreen %d, hDisplay %d.\n", (*screenCtx)->hScreen, (*screenCtx)->pDispCtx->hDisplay);

    return BG_SUCCESS;
}


BG_CODES_e screen_register(uint8_t hScreen){
    return BG_SUCCESS;
}

BG_CODES_e screen_init(ScreenContext_s *pScreenCtx){
    return pScreenCtx->pDispCtx->display_init(pScreenCtx->pDispCtx);
}

BG_CODES_e screen_update(uint8_t hScreen){
    ScreenContext_s *pScreenCtx;

    if(BG_SUCCESS != screen_context_for_handle(hScreen, &pScreenCtx)){
        return BG_FAIL;
    }

    if(BG_SUCCESS != pScreenCtx->pDispCtx->display_draw_buffer(pScreenCtx->pDispCtx, 0, 0, 
        pScreenCtx->pImageBuf->width, pScreenCtx->pImageBuf->height, (BG_COLOR_e*)pScreenCtx->pImageBuf->pImgData) ){
        printf("Draw error.\n");
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e screen_deinit(ScreenContext_s *pScreenCtx){

    return pScreenCtx->pDispCtx->display_deinit(pScreenCtx->pDispCtx);
}




