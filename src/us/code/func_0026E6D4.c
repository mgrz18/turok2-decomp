#include "common.h"
#include "structs/event.h"
#include "structs/game_object.h"

s32 Action_CheckRequiredItem(CGameObjectInstance *, CAI *, s32); /* extern */
extern CGameObjectInstance *D_800F7158;            /* level actors */
extern s32 D_80110020;                              /* current level */

s32 LevelEvent_CheckRequiredItem(LevelEvent *arg0, s32 arg1) {
    CGameObjectInstance *temp_a1;

    if (arg0->level == D_80110020) {
        if (arg1 != 0) {
            temp_a1 = &D_800F7158[arg0->actor];
            if ((arg0->kind == 0xA) && (arg0->handler == 1)) {
                return Action_CheckRequiredItem(temp_a1, &temp_a1->m_AI, arg1);
            }
        }
        return 1;
    }
    return 1;
}
