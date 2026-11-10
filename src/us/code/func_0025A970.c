#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_ViolentDeath_B0270(CGameObjectInstance *obj, s32 arg1) {
    return AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimViolentDeath_B0270, obj->header.m_pIntelligence->m_Enemy.m_wViolentDeathAnimFlags);
}
#else
void EnemyAnim_ViolentDeath_B0270(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimViolentDeath_B0270, obj->header.m_pIntelligence->m_Enemy.m_wViolentDeathAnimFlags);
}
#endif
