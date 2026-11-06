#include "common.h"
#include "structs/game_object.h"

/* The point turned to, as three globals (gEnemyAlertPoint is a CVector3 elsewhere). */
extern f32 gEnemyAlertPoint;
extern f32 gEnemyAlertPoint_y;
extern f32 gEnemyAlertPoint_z;

f32 AI_AngleToPoint(CGameObjectInstance *self, f32 x, f32 y, f32 z); /* angle to a point (a CVector3 by value) */
void AI_TurnClamped(CGameObjectInstance *self, CAI *ai, f32 rate, f32 angle);
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
    AI_TurnClamped(self, ai, TurnRate(self, ai), AI_AngleToPoint(self, gEnemyAlertPoint, gEnemyAlertPoint_y, gEnemyAlertPoint_z));
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
