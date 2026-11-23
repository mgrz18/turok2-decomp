/* Cut fixed in versions/cuts.us.txt: 0x26EFAC is one junk word (`move $v1, $v0`, alignment padding after
 * RumbleCurve_GetValue's object); the real function is an empty one at 0x26EFB0
 * (0x8 bytes: jr $ra; nop), the start of Level_GetHub's object. Nothing
 * calls it or points at it. */
#include "common.h"

void func_0026EFB0(void) {
}

/* Which thousand-block an id falls in (1..6); 0 for the block-end ids, -1 otherwise. */
s32 Level_GetHub(s32 id) {
    if (id == 1999 || id == 2999 || id == 3999 || id == 4999 || id == 5999 || id == 6999 ||
        id == 99) {
        return 0;
    }
    if (id >= 1000 && id < 2000) {
        return 1;
    }
    if (id >= 2000 && id < 3000) {
        return 2;
    }
    if (id >= 3000 && id < 4000) {
        return 3;
    }
    if (id >= 4000 && id < 5000) {
        return 4;
    }
    if (id >= 5000 && id < 6000) {
        return 5;
    }
    if (id >= 6000 && id < 7000) {
        return 6;
    }
    return -1;
}
