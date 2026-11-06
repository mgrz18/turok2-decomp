#include "common.h"

s32 CScene__SetInstanceSavedBit(s32 *, s32, s32);     /* extern */
extern s32 gScene;

void Generator_EventOn(s32 arg0) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 1);
}
