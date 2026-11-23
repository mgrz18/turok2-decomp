#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *out, s32 f1, s32 f2);

/* AI callback: turns toward the target, 60 degrees at a time in modes 1-2,
 * else 15. */
void EnemyAI_TurnToTarget(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    f32 rate;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    rate = 0.261799425f; /* 15 deg */
    if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
        rate = 1.0471977f; /* 60 deg */
    }
    /* FAKEMATCH: the identical branches are merged by cross-jumping; the extra
     * use of obj only gives it the register priority it has in the ROM ($s0
     * for obj, $s1 for ai). */
    if (obj->header.m_pIntelligence != NULL) {
        AI_TurnClamped(obj, ai, rate, t.m_Sight.m_Angle);
    } else {
        AI_TurnClamped(obj, ai, rate, t.m_Sight.m_Angle);
    }
}
