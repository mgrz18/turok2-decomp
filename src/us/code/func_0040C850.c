#include "common.h"
#include "structs/boss.h"

/* One file: func_0040C850, func_0040CB28, func_0040CCC0 and func_0040CFC4.
 * func_0040CCC0's only literal, the string "Blind", sits at 0x800C08D4, 4 past
 * an 8-byte boundary, after func_0040C850's float; alone its object put the
 * string 4 bytes off. func_0040CFC4 starts at 0x40CFC4, which is not 16-aligned,
 * so it is no object start: the file runs on to it (its literal follows the
 * string's padding, at 0x800C08DC). Each part keeps its own types (suffixed
 * by address) from when it was matched alone. */

/* Updates one of the second boss' movers: when its timer runs out, plays the next masks of its sequence
 * (several in a row while bit 0x800 is set and no enabled actor fired). */

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ u8 ai[4];
    /* 0x144 */ s32 health;
    /* 0x148 */ s32 maxHealth;
    /* 0x14C */ u8 pad14C[0x24C - 0x14C];
    /* 0x24C */ s32 unk24C;
    /* 0x250 */ u8 pad250[0x25C - 0x250];
    /* 0x25C */ f32 unk25C;
} LevelActor_40C850;

struct BlindOneState;

typedef struct {
    /* 0x00 */ void (*init)(struct BlindOneState *state);
    /* 0x04 */ void (*update)(struct BlindOneState *state);
} PhaseFuncs_40C850;

typedef struct Obj2_40C850 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj2_40C850;

extern f32 D_800B6D28; /* frame time */
extern void func_00243414(LevelActor_40C850 *obj, void *ai, s32 state);

void func_0040C850(BlindOneMover *mover) {
    BlindOneState *state;
    LevelActor_40C850 *actor;
    s32 bits;
    s32 more;
    s32 i;
    s32 n;

    if (mover->seq != NULL) {
        mover->timer -= D_800B6D28;
        state = &D_80130200;
        if (mover->timer <= 0.0f) {
            mover->timer += mover->interval;
            i = 0;
            do {
                bits = mover->seq[mover->pos++];
                more = 0;
                if (bits & 0x800) {
                    more = 1;
                }
                for (i = 0; i < state->count1; i++) {
                    n = (i + mover->offset) % state->count1;
                    if ((bits >> (n)) & 1) {
                        if (state->alive1 == 0) {
                            more = 0;
                        }
                        actor = (LevelActor_40C850 *)state->actors1[i];
                        if (actor->health > 0) {
                            more = 0;
                            func_00243414(actor, actor->ai, 3);
                        }
                    }
                }
                for (i = 0; i < state->count2; i++) {
                    n = (i + mover->offset) % state->count2;
                    if ((bits >> (n + 4)) & 1) {
                        if (state->alive2 == 0) {
                            more = 0;
                        }
                        actor = (LevelActor_40C850 *)state->actors2[i];
                        if (actor->health != 0) {
                            more = 0;
                            if (actor->unk25C >= 1228.7999f) {
                                func_00243414(actor, actor->ai, 5);
                            } else {
                                func_00243414(actor, actor->ai, 4);
                            }
                        }
                    }
                }
                for (i = 0; i < state->count3; i++) {
                    n = (i + mover->offset) % state->count3;
                    if ((bits >> (n + 8)) & 1) {
                        if (state->alive3 == 0) {
                            more = 0;
                        }
                        actor = (LevelActor_40C850 *)state->actors3[i];
                        if (actor->health != 0) {
                            more = 0;
                            func_00243414(actor, actor->ai, 4);
                        }
                    }
                }
                if (mover->seq[mover->pos] == -1) {
                    mover->pos = 0;
                    mover->offset += mover->step;
                }
            } while (more);
        }
    }
}

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
} LevelActor_40CB28;

struct BlindOneState;

typedef struct {
    /* 0x00 */ void (*init)(struct BlindOneState *state);
    /* 0x04 */ void (*update)(struct BlindOneState *state);
} PhaseFuncs_40CB28;

typedef struct Obj2_40CB28 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj2_40CB28;

static __inline__ void sumHealth_40CB28(s32 count, LevelActor_40CB28 **actors, s32 *total, s32 *alive) {
    LevelActor_40CB28 *actor;
    s32 i;

    for (i = 0; i < count; i++) {
        actor = actors[i];
        *total += actor->maxHealth;
        if (actor->health != 0 && (actor->flags & 0x100)) {
            *alive += actor->maxHealth;
        }
    }
}

s32 func_0040CB28(BlindOneState *state) {
    s32 total = 0;
    s32 alive = 0;

    sumHealth_40CB28(state->count1, (LevelActor_40CB28 **)state->actors1, &total, &alive);
    sumHealth_40CB28(state->count2, (LevelActor_40CB28 **)state->actors2, &total, &alive);
    sumHealth_40CB28(state->count3, (LevelActor_40CB28 **)state->actors3, &total, &alive);
    if (total == 0) {
        return 0;
    }
    return alive * 100 / total;
}

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
} LevelActor_40CCC0;

struct BlindOneState;

typedef struct {
    /* 0x00 */ void (*init)(struct BlindOneState *state);
    /* 0x04 */ void (*update)(struct BlindOneState *state);
} PhaseFuncs_40CCC0;

typedef struct Obj2_40CCC0 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[4];
    /* 0x144 */ s32 health;
    /* 0x148 */ s32 maxHealth;
    /* 0x14C */ u8 pad14C[0x254 - 0x14C];
    /* 0x254 */ s32 slot;
} Obj2_40CCC0;

extern PhaseFuncs_40CCC0 D_0043240C[];
extern u8 D_800F7078[];
extern u8 D_801100F0[];
extern s32 func_00225F6C(void *container, s32 key0, s32 key1, s32 key2, LevelActor_40CCC0 **out, s32 max);
extern void func_002362E4(void *slot, s32 arg1);
extern void *func_00225EBC(void *level, s32 arg1, s32 id);
extern void func_0027AD24(void *hud, Obj2_40CCC0 *obj, char *name);
extern void func_00225D0C(void *level);

/* Steps the script: clears the movers and the actors' 0x24C, enters the next phase and runs its init. */
static __inline__ void nextPhase_40CCC0(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = NULL;
    state->movers[1].seq = NULL;
    state->movers[2].seq = NULL;
    for (i = 0; i < state->count1; i++) {
        ((LevelActor_40CCC0 *)state->actors1[i])->unk24C = 0;
    }
    for (i = 0; i < state->count2; i++) {
        ((LevelActor_40CCC0 *)state->actors2[i])->unk24C = 0;
    }
    for (i = 0; i < state->count3; i++) {
        ((LevelActor_40CCC0 *)state->actors3[i])->unk24C = 0;
    }
    state->phase = phase;
    if (D_0043240C[phase].init != NULL) {
        D_0043240C[phase].init(state);
    }
}

void func_0040CCC0(BlindOneState *state) {
    Obj2_40CCC0 *obj;
    LevelActor_40CCC0 *actor;
    s32 i;

    state->count1 = func_00225F6C(D_800F7078, 0x454, 1, -1, (LevelActor_40CCC0 **)state->actors1, 4);
    state->count2 = func_00225F6C(D_800F7078, 0x455, 1, -1, (LevelActor_40CCC0 **)state->actors2, 4);
    state->count3 = func_00225F6C(D_800F7078, 0x456, 1, -1, (LevelActor_40CCC0 **)state->actors3, 3);
    obj = (Obj2_40CCC0 *)state->obj;
    state->slotCount = 0;
    obj->health = obj->maxHealth = 0x3200;
    obj->slot = state->slotCount++;
    for (i = 0; i < state->count1; i++) {
        actor = (LevelActor_40CCC0 *)state->actors1[i];
        actor->health = 0;
        actor->maxHealth = 0x6400;
        actor->index = i;
        actor->slot = state->slotCount++;
    }
    for (i = 0; i < state->count2; i++) {
        actor = (LevelActor_40CCC0 *)state->actors2[i];
        actor->health = 0;
        actor->maxHealth = 0x5000;
        actor->index = i;
        actor->slot = state->slotCount++;
    }
    for (i = 0; i < state->count3; i++) {
        actor = (LevelActor_40CCC0 *)state->actors3[i];
        actor->health = 0;
        actor->maxHealth = 0x6400;
        actor->index = i;
        actor->slot = state->slotCount++;
    }
    for (i = 0; i < state->slotCount; i++) {
        func_002362E4(&state->slots[i], 0);
    }
    state->minionIntel = func_00225EBC(D_800F7078, 1, 0x1CE9);
    state->minionIntel2 = func_00225EBC(D_800F7078, 1, 0x1CEA);
    state->toSpawn = 0;
    state->minionsAlive = 0;
    state->maxMinions = 0;
    state->alive1 = 0;
    state->alive2 = 0;
    state->alive3 = 0;
    func_0027AD24(D_801100F0, (Obj2_40CCC0 *)state->obj, "Blind");
    func_00225D0C(D_800F7078);
    nextPhase_40CCC0(state);
}

/* Second boss phase update: steps the script to the next phase once the player comes within 716.8 of the
 * origin. */

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x24C - 0xD8];
    /* 0x24C */ s32 unk24C;
} LevelActor_40CFC4;

struct BlindOneState;

typedef struct {
    /* 0x00 */ void (*init)(struct BlindOneState *state);
    /* 0x04 */ void (*update)(struct BlindOneState *state);
} PhaseFuncs_40CFC4;

typedef struct {
    f32 x, y, z;
} Vec3f_40CFC4;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f_40CFC4 pos;
} Player_40CFC4;

extern Player_40CFC4 *D_80119890; /* current player */
extern f32 func_00298470(f32 x); /* sqrt */

/* Steps the script: clears the movers and the actors' 0x24C, enters the next phase and runs its init. */
static __inline__ void nextPhase_40CFC4(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = 0;
    state->movers[1].seq = 0;
    state->movers[2].seq = 0;
    for (i = 0; i < state->count1; i++) {
        ((LevelActor_40CFC4 *)state->actors1[i])->unk24C = 0;
    }
    for (i = 0; i < state->count2; i++) {
        ((LevelActor_40CFC4 *)state->actors2[i])->unk24C = 0;
    }
    for (i = 0; i < state->count3; i++) {
        ((LevelActor_40CFC4 *)state->actors3[i])->unk24C = 0;
    }
    state->phase = phase;
    if (D_0043240C[phase].init != NULL) {
        D_0043240C[phase].init((void *) state);
    }
}

void func_0040CFC4(BlindOneState *state) {
    Player_40CFC4 *player = D_80119890;

    if (player != NULL &&
        func_00298470(player->pos.x * player->pos.x + player->pos.y * player->pos.y + player->pos.z * player->pos.z) <
            716.8f) {
        nextPhase_40CFC4(state);
    }
}
