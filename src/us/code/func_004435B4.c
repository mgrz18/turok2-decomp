/* Second-module twin of func_0040A5B4 (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Module phase 1 setup: timers, the anim for the phase, slot values; unless ai->m_Mode is 9, resets the
 * phase's active slots and runs func_00409500 three times. */

extern s32 D_00431D04[];
extern s32 D_00431D10[]; /* active slot mask per phase */
extern s32 D_00431D1C[];
extern u8 D_00431D28[];
extern u8 D_00431CFC[];
extern void func_00236314(void *dst, void *src);
extern void func_00409500(s32 arg0, void *arg1);

static __inline__ void resetSlots(s32 mask, void *src) {
    MotherBossState *state = &D_8012FB70;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            func_00236314(state->slots[i].anim, src);
        }
    }
}

void func_004435B4(CGameObjectInstance *obj, CAI *ai) {
    MotherBossState *state = &D_8012FB70;
    s32 mask;
    s32 i;

    state->minRadius = 0x600;
    state->maxRadius = 0x900;
    state->phase = 1;
    state->counter = 0;
    obj->m_dwHotPointFlags &= ~0x18000000;
    ai->m_VisBits = D_00431D04[state->phase];
    state->liveMask = D_00431D1C[state->phase];
    mask = D_00431D10[state->phase];
    state->slots[1].health = state->slots[5].health = 0x9600;
    state->totalHealth = state->health = 0x12C00;
    state->activeMask = mask;
    if (ai->m_Mode != 9) {
        resetSlots(state->activeMask, D_00431D28);
        for (i = 0; i < 3; i++) {
            func_00409500(0, D_00431CFC);
        }
    }
}
