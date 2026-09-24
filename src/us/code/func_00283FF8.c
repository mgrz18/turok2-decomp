#include "common.h"
#include "m2c_macros.h"

s32 func_00412438();                                /* extern */
extern void *D_800C1BB0;

s32 func_00283FF8(s32 arg0, M2C_UNK (*arg1)(void *), M2C_UNK (*arg2)()) {
    s32 temp_s3;

    temp_s3 = func_00412438();
    if (M2C_FIELD(D_800C1BB0, s32 *, 0xE0) == arg0) {
        M2C_FIELD(D_800C1BB0, M2C_UNK (**)(void *), 0xE4) = arg1;
        M2C_FIELD(D_800C1BB0, M2C_UNK (**)(), 0xE8) = arg2;
    } else {
        if (arg1 != NULL) {
            arg1(D_800C1BB0);
        }
        if (arg2 != NULL) {
            arg2();
        }
    }
    return temp_s3;
}
