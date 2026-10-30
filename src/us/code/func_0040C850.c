#include "common.h"

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
    /* 0x500 */ u8 pad500[0x50C - 0x500];
    /* 0x50C */ s32 enabled1;
    /* 0x510 */ s32 enabled2;
    /* 0x514 */ s32 enabled3;
    /* 0x518 */ Mover movers[3];
} State2;

extern State2 D_80130200;
extern f32 D_800B6D28; /* frame time */
extern void func_00243414(LevelActor *obj, void *ai, s32 state);

void func_0040C850(Mover *mover) {
    State2 *state;
    LevelActor *actor;
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
                        if (state->enabled1 == 0) {
                            more = 0;
                        }
                        actor = state->actors1[i];
                        if (actor->health > 0) {
                            more = 0;
                            func_00243414(actor, actor->ai, 3);
                        }
                    }
                }
                for (i = 0; i < state->count2; i++) {
                    n = (i + mover->offset) % state->count2;
                    if ((bits >> (n + 4)) & 1) {
                        if (state->enabled2 == 0) {
                            more = 0;
                        }
                        actor = state->actors2[i];
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
                        if (state->enabled3 == 0) {
                            more = 0;
                        }
                        actor = state->actors3[i];
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
