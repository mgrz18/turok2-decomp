#include "common.h"

s32 func_00228B4C(s32 *, s32);              /* extern */
extern s32 D_8010FF34;

void func_00256FD4(s32 arg0, s32 *arg1) {
    s32 temp_a1;

    temp_a1 = *arg1;
    if (temp_a1 != 0) {
        func_00228B4C(&D_8010FF34, temp_a1);
        *arg1 = 0;
    }
}
