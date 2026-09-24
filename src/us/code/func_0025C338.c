#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

extern f32 D_800B6D28;

s32 func_0025C338(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    s8 temp_v1;

    if (M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), s32 *, 0x14) & 0x100000) {
        temp_f0 = M2C_FIELD(arg1, f32 *, 0x134) - D_800B6D28;
        M2C_FIELD(arg1, f32 *, 0x134) = temp_f0;
        if (temp_f0 < 0.0f) {
            temp_f0_2 = M2C_FIELD(arg1, f32 *, 0x138) - D_800B6D28;
            M2C_FIELD(arg1, f32 *, 0x134) = 0.0f;
            M2C_FIELD(arg1, f32 *, 0x138) = temp_f0_2;
            if (temp_f0_2 < 0.0f) {
                M2C_FIELD(arg1, f32 *, 0x138) = 0.0f;
            }
        }
        if ((M2C_FIELD(arg1, s32 *, 4) != 0) && (M2C_FIELD(arg1, f32 *, 0x134) <= 0.0f)) {
            if (M2C_FIELD(arg1, f32 *, 0x138) <= 0.0f) {
                temp_v1 = M2C_FIELD(arg1, s8 *, 0x34);
                if ((temp_v1 == 4) || (temp_v1 == 0xE) || (temp_v1 == 0xF) || (temp_v1 == 0x10) || (temp_v1 == 0x11) || (temp_v1 == 0x12) || (temp_v1 == 0xA)) {
                    func_00243414((s32) arg0, (s32) arg1, 0x19);
                    return 1;
                }
                /* Duplicate return node #16. Try simplifying control flow for better match */
                return 0;
            }
            goto block_15;
        }
        /* Duplicate return node #16. Try simplifying control flow for better match */
        return 0;
    }
block_15:
    return 0;
}
