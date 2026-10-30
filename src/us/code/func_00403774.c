#include "common.h"

extern s32 D_8012F5F4;
s32 func_002451AC(s32 a0, s32 a1, s32 a2, f32 a3);

s32 func_00403774(s32 a0, s32 a1, s32 a2) {
    s32 result;

    if (D_8012F5F4 != 0) {
        result = func_002451AC(a0, a1, a2, 0.785398245f);
    } else {
        result = 0;
    }
    return result;
}
