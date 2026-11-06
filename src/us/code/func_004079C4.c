#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1F50[];
extern f32 D_8012F628;          /* last angle to the target */

void Primagen_UpdateMode10(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
    if (t.m_Sight.m_pTarget != NULL) {
        AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
        D_8012F628 = t.m_Sight.m_Angle;
    } else {
        AI_TurnClamped(obj, ai, 0.261799425f, D_8012F628);
    }
    if (ai->m_AnimRepeatedCount != 0) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        D_8012F9D8.script.current = -1;
        D_8012F9D8.mode = 6;
        CAI__SetMode(obj, ai, 1);
    }
}
