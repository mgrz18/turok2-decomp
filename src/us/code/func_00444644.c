/* Second-module twin of MotherBoss_SetupPhase2 (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Module phase 2 setup: timers, the anim for the phase, slot values; unless ai->m_Mode is 0x13, resets the
 * phase's active slots. */


extern s32 D_00431D04[];
extern s32 D_00431D10[]; /* active slot mask per phase */
extern s32 D_00431D1C[];
extern u8 D_00431D28[];
extern void ScriptAnimator_SetScript(void *dst, void *src);

static __inline__ void resetSlots(s32 mask, void *src) {
    MotherBossState *state = &D_8012FB70;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            ScriptAnimator_SetScript(&state->slots[i].anim, src);
        }
    }
}

void MotherBoss_SetupPhase2_2(void *obj, CAI *ai) {
    MotherBossState *state = &D_8012FB70;
    s32 mask;

    state->minRadius = 0x200;
    state->maxRadius = 0x600;
    state->phase = 2;
    state->counter = 0;
    ai->m_VisBits = D_00431D04[state->phase];
    state->liveMask = D_00431D1C[state->phase];
    mask = D_00431D10[state->phase];
    state->totalHealth = state->health = state->slots[13].health = 0x1F400;
    state->activeMask = mask;
    if (ai->m_Mode != 0x13) {
        resetSlots(state->activeMask, D_00431D28);
    }
}
