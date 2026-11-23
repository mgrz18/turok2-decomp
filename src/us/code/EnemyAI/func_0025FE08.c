#include "common.h"
#include "structs/game_object.h"

s16 EnemyAnim_BodyHoleDeath(CGameObjectInstance *arg0) {
    return arg0->header.m_pIntelligence->m_Enemy.m_DeathAnims[4];
}
