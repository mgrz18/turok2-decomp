#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Module phase 2 setup: timers, the anim for the phase, slot values; unless ai->m_Mode is 0x13, resets the
 * phase's active slots. */


extern void ScriptAnimator_SetScript(void *dst, void *src);

static __inline__ void resetSlots(s32 mask, void *src) {
    MotherBossState *state = &gMotherBoss;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            ScriptAnimator_SetScript(&state->slots[i].anim, src);
        }
    }
}

void MotherBoss_SetupPhase2(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 mask;

    state->minRadius = 0x200;
    state->maxRadius = 0x600;
    state->phase = 2;
    state->counter = 0;
    ai->m_VisBits = sMotherBossPhaseVisBits[state->phase];
    state->liveMask = sMotherBossPhaseLiveMasks[state->phase];
    mask = sMotherBossPhaseActiveMasks[state->phase];
    state->totalHealth = state->health = state->slots[13].health = 0x1F400;
    state->activeMask = mask;
    if (ai->m_Mode != 0x13) {
        resetSlots(state->activeMask, sMotherBossSpotResetScript);
    }
}
