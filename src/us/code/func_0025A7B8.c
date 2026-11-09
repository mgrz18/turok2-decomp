#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Weapon_B0068(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimWeapon_B0068, obj->header.m_pIntelligence->m_Enemy.m_wWeaponAnimFlags);
}
