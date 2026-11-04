#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Module phase 1 setup: timers, the anim for the phase, slot values; unless ai->m_Mode is 9, resets the
 * phase's active slots and runs func_00409500 three times. */


typedef struct {
    /* 0x000 */ u8 pad0[0x27C];
    /* 0x27C */ s32 flags;
} Obj;

extern s32 D_00431DE4[];
extern s32 D_00431DF0[]; /* active slot mask per phase */
extern s32 D_00431DFC[];
extern u8 D_00431E08[];
extern u8 D_00431DDC[];
extern void func_00236314(void *dst, void *src);
extern void func_00409500(s32 arg0, void *arg1);

static __inline__ void resetSlots(s32 mask, void *src) {
    MotherBossState *state = &D_8012FB70;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            func_00236314(&state->slots[i].anim, src);
        }
    }
}

void func_0040A5B4(Obj *obj, CAI *ai) {
    MotherBossState *state = &D_8012FB70;
    s32 mask;
    s32 i;

    state->minRadius = 0x600;
    state->maxRadius = 0x900;
    state->phase = 1;
    state->counter = 0;
    obj->flags &= ~0x18000000;
    ai->m_VisBits = D_00431DE4[state->phase];
    state->liveMask = D_00431DFC[state->phase];
    mask = D_00431DF0[state->phase];
    state->slots[1].health = state->slots[5].health = 0x9600;
    state->totalHealth = state->health = 0x12C00;
    state->activeMask = mask;
    if (ai->m_Mode != 9) {
        resetSlots(state->activeMask, D_00431E08);
        for (i = 0; i < 3; i++) {
            func_00409500(0, D_00431DDC);
        }
    }
}
