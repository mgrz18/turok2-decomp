#include "common.h"

s32 CEngineApp__Idle(s32 *, s32);              /* extern */
extern s32 D_800F6CB0;

void Engine_Idle(s32 arg0) {
    CEngineApp__Idle(&D_800F6CB0, arg0);
}
