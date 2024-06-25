#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <stdint.h>
#include <stdlib.h>
#include "utils/bg_colors.h"

#define ROW_MAX 32
#define COL_MAX 32
#define PIX_COUNT (ROW_MAX * COL_MAX)

// Grain bit fields:
//  7: valid flag
//  6: active flag
//  0-5: ID (only color for now)

#define GRAIN_2D_TO_1D(row, col) ((row) * (ROW_MAX) + (col))
#define GET_GRAIN_VALID(grain) (((grain) >> 7) & 0x1)
#define GET_GRAIN_ACTIVE(grain) (((grain) >> 6) & 0x1)

#define SET_GRAIN_VALID(grain) ((grain) |= (1 << 7))
#define CLEAR_GRAIN_VALID(grain) ((grain) &= ~(1 << 7))

#define SET_GRAIN_ACTIVE(grain) ((grain) |= (1 << 6))
#define CLEAR_GRAIN_ACTIVE(grain) ((grain) &= ~(1 << 6))

#define SET_GRAIN_COLOR(grain, color) ((grain) = ((grain) & ~0x3F) | ((color) & 0x3F))
#define GET_GRAIN_COLOR(grain) ((grain) & 0x3F)

#define CLEAR_GRAIN(grain) ((grain) = 0x0)
#define IS_EMPTY_GRAIN(grain) (BG_COLOR_WHITE == GET_GRAIN_COLOR(grain))




void init_canvas(uint8_t *buf, int color);
void add_grain(uint8_t *buf, int row, int col, int color);
void move_grain_to_index(uint8_t *buf, int current, int new);
void update_grains(uint8_t *buf);


#endif