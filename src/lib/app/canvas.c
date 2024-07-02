#include "canvas.h"

static GrainPropS_s grainPropWater = {
    .density = 2,
    .viscosity = 4,
};

static GrainPropS_s grainPropSand = {
    .density = 5,
    .viscosity = 0,
};


void init_canvas(uint8_t *buf, int color)
{
    int i;
    for (i = 0; i < PIX_COUNT; i++)
    {
        buf[i] = color;
    }
}



void add_grain(uint8_t *buf, int row, int col, int color)
{
    buf[row * ROW_MAX + col] = color;
}


void move_grain_to_index(uint8_t *buf, int current, int new){
    buf[new] = buf[current];
    CLEAR_GRAIN(buf[current]);

    SET_GRAIN_VALID(buf[new]);
    SET_GRAIN_VALID(buf[current]);
}


void canvas_update(uint8_t *buf)
{
    int row, col, idx;
    for (row = 0; row < ROW_MAX; row++)
    {
        for (col = 0; col < COL_MAX; col++)
        {
            grain_update(buf, row, col);
        }
    }
    for (idx = 0; idx < PIX_COUNT; idx++)
    {
        CLEAR_GRAIN_VALID(buf[idx]);
    }
}


void grain_update(uint8_t *buf, int row, int col){
    int below, current, belowLeft, belowRight;

    if (row + 1 == ROW_MAX)
    {
        return;
    }
    current = GRAIN_2D_TO_1D(row, col);
    below = GRAIN_2D_TO_1D(row + 1, col);
    belowLeft = GRAIN_2D_TO_1D(row + 1, col - 1);
    belowRight = GRAIN_2D_TO_1D(row + 1, col + 1);

    if (1 == GET_GRAIN_VALID(buf[current]))
    {
        return;
    }

    if (IS_EMPTY_GRAIN(buf[current]))
    {
        return;
    }

    if (IS_EMPTY_GRAIN(buf[below]))
    {
        // There is an empty space below us
        move_grain_to_index(buf, current, below);

        // Mark the current grain as 'active' (in motion)
        SET_GRAIN_ACTIVE(buf[below]);

    }
    else if (1 == GET_GRAIN_ACTIVE(buf[current]))
    {
        int firstCheck, secondCheck;

        // firstCheck = ((rand() % 2) == 0) ? belowLeft : belowRight;
        if((rand() % 2) == 0){
            firstCheck = belowLeft;
            secondCheck = belowRight;
        }else{
            firstCheck = belowRight;
            secondCheck = belowLeft;
        }

        if (IS_EMPTY_GRAIN(buf[firstCheck])){
            move_grain_to_index(buf, current, firstCheck);
            return;
        }else if(IS_EMPTY_GRAIN(buf[secondCheck])){
            move_grain_to_index(buf, current, secondCheck);
            return;
        }

        CLEAR_GRAIN_ACTIVE(buf[current]);
        SET_GRAIN_VALID(buf[current]);
    }
    else
    {
        SET_GRAIN_VALID(buf[current]);
    }


}





