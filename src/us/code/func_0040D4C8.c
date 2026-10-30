#include "common.h"

/* Second boss module: plays sound 0x284F, sends the module object to anim state 1 and steps the script to
 * the next phase. */

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

extern PhaseFuncs D_0043240C[];
extern void func_00275624(s32 sound);
extern void func_00243414(Obj2 *obj, void *ai, s32 state);

/* Steps the script: clears the movers and the actors' 0x24C, enters the next phase and runs its init. */
static __inline__ void nextPhase(State2 *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].unk0 = 0;
    state->movers[1].unk0 = 0;
    state->movers[2].unk0 = 0;
    for (i = 0; i < state->count1; i++) {
        state->actors1[i]->unk24C = 0;
    }
    for (i = 0; i < state->count2; i++) {
        state->actors2[i]->unk24C = 0;
    }
    for (i = 0; i < state->count3; i++) {
        state->actors3[i]->unk24C = 0;
    }
    state->phase = phase;
    if (D_0043240C[phase].init != NULL) {
        D_0043240C[phase].init(state);
    }
}

void func_0040D4C8(State2 *state) {
    func_00275624(0x284F);
    func_00243414(state->obj, state->obj->ai, 1);
    nextPhase(state);
}
