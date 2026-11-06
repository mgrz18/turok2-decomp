/* Second-module twin of MotherBoss_Mode01_FlashSpotsThenIdle (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* AI state: once the anim is done, reset every active slot of the module state, then go to anim state 2. */

extern s32 gMotherBoss_activeMask; /* local type: the field's real type (or the member path) changes the ROM's code here */ /* active slot mask */
extern void ScriptAnimator_SetScript(void *dst, void *src);
extern void CAI__SetMode(void *obj, CAI *ai, s32 state);

void MotherBoss_Mode01_FlashSpotsThenIdle_2(void *obj, CAI *ai) {
    MotherBossState *state;
    s32 mask;
    s32 i;

    if (ai->m_AnimRepeatedCount > 0) {
        mask = gMotherBoss_activeMask;
        state = &gMotherBoss;
        for (i = 0; i < 15; i++) {
            if ((mask >> i) & 1) {
                ScriptAnimator_SetScript(&state->slots[i].anim, sMotherBossSpotFlashScript_2);
            }
        }
        CAI__SetMode(obj, ai, 2);
    }
}
