/* Second-module copy of func_0040C5DC (src/us/code/func_0040C5DC.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Resets every active slot of the module state with D_00431E04. */

extern u8 D_00431E04[];
extern void func_00236314(void *dst, void *src);

static __inline__ void resetSlots(s32 mask, void *src) {
    MotherBossState *state = &D_8012FB70;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            func_00236314(&state->slots[i].anim, src);
        }
    }
}

void func_004455DC(void) {
    MotherBossState *state = &D_8012FB70;

    resetSlots(state->activeMask, D_00431E04);
}
