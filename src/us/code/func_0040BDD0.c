#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Resets the slots of the module state selected by mask with src. */

extern void ScriptAnimator_SetScript(void *dst, void *src);

void MotherBoss_StartSpotAnims(void *obj, void *src, s32 mask) {
    MotherBossState *state = &gMotherBoss;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            ScriptAnimator_SetScript(&state->slots[i].anim, src);
        }
    }
}
