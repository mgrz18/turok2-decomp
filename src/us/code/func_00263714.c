#include "common.h"
#include "globals/scene.h"

s32 CScene__GetInstanceSavedBit(void *, s32);                  /* extern */

s32 Generator_IsOn(s32 arg0) {
    return CScene__GetInstanceSavedBit(&gScene, arg0) == 1;
}
