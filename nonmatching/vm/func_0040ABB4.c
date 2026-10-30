/* NON-MATCHING: 71 of 93 words differ: same logic as func_0040B644 plus the blend/fade resets, but the ROM
 * reaches phase/counter/unk664/activeMask through their own symbols (D_801301CC/EC/D4/D0, reloading phase
 * after the ai store) and rematerialises the state address after the first loop; not reproduced yet. */
#include "common.h"

/* Module phase 2 entry: resets the actors' blend values and the fade timer, sets up phase 2 (timers, anim,
 * slot values), resets its active slots unless ai->unk34 is 0x13, and queues func_0040C7B4. */

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
    /* 0x250 */ f32 blend2;
    /* 0x254 */ f32 blend1;
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
extern f32 D_800C1900; /* fade timer (func_0040A25C) */
extern s32 D_800C1904;
extern s32 D_00431DE4[];
extern s32 D_00431DF0[]; /* active slot mask per phase */
extern s32 D_00431DFC[];
extern u8 D_00431E08[];
extern void func_00236314(void *dst, void *src);
extern void func_0040C7B4(void);
extern s32 func_00283FF8(s32 id, void *arg1, void *arg2);

static __inline__ void resetSlots(s32 mask, void *src) {
    State *state = &D_8012FB70;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            func_00236314(state->slots[i].data, src);
        }
    }
}

void func_0040ABB4(void *obj, CAI *ai) {
    State *state = &D_8012FB70;
    LevelActor *actor;
    s32 mask;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->blend1 = 1.0f;
        actor->blend2 = 0.0f;
    }
    state->unk688 = 0x200;
    state->phase = 2;
    state->unk68C = 0x600;
    state->counter = 0;
    ai->unkC = D_00431DE4[state->phase];
    state->unk664 = D_00431DFC[state->phase];
    mask = D_00431DF0[state->phase];
    D_800C1900 = 0.0f;
    D_800C1904 = 0;
    state->unk674 = state->unk678 = state->slots[13].unk0 = 0x1F400;
    state->activeMask = mask;
    if (ai->unk34 != 0x13) {
        resetSlots(state->activeMask, D_00431E08);
    }
    func_00283FF8(0xDC, func_0040C7B4, NULL);
}
