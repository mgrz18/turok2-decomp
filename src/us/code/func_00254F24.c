#include "common.h"
#include "globals/engine.h"


s32 CBarrel__ShouldClose(u8 *self, u8 *other) {
    if (*(s32 *)(self + 0x380) != 0) {
        if (*(s32 *)(*(u8 **)(other + 0xA50) + 0xB0) & 0xC000) {
            return 0;
        }
        *(s32 *)(self + 0x37C) = -1;
        return 1;
    }
    *(f32 *)(self + 4) -= gFrameIncrement;
    return *(f32 *)(self + 4) <= 0.0f;
}
