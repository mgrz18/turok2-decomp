#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Resets every active slot of the module state with sMotherBossSpotFlashScript. */

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

void MotherBoss_FlashActiveSpots(void) {
    MotherBossState *state = &gMotherBoss;

    resetSlots(state->activeMask, sMotherBossSpotFlashScript);
}
