#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;
typedef struct { f32 x, y, z; } V3f;

extern void func_00237FAC(s32, V3, s32);

void func_00238824(s32 arg0, s32 arg1, s32 arg2, V3 arg3) {
    func_00237FAC(arg0, arg3, 1);
}
