#ifndef _BG_CODES_H_
#define _BG_CODES_H_

typedef enum{
    BG_SUCCESS = 0,
    BG_FAIL = 1,
    BG_TIMEOUT = 2,
} BG_CODES_e;


typedef enum{
    BG_False = 0,
    BG_True = 1,
} BG_BOOL_e;

#define ASSERT_TRUE(test) {if((test) != BG_True) return BG_FAIL;}

#endif
