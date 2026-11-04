#include "common.h"
#include "structs/game_object.h"

/* Flying CAI state: steer towards the target, attack when possible (resetting
 * the attack delay), and fall back to state 10 once the timer runs out. */

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
} Steer;

extern f32 D_800B6D28;
void func_00246108(void *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void func_0024599C(void *self, CAI *ai, Steer *out);
void func_00260730(void *self, CAI *ai, CAITargets *t, Steer *steer);
s32 func_0025D030(void *self, CAI *ai, CAITargets *t);
void func_00243414(void *self, CAI *ai, s32 mode);
f32 func_0025EB50(void *self, CAI *ai, f32 lo, f32 hi);

void func_00260A34(void *self, CAI *ai) {
    CAITargets t;
    Steer steer;
    s32 mode;

    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);
    func_00260730(self, ai, &t, &steer);
    mode = func_0025D030(self, ai, &t);
    if (mode != 0) {
        ai->m_ModeLastFrame = -1;
        func_00243414(self, ai, mode);
        ai->m_Action = 0;
        ai->m_ActionTime = func_0025EB50(self, ai, 90.0f, 30.0f);
    }
    ai->m_ConditionTimer += D_800B6D28;
    if (func_0025EB50(self, ai, 60.0f, 120.0f) < ai->m_ConditionTimer) {
        ai->m_ConditionTimer = 0.0f;
        func_00243414(self, ai, 10);
    }
}
