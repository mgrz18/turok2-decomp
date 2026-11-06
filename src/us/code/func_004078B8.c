#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1F50[];

void Primagen_UpdateMode9(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
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
