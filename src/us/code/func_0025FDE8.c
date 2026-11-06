#include "common.h"
#include "structs/game_object.h"

s16 EnemyAnim_LeftArmBlownOffDeath(CGameObjectInstance *obj) {
    return obj->header.m_pIntelligence->m_Enemy.m_DeathAnims[2];
}
