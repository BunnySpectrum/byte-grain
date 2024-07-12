#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/bg_colors.h"
#include "utils/bg_codes.h"
#include "app/grain_def.h"
#include "app/grain.h"

#define ROW_MAX 32
#define COL_MAX 32
#define PIX_COUNT (ROW_MAX * COL_MAX)

// Grain bit fields:
//  7: valid flag
//  6: active flag
//  0-5: ID (only color for now)

#define GRAIN_2D_TO_1D(row, col) ((row) * (ROW_MAX) + (col))



void init_canvas(uint8_t *buf, int color);
void add_grain(uint8_t *buf, int row, int col, int color);
void move_grain_to_index(uint8_t *buf, int current, int new);
void canvas_update(uint8_t *buf);
void grain_update(uint8_t *buf, int row, int col);

BG_BOOL_e is_static_solid(uint8_t grain);
BG_BOOL_e is_dynamic_solid(uint8_t grain);
BG_BOOL_e is_liquid(uint8_t grain);
BG_BOOL_e gravity_on_grain(uint8_t *buf, int row, int col);
BG_BOOL_e slide_on_grain(uint8_t *buf, int row, int col);


#endif