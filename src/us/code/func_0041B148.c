/* WRONG CUT: real function 0x0041B148, size 0x80 (through 0x0041B1C8): splat cut each
 * switch case (func_0041B178 ... func_0041B1C0, targets of jtbl_800C12E0) as its own
 * function. */
#include "common.h"

extern s32 D_80130924;

/* A time/score threshold per setting index. */
s32 func_0041B148(void) {
    switch ((u32) D_80130924) {
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
