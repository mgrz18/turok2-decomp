#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Leap_AFEB8(CGameObjectInstance *obj, CAI *ai) {
    return AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimLeap_AFEB8, obj->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}
#else
void EnemyAnim_Leap_AFEB8(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimLeap_AFEB8, obj->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}
#endif
