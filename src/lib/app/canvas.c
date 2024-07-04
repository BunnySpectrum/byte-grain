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

BG_BOOL_e is_static_solid(uint8_t grain){
    return GET_GRAIN_COLOR(grain) == BG_COLOR_BLACK;
};


BG_BOOL_e is_dynamic_solid(uint8_t grain){
    return GET_GRAIN_COLOR(grain) == BG_COLOR_YELLOW;
};

BG_BOOL_e is_liquid(uint8_t grain){
    return GET_GRAIN_COLOR(grain) == BG_COLOR_BLUE;
};

//returns true if grain moved downward. False if not
BG_BOOL_e gravity_on_grain(uint8_t *buf, int row, int col){
    int below, current, left, right;

    
    // We are at the bottom of the screen
    if (row + 1 == ROW_MAX){
        return BG_False;
    }

    current = GRAIN_2D_TO_1D(row, col);
    below = GRAIN_2D_TO_1D(row + 1, col);

    // Try to move directly down
    if (IS_EMPTY_GRAIN(buf[below])){
        // There is an empty space below us
        move_grain_to_index(buf, current, below);

        // Mark the current grain as 'active' (in motion)
        SET_GRAIN_ACTIVE(buf[below]);

        return BG_True;
    }
    // printf("Below is full\n");


    // There's a grain below us, so check if we can roll to the left or right
    if(1 == GET_GRAIN_ACTIVE(buf[current])){
        int firstCheck, secondCheck;
        int belowLeft = GRAIN_2D_TO_1D(row + 1, col - 1);
        int belowRight = GRAIN_2D_TO_1D(row + 1, col + 1);

        if(col == 0){
            firstCheck = belowRight;
            secondCheck = -1;
        }else if(col + 1 == COL_MAX){
            firstCheck = belowLeft;
            secondCheck = -1;
        }else{
            if((rand() % 2) == 0){
                firstCheck = belowLeft;
                secondCheck = belowRight;
            }else{
                firstCheck = belowRight;
                secondCheck = belowLeft;
            }
        }


        if (IS_EMPTY_GRAIN(buf[firstCheck])){
            move_grain_to_index(buf, current, firstCheck);
            return BG_True;
        }else if( (secondCheck != -1) && (IS_EMPTY_GRAIN(buf[secondCheck])) ){
            move_grain_to_index(buf, current, secondCheck);
            return BG_True;
        }
    }

    return BG_False;
}

BG_BOOL_e slide_on_grain(uint8_t *buf, int row, int col){
    int below, current, belowLeft, belowRight, left, right;
    return BG_True;
}

void grain_update(uint8_t *buf, int row, int col){
    int below, current, belowLeft, belowRight, left, right;
    
    current = GRAIN_2D_TO_1D(row, col);

    // grain has already been processed
    if (1 == GET_GRAIN_VALID(buf[current]))
    {
        return;
    }

    // nothing to process for empty square
    if (IS_EMPTY_GRAIN(buf[current]))
    {
        return;
    }

    // this grain doesn't (currently) react with anything
    if(is_static_solid(buf[current])){
        return;
    }

    below = GRAIN_2D_TO_1D(row + 1, col);


    // else, we are a falling grain and/or liquid
    if(BG_False == gravity_on_grain(buf, row, col) ){
        //both bottom corners are full
        if( (1 == GET_GRAIN_ACTIVE(buf[current])) && (is_liquid(buf[current])) ){
            printf("Active? %d.\n", GET_GRAIN_ACTIVE(buf[current]));
            // CLEAR_GRAIN_ACTIVE(buf[current]);
            // SET_GRAIN_VALID(current);

            int dir;

            if(col == 0){
                dir = (rand() % 2) == 0 ? -1 : GRAIN_2D_TO_1D(row, col+1);
            }else if(col + 1 == COL_MAX){
                dir = (rand() % 2) == 0 ? GRAIN_2D_TO_1D(row, col-1) : -1;
            }else{
                dir = (rand() % 2) == 0 ? GRAIN_2D_TO_1D(row, col-1) : GRAIN_2D_TO_1D(row, col+1);
            }

            if(dir == -1){
                // hit boundary wall
                CLEAR_GRAIN_ACTIVE(buf[current]);
                SET_GRAIN_VALID(buf[current]);
                return;
            }else{
                if(IS_EMPTY_GRAIN(buf[dir])){
                    move_grain_to_index(buf, current, dir);
                    // CLEAR_GRAIN_ACTIVE(buf[dir]);
                    SET_GRAIN_VALID(buf[dir]);
                    return;
                }else{
                    // hit another grain
                    CLEAR_GRAIN_ACTIVE(buf[current]);
                    SET_GRAIN_VALID(buf[current]);
                    return;
                }
            }

            CLEAR_GRAIN_ACTIVE(buf[current]);
            SET_GRAIN_VALID(buf[current]);

        }else{
            CLEAR_GRAIN_ACTIVE(buf[current]);
            SET_GRAIN_VALID(buf[current]);
        }

    }
    else
    {
        SET_GRAIN_VALID(buf[current]);
    }


}





