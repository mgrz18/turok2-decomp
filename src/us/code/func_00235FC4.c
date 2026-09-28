#include "common.h"
#include "m2c_macros.h"

extern s32 D_800B6D1C;

void func_00235FC4(void *arg0) {
    s32 i = D_800B6D1C;

    M2C_FIELD(arg0, s32 *, 0x2580) = 0x12C;
    M2C_FIELD(arg0, void **, 0x2584) = (s8 *)arg0 + i * 0x12C0;
}
