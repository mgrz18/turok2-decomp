#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* AI update for the module's actor: runs the AI step and the current anim's callback; in phase 0, a target
 * closer than 1228.8 makes the gScene_m_InstancePool_m_ActiveList actors drop their health; then updates the module (MotherBoss_UpdateSpotsAndPickups). */

extern void AI_TurnHeadToTarget(void *obj, CAI *ai, f32 pitchOffset);
extern f32 MotherBoss_GetTargetDist(void *obj, void **found);
extern void MotherBoss_KillMinions(void);
extern void MotherBoss_UpdateSpotsAndPickups(MotherBossState *state);

void MotherBoss_Step(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    void *found;
    f32 dist;

    AI_TurnHeadToTarget(obj, ai, 0.0f);
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    if (state->phase == 0) {
        dist = MotherBoss_GetTargetDist(obj, &found);
        if (found != NULL && dist < 1228.7999f) {
            MotherBoss_KillMinions();
        }
    }
    MotherBoss_UpdateSpotsAndPickups(&gMotherBoss);
}
