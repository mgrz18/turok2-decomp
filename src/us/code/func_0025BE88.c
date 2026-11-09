#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"

/* AI init dispatcher: the 0x44E..0x456 types have their own init in the paged
 * VM code; everything else gets the generic init. */


extern void MantidQueen_InitAI(CGameObjectInstance *actor, CAI *ai);
extern void MotherBoss_Init(CGameObjectInstance *actor, CAI *ai);
extern void Primagen_InitAI(CGameObjectInstance *actor, CAI *ai);
extern void BlindOne_BossInit(CGameObjectInstance *actor, CAI *ai);
extern void BlindOne_Group1Init(CGameObjectInstance *actor, CAI *ai);
extern void BlindOne_Group2Init(CGameObjectInstance *actor, CAI *ai);
extern void BlindOne_Group3Init(CGameObjectInstance *actor, CAI *ai);
extern void AI_HearNoise(CGameObjectInstance *actor, CAI *ai, f32 *pos, s32 arg3);

void EnemyAI_InitDispatch(CGameObjectInstance *actor, CAI *ai) {
    u32 *defFlags = &actor->header.m_pIntelligence->m_dwTypeFlags;

    switch (actor->m_nTypeFlag) {
    case 0x451:
        MantidQueen_InitAI(actor, ai);
        break;
    case 0x44E:
        MotherBoss_Init(actor, ai);
        break;
    case 0x453:
        Primagen_InitAI(actor, ai);
        break;
    case 0x450:
        BlindOne_BossInit(actor, ai);
        break;
    case 0x454:
        BlindOne_Group1Init(actor, ai);
        break;
    case 0x455:
        BlindOne_Group2Init(actor, ai);
        break;
    case 0x456:
        BlindOne_Group3Init(actor, ai);
        break;
    default:
        if (actor->m_nTypeFlag == 0x467) {
            CAI__SetMode(actor, ai, 0x31);
        } else if (actor->m_dwFlags & 0x80000) {
            CAI__SetMode(actor, ai, 2);
        } else {
            CAI__SetMode(actor, ai, 0);
        }
        ai->m_Action = 0;
        break;
    }
    if (!(*defFlags & 0x8000)) {
        ai->m_Action = 1;
        AI_HearNoise(actor, ai, &actor->header.m_vPos.x, 3);
    }
    if (CScene__GetInstanceSavedBit(&gScene, actor) == 1) {
        actor->m_dwFlags &= ~0x100;
    }
}
