#include "common.h"

s32 CScene__GetInstanceSavedBit(s32 *, s32);              /* extern */
extern s32 D_800F7078;

void Platform_IsOn(s32 arg0) {
    CScene__GetInstanceSavedBit(&D_800F7078, arg0);
}
