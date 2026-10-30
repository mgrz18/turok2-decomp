#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 padC[0x44 - 0xC];
} VecDst;

typedef struct {
    /* 0x00 */ VecDst a;
    /* 0x44 */ VecDst b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 unk8;
} DefSub;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ DefSub sub;
} ActorDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorDef *def;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x36];
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 pad37[0xC7 - 0x37];
    /* 0xC7 */ s8 unkC7;
} AI;

void func_00246108(Actor *self, AI *ai, Targets *out, s32 f1, s32 f2);
void func_00245BAC(Actor *self, AI *ai, f32 angle, f32 arg3);
void func_00243414(Actor *self, AI *ai, s32 mode);

/* Like func_0025EB24: the turn rate, pi/3 in modes 1-2, else pi/12. */
static __inline__ f32 TurnRate(Actor *self, AI *ai) {
    f32 rate = 0.261799425f;

    if (ai->unk36 == 1 || ai->unk36 == 2) {
        rate = 1.0471977f;
    }
    return rate;
}

/* AI callback: turns toward the target (pi/3 at a time in modes 1-2, else
 * pi/12) and goes to mode 4 when done. */
void func_0025E7E0(Actor *self, AI *ai) {
    DefSub *sub = &self->def->sub;
    Targets t;

    func_00246108(self, ai, &t, 0, 1);
    func_00245BAC(self, ai, TurnRate(self, ai), t.a.unk8);
    if (sub->unk8 == 0 || ai->unkC7 != 0) {
        /* FAKEMATCH: the identical branches are merged by cross-jumping; the
         * extra use of self only gives it the register priority it has in the
         * ROM ($s0 for self, $s1 for ai). */
        if (self->def != NULL) {
            func_00243414(self, ai, 4);
        } else {
            func_00243414(self, ai, 4);
        }
    }
}
