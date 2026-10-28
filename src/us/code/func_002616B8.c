#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x174];
    /* 0x174 */ s8 kind;
} TargetObj;

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ TargetObj *obj;
    /* 0x08 */ f32 angle;
    /* 0x0C */ u8 padC[0x1C - 0xC];
    /* 0x1C */ f32 dy;
    /* 0x20 */ u8 pad20[0x24 - 0x20];
    /* 0x24 */ f32 dist;
    /* 0x28 */ u8 pad28[0x44 - 0x28];
} VecDst; /* 0x44 */

typedef struct {
    /* 0x00 */ VecDst a;
    /* 0x44 */ VecDst b;
} Targets;

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

typedef struct {
    /* 0x000 */ u8 pad0[0x40];
    /* 0x040 */ f32 timer;
    /* 0x044 */ u8 pad44[0xA0 - 0x44];
    /* 0x0A0 */ f32 homeY;
    /* 0x0A4 */ u8 padA4[0xB0 - 0xA4];
    /* 0x0B0 */ f32 targetY;
    /* 0x0B4 */ u8 padB4[0xBC - 0xB4];
    /* 0x0BC */ f32 speed;
    /* 0x0C0 */ u8 padC0[0x118 - 0xC0];
    /* 0x118 */ f32 turnRate;
} AI;

void func_00246108(Self *self, AI *ai, Targets *out, s32 f1, s32 f2);
void func_0024599C(Self *self, AI *ai, Steer *out);
f32 func_002974C0(f32 x);
f32 func_00211774(f32 cur, f32 target, f32 rate);
f32 func_0026872C(Self *self, s32 arg1);
f32 func_00268A5C(Self *self);
f32 func_0021160C(f32 value, f32 target, f32 rate);
void func_00245BAC(Self *self, AI *ai, f32 turnRate, f32 angle);
void func_00243414(Self *self, AI *ai, s32 mode);

void func_002616B8(Self *self, AI *ai) {
    Targets t;
    Steer steer;
    Params *params;
    Targets *tp;
    f32 speed;
    f32 turn;
    f32 y;
    f32 lo;
    f32 hi;

    params = &self->def->params;
    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);
    if (t.b.type == 2 && t.b.obj->kind == 6) {
        speed = 4.0f;
        turn = 0.20943953f;
    } else if (t.b.type == 0) {
        speed = 1.5f;
        turn = 0.104719765f;
    } else {
        speed = (func_002974C0(ai->timer * 0.052359883f) + 1.0f) * 0.25f + 1.0f;
        turn = (func_002974C0(ai->timer * 2.0f * 0.017453294f) + 1.0f) * 0.043633237f + 0.017453294f;
    }
    ai->turnRate = func_00211774(ai->turnRate, turn, 0.25f);
    ai->speed = func_00211774(ai->speed, speed, 0.25f);

    lo = func_0026872C(self, 0);
    hi = func_00268A5C(self);
    y = ai->homeY + func_002974C0(ai->timer * 0.0017453295f) * 102.399994f;
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
    ai->targetY = func_00211774(ai->targetY, y, 5.12f);

    tp = &t;
    self->pitch = func_0021160C(self->pitch, tp->b.dy, steer.pitchRate * 2.0f);
    if (self->pitch > 81.92f) {
        self->pitch = 81.92f;
    } else if (self->pitch < -81.92f) {
        self->pitch = -81.92f;
    }
    func_00245BAC(self, ai, ai->turnRate, tp->b.angle);

    if (ai->timer > 15.0f) {
        if (params->kind != 5) {
            if (t.b.type == 0 && t.b.dist < 81.92f
                && (t.b.angle < 0.0f ? -t.b.angle : t.b.angle) < 0.26179942f) {
                func_00243414(self, ai, 6);
            }
        } else {
            if (t.b.type == 0 && t.b.dist < params->biteRange) {
                func_00243414(self, ai, 16);
            }
        }
    }
}
