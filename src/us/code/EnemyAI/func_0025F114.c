#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"
#include "protos/ai.h"

/* The point turned to is passed as its three floats; .y and .z go through the
 * interior symbols gEnemyAlertPoint_y/_z (the member path shares one base register
 * and does not match). */

#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_AngleToPoint AI_AngleToPoint_f3
#endif
f32 AI_AngleToPoint(CGameObjectInstance *self, f32 x, f32 y, f32 z); /* angle to a point (a CVector3 by value) */
void EnemyMode_Move_Update(CGameObjectInstance *self, CAI *ai);

/* Like EnemyAI_GetModeTurnRate: the turn rate, pi/3 in modes 1-2, else pi/12. */
static __inline__ f32 TurnRate(CGameObjectInstance *self, CAI *ai) {
    f32 rate = 0.261799425f;

    if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
        rate = 1.0471977f;
    }
    return rate;
}

/* AI callback: turns toward the point gEnemyAlertPoint, then calls EnemyMode_Move_Update
 * once 0xC7 reaches 5. */
void EnemyMode_Alert_Update(CGameObjectInstance *self, CAI *ai) {
    AI_TurnClamped(self, ai, TurnRate(self, ai), AI_AngleToPoint(self, gEnemyAlertPoint.x, gEnemyAlertPoint_y, gEnemyAlertPoint_z));
    if (ai->m_AnimRepeatedCount >= 5) {
        /* FAKEMATCH: the identical branches are merged by cross-jumping; the
         * extra use of self only gives it the register priority it has in the
         * ROM ($s0 for self, $s1 for ai). */
        if (self->header.m_pIntelligence != NULL) {
            EnemyMode_Move_Update(self, ai);
        } else {
            EnemyMode_Move_Update(self, ai);
        }
    }
}
