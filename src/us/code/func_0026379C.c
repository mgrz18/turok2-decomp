#include "common.h"

s32 CScene__SetInstanceSavedBit(s32 *, s32, s32);     /* extern */
extern s32 D_800F7078;

void Generator_EventOff(s32 arg0) {
    CScene__SetInstanceSavedBit(&D_800F7078, arg0, 0);
}
