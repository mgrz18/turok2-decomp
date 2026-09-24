#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200500(void *, M2C_UNK, M2C_UNK);    /* extern */
M2C_UNK func_00200574(void *, s32);                 /* extern */
s32 func_0029DFF0();                                /* extern */
M2C_UNK func_0029E010(s32);                         /* extern */

void func_0026EB3C(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_s4;
    s32 var_s0;
    s32 var_s1;

    var_s1 = arg1;
    temp_s4 = func_0029DFF0();
    func_00200500(arg0, 0, 4);
    func_00200500(arg0 + 0x14, 0, 4);
    var_s0 = 0;
    if (arg2 > 0) {
        do {
            func_00200574(arg0, var_s1);
            var_s0 += 1;
            var_s1 += 0x3C;
        } while (var_s0 < arg2);
    }
    M2C_FIELD(arg0, s32 *, 0x28) = 0;
    func_0029E010(temp_s4);
}
