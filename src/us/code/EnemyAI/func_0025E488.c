#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"


void EnemyAI_SetGrabbed(sptr arg0, CAI *arg1, sptr arg2) {
    /* arg2 is the grabbed actor: pointer-wide m_pGrabbed, its CAI at the host offset */
    arg1->m_Enemy.m_pGrabbed = (CGameObjectInstance *) arg2;
    if (arg2 != 0) {
        Throwable_SetModeAnim(arg2, (sptr) &((CGameObjectInstance *) arg2)->m_AI, arg0);
    }
}
