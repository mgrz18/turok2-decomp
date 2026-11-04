#include "common.h"
#include "m2c_macros.h"
#include "structs/vector.h"

typedef struct { s32 a; CVector3 v; s32 d; } S;

void func_0021810C(S *arg0, s32 arg1, CVector3 arg2, s32 arg3) {
    arg0->a = arg1;
    arg0->v = arg2;
    arg0->d = arg3;
}
