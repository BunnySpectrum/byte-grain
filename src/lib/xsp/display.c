#include "display.h"

static uint8_t activeDisplays = 0;
static DisplayContext_s displayContexts[DISPLAY_MAX_COUNT] = {};

BG_CODES_e display_new_handle(uint8_t *handle){
    activeDisplays++;

    if(activeDisplays == 0){
        *handle = 0;
        return BG_FAIL;
    }

    *handle = activeDisplays;

    return BG_SUCCESS;
}

BG_CODES_e display_context_for_handle(uint8_t handle, DisplayContext_s **ctx){
    if(handle == 0 || handle > DISPLAY_MAX_COUNT){
        ctx = 0;
        return BG_FAIL;
    }

    *ctx = &displayContexts[handle - 1];

    return BG_SUCCESS;
}