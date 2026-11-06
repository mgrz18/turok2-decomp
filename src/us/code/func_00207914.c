#include "common.h"
#include "m2c_macros.h"

extern void CScene__GetTextureSet(void *, s32, s32, s32);
extern u8 D_800F7078[];

void func_00207914(s32 arg0, s32 arg1) {
    CScene__GetTextureSet(D_800F7078, arg1, arg0, arg0 + 4);
}
