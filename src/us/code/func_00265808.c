#include "common.h"

s32 CScene__GetInstanceSavedBit(s32 *, s32);                  /* extern */
extern s32 D_800F7078;

s32 Door_IsOpen(s32 arg0) {
    return CScene__GetInstanceSavedBit(&D_800F7078, arg0) == 1;
}
