#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"

s16 EnemyAnim_LeftArmBlownOffDeath(CGameObjectInstance *obj) {
    return obj->header.m_pIntelligence->m_Enemy.m_DeathAnims[2];
}
