#include "common.h"
#include "globals/scene.h"

s32 CScene__SetInstanceSavedBit(s32 *, s32, s32);     /* extern */

void Generator_EventOff(s32 arg0) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
}
