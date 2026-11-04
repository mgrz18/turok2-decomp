#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
} Steer;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ f32 y;
} Self;

void func_00246108(Self *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void func_0024599C(Self *self, CAI *ai, Steer *out);
void func_00260D60(Self *self, CAI *ai, CAITargets *t, Steer *steer, f32 speed, f32 arg5);
s32 func_0025D030(Self *self, CAI *ai, CAITargets *t);
void func_00243414(Self *self, CAI *ai, s32 mode);

void func_0026115C(Self *self, CAI *ai) {
    CAITargets t;
    Steer steer;
    s32 next;

    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);
    t.m_Path.m_vDelta.y = t.m_Sight.m_vDelta.y = ai->m_vLeashPos.y - self->y;
    if (ai->m_Action == 1) {
        func_00260D60(self, ai, &t, &steer, 2.0f, 2.0f);
    } else {
        func_00260D60(self, ai, &t, &steer, 1.0f, 1.0f);
    }
    if (t.m_Sight.m_pTarget != 0 && (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) <= 1.5707965f) {
        next = func_0025D030(self, ai, &t);
        if (next != 0) {
            ai->m_ModeLastFrame = -1;
            func_00243414(self, ai, next);
        }
    }
}
