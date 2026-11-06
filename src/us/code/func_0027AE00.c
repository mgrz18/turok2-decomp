#include "common.h"
#include "m2c_macros.h"
#include "globals/engine.h"

extern u8 gScene[];
extern s32 CScene__LoadGraphicSet(void *, s32, s32, s32);

s32 func_0027AE00(s32 arg0) {
    s32 r;

    if (gPersistentDataLoaded != 0) {
        r = CScene__LoadGraphicSet(gScene, arg0, 3, 1);
    } else {
        r = 0;
    }
    return r;
}
