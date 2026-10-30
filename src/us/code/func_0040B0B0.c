#include "common.h"

/* AI state: turn toward the target (when there is one); once the anim is done, sends the module's six other
 * objects to anim state 0 and goes to anim state 2. */

typedef struct Obj {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

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
    /* 0x000 */ void *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ LevelActor *actors[4];
    /* 0x018 */ Slot slots[15];
    /* 0x630 */ struct Obj *others[6];
    /* 0x648 */ u8 pad648[0x65C - 0x648];
    /* 0x65C */ s32 phase;
    /* 0x660 */ s32 activeMask;
    /* 0x664 */ s32 unk664;
    /* 0x668 */ u8 pad668[0x674 - 0x668];
    /* 0x674 */ s32 unk674;
    /* 0x678 */ s32 unk678;
    /* 0x67C */ s32 counter;
    /* 0x680 */ u8 pad680[0x688 - 0x680];
    /* 0x688 */ s32 unk688;
    /* 0x68C */ s32 unk68C;
} State;

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

extern State D_8012FB70;
extern void func_00246108(Obj *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00245BAC(Obj *obj, CAI *ai, f32 angle, f32 dist);
extern void func_00243414(Obj *obj, void *ai, s32 state);

void func_0040B0B0(Obj *obj, CAI *ai) {
    Targets t;
    Targets t2;
    State *state;
    s32 i;

    func_00246108(obj, ai, &t, 0, 1);
    if (t.a.unk4 != 0) {
        func_00246108(obj, ai, &t2, 0, 1);
        func_00245BAC(obj, ai, 0.261799425f, t2.a.dist);
    }
    if (ai->animDone) {
        state = &D_8012FB70;
        for (i = 0; i < 6; i++) {
            func_00243414(state->others[i], state->others[i]->ai, 0);
        }
        func_00243414(obj, ai, 2);
    }
}
