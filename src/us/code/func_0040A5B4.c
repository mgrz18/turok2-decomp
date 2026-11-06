#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Module phase 1 setup: timers, the anim for the phase, slot values; unless ai->m_Mode is 9, resets the
 * phase's active slots and runs MotherBoss_DropPickupsAtSpawnPoints three times. */

extern s32 sMotherBossPhaseVisBits[];
extern s32 sMotherBossPhaseActiveMasks[]; /* active slot mask per phase */
extern s32 sMotherBossPhaseLiveMasks[];
extern u8 sMotherBossSpotResetScript[];
extern u8 sMotherBossPhase1SpawnDrops[];
extern void ScriptAnimator_SetScript(void *dst, void *src);
extern void MotherBoss_DropPickupsAtSpawnPoints(s32 arg0, void *arg1);

static __inline__ void resetSlots(s32 mask, void *src) {
    MotherBossState *state = &gMotherBoss;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            ScriptAnimator_SetScript(&state->slots[i].anim, src);
        }
    }
}

void MotherBoss_SetupPhase1(CGameObjectInstance *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 mask;
    s32 i;

    state->minRadius = 0x600;
    state->maxRadius = 0x900;
    state->phase = 1;
    state->counter = 0;
    obj->m_dwHotPointFlags &= ~0x18000000;
    ai->m_VisBits = sMotherBossPhaseVisBits[state->phase];
    state->liveMask = sMotherBossPhaseLiveMasks[state->phase];
    mask = sMotherBossPhaseActiveMasks[state->phase];
    state->slots[1].health = state->slots[5].health = 0x9600;
    state->totalHealth = state->health = 0x12C00;
    state->activeMask = mask;
    if (ai->m_Mode != 9) {
        resetSlots(state->activeMask, sMotherBossSpotResetScript);
        for (i = 0; i < 3; i++) {
            MotherBoss_DropPickupsAtSpawnPoints(0, sMotherBossPhase1SpawnDrops);
        }
    }
}
