#include "common.h"
#include "m2c_macros.h"

extern void CScene__GetTextureSet(void *, s32, s32, s32);
extern u8 gScene[];

void func_00207914(s32 arg0, s32 arg1) {
    CScene__GetTextureSet(gScene, arg1, arg0, arg0 + 4);
}
