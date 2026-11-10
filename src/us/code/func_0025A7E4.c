#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Weapon_B00C0(CGameObjectInstance *obj, CAI *ai) {
    return AI_PickAnimFromTable(obj, ai, sEnemyAnimWeapon_B00C0, obj->header.m_pIntelligence->m_Enemy.m_wWeaponAnimFlags);
}
#else
void EnemyAnim_Weapon_B00C0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimWeapon_B00C0, obj->header.m_pIntelligence->m_Enemy.m_wWeaponAnimFlags);
}
#endif
