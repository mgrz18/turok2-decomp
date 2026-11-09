#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/game.h"


void EnemyAI_SaveDeadAndFireTriggers(CGameObjectInstance *arg0) {
    CIntelligence *sub = arg0->header.m_pIntelligence;

    if (!(arg0->m_dwFlags & 0x80000)) {
        if (!(sub->m_dwTypeFlags & 0x20)) {
            CScene__SetInstanceSavedBit((s32) &gScene, (s32) arg0, 1);
        }
        GameObject_FireTriggers(arg0, 1, arg0);
    }
}
