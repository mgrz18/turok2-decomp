#include "common.h"

/* AI state: turn toward the target (15 degrees per step), then go to anim state 0xC. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 dist;
    /* 0x0C */ u8 padC[0x44 - 0xC];
} Target;

typedef struct {
    /* 0x00 */ Target a;
    /* 0x44 */ Target b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0xC7];
    /* 0xC7 */ s8 animDone;
} CAI;

extern void func_00246108(void *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00245BAC(void *obj, CAI *ai, f32 angle, f32 dist);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_0040B3A8(void *obj, CAI *ai) {
    Targets t;

    func_00246108(obj, ai, &t, 0, 1);
    func_00245BAC(obj, ai, 0.261799425f, t.a.dist);
    if (ai->animDone) {
        func_00243414(obj, ai, 0xC);
    }
}
