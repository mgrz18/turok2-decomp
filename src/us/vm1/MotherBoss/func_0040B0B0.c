#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"

/* AI state: turn toward the target (when there is one); once the anim is done, sends the module's six other
 * objects to anim state 0 and goes to anim state 2. */

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);

void MotherBoss_Mode06_RecallParts(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    CAITargets t2;
    MotherBossState *state;
    s32 i;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    if (t.m_Sight.m_pTarget != 0) {
        AI_UpdateTargets(obj, ai, &t2, 0, 1);
        AI_TurnClamped(obj, ai, 0.261799425f, t2.m_Sight.m_Angle);
    }
    if (ai->m_AnimRepeatedCount) {
        state = &gMotherBoss;
        for (i = 0; i < 6; i++) {
            CAI__SetMode(state->parts[i], &state->parts[i]->m_AI, 0);
        }
        CAI__SetMode(obj, ai, 2);
    }
}
