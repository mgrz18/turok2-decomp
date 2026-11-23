#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* Enemies only: bit 0x4000 of the enemy definition flags. Round 22 rewrote the m2c draft (two equal arms or a dead
 * do-while, an unmarked fake) as the getters round 21 de-faked: the case that returns the
 * field is tested first. */
s32 GameObject_CanLeaveRegionFlag80(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_dwEnemyFlags & 0x4000;
    }
    return 0;
}
