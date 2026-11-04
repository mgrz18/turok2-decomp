#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 pitchRate;
    /* 0x1C */ u8 pad1C[0x40 - 0x1C];
} Steer;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 pitch;
} Self;

void func_00246108(Self *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void func_0024599C(Self *self, CAI *ai, Steer *out);
f32 func_00211774(f32 cur, f32 target, f32 rate);
f32 func_0021160C(f32 value, f32 target, f32 rate);
void func_00245BAC(Self *self, CAI *ai, f32 turnRate, f32 angle);

void func_00261FB0(Self *self, CAI *ai) {
    CAITargets t;
    Steer steer;
    CAITargets *tp;

    ai->m_vTargetOffset.x = 0.0f;
    ai->m_vTargetOffset.z = 0.0f;
    ai->m_AnimSpeed = func_00211774(ai->m_AnimSpeed, 6.0f, 0.2f);
    ai->m_VarF32[3] = func_00211774(ai->m_VarF32[3], 0.20943953f, 0.5f);
    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);

    tp = &t;
    self->pitch = func_0021160C(self->pitch, tp->m_Path.m_vDelta.y, steer.pitchRate * 2.0f);
    if (self->pitch > 81.92f) {
        self->pitch = 81.92f;
    } else if (self->pitch < -81.92f) {
        self->pitch = -81.92f;
    }
    func_00245BAC(self, ai, ai->m_VarF32[3], tp->m_Path.m_Angle);
}
