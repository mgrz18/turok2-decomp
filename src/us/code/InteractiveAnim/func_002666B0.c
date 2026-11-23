#include "common.h"
#include "structs/game_object.h"

s32 CGameObjectInstance__AdvanceAnimation(CGameObjectInstance *);
s32 CGameObjectInstance__SetAnimByAction(CGameObjectInstance *, s32, s32);

/* ai: the CAI (protos/ai.h declares it void *) */
void AI_PlayAnim(sptr arg0, void *ai, s32 arg2) {
    CAI *arg1 = ai;

    arg1->m_ModeLastFrame = -1;
    arg1->m_AnimRepeatedCount = 0;
    arg1->m_VarBuffer[0] = arg2;
    CGameObjectInstance__SetAnimByAction((CGameObjectInstance *) arg0, arg2, 1);
    CGameObjectInstance__AdvanceAnimation((CGameObjectInstance *) arg0);
}
