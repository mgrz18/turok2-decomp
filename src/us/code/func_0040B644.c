#include "common.h"
#include "structs/boss.h"

/* Module phase 2 setup: timers, the anim for the phase, slot values; unless ai->unk34 is 0x13, resets the
 * phase's active slots. */

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
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x34 - 0x10];
    /* 0x34 */ s8 unk34;
} CAI;

extern s32 D_00431DE4[];
extern s32 D_00431DF0[]; /* active slot mask per phase */
extern s32 D_00431DFC[];
extern u8 D_00431E08[];
extern void func_00236314(void *dst, void *src);

static __inline__ void resetSlots(s32 mask, void *src) {
    MotherBossState *state = &D_8012FB70;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            func_00236314(state->slots[i].anim, src);
        }
    }
}

void func_0040B644(void *obj, CAI *ai) {
    MotherBossState *state = &D_8012FB70;
    s32 mask;

    state->minRadius = 0x200;
    state->maxRadius = 0x600;
    state->phase = 2;
    state->counter = 0;
    ai->unkC = D_00431DE4[state->phase];
    state->liveMask = D_00431DFC[state->phase];
    mask = D_00431DF0[state->phase];
    state->totalHealth = state->health = state->slots[13].health = 0x1F400;
    state->activeMask = mask;
    if (ai->unk34 != 0x13) {
        resetSlots(state->activeMask, D_00431E08);
    }
}
