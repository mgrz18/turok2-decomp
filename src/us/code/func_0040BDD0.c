#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Resets the slots of the module state selected by mask with src. */

extern void func_00236314(void *dst, void *src);

void func_0040BDD0(void *obj, void *src, s32 mask) {
    MotherBossState *state = &D_8012FB70;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            func_00236314(state->slots[i].anim, src);
        }
    }
}
