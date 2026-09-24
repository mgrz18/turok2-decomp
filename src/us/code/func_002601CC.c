#include "common.h"
#include "m2c_macros.h"

f32 func_002119FC(f32, f32);                        /* extern */
M2C_UNK func_00243414(void *, void *, M2C_UNK);     /* extern */
extern f32 D_800A797C;
extern f32 D_800A7980;
extern f32 D_800B6D28;

s32 func_002601CC(void *arg0, void *arg1) {
    f32 temp_f0;
    s8 temp_v1;

    if (M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), s32 *, 0x14) & 0x40) {
        temp_f0 = M2C_FIELD(arg1, f32 *, 0x124) - D_800B6D28;
        M2C_FIELD(arg1, f32 *, 0x124) = temp_f0;
        if (temp_f0 < 0.0f) {
            M2C_FIELD(arg1, f32 *, 0x124) = 0.0f;
        }
        if (M2C_FIELD(arg1, s32 *, 4) != 0) {
            if (M2C_FIELD(arg1, f32 *, 0x124) <= 0.0f) {
                temp_v1 = M2C_FIELD(arg1, s8 *, 0x34);
                if ((temp_v1 == 4) || (temp_v1 == 0xE) || (temp_v1 == 0xF) || (temp_v1 == 0x10) || (temp_v1 == 0x11) || (temp_v1 == 0x12) || (temp_v1 == 0xA)) {
                    M2C_FIELD(arg1, f32 *, 0x124) = func_002119FC(D_800A797C, D_800A7980);
                    func_00243414(arg0, arg1, 0x17);
                    return 1;
                }
                /* Duplicate return node #14. Try simplifying control flow for better match */
                return 0;
            }
            goto block_13;
        }
        /* Duplicate return node #14. Try simplifying control flow for better match */
        return 0;
    }
block_13:
    return 0;
}
