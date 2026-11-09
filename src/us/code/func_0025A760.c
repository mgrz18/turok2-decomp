#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Projectile_B0008(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimProjectile_B0008, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wProjectileAnimFlags);
}
