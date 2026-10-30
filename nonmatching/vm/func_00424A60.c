/* NON-MATCHING: 74 of 78 words differ: GCC CSEs &D_800F7078 into an s register across the two calls of each case; the ROM reloads it */
#include "common.h"

s32 func_00224C08(void *arg0, s32 arg1);

extern s32 D_800F7078;

s32 func_00424A60(s32 level) {
    s32 open = 0;

    switch (level) {
        case 1:
            if (func_00224C08(&D_800F7078, 2) == 0 && func_00224C08(&D_800F7078, 3) == 0) {
                open = 1;
            }
            break;
        case 2:
            if (func_00224C08(&D_800F7078, 4) == 0 && func_00224C08(&D_800F7078, 5) == 0) {
                open = 1;
            }
            break;
        case 3:
            if (func_00224C08(&D_800F7078, 6) == 0 && func_00224C08(&D_800F7078, 7) == 0) {
                open = 1;
            }
            break;
        case 4:
            if (func_00224C08(&D_800F7078, 8) == 0) {
                open = 1;
            }
            break;
        case 5:
            if (func_00224C08(&D_800F7078, 9) == 0 && func_00224C08(&D_800F7078, 10) == 0) {
                open = 1;
            }
            break;
        case 6:
            if (func_00224C08(&D_800F7078, 11) == 0 && func_00224C08(&D_800F7078, 12) == 0) {
                open = 1;
            }
            break;
        default:
            open = 1;
            break;
    }
    return open;
}
