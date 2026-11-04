#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* AI update for the module's actor: runs the AI step and the current anim's callback; in phase 0, a target
 * closer than 1228.8 makes the D_8010FEB4 actors drop their health; then updates the module (func_00409784). */

extern void func_0025AD90(void *obj, CAI *ai, f32 pitchOffset);
extern f32 func_0040BD60(void *obj, void **found);
extern void func_0040BD10(void);
extern void func_00409784(MotherBossState *state);

void func_0040BC64(void *obj, CAI *ai) {
    MotherBossState *state = &D_8012FB70;
    void *found;
    f32 dist;

    func_0025AD90(obj, ai, 0.0f);
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    if (state->phase == 0) {
        dist = func_0040BD60(obj, &found);
        if (found != NULL && dist < 1228.7999f) {
            func_0040BD10();
        }
    }
    func_00409784(&D_8012FB70);
}
