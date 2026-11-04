#include "common.h"
#include "structs/game_object.h"

/* Requests animation arg1; marks the animation as changed if it is not the current one. */
void func_0021E960(CGameObjectInstance *arg0, s32 arg1) {
    arg0->m_CurrentAnimation.m_nRequestedAnimIndex = arg1;
    arg0->m_CurrentAnimation.m_bCycleCompleted = 0;
    if (arg0->m_CurrentAnimation.m_nAnimIndex != arg1) {
        arg0->m_CurrentAnimation.m_bAnimActive = 1;
    }
}
