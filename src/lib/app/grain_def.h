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




#endif