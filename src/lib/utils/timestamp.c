#include "timestamp.h"

void print_build_time(void){
    printf(" \"build time\", \"value\": \"%s\"", __TIME__);
}