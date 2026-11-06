#include "common.h"
#include "globals/scene.h"

s32 CScene__SetInstanceSavedBit(s32, s32, s32);
s32 CAI__SetMode(s32, s32, s32);


void Destructible_EventGo(s32 arg0, s32 arg1) {
    CAI__SetMode(arg0, arg1, 1);
    CScene__SetInstanceSavedBit((s32) &gScene, arg0, 1);
}
