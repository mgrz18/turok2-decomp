#include "common.h"

s32 CScene__GetInstanceSavedBit(s32 *, s32);              /* extern */
extern s32 gScene;

void Platform_IsOn(s32 arg0) {
    CScene__GetInstanceSavedBit(&gScene, arg0);
}
