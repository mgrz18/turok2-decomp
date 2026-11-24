#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"

struct Trigger;
void LevelEvent_ProcessTriggers(struct Trigger *triggers, s32 count, s32 require, s32 enter, s32 exclude, s32 group, sptr arg);

void CGameRegion__FireTriggers(sptr arg0, s32 arg1, sptr arg2) {
    sptr sp20;
    s32 sp24;

    CScene__GetRegionEventRecords(&gScene, arg0, &sp20, &sp24);
    if (sp24 != 0) {
        LevelEvent_ProcessTriggers((struct Trigger *) sp20, sp24, 0, arg1 & 0x7FF, arg1 & 0x3800, 0x3800, arg2);
    }
}
