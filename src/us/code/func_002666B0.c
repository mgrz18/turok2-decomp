#include "common.h"
#include "structs/game_object.h"

s32 CGameObjectInstance__AdvanceAnimation(s32);
s32 CGameObjectInstance__SetAnimByAction(sptr, s32, s32);

void AI_PlayAnim(sptr arg0, CAI *arg1, s32 arg2) {
    arg1->m_ModeLastFrame = -1;
    arg1->m_AnimRepeatedCount = 0;
    arg1->m_VarBuffer[0] = arg2;
    CGameObjectInstance__SetAnimByAction(arg0, arg2, 1);
    CGameObjectInstance__AdvanceAnimation(arg0);
}
