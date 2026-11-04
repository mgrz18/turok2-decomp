#include "common.h"

typedef struct {
    /* 0x0 */ s32 a;
    /* 0x4 */ s32 b;
} Pair;

void func_00207908(Pair *arg0, s32 arg1, s32 arg2) {
    arg0->a = arg1;
    arg0->b = arg2;
}
