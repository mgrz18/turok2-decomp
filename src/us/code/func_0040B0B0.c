#include "common.h"
#include "structs/boss.h"

/* AI state: turn toward the target (when there is one); once the anim is done, sends the module's six other
 * objects to anim state 0 and goes to anim state 2. */

typedef struct Obj {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj;

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

extern void func_00246108(Obj *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00245BAC(Obj *obj, CAI *ai, f32 angle, f32 dist);
extern void func_00243414(Obj *obj, void *ai, s32 state);

void func_0040B0B0(Obj *obj, CAI *ai) {
    Targets t;
    Targets t2;
    MotherBossState *state;
    s32 i;

    func_00246108(obj, ai, &t, 0, 1);
    if (t.a.unk4 != 0) {
        func_00246108(obj, ai, &t2, 0, 1);
        func_00245BAC(obj, ai, 0.261799425f, t2.a.dist);
    }
    if (ai->animDone) {
        state = &D_8012FB70;
        for (i = 0; i < 6; i++) {
            func_00243414((Obj *)state->parts[i], ((Obj *)state->parts[i])->ai, 0);
        }
        func_00243414(obj, ai, 2);
    }
}
