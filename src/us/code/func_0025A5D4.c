#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Combat_AFE58(CGameObjectInstance *arg0, s32 arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimCombat_AFE58, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wCombatAnimFlags);
}
#else
void EnemyAnim_Combat_AFE58(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimCombat_AFE58, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wCombatAnimFlags);
}
#endif
