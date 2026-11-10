#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_NormalDeath_B0120(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimNormalDeath_B0120, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}
#else
void EnemyAnim_NormalDeath_B0120(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimNormalDeath_B0120, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}
#endif
