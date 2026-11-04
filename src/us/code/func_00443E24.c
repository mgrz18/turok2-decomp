/* Second-module twin of func_0040AE24 (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
#include "structs/boss.h"

/* Module phase 0 setup: timers, the anim for the phase, slot values, and reset of the phase's active slots
 * when ai->unk34 is set. */

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

extern s32 D_00431D04[];
extern s32 D_00431D10[]; /* active slot mask per phase */
extern s32 D_00431D1C[];
extern u8 D_00431D28[];
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

void func_00443E24(void *obj, CAI *ai) {
    MotherBossState *state = &D_8012FB70;
    s32 mask;

    state->minRadius = 0x200;
    state->maxRadius = 0x600;
    state->phase = 0;
    state->counter = 0;
    ai->unkC = D_00431D04[state->phase];
    state->liveMask = D_00431D1C[state->phase];
    mask = D_00431D10[state->phase];
    state->slots[2].health = state->slots[3].health = state->slots[4].health = 0x2800;
    state->slots[6].health = state->slots[7].health = state->slots[8].health = 0x2800;
    state->totalHealth = state->health = 0xF000;
    state->activeMask = mask;
    if (ai->unk34 != 0) {
        resetSlots(state->activeMask, D_00431D28);
    }
}
