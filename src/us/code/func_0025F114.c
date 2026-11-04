#include "common.h"
#include "structs/game_object.h"

/* The point turned to, as three globals (D_800B06A0 is a CVector3 elsewhere). */
extern f32 D_800B06A0;
extern f32 D_800B06A4;
extern f32 D_800B06A8;

f32 func_00246310(CGameObjectInstance *self, f32 x, f32 y, f32 z); /* angle to a point (a CVector3 by value) */
void func_00245BAC(CGameObjectInstance *self, CAI *ai, f32 rate, f32 angle);
void func_0025D474(CGameObjectInstance *self, CAI *ai);

/* Like func_0025EB24: the turn rate, pi/3 in modes 1-2, else pi/12. */
static __inline__ f32 TurnRate(CGameObjectInstance *self, CAI *ai) {
    f32 rate = 0.261799425f;

    if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
        rate = 1.0471977f;
    }
    return rate;
}

/* CAI callback: turns toward the point D_800B06A0, then calls func_0025D474
 * once 0xC7 reaches 5. */
void func_0025F114(CGameObjectInstance *self, CAI *ai) {
    func_00245BAC(self, ai, TurnRate(self, ai), func_00246310(self, D_800B06A0, D_800B06A4, D_800B06A8));
    if (ai->m_AnimRepeatedCount >= 5) {
        /* FAKEMATCH: the identical branches are merged by cross-jumping; the
         * extra use of self only gives it the register priority it has in the
         * ROM ($s0 for self, $s1 for ai). */
        if (self->header.m_pIntelligence != NULL) {
            func_0025D474(self, ai);
        } else {
            func_0025D474(self, ai);
        }
    }
}
