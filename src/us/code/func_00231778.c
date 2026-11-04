#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x40];
    /* 0x040 */ s32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ s32 *cfg;               /* cfg[0] & 0x02000000 */
} EmitActor;

extern u8 D_8011AAEF;

s32 func_00231778(EmitActor *arg0) {
    s32 var_a1;

    var_a1 = 0;
    if (*arg0->cfg & 0x02000000) {
        if (arg0->flags & 2) {
            var_a1 = 2;
        } else if (D_8011AAEF == 2) {
            var_a1 = 1;
        }
    }
    return var_a1;
}
