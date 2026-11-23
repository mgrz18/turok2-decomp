#include "common.h"
#include "structs/game_object.h"

/* Restarts the current animation. */
void CGameObjectInstance__RestartAnim(CGameObjectInstance *arg0) {
    arg0->m_CurrentAnimation.m_bAnimActive = 1;
    arg0->m_CurrentAnimation.m_AnimationFrame = 0;
    arg0->m_CurrentAnimation.m_bCycleCompleted = 0;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x400);
}
