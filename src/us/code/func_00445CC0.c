/* Second-module copy of func_0040CCC0 (in src/us/code/func_0040C850.c), with its data addresses 0xE0 lower. */
#include "common.h"

/* Second boss setup: collects its three groups of actors (types 0x454-0x456), sets their health and slots,
 * finds its two level objects, shows its health bar ("Blind") and enters the first phase of its script. */

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ u8 ai[4];
    /* 0x144 */ s32 health;
    /* 0x148 */ s32 maxHealth;
    /* 0x14C */ u8 pad14C[0x24C - 0x14C];
    /* 0x24C */ s32 unk24C;
    /* 0x250 */ s32 index;
    /* 0x254 */ s32 slot;
    /* 0x258 */ u8 pad258[0x25C - 0x258];
    /* 0x25C */ f32 unk25C;
} LevelActor;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} ScriptStep;

/* Plays a sequence of bit masks: every interval seconds, each set bit sends the matching actor (bits 0-3:
 * group 1, 4-7: group 2, 8-11: group 3, rotated by offset) to an attack state. */
typedef struct {
    /* 0x00 */ s32 *seq;
    /* 0x04 */ s32 pos;
    /* 0x08 */ f32 interval;
    /* 0x0C */ f32 timer;
    /* 0x10 */ s32 step;
    /* 0x14 */ s32 offset;
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
    /* 0x140 */ u8 ai[4];
    /* 0x144 */ s32 health;
    /* 0x148 */ s32 maxHealth;
    /* 0x14C */ u8 pad14C[0x254 - 0x14C];
    /* 0x254 */ s32 slot;
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
    /* 0x4F0 */ void *unk4F0;
    /* 0x4F4 */ void *unk4F4;
    /* 0x4F8 */ ScriptStep *script;
    /* 0x4FC */ s32 phase;
    /* 0x500 */ s32 unk500;
    /* 0x504 */ s32 unk504;
    /* 0x508 */ s32 unk508;
    /* 0x50C */ s32 enabled1;
    /* 0x510 */ s32 enabled2;
    /* 0x514 */ s32 enabled3;
    /* 0x518 */ Mover movers[3];
} State2;

extern PhaseFuncs D_0043232C[];
extern u8 D_800F7078[];
extern u8 D_801100F0[];
extern s32 func_00225F6C(void *container, s32 key0, s32 key1, s32 key2, LevelActor **out, s32 max);
extern void func_002362E4(void *slot, s32 arg1);
extern void *func_00225EBC(void *level, s32 arg1, s32 id);
extern void func_0027AD24(void *hud, Obj2 *obj, char *name);
extern void func_00225D0C(void *level);

/* Steps the script: clears the movers and the actors' 0x24C, enters the next phase and runs its init. */
static __inline__ void nextPhase(State2 *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = NULL;
    state->movers[1].seq = NULL;
    state->movers[2].seq = NULL;
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
    if (D_0043232C[phase].init != NULL) {
        D_0043232C[phase].init(state);
    }
}

void func_00445CC0(State2 *state) {
    Obj2 *obj;
    LevelActor *actor;
    s32 i;

    state->count1 = func_00225F6C(D_800F7078, 0x454, 1, -1, state->actors1, 4);
    state->count2 = func_00225F6C(D_800F7078, 0x455, 1, -1, state->actors2, 4);
    state->count3 = func_00225F6C(D_800F7078, 0x456, 1, -1, state->actors3, 3);
    obj = state->obj;
    state->slotCount = 0;
    obj->health = obj->maxHealth = 0x3200;
    obj->slot = state->slotCount++;
    for (i = 0; i < state->count1; i++) {
        actor = state->actors1[i];
        actor->health = 0;
        actor->maxHealth = 0x6400;
        actor->index = i;
        actor->slot = state->slotCount++;
    }
    for (i = 0; i < state->count2; i++) {
        actor = state->actors2[i];
        actor->health = 0;
        actor->maxHealth = 0x5000;
        actor->index = i;
        actor->slot = state->slotCount++;
    }
    for (i = 0; i < state->count3; i++) {
        actor = state->actors3[i];
        actor->health = 0;
        actor->maxHealth = 0x6400;
        actor->index = i;
        actor->slot = state->slotCount++;
    }
    for (i = 0; i < state->slotCount; i++) {
        func_002362E4(&state->slots[i], 0);
    }
    state->unk4F0 = func_00225EBC(D_800F7078, 1, 0x1CE9);
    state->unk4F4 = func_00225EBC(D_800F7078, 1, 0x1CEA);
    state->unk500 = 0;
    state->unk504 = 0;
    state->unk508 = 0;
    state->enabled1 = 0;
    state->enabled2 = 0;
    state->enabled3 = 0;
    func_0027AD24(D_801100F0, state->obj, "Blind");
    func_00225D0C(D_800F7078);
    nextPhase(state);
}


