#ifndef _GRAIN_H_
#define _GRAIN_H_

#include <stdint.h>

#include "app/grain_def.h"
#include "utils/bg_codes.h"

BG_CODES_e byte_to_grain(const uint8_t *byte, Grain_s *grain);
BG_CODES_e grain_to_byte(uint8_t *byte, const Grain_s *grain);
BG_CODES_e print_grain(const Grain_s *grain);

#endif