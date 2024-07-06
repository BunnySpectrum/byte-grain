#ifndef _GRAIN_DEF_H_
#define _GRAIN_DEF_H_

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

#endif