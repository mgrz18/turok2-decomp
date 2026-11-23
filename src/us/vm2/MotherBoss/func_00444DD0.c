/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BDD0.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Resets the slots of the module state selected by mask with src. */

extern void ScriptAnimator_SetScript(void *dst, void *src);

void MotherBoss_StartSpotAnims_2(void *obj, void *src, s32 mask) {
    MotherBossState *state = &gMotherBoss;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            ScriptAnimator_SetScript(&state->slots[i].anim, src);
        }
    }
}
