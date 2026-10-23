#include "common.h"

extern f32 D_800B6D28;

s32 func_00254F24(u8 *self, u8 *other) {
    if (*(s32 *)(self + 0x380) != 0) {
        if (*(s32 *)(*(u8 **)(other + 0xA50) + 0xB0) & 0xC000) {
            return 0;
        }
        *(s32 *)(self + 0x37C) = -1;
        return 1;
    }
    *(f32 *)(self + 4) -= D_800B6D28;
    return *(f32 *)(self + 4) <= 0.0f;
}
