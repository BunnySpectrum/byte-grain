#include "display.h"

static uint8_t activeDisplays = 0;

BG_CODES_e display_new_handle(uint8_t *handle){
    activeDisplays++;

    if(activeDisplays == 0){
        *handle = 0;
        return BG_FAIL;
    }

    *handle = activeDisplays;

    return BG_SUCCESS;
}