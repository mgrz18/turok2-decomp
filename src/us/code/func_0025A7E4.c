#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_Weapon_B00C0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimWeapon_B00C0, obj->header.m_pIntelligence->m_Enemy.m_wWeaponAnimFlags);
}
