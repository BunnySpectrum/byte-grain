#include "bg_log.h"


BG_BOOL_e log_assert(BG_BOOL_e actual, BG_BOOL_e expected, char* message){
    if(actual != expected){
        printf("%s\n", message);
    }
    return (actual == expected) ? BG_True : BG_False;
}

BG_BOOL_e log_if_err(BG_CODES_e actual, char* message){
    return log_assert(actual == BG_SUCCESS, BG_True, message);
}

BG_BOOL_e log_if_false(BG_BOOL_e actual, char* message){
    return log_assert(actual, BG_True, message);

}