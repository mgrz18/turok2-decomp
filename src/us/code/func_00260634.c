#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
} Steer;

void func_00246108(void *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void func_0024599C(void *self, CAI *ai, Steer *out);
f32 func_0025EB24(void *self, CAI *ai);
void func_00245BAC(void *self, CAI *ai, f32 angle, f32 arg3);
f32 func_0025EAFC(void *self, CAI *ai);
s32 func_0025D030(void *self, CAI *ai, CAITargets *t);
void func_00243414(void *self, CAI *ai, s32 mode);

void func_00260634(void *self, CAI *ai) {
    CAITargets t;
    Steer steer;
    f32 range;
    s32 mode;

    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);
    func_00245BAC(self, ai, func_0025EB24(self, ai), t.m_Sight.m_Angle);
    range = func_0025EAFC(self, ai);
    if ((t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) <= range) {
        mode = func_0025D030(self, ai, &t);
        if (mode != 0) {
            ai->m_ModeLastFrame = -1;
            func_00243414(self, ai, mode);
        } else {
            func_00243414(self, ai, 10);
        }
    }
}
