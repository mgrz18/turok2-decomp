#include "common.h"
#include "structs/game_object.h"

s32 CScene__GetInstanceEventRecords(s32 *, void *, s32 *, s32 *); /* extern */
s32 LevelEvent_ProcessTriggers(s32, s32, s32, s32, s32, s32, s32); /* extern */
extern s32 gScene;

void GameObject_FireTriggers(CGameObjectInstance *arg0, s32 arg1, s32 arg2) {
    s32 sp20;
    s32 sp24;

    if (!(arg0->m_dwFlags & 0x80000)) {
        CScene__GetInstanceEventRecords(&gScene, arg0, &sp20, &sp24);
        if (sp24 != 0) {
            LevelEvent_ProcessTriggers(sp20, sp24, 0, arg1 & 0x3FC1FF, arg1 & 0x400000, 0x400000, arg2);
        }
    }
}
