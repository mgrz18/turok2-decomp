#include "common.h"
#include "globals/scene.h"

s32 CScene__GetInstanceSavedBit(s32 *, s32);                  /* extern */

s32 Door_IsOpen(s32 arg0) {
    return CScene__GetInstanceSavedBit(&gScene, arg0) == 1;
}
