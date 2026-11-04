#include "common.h"

/* A swoosh trail (see func_00234348 / func_0023417C). */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s32 texture;            /* < 0: drawn untextured (func_00234928) */
    /* 0x38 */ u8 pad38[0x3C - 0x38];
    /* 0x3C */ s32 flags;
    /* 0x40 */ u8 pad40[0x48 - 0x40];
    /* 0x48 */ s32 nodeCount;
} Trail;

s32 func_00233FCC(Trail *, s32);             /* extern */
s32 func_0023417C(Trail *);                  /* extern */
s32 func_00234348(s32, Trail *, s32);        /* extern */
s32 func_00234928(s32, Trail *, s32);        /* extern */

void func_00236BC4(s32 arg0, Trail *arg1, s32 arg2) {
    if (arg1->flags & 8) {
        func_0023417C(arg1);
    }
    if (arg1->flags & 4) {
        func_00233FCC(arg1, arg2);
    }
    if (arg1->nodeCount >= 2) {
        if (arg1->texture >= 0) {
            func_00234348(arg0, arg1, arg2);
            return;
        }
        func_00234928(arg0, arg1, arg2);
    }
}
