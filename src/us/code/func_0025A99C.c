#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_ViolentDeath_B02A0(CGameObjectInstance *obj, CAI *ai) {
    return AI_PickAnimFromTable(obj, ai, sEnemyAnimViolentDeath_B02A0, obj->header.m_pIntelligence->m_Enemy.m_wViolentDeathAnimFlags);
}
#else
void EnemyAnim_ViolentDeath_B02A0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimViolentDeath_B02A0, obj->header.m_pIntelligence->m_Enemy.m_wViolentDeathAnimFlags);
}
#endif
