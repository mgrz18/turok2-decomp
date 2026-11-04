#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* AI state: once the anim is done, reset every active slot of the module state, then go to anim state 0xC. */

extern s32 D_801301D0; /* active slot mask */
extern u8 D_00431EE4[];
extern void func_00236314(void *dst, void *src);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_0040B1E8(void *obj, CAI *ai) {
    MotherBossState *state;
    s32 mask;
    s32 i;

    if (ai->m_AnimRepeatedCount > 0) {
        mask = D_801301D0;
        state = &D_8012FB70;
        for (i = 0; i < 15; i++) {
            if ((mask >> i) & 1) {
                func_00236314(&state->slots[i].anim, D_00431EE4);
            }
        }
        func_00243414(obj, ai, 0xC);
    }
}
