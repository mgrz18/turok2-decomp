#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"
#include "globals/collision.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);


void Primagen_UpdateMode10_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;
    if (t.m_Sight.m_pTarget != NULL) {
        AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
        gLevelScript.targetAngle = t.m_Sight.m_Angle;
    } else {
        AI_TurnClamped(obj, ai, 0.261799425f, gLevelScript.targetAngle);
    }
    if (ai->m_AnimRepeatedCount != 0) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        gLevelScript_bossState.script.current = -1;
        gLevelScript_bossState.mode = 6;
        CAI__SetMode(obj, ai, 1);
    }
}
