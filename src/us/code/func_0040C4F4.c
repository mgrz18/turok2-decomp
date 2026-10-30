/* FAKEMATCH: identical if/else arms in the counting loop (found by the permuter); the plain loop puts the
 * counter in $v0 instead of $a1. */
#include "common.h"

/* Returns whether fewer than max actors are in the D_8010FEB4 list. */

typedef struct Actor {
    /* 0x000 */ u8 pad0[0x28C];
    /* 0x28C */ struct Actor *next;
} Actor;

extern Actor *D_8010FEB4;

s32 func_0040C4F4(s32 max) {
    Actor *actor;
    s32 count;

    count = 0;
    for (actor = D_8010FEB4; actor != NULL; actor = actor->next) {
        /* both arms are merged by cross-jumping; the extra block gives the ROM's register allocation */
        if (actor->next != NULL) {
            count++;
        } else {
            count++;
        }
    }
    return count < max;
}
