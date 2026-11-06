#include "common.h"

s32 CScene__GetInstanceSavedBit(void *, s32);                  /* extern */
extern u8 gScene[];

s32 Generator_IsOn(s32 arg0) {
    return CScene__GetInstanceSavedBit(gScene, arg0) == 1;
}
