#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ u8 count;
} Bank;

s32 func_002929A0(void *, s32);                 /* extern */
s32 func_00292A20(void *, s32, s32);            /* extern */

void func_00292844(Bank *arg0, s32 *arg1) {
    s32 temp_s1;
    s32 var_s0;
    s32 *var_v0;

    var_v0 = arg1;
    do {
        temp_s1 = var_v0[3];
        var_v0++;
    } while (temp_s1 == 0);
    var_s0 = 0;
    if (arg0->count != 0) {
        do {
            func_002929A0(arg0, var_s0);
            func_00292A20(arg0, temp_s1, var_s0);
            var_s0 += 1;
        } while (var_s0 < (s32) arg0->count);
    }
    if (arg1[2] != 0) {
        func_002929A0(arg0, var_s0);
        func_00292A20(arg0, arg1[2], 9);
    }
}
