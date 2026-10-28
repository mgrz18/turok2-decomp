#include "common.h"

/* AI recoil update: turns toward the target at double speed, counts the
 * recoil timer down, and on being hit either reacts (anim 0xA / 0x14) or
 * clears the recoil flag; dodges once the timer is above 15. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 padC[0x44 - 0xC];
} Target;

typedef struct {
    /* 0x00 */ Target a;
    /* 0x44 */ Target b;
} Targets;

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ u32 flags;
} Actor;

typedef struct {
    /* 0x000 */ u32 flags;
    /* 0x004 */ u8 pad4[0x35 - 0x4];
    /* 0x035 */ s8 prevState;
    /* 0x036 */ u8 unk36;
    /* 0x037 */ u8 pad37[0x64 - 0x37];
    /* 0x064 */ f32 timer;
    /* 0x068 */ u8 pad68[0xC4 - 0x68];
    /* 0x0C4 */ u16 animId;
    /* 0x0C6 */ u8 padC6;
    /* 0x0C7 */ s8 hits;
} CAI;

extern f32 D_800B6D28; /* frame delta */

extern void func_00246108(Actor *actor, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00245BAC(Actor *actor, CAI *ai, f32 angle, f32 dist);
extern void func_00243414(Actor *actor, CAI *ai, s32 anim);
extern void func_0025D308(Actor *actor, CAI *ai);

void func_0025DDBC(Actor *actor, CAI *ai) {
    Targets t;
    f32 angle;

    func_00246108(actor, ai, &t, 0, 1);
    angle = 0.261799425f;
    if (ai->unk36 == 1 || ai->unk36 == 2) {
        angle = 1.0471977f;
    }
    func_00245BAC(actor, ai, angle * 2.0f, t.a.unk8);
    if (ai->animId >= 4400 && ai->animId < 4555) {
        ai->flags |= 0x4000000;
    }
    ai->timer -= D_800B6D28;
    if (ai->hits != 0) {
        if (!(ai->flags & 0x4000000)) {
            func_00243414(actor, ai, 0xA);
            actor->flags |= 0x20000;
        } else if (ai->timer <= 0.0f) {
            func_00243414(actor, ai, 0x14);
            actor->flags |= 0x20000;
        } else {
            ai->flags &= ~0x4000000;
            ai->prevState = -1;
        }
    }
    if (ai->timer > 15.0f) {
        func_0025D308(actor, ai);
    }
}
