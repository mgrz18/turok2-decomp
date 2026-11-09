#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"

void LevelEvent_ProcessTriggers(s32, s32, s32, s32, s32, s32, sptr); /* extern */

void CGameRegion__FireTriggers(sptr arg0, s32 arg1, s32 arg2) {
    s32 sp20;
    s32 sp24;

    CScene__GetRegionEventRecords(&gScene, arg0, &sp20, &sp24);
    if (sp24 != 0) {
        LevelEvent_ProcessTriggers(sp20, sp24, 0, arg1 & 0x7FF, arg1 & 0x3800, 0x3800, arg2);
    }
}
