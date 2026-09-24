#include "common.h"
#include "m2c_macros.h"

void func_0023624C(void *arg0, s32 arg1) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    void *temp_a0;
    void *temp_a0_2;
    void *var_v1;

    var_v1 = M2C_FIELD(arg0, void **, 0x7520);
    if (var_v1 != NULL) {
        do {
            temp_a0 = M2C_FIELD(var_v1, void **, 0x1C);
            if (temp_a0 == arg1) {
                if (temp_a0 != NULL) {
                    if (M2C_FIELD(var_v1, s32 *, 0x3C) & 1) {
                        temp_v0 = M2C_FIELD(temp_a0, u8 *, 0x10F);
                        if (temp_v0 != 0) {
                            M2C_FIELD(temp_a0, u8 *, 0x10F) = (u8) (temp_v0 - 1);
                        }
                    }
                    if (M2C_FIELD(var_v1, s32 *, 0x3C) & 2) {
                        temp_a0_2 = M2C_FIELD(var_v1, void **, 0x1C);
                        temp_v0_2 = M2C_FIELD(temp_a0_2, u8 *, 0x1B5);
                        if (temp_v0_2 != 0) {
                            M2C_FIELD(temp_a0_2, u8 *, 0x1B5) = (u8) (temp_v0_2 - 1);
                        }
                    }
                }
                M2C_FIELD(var_v1, void **, 0x1C) = NULL;
                temp_f1 = M2C_FIELD(M2C_FIELD(var_v1, void **, 8), f32 *, 4);
                if (M2C_FIELD(var_v1, f32 *, 0x24) < temp_f1) {
                    M2C_FIELD(var_v1, f32 *, 0x24) = temp_f1;
                }
            }
            var_v1 = M2C_FIELD(var_v1, void **, 4);
        } while (var_v1 != NULL);
    }
}
