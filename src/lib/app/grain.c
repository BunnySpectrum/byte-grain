#include "grain.h"



BG_CODES_e byte_to_grain(const uint8_t *byte, Grain_s *grain){

    uint8_t color, active;
    color = GET_GRAIN_COLOR(*byte);
    active = GET_GRAIN_ACTIVE(*byte);

    switch(color){
        case BG_COLOR_WHITE:
            grain->type = GRAIN_TYPE_SPACE;
            
            grain->data.space.type = SPACE_VOID;
            
            grain->data.space.base.isParticle = BG_False;
            grain->data.space.base.id = BG_COLOR_WHITE;
            break;
        case BG_COLOR_BLUE:
            grain->type = GRAIN_TYPE_WATER_PARTICLE;
            
            grain->data.waterParticle.isAcidic = BG_False;
            grain->data.waterParticle.temperature = 0;

            grain->data.waterParticle.base.isParticle = BG_False;
            grain->data.waterParticle.base.id = BG_COLOR_BLUE;
            break;
        case BG_COLOR_YELLOW:
            grain->type = GRAIN_TYPE_SOLID_PARTICLE;
            
            grain->data.solidParticle.isFalling = active;
            grain->data.solidParticle.temperature = 0;

            grain->data.solidParticle.base.isParticle = BG_False;
            grain->data.solidParticle.base.id = BG_COLOR_YELLOW;
            break;
        default:
            return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e grain_to_byte(uint8_t *byte, const Grain_s *grain){
    switch(grain->type){
        case GRAIN_TYPE_SPACE:
            SET_GRAIN_COLOR(*byte, BG_COLOR_WHITE);
            break;
        case GRAIN_TYPE_WATER_PARTICLE:
            SET_GRAIN_COLOR(*byte, BG_COLOR_BLUE);
            break;
        case GRAIN_TYPE_SOLID_PARTICLE:
            SET_GRAIN_COLOR(*byte, BG_COLOR_YELLOW);
            if(grain->data.solidParticle.isFalling > 0){
                SET_GRAIN_ACTIVE(*byte);
            }else{
                CLEAR_GRAIN_ACTIVE(*byte);
            }
            break;
        default:
            return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e print_grain(const Grain_s *grain){
    switch(grain->type){
        case GRAIN_TYPE_SPACE:
            printf("Space: type = %d, id = %d.\n", grain->data.space.type, grain->data.space.base.id);
            break;
        case GRAIN_TYPE_WATER_PARTICLE:
            printf("Water particle: isAcidic = %d, temp = %d, id = %d.\n", grain->data.waterParticle.isAcidic, grain->data.waterParticle.temperature, grain->data.waterParticle.base.id);
            break;
        case GRAIN_TYPE_SOLID_PARTICLE:
            printf("Solid particle: isFalling = %d, temp = %d, id = %d.\n", grain->data.solidParticle.isFalling, grain->data.solidParticle.temperature, grain->data.solidParticle.base.id);
            break;
        default:
            return BG_FAIL;
    }

    return BG_SUCCESS;
}

