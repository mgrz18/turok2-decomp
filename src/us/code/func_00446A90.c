/* Second-module copy of func_0040DA90 (src/us/code/func_0040DA90.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/boss.h"

#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Update callback of the second boss: runs the anim callback, counts the hit timer down, runs the phase's
 * update, then updates the slots, the three movers and the boss' health bar. */

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x24C - 0xD8];
    /* 0x24C */ s32 unk24C;
} LevelActor;

struct BlindOneState;

typedef struct {
    /* 0x00 */ void (*init)(struct BlindOneState *state);
    /* 0x04 */ void (*update)(struct BlindOneState *state);
} PhaseFuncs;

typedef struct Obj2 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj2;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ void (*callback)(void *obj, void *ai);
} Anim;

typedef struct {
    /* 0x000 */ u8 pad0[0x30];
    /* 0x030 */ Anim *anim;
    /* 0x034 */ u8 pad34[0x118 - 0x34];
    /* 0x118 */ f32 hitTimer;
} CAI;

extern f32 D_800B6D28; /* frame time */
extern PhaseFuncs D_0043232C[];
extern u8 D_801100F0[];
extern void func_00235510(void *slot);
extern void func_0040C850(BlindOneMover *mover);
extern s32 func_0040CB28(BlindOneState *state);
extern void func_0027AD88(void *hud, s32 value);

void func_00446A90(void *obj, CAI *ai) {
    BlindOneState *state = &D_80130200;
    s32 i;

    if (ai->anim != NULL && ai->anim->callback != NULL) {
        ai->anim->callback(obj, ai);
    }
    ai->hitTimer = MAX(ai->hitTimer - D_800B6D28, 0.0f);
    if (D_0043232C[state->phase].update != NULL) {
        D_0043232C[state->phase].update(state);
    }
    for (i = 0; i < state->slotCount; i++) {
        func_00235510(&state->slots[i]);
    }
    func_0040C850(&state->movers[0]);
    func_0040C850(&state->movers[1]);
    func_0040C850(&state->movers[2]);
    func_0027AD88(D_801100F0, func_0040CB28(state));
}
