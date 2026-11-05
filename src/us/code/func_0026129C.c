#include "common.h"
#include "structs/game_object.h"

void func_00246108(void *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void func_0024599C(void *self, CAI *ai, CAIFrameMotion *out);
void func_00260D60(void *self, CAI *ai, CAITargets *t, CAIFrameMotion *steer, f32 speed, f32 arg5);
void func_00243414(void *self, CAI *ai, s32 mode);

void func_0026129C(void *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    f32 dist;

    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);
    dist = t.m_Path.m_FlatDist;
    if (!(dist >= 1024.0f)) {
        dist = 1024.0f;
    }
    func_00260D60(self, ai, &t, &steer, 6.0f - dist * (1.0f / 1024) * 3.0f, 2.0f);
    if ((t.m_Sight.m_pTarget != 0 && (t.m_Sight.m_vDelta.y < 0.0f ? -t.m_Sight.m_vDelta.y : t.m_Sight.m_vDelta.y) < 51.199997f) || ai->m_ModeTime > 90.0f) {
        func_00243414(self, ai, 4);
    }
}
