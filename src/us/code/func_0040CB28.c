#include "common.h"

/* Returns the second boss' remaining health in percent, summed over its three groups of actors (an actor
 * counts while it has health and flag 0x100). */

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x144 - 0xD8];
    /* 0x144 */ s32 health;
    /* 0x148 */ s32 maxHealth;
    /* 0x14C */ u8 pad14C[0x24C - 0x14C];
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

static __inline__ void sumHealth(s32 count, LevelActor **actors, s32 *total, s32 *alive) {
    LevelActor *actor;
    s32 i;

    for (i = 0; i < count; i++) {
        actor = actors[i];
        *total += actor->maxHealth;
        if (actor->health != 0 && (actor->flags & 0x100)) {
            *alive += actor->maxHealth;
        }
    }
}

s32 func_0040CB28(State2 *state) {
    s32 total = 0;
    s32 alive = 0;

    sumHealth(state->count1, state->actors1, &total, &alive);
    sumHealth(state->count2, state->actors2, &total, &alive);
    sumHealth(state->count3, state->actors3, &total, &alive);
    if (total == 0) {
        return 0;
    }
    return alive * 100 / total;
}
