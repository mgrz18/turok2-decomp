#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);
s32 Throwable_SetModeAnim(void *, void *, s32);         /* extern */

/* D_800F1CE0 is the collision results (CCollisionResults, structs/collision.h);
 * this reads its first word, m_pInstance (what the last test hit), as a
 * pointer variable: D_800F1CE0.m_pInstance does not give this code. */
extern CGameObjectInstance *D_800F1CE0;

void EnemyAI_CheckGrab(s32 arg0, CAI *arg1) {
    void *owner;

    if ((arg1->m_Health != 0) && (D_800F1CE0 != NULL) && (owner = arg1->m_pPathTarget, !(arg1->m_ModeFlags & 0x1000)) && (D_800F1CE0 == owner) && (D_800F1CE0->header.m_pIntelligence->m_Type == 5)) {
        arg1->m_VarBuffer[2] = (u32) D_800F1CE0;
        Throwable_SetModeAnim(D_800F1CE0, &D_800F1CE0->m_AI, arg0);
        CAI__SetMode(arg0, (s32) arg1, 0x15);
    }
}
