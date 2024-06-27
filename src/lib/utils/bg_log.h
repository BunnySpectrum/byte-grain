#ifndef _BG_LOG_H_
#define _BG_LOG_H_

#include <stdio.h>
#include "utils/bg_codes.h"

BG_BOOL_e log_assert(BG_BOOL_e actual, BG_BOOL_e expected, char* message);
BG_BOOL_e log_if_err(BG_CODES_e actual, char* message);
BG_BOOL_e log_if_false(BG_BOOL_e actual, char* message);


#endif