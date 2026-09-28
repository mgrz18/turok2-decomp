#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;
typedef struct { f32 x, y, z; } V3f;

void func_0020EF60(V3f *arg0, V3f *arg1, f32 arg2) {
    arg0->x = arg1->x * arg2;
    arg0->y = arg1->y * arg2;
    arg0->z = arg1->z * arg2;
}
