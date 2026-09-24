#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0024E5F0(s32, void *, M2C_UNK);        /* extern */
s32 func_002685F0(void *);                          /* extern */

s32 func_00257644(s32 arg0, void *arg1) {
    if ((M2C_FIELD(arg1, f32 *, 0x1C) <= 0.0f) && (func_002685F0(arg1) != 0)) {
        func_0024E5F0(arg0, arg1, 2);
        return 1;
    }
    return 0;
}
