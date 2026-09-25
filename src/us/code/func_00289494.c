#include "common.h"

extern s32 func_00288D3C(s32, f32, s32, s32);
extern s32 D_800B6D68;

void func_00289494(s32 arg0) {
    if (func_00288D3C(arg0, 4.0f, 4, 4) != 0) {
        D_800B6D68 = 0;
    }
}
