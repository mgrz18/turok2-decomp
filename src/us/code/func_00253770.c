#include "common.h"
#include "m2c_macros.h"

s32 func_0024E5F0(s32, s32, s32);
s32 func_002548FC(s32);

void func_00253770(void *arg0, s32 arg1, s32 arg2, void *arg3) {
    f32 temp_f0;
    f32 temp_f1;
    f32 var_f0;
    u16 temp_v1;

    if ((M2C_FIELD(arg0, s32 *, 0x524) != 0) && (func_002548FC((s32) arg0) == 0)) {
        temp_v1 = M2C_FIELD(arg3, u16 *, 2);
        switch (temp_v1) {                          /* irregular */
        case 0x128:
            if (M2C_FIELD(arg0, s16 *, 0xA08) != 0x23) {
                temp_f1 = M2C_FIELD(arg0, f32 *, 0x1298);
                if (temp_f1 > 0.0f) {
                    temp_f0 = temp_f1 + 30.0f;
                    if (!(temp_f0 <= 150.0f)) {
                        var_f0 = 150.0f;
                        goto block_12;
                    }
                    M2C_FIELD(arg0, f32 *, 0x1298) = temp_f0;
                } else {
                    var_f0 = 60.0f;
block_12:
                    M2C_FIELD(arg0, f32 *, 0x1298) = var_f0;
                }
                M2C_FIELD(arg0, s32 *, 0x12AC) = (s32) M2C_FIELD(arg3, s32 *, 0x10C);
                return;
            }
            break;
        case 0x129:
            if (M2C_FIELD(arg0, s32 *, 0x12A8) == 0) {
                M2C_FIELD(arg0, s32 *, 0x12A8) = (s32) M2C_FIELD(arg3, s32 *, 0x10C);
            }
            M2C_FIELD(arg0, f32 *, 0x1298) = 0.0f;
            func_0024E5F0((s32) arg0, (s32) arg0, 0x23);
            break;
        }
    }
}
