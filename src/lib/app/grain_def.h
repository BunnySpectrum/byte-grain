#ifndef _GRAIN_DEF_H_
#define _GRAIN_DEF_H_

#include "utils/bg_colors.h"

typedef enum MatterState{
    STATE_SOLID = 0,
    STATE_LIQUID = 1,
    STATE_GAS = 2,
    STATE_PLASMA = 3,
}MatterState_e;


typedef struct GrainPropS{
    MatterState_e state;
    char density;
    char viscosity;
    // char friction;
    // char corrosionResist;
    // char fireResist;
}GrainPropS_s;

typedef struct GrainBase{
    BG_BOOL_e isParticle;
    uint8_t id;
}GrainBase_s;

// gravel, sand, swarf
#define GRAIN_ID_GRAVEL 0b0000
#define GRAIN_ID_SAND 0b0001
#define GRAIN_ID_SWARF 0b0010
typedef struct GrainSolidParticle{
    GrainBase_s base;
    BG_BOOL_e isFalling;
    uint8_t temperature;
}GrainSolidParticle_s;

// seed, detritus, acid
#define GRAIN_ID_SEED 0b0011
#define GRAIN_ID_DETRITUS 0b0100
#define GRAIN_ID_ACID 0b0101
typedef struct GrainFlammableParticle{
    GrainBase_s base;
    BG_BOOL_e isFalling;
    BG_BOOL_e isBurning;
}GrainFlammableParticle_s;

// water
#define GRAIN_ID_WATER 0b0110
typedef struct GrainWaterParticle{
    GrainBase_s base;
    BG_BOOL_e isAcidic;
    uint8_t temperature;
}GrainWaterParticle_s;

// lava
typedef enum LavaComposition{
    LAVA_STONE = 0,
    LAVA_SAND = 1,
    LAVA_METAL = 2,
    LAVA_SAND_STONE = 3
}LavaComposition_e;

#define GRAIN_ID_LAVA 0b0111
typedef struct GrainLavaParticle{
    GrainBase_s base;
    LavaComposition_e composition;
}GrainLavaParticle_s;


typedef enum ParticleDirection{
    DIR_STATIC = 0,
    DIR_DOWN_RIGHT,
    DIR_RIGHT,
    DIR_UP_RIGHT,
    DIR_UP,
    DIR_UP_LEFT,
    DIR_LEFT,
    DIR_DOWM_LEFT
}ParticleDirection_e;

typedef struct GrainParticle{
    GrainBase_s base;
    ParticleDirection_e direction;
}GrainParticle_s;

#define GRAIN_ID_GLASS 0b1000
#define GRAIN_ID_STONE 0b1001
#define GRAIN_ID_METAL 0b1010
typedef struct GrainSolidBlock{
    GrainBase_s base;
    BG_BOOL_e isSolid;
    uint8_t temperature;
}GrainSolidBlock_s;


#define GRAIN_ID_WOOD 0b1011
#define GRAIN_ID_FUNGUS 0b1100
#define GRAIN_ID_SPORE 0b1101
typedef struct GrainFlammableBlock{
    GrainBase_s base;
    BG_BOOL_e isGrowing;
    BG_BOOL_e isBurning;
}GrainFlammableBlock_s;


typedef enum ConveyorAxis{
    CONVEYOR_AXIS_H = 0,
    CONVEYOR_AXIS_V = 1,
}ConveyorAxis_e;

typedef enum ConveyorDirection{
    CONVEYOR_DIR_RIGHT_UP = 0,
    CONVEYOR_DIR_LEFT_DOWN = 1,
}ConveyorDirection_e;

#define GRAIN_ID_CONVEYOR 0b1110
typedef struct GrainConveyor{
    GrainBase_s base;
    ConveyorAxis_e axis;
    ConveyorDirection_e direction;
}GrainConveyor_s;


typedef enum SpaceType{
    SPACE_VOID = 0,
    SPACE_HEAT = 1,
    SPACE_COLD = 2,
    SPACE_ATOMIZER = 3,
}SpaceType_e;
#define GRAIN_ID_SPACE 0b1111
typedef struct GrainSpace{
    GrainBase_s base;
    SpaceType_e type;
}GrainSpace_s;

typedef enum GrainType{
    GRAIN_TYPE_SOLID_PARTICLE,
    GRAIN_TYPE_FLAMMABLE_PARTICLE,
    GRAIN_TYPE_WATER_PARTICLE,
    GRAIN_TYPE_LAVA_PARTICLE,
    GRAIN_TYPE_PARTICLE,
    GRAIN_TYPE_SOLID_BLOCK,
    GRAIN_TYPE_FLAMMABLE_BLOCK,
    GRAIN_TYPE_CONVEYOR,
    GRAIN_TYPE_SPACE,
}GrainType_e;

typedef struct Grain{
    GrainType_e type;
    union{
        GrainSolidParticle_s solidParticle;
        GrainFlammableParticle_s flammableParticle;
        GrainWaterParticle_s waterParticle;
        GrainLavaParticle_s lavaParticle;
        GrainParticle_s particle;
        GrainSolidBlock_s solidBlock;
        GrainFlammableBlock_s flammableBlock;
        GrainConveyor_s conveyor;
        GrainSpace_s space;
    } data;
}Grain_s;


#define GET_GRAIN_VALID(grain) (((grain) >> 7) & 0x1)
#define SET_GRAIN_VALID(grain) ((grain) |= (1 << 7))
#define CLEAR_GRAIN_VALID(grain) ((grain) &= ~(1 << 7))

#define GET_GRAIN_ACTIVE(grain) (((grain) >> 6) & 0x1)
#define SET_GRAIN_ACTIVE(grain) ((grain) |= (1 << 6))
#define CLEAR_GRAIN_ACTIVE(grain) ((grain) &= ~(1 << 6))

#define GET_GRAIN_COLOR(grain) ((grain) & 0x3F)
#define SET_GRAIN_COLOR(grain, color) ((grain) = ((grain) & ~0x3F) | ((color) & 0x3F))
#define IS_EMPTY_GRAIN(grain) (BG_COLOR_WHITE == GET_GRAIN_COLOR(grain))

#define CLEAR_GRAIN(grain) ((grain) = 0x0)


#endif