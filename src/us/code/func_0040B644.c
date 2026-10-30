#include "common.h"

/* Module phase 2 setup: timers, the anim for the phase, slot values; unless ai->unk34 is 0x13, resets the
 * phase's active slots. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

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
    /* 0x000 */ void *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ LevelActor *actors[4];
    /* 0x018 */ Slot slots[15];
    /* 0x630 */ u8 pad630[0x65C - 0x630];
    /* 0x65C */ s32 phase;
    /* 0x660 */ s32 activeMask;
    /* 0x664 */ s32 unk664;
    /* 0x668 */ u8 pad668[0x674 - 0x668];
    /* 0x674 */ s32 unk674;
    /* 0x678 */ s32 unk678;
    /* 0x67C */ s32 counter;
    /* 0x680 */ u8 pad680[0x688 - 0x680];
    /* 0x688 */ s32 unk688;
    /* 0x68C */ s32 unk68C;
} State;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x34 - 0x10];
    /* 0x34 */ s8 unk34;
} CAI;

extern State D_8012FB70;
extern s32 D_00431DE4[];
extern s32 D_00431DF0[]; /* active slot mask per phase */
extern s32 D_00431DFC[];
extern u8 D_00431E08[];
extern void func_00236314(void *dst, void *src);

static __inline__ void resetSlots(s32 mask, void *src) {
    State *state = &D_8012FB70;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            func_00236314(state->slots[i].data, src);
        }
    }
}

void func_0040B644(void *obj, CAI *ai) {
    State *state = &D_8012FB70;
    s32 mask;

    state->unk688 = 0x200;
    state->unk68C = 0x600;
    state->phase = 2;
    state->counter = 0;
    ai->unkC = D_00431DE4[state->phase];
    state->unk664 = D_00431DFC[state->phase];
    mask = D_00431DF0[state->phase];
    state->unk674 = state->unk678 = state->slots[13].unk0 = 0x1F400;
    state->activeMask = mask;
    if (ai->unk34 != 0x13) {
        resetSlots(state->activeMask, D_00431E08);
    }
}
