#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"
#include "protos/enemy_ai.h"

/* The point turned to is one CVector3 by value; the ROM loads its three words through the
 * interior symbols gEnemyAlertPoint_y/_z (a struct argument shares one base register and does
 * not match), so the N64 build calls AI_AngleToPoint as taking three floats, which on MIPS go in
 * the same registers. */
#ifdef TARGET_PORT
#define AlertPointAngle(self) AI_AngleToPoint(self, gEnemyAlertPoint)
#else
#define AlertPointAngle(self) \
    ((f32 (*)(CGameObjectInstance *, f32, f32, f32)) AI_AngleToPoint)(self, gEnemyAlertPoint.x, gEnemyAlertPoint_y, gEnemyAlertPoint_z)
#endif


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
    AI_TurnClamped(self, ai, TurnRate(self, ai), AlertPointAngle(self));
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
