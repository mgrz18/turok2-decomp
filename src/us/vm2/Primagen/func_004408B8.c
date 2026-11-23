/* The second VM module's copy of src/us/vm1/Primagen/func_004078B8.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"
#include "globals/collision.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);


void Primagen_UpdateMode9_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount == 0) {
        return;
    }
    gLevelScript_bossState.script.current = -1;
    obj->m_AI.m_AnimSpeed = 1.0f;
    if (gLevelScript_bossState.state == 6 || t.m_Sight.m_Dist <= 460.8f) {
        gLevelScript_bossState.mode = 6;
        CAI__SetMode(obj, ai, 1);
    } else if (gLevelScript_bossState.state == 2) {
        CAI__SetMode(obj, ai, 7);
    }
}
