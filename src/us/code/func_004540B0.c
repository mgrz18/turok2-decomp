/* WRONG CUT: the real function is CheatMenu_GetWarpLevel_2, size 0x80: splat cut it after the jump-table dispatch
 * (0x30 bytes); the cases follow up to 0x454130. */
#include "common.h"
#include "globals/menu.h"


/* The level of the selected option. */
s32 CheatMenu_GetWarpLevel_2(void) {
    switch (sCheatsMenuSelection) {
    case 0:
    default:
        return 1000;
    case 1:
        return 2000;
    case 2:
        return 3000;
    case 3:
        return 4000;
    case 4:
        return 5000;
    case 5:
        return 6000;
    case 6:
        return 7401;
    case 7:
        return 7501;
    case 8:
        return 7601;
    case 9:
        return 7602;
    case 10:
        return 8000;
    }
}
