#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } Vec3i;

void func_00273E98(void *arg0, Vec3i *arg1, s32 arg2) {
    *(Vec3i *)((s8 *)arg0 + 0x44) = *arg1;
    M2C_FIELD(arg0, s32 *, 0x50) = arg2;
}
