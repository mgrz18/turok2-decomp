#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* AI state: turn toward the target, fire (func_00409A64) every third step, and go to anim state 0x16 once
 * the anim is done. */


extern void func_00246108(void *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void func_00245BAC(void *obj, CAI *ai, f32 angle, f32 dist);
extern void func_00409A64(void *obj, s32 first);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_0040BB30(void *obj, CAI *ai) {
    CAITargets t;
    MotherBossState *state;

    func_00246108(obj, ai, &t, 0, 1);
    func_00245BAC(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    state = &D_8012FB70;
    if (state->counter % 3 == 0) {
        func_00409A64(obj, state->counter == 0);
    }
    state->counter++;
    if (ai->m_AnimRepeatedCount) {
        state->counter = 0;
        func_00243414(obj, ai, 0x16);
    }
}
