#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* AI state: turn toward the target; after 100 steps, or when the target is within 460.8 and less than 20
 * degrees off, go to anim state 0x16. */

extern void AI_UpdateTargets(void *obj, void *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_TurnClamped(void *obj, void *ai, f32 turnRate, f32 angle);
extern void CAI__SetMode(void *obj, void *ai, s32 state);

void MotherBoss_Mode23_Approach(void *obj, void *ai) {
    CAITargets t;
    MotherBossState *state;

    AI_UpdateTargets(obj, ai, &t, 1, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Path.m_Angle);
    state = &gMotherBoss;
    if (++state->counter > 100 ||
        (t.m_Path.m_pTarget != NULL && t.m_Sight.m_pTarget != NULL && t.m_Path.m_FlatDist < 460.8f &&
         (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle <= 0.34906590f : t.m_Sight.m_Angle <= 0.34906590f))) {
        CAI__SetMode(obj, ai, 0x16);
    }
}
