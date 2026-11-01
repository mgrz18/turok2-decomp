/* Second-module copy of func_00424A60 (first module). The flag test is an inline helper, so &D_800F7078 is re-addressed by symbol at each call. */
#include "common.h"

s32 func_00224C08(void *arg0, s32 arg1);

extern s32 D_800F7078;

static __inline__ s32 IsSet(s32 flag) {
    return func_00224C08(&D_800F7078, flag);
}

s32 func_0045D980(s32 level) {
    s32 open = 0;

    switch (level) {
        case 1:
            if (!IsSet(2) && !IsSet(3)) {
                open = 1;
            }
            break;
        case 2:
            if (!IsSet(4) && !IsSet(5)) {
                open = 1;
            }
            break;
        case 3:
            if (!IsSet(6) && !IsSet(7)) {
                open = 1;
            }
            break;
        case 4:
            if (!IsSet(8)) {
                open = 1;
            }
            break;
        case 5:
            if (!IsSet(9) && !IsSet(10)) {
                open = 1;
            }
            break;
        case 6:
            if (!IsSet(11) && !IsSet(12)) {
                open = 1;
            }
            break;
        default:
            open = 1;
            break;
    }
    return open;
}
