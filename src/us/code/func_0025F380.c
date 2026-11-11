#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void EnemyMode_PickupThrowObject_Enter(sptr arg0, CAI *arg1) {
    sptr temp_a0;

#ifdef TARGET_PORT
    /* the grabbed throwable: pointer-wide m_pGrabbed, its CAI at the host offset */
    temp_a0 = (sptr) arg1->m_Enemy.m_pGrabbed;
    CAI__SetMode(temp_a0, (sptr) &((CGameObjectInstance *) temp_a0)->m_AI, 1);
#else
    temp_a0 = arg1->m_VarBuffer[2];
    CAI__SetMode(temp_a0, temp_a0 + 0x140, 1);
#endif
}
