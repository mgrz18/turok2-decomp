#include "common.h"
#include "structs/game_object.h"

/* AI state: turn toward the target (15 degrees per step), resetting the step counter, then go to anim state 0xC. */

extern s32 D_801301EC; /* module state +0x67C: step counter */
extern void func_00246108(void *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void func_00245BAC(void *obj, CAI *ai, f32 angle, f32 dist);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_0040B524(void *obj, CAI *ai) {
    CAITargets t;

    D_801301EC = 0;
    func_00246108(obj, ai, &t, 0, 1);
    func_00245BAC(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        func_00243414(obj, ai, 0xC);
    }
}
