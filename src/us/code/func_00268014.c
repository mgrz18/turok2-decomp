#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* Enemies only: bit 0x1000 of the enemy definition's m_dwEnemyFlags. Round 22 rewrote the m2c
 * draft (identical arms, an empty do-while) as the getters round 21 de-faked: the test for the
 * case that returns the field first. */
s32 GameObject_CanLeaveInfoFlag52_80(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_dwEnemyFlags & 0x1000;
    }
    return 0;
}
