#include "common.h"
#include "structs/game_object.h"

/* pointer-wide on the host (round 8 w2): the grabbed throwable (m_Enemy.m_pGrabbed) */
sptr EnemyAI_GetGrabbed(sptr arg0, CAI *arg1) {
    return (sptr) arg1->m_Enemy.m_pGrabbed;
}
