#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

void func_00246108(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void func_00245BAC(CGameObjectInstance *self, CAI *ai, f32 angle, f32 arg3);
void func_00243414(CGameObjectInstance *self, CAI *ai, s32 mode);

/* Like func_0025EB24: the turn rate, pi/3 in modes 1-2, else pi/12. */
static __inline__ f32 TurnRate(CGameObjectInstance *self, CAI *ai) {
    f32 rate = 0.261799425f;

    if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
        rate = 1.0471977f;
    }
    return rate;
}

/* AI callback: turns toward the target (pi/3 at a time in modes 1-2, else
 * pi/12) and goes to mode 4 when done. */
void func_0025E7E0(CGameObjectInstance *self, CAI *ai) {
    CEnemyIntelligence *sub = &self->header.m_pIntelligence->m_Enemy;
    CAITargets t;

    func_00246108(self, ai, &t, 0, 1);
    func_00245BAC(self, ai, TurnRate(self, ai), t.m_Sight.m_Angle);
    if (sub->m_StartAnim == 0 || ai->m_AnimRepeatedCount != 0) {
        /* FAKEMATCH: the identical branches are merged by cross-jumping; the
         * extra use of self only gives it the register priority it has in the
         * ROM ($s0 for self, $s1 for ai). */
        if (self->header.m_pIntelligence != NULL) {
            func_00243414(self, ai, 4);
        } else {
            func_00243414(self, ai, 4);
        }
    }
}
