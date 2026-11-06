#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* AI state: once the anim is done, reset every active slot of the module state, then go to anim state 0x16. */

extern s32 gMotherBoss_activeMask; /* local type: the field's real type (or the member path) changes the ROM's code here */ /* active slot mask */
extern u8 sMotherBossSpotFlashScript[];
extern void ScriptAnimator_SetScript(void *dst, void *src);
extern void CAI__SetMode(void *obj, CAI *ai, s32 state);

void MotherBoss_Mode21_FlashSpotsThenIdle(void *obj, CAI *ai) {
    MotherBossState *state;
    s32 mask;
    s32 i;

    if (ai->m_AnimRepeatedCount != 0) {
        mask = gMotherBoss_activeMask;
        state = &gMotherBoss;
        for (i = 0; i < 15; i++) {
            if ((mask >> i) & 1) {
                ScriptAnimator_SetScript(&state->slots[i].anim, sMotherBossSpotFlashScript);
            }
        }
        CAI__SetMode(obj, ai, 0x16);
    }
}
