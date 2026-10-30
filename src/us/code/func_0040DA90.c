#include "common.h"

#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Update callback of the second boss: runs the anim callback, counts the hit timer down, runs the phase's
 * update, then updates the slots, the three movers and the boss' health bar. */

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x24C - 0xD8];
    /* 0x24C */ s32 unk24C;
} LevelActor;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} ScriptStep;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x18 - 0x4];
} Mover;

typedef struct {
    /* 0x00 */ u8 data[0x64];
} Slot2;

struct State2;

typedef struct {
    /* 0x00 */ void (*init)(struct State2 *state);
    /* 0x04 */ void (*update)(struct State2 *state);
} PhaseFuncs;

typedef struct Obj2 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj2;

typedef struct State2 {
    /* 0x000 */ Obj2 *obj;
    /* 0x004 */ s32 count1;
    /* 0x008 */ LevelActor *actors1[4];
    /* 0x018 */ s32 count2;
    /* 0x01C */ LevelActor *actors2[4];
    /* 0x02C */ s32 count3;
    /* 0x030 */ LevelActor *actors3[3];
    /* 0x03C */ Slot2 slots[12];
    /* 0x4EC */ s32 slotCount;
    /* 0x4F0 */ u8 pad4F0[0x4F8 - 0x4F0];
    /* 0x4F8 */ ScriptStep *script;
    /* 0x4FC */ s32 phase;
    /* 0x500 */ u8 pad500[0x518 - 0x500];
    /* 0x518 */ Mover movers[3];
} State2;

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

extern State2 D_80130200;
extern f32 D_800B6D28; /* frame time */
extern PhaseFuncs D_0043240C[];
extern u8 D_801100F0[];
extern void func_00235510(void *slot);
extern void func_0040C850(Mover *mover);
extern s32 func_0040CB28(State2 *state);
extern void func_0027AD88(void *hud, s32 value);

void func_0040DA90(void *obj, CAI *ai) {
    State2 *state = &D_80130200;
    s32 i;

    if (ai->anim != NULL && ai->anim->callback != NULL) {
        ai->anim->callback(obj, ai);
    }
    ai->hitTimer = MAX(ai->hitTimer - D_800B6D28, 0.0f);
    if (D_0043240C[state->phase].update != NULL) {
        D_0043240C[state->phase].update(state);
    }
    for (i = 0; i < state->slotCount; i++) {
        func_00235510(&state->slots[i]);
    }
    func_0040C850(&state->movers[0]);
    func_0040C850(&state->movers[1]);
    func_0040C850(&state->movers[2]);
    func_0027AD88(D_801100F0, func_0040CB28(state));
}
