#include "common.h"
#include "m2c_macros.h"

extern u8 gScene[];
extern s32 D_800B6D54;
extern s32 CScene__LoadGraphicSet(void *, s32, s32, s32);

s32 func_0027AE00(s32 arg0) {
    s32 r;

    if (D_800B6D54 != 0) {
        r = CScene__LoadGraphicSet(gScene, arg0, 3, 1);
    } else {
        r = 0;
    }
    return r;
}
