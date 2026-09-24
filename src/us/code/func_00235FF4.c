#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0026D288(s32, s32);                    /* extern */
M2C_UNK func_0026D2D0(void *, s32);                 /* extern */

s32 func_00235FF4(void *arg0, s32 arg1) {
    s32 temp_s0;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0x6A88);
    if (temp_s0 != 0) {
        func_0026D2D0(arg0 + 0x6A88, temp_s0);
        func_0026D288(arg1 + 0x40, temp_s0);
    }
    return temp_s0;
}
