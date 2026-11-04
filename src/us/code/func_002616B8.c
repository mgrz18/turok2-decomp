#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 pitchRate;
    /* 0x1C */ u8 pad1C[0x40 - 0x1C];
} Steer;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 biteRange;
    /* 0x44 */ u8 pad44[0x4E - 0x44];
    /* 0x4E */ u8 kind;
} Params;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Params params;
} Def;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Def *def;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
    /* 0x1C */ f32 pitch;
} Self;

void func_00246108(Self *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void func_0024599C(Self *self, CAI *ai, Steer *out);
f32 func_002974C0(f32 x);
f32 func_00211774(f32 cur, f32 target, f32 rate);
f32 func_0026872C(Self *self, s32 arg1);
f32 func_00268A5C(Self *self);
f32 func_0021160C(f32 value, f32 target, f32 rate);
void func_00245BAC(Self *self, CAI *ai, f32 turnRate, f32 angle);
void func_00243414(Self *self, CAI *ai, s32 mode);

void func_002616B8(Self *self, CAI *ai) {
    CAITargets t;
    Steer steer;
    Params *params;
    CAITargets *tp;
    f32 speed;
    f32 turn;
    f32 y;
    f32 lo;
    f32 hi;

    params = &self->def->params;
    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);
    if (t.m_Path.m_Kind == 2 && t.m_Path.m_pTarget->m_AI.m_Mode == 6) {
        speed = 4.0f;
        turn = 0.20943953f;
    } else if (t.m_Path.m_Kind == 0) {
        speed = 1.5f;
        turn = 0.104719765f;
    } else {
        speed = (func_002974C0(ai->m_ModeTime * 0.052359883f) + 1.0f) * 0.25f + 1.0f;
        turn = (func_002974C0(ai->m_ModeTime * 2.0f * 0.017453294f) + 1.0f) * 0.043633237f + 0.017453294f;
    }
    ai->m_VarF32[3] = func_00211774(ai->m_VarF32[3], turn, 0.25f);
    ai->m_AnimSpeed = func_00211774(ai->m_AnimSpeed, speed, 0.25f);

    lo = func_0026872C(self, 0);
    hi = func_00268A5C(self);
    y = ai->m_vInitialPos.y + func_002974C0(ai->m_ModeTime * 0.0017453295f) * 102.399994f;
    if (y < lo) {
        y += 102.399994f;
    }
    if (hi < y) {
        y -= 102.399994f;
    }
    if (y < lo) {
        y = lo;
    } else if (hi < y) {
        y = hi;
    }
    ai->m_vLeashPos.y = func_00211774(ai->m_vLeashPos.y, y, 5.12f);

    tp = &t;
    self->pitch = func_0021160C(self->pitch, tp->m_Path.m_vDelta.y, steer.pitchRate * 2.0f);
    if (self->pitch > 81.92f) {
        self->pitch = 81.92f;
    } else if (self->pitch < -81.92f) {
        self->pitch = -81.92f;
    }
    func_00245BAC(self, ai, ai->m_VarF32[3], tp->m_Path.m_Angle);

    if (ai->m_ModeTime > 15.0f) {
        if (params->kind != 5) {
            if (t.m_Path.m_Kind == 0 && t.m_Path.m_Dist < 81.92f
                && (t.m_Path.m_Angle < 0.0f ? -t.m_Path.m_Angle : t.m_Path.m_Angle) < 0.26179942f) {
                func_00243414(self, ai, 6);
            }
        } else {
            if (t.m_Path.m_Kind == 0 && t.m_Path.m_Dist < params->biteRange) {
                func_00243414(self, ai, 16);
            }
        }
    }
}
