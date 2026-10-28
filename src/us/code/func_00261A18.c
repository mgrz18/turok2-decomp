#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x4E];
    /* 0x4E */ u8 kind;
} Params;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Params params;
} Def;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Def *def;
} Self;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ f32 pos[3];
    /* 0x10 */ u8 pad10[0x50 - 0x10];
    /* 0x50 */ f32 rotY;
} Target;

typedef struct {
    /* 0x00 */ u8 pad0[0x36];
    /* 0x36 */ s8 state;
    /* 0x37 */ u8 pad37[0x88 - 0x37];
    /* 0x88 */ Target *target;
    /* 0x8C */ u8 pad8C[0xA0 - 0x8C];
    /* 0xA0 */ f32 destY;
    /* 0xA4 */ u8 padA4[0xAC - 0xA4];
    /* 0xAC */ f32 destX;
    /* 0xB0 */ f32 homeY;
    /* 0xB4 */ f32 destZ;
} AI;

f32 func_002119FC(f32 lo, f32 hi);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_00243414(Self *self, AI *ai, s32 mode);

void func_00261A18(Self *self, AI *ai) {
    Params *params;
    Target *target;
    f32 dist;
    f32 angle;

    params = &self->def->params;
    target = ai->target;
    if (target != NULL && params->kind != 5) {
        dist = func_002119FC(-102.399994f, 102.399994f) + 512.0f;
        angle = target->rotY + 3.1415927f + func_002119FC(-0.78539824f, 0.78539824f);
        ai->destX = target->pos[0] + dist * func_002982F0(angle);
        ai->destY = (ai->homeY + target->pos[1]) * 0.5f + func_002119FC(-51.199997f, 51.199997f);
        ai->destZ = target->pos[2] + dist * func_002974C0(angle);
    }
    if (params->kind == 4 && ai->state != 27) {
        func_00243414(self, ai, 27);
    } else if (params->kind == 3 && ai->state != 10) {
        func_00243414(self, ai, 10);
    }
}
