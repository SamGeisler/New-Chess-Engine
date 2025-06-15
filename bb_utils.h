#ifndef BB_UTILS_H
#define BB_UTILS_H
#include "global.h"

extern const int index64[64];

int bitScanForward(uint64_t bb);

void print_bb(uint64_t bb);

#endif