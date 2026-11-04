#include "common.h"
#include "structs/boss.h"

/* AI state: turn toward the target, fire (func_00409A64) every third step, and go to anim state 0x16 once
 * the anim is done. */

typedef struct {
    /* 0x000 */ u8 pad0[0x234];
    /* 0x234 */ void *callback234;
    /* 0x238 */ u8 pad238[0x248 - 0x238];
    /* 0x248 */ void *callback248;
    /* 0x24C */ u8 pad24C[0x250 - 0x24C];
    /* 0x250 */ s32 unk250;
    /* 0x254 */ s32 unk254;
} LevelActor;

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
extern void func_00409A64(void *obj, s32 first);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_0040BB30(void *obj, CAI *ai) {
    Targets t;
    MotherBossState *state;

    func_00246108(obj, ai, &t, 0, 1);
    func_00245BAC(obj, ai, 0.261799425f, t.a.dist);
    state = &D_8012FB70;
    if (state->counter % 3 == 0) {
        func_00409A64(obj, state->counter == 0);
    }
    state->counter++;
    if (ai->animDone) {
        state->counter = 0;
        func_00243414(obj, ai, 0x16);
    }
}
