#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/game.h"

struct Trigger;
void LevelEvent_ProcessTriggers(struct Trigger *triggers, s32 count, s32 require, s32 enter, s32 exclude, s32 group, sptr arg);

void GameObject_FireTriggers(CGameObjectInstance *arg0, s32 arg1, sptr arg2) {
    sptr sp20;
    s32 sp24;

    if (!(arg0->m_dwFlags & 0x80000)) {
        CScene__GetInstanceEventRecords(&gScene, arg0, &sp20, &sp24);
        if (sp24 != 0) {
            LevelEvent_ProcessTriggers((struct Trigger *) sp20, sp24, 0, arg1 & 0x3FC1FF, arg1 & 0x400000, 0x400000, arg2);
        }
    }
}
