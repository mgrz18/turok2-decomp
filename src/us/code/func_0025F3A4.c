#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

/* AI state: turn towards the target with the 10000 animation, the turn rate
 * depending on the stance; release the grabbed actor near the end of it. */

s32 CGameObjectInstance__PickRandomAnim(void *self, s32 anim, s32 arg2);
void AI_UpdateTargets(void *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);

void EnemyMode_PickupThrowObject_Update(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    u8 *held = (u8 *) ai->m_Enemy.m_pGrabbed;
    f32 rate;
    s32 anim;

    anim = CGameObjectInstance__PickRandomAnim(self, 10000, -1);
    if (self->m_CurrentAnimation.m_nAnimIndex == anim) {
        AI_UpdateTargets(self, ai, &t, 0, 1);
        rate = 0.26179942f;
        if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
            rate = 1.0471977f;
        }
        AI_TurnClamped(self, ai, rate * 1.5f, t.m_Sight.m_Angle);
        if (held != NULL && self->m_CurrentAnimation.m_nFrames * 0.7f <= self->m_CurrentAnimation.m_AnimationFrame) {
            CAI__SetMode(held, held + 0x140, 2);
            ai->m_Enemy.m_pGrabbed = NULL;
        }
    }
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(self, ai, 10);
    }
}
