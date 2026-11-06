#include "common.h"
#include "m2c_macros.h"
#include "globals/scene.h"

extern void CScene__GetTextureSet(void *, s32, s32, s32);

void func_00207914(s32 arg0, s32 arg1) {
    CScene__GetTextureSet(&gScene, arg1, arg0, arg0 + 4);
}
