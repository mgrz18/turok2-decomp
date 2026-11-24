#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"
#include "globals/collision.h"

/* What the last collision test hit, gCollisionResults.m_pInstance (its first word), read as a
 * pointer variable: the member access loads the struct's address first and does not match. */
#define sHit (*(CGameObjectInstance **) &gCollisionResults)

void EnemyAI_CheckGrab(CGameObjectInstance *arg0, CAI *arg1) {
    void *owner;

    if ((arg1->m_Health != 0) && (sHit != NULL) && (owner = arg1->m_pPathTarget, !(arg1->m_ModeFlags & 0x1000)) && (sHit == owner) && (sHit->header.m_pIntelligence->m_Type == 5)) {
        arg1->m_Enemy.m_pGrabbed = sHit; /* pointer-wide on the host (m_VarBuffer[2] is a u32) */
        Throwable_SetModeAnim(sHit, &sHit->m_AI, arg0);
        CAI__SetMode(arg0, arg1, 0x15);
    }
}
