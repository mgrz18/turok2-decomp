/* NON-MATCHING: 152 of 174 words differ, but only by a one-instruction shift: the ROM keeps the first random
 * value in $f20 and copies it into $f21 (mov.s) before the spawn checks; here GCC puts it in $f21 directly.
 * Everything after the copy is identical. */
#include "common.h"

/* Second boss phase update: spawns a minion at a random point 1228.8-1536 around the boss while the phase has
 * some left (and fewer than its maximum are alive); once none are left and fewer than 5 are alive, steps the
 * script to the next phase. */

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

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct Obj2 {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x140 - 0x10];
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
    /* 0x500 */ s32 toSpawn;
    /* 0x504 */ s32 alive;
    /* 0x508 */ s32 maxAlive;
    /* 0x50C */ s32 enabled1;
    /* 0x510 */ s32 enabled2;
    /* 0x514 */ s32 enabled3;
    /* 0x518 */ Mover movers[3];
} State2;

extern PhaseFuncs D_0043240C[];
extern u8 D_8010ABA0[];
extern f32 func_002119FC(f32 min, f32 max);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern void func_002688C8(Obj2 *obj, Vec3f pos, Vec3f *outPos, s32 *outRegion, s32 x, s32 y);
extern void *func_00228108(void *world, s32 model, s32 type, void *intelligence, s32 region, Vec3f pos, f32 rotY,
                           Vec3f vel, Vec3f scale, s32 *owner);

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
    if (D_0043240C[phase].init != NULL) {
        D_0043240C[phase].init(state);
    }
}

/* Spawns one minion around the boss when *toSpawn allows it and fewer than maxAlive are alive. */
static __inline__ void spawnMinion(State2 *state, s32 *toSpawn, s32 *alive, f32 dist, s32 maxAlive,
                                   void *intelligence, Obj2 *obj, f32 angle) {
    Vec3f pos;
    Vec3f vel;
    Vec3f scale;
    s32 region;

    if (toSpawn != NULL && *toSpawn == 0) {
        return;
    }
    if (alive != NULL && maxAlive != -1 && *alive >= maxAlive) {
        return;
    }
    vel.z = vel.y = vel.x = 0.0f;
    scale.z = scale.y = scale.x = 0.5f;
    pos.x = obj->pos.x + dist * func_002982F0(angle);
    pos.y = 0.0f;
    pos.z = obj->pos.z + dist * func_002974C0(angle);
    func_002688C8(state->obj, pos, &pos, &region, 0, 0);
    if (func_00228108(D_8010ABA0, -1, 0x1FF, intelligence, region, pos, angle, vel, scale, alive) && toSpawn != NULL) {
        (*toSpawn)--;
    }
}

void func_0040D0F0(State2 *state) {
    f32 dist;

    dist = func_002119FC(1228.7999f, 1536.0f);
    spawnMinion(state, &state->toSpawn, &state->alive, dist, state->maxAlive, state->unk4F0, state->obj,
                func_002119FC(-3.141593f, 3.141593f));
    if (state->toSpawn == 0 && state->alive < 5) {
        nextPhase(state);
    }
}
