#include "common.h"
#include "structs/game_object.h"

s16 EnemyAnim_RightArmBlownOffDeath(CGameObjectInstance *arg0) {
    return arg0->header.m_pIntelligence->m_Enemy.m_DeathAnims[3];
}
