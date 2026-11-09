#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"



void Destructible_EventGo(s32 arg0, s32 arg1) {
    CAI__SetMode(arg0, arg1, 1);
    CScene__SetInstanceSavedBit(&gScene, arg0, 1);
}
