#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;

typedef struct { s32 a; V3 v; } S;

void func_0021836C(S *arg0, s32 arg1, V3 arg2) {
    arg0->a = arg1;
    arg0->v = arg2;
}
