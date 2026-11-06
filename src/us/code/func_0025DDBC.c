#include "common.h"
#include "structs/game_object.h"

/* AI recoil update: turns toward the target at double speed, counts the
 * recoil timer down, and on being hit either reacts (anim 0xA / 0x14) or
 * clears the recoil flag; dodges once the timer is above 15. */

extern f32 D_800B6D28; /* frame delta */

extern void AI_UpdateTargets(CGameObjectInstance *actor, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_TurnClamped(CGameObjectInstance *actor, CAI *ai, f32 angle, f32 dist);
extern void CAI__SetMode(CGameObjectInstance *actor, CAI *ai, s32 anim);
extern void EnemyAI_TryDodge(CGameObjectInstance *actor, CAI *ai);

void EnemyMode_Combo_Update(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    f32 angle;

    AI_UpdateTargets(actor, ai, &t, 0, 1);
    angle = 0.261799425f;
    if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
        angle = 1.0471977f;
    }
    AI_TurnClamped(actor, ai, angle * 2.0f, t.m_Sight.m_Angle);
    if (ai->m_AnimType >= 4400 && ai->m_AnimType < 4555) {
        ai->m_dwFlags |= 0x4000000;
    }
    ai->m_ConditionTimer -= D_800B6D28;
    if (ai->m_AnimRepeatedCount != 0) {
        if (!(ai->m_dwFlags & 0x4000000)) {
            CAI__SetMode(actor, ai, 0xA);
            actor->m_dwFlags |= 0x20000;
        } else if (ai->m_ConditionTimer <= 0.0f) {
            CAI__SetMode(actor, ai, 0x14);
            actor->m_dwFlags |= 0x20000;
        } else {
            ai->m_dwFlags &= ~0x4000000;
            ai->m_ModeLastFrame = -1;
        }
    }
    if (ai->m_ConditionTimer > 15.0f) {
        EnemyAI_TryDodge(actor, ai);
    }
}
