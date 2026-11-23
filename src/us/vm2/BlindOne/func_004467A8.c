/* The second VM module's copy of src/us/vm1/BlindOne/func_0040D7A8.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "globals/game.h"
#include "globals/boss_ai.h"
#include "protos/ai.h"

/* AI init for the module's minions (types 0x456 and others): sets the anim table and callbacks, targets the
 * current player and starts anim state 1. */

extern void BlindOne_ActorUpdate_2(void);
extern void BlindOne_ActorHurt_2(void);
extern void EnemyAI_HeadNodeCallback(void);
extern void BlindOne_Group3Draw_2(void);
extern void BlindOne_Draw_2(void);

void BlindOne_Group2Init_2(CGameObjectInstance *obj, CAI *ai) {
    void *player;

    ai->m_pModeTable = gBlindOneMinionModeTable;
    ai->m_pfStep = BlindOne_ActorUpdate_2;
    ai->m_pfHurt = BlindOne_ActorHurt_2;
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    if (obj->m_nTypeFlag == 0x456) {
        ai->m_pfDraw = BlindOne_Group3Draw_2;
    } else {
        ai->m_pfDraw = BlindOne_Draw_2;
    }
    ai->m_BlindOne.m_nPhaseHits = 0;
    ai->m_BlindOne.m_HitTimer = 0;
    ai->m_BlindOne.m_SightDist = 0;
    ai->m_Health = 0;
    obj->m_dwFlags &= ~0x10000000;
    if (obj->m_nTypeFlag == 0x456) {
        obj->m_dwFlags |= 0x20000;
    } else {
        obj->m_dwFlags &= ~0x20000;
    }
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    if (player != NULL) {
        ai->m_pSightTarget = player;
        ai->m_pPathTarget = player;
        ai->m_dwFlags |= 0x60000000;
    }
    CAI__SetMode(obj, ai, 1);
    obj->m_dwFlags &= ~0x100;
}
