#include "common.h"

s32 CScene__GetInstanceSavedBit(void *, s32);                  /* extern */
extern u8 D_800F7078[];

s32 Generator_IsOn(s32 arg0) {
    return CScene__GetInstanceSavedBit(D_800F7078, arg0) == 1;
}
