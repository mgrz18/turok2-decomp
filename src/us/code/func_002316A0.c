#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x40];
    /* 0x040 */ s32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ s32 *cfg;               /* cfg[0] & 0x02000000 */
} EmitActor;

extern u8 D_8011AAEF;

void func_002316A0(EmitActor *arg0, s32 *arg1) {
    s32 var_a2;

    var_a2 = 0;
    if (*arg0->cfg & 0x02000000) {
        if (arg0->flags & 2) {
            var_a2 = 2;
        } else if (D_8011AAEF == 2) {
            var_a2 = 1;
        }
    }
    *arg1 |= var_a2 << 0x1E;
}
