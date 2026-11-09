#include "common.h"
#include "structs/event.h"
#include "structs/game_object.h"
#include "globals/scene.h"

s32 Action_CheckRequiredItem(CGameObjectInstance *, CAI *, s32); /* extern */

s32 LevelEvent_CheckRequiredItem(LevelEvent *arg0, sptr arg1) {
    CGameObjectInstance *temp_a1;

    if (arg0->level == gScene.m_nLevel) {
        if (arg1 != 0) {
            temp_a1 = &gScene.m_pActors[arg0->actor];
            if ((arg0->kind == 0xA) && (arg0->handler == 1)) {
                return Action_CheckRequiredItem(temp_a1, &temp_a1->m_AI, arg1);
            }
        }
        return 1;
    }
    return 1;
}
