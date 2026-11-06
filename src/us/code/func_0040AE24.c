#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Module phase 0 setup: timers, the anim for the phase, slot values, and reset of the phase's active slots
 * when ai->m_Mode is set. */


extern s32 sMotherBossPhaseVisBits[];
extern s32 sMotherBossPhaseActiveMasks[]; /* active slot mask per phase */
extern s32 sMotherBossPhaseLiveMasks[];
extern u8 sMotherBossSpotResetScript[];
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

void MotherBoss_SetupPhase0(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 mask;

    state->minRadius = 0x200;
    state->maxRadius = 0x600;
    state->phase = 0;
    state->counter = 0;
    ai->m_VisBits = sMotherBossPhaseVisBits[state->phase];
    state->liveMask = sMotherBossPhaseLiveMasks[state->phase];
    mask = sMotherBossPhaseActiveMasks[state->phase];
    state->slots[2].health = state->slots[3].health = state->slots[4].health = 0x2800;
    state->slots[6].health = state->slots[7].health = state->slots[8].health = 0x2800;
    state->totalHealth = state->health = 0xF000;
    state->activeMask = mask;
    if (ai->m_Mode != 0) {
        resetSlots(state->activeMask, sMotherBossSpotResetScript);
    }
}
