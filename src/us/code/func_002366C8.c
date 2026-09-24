#include "common.h"
#include "m2c_macros.h"

s32 func_00233BD4(void *, void *);                  /* extern */
M2C_UNK func_0026D474(void *, void *);              /* extern */

void func_002366C8(void *arg0) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    void *temp_s1;
    void *temp_v1;
    void *temp_v1_2;
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x7520);
    if (var_s0 != NULL) {
        do {
            temp_s1 = M2C_FIELD(var_s0, void **, 4);
            if (func_00233BD4(arg0, var_s0) == 0) {
                temp_v1 = M2C_FIELD(var_s0, void **, 0x1C);
                if (temp_v1 != NULL) {
                    if (M2C_FIELD(var_s0, s32 *, 0x3C) & 1) {
                        temp_v0 = M2C_FIELD(temp_v1, u8 *, 0x10F);
                        if (temp_v0 != 0) {
                            M2C_FIELD(temp_v1, u8 *, 0x10F) = (u8) (temp_v0 - 1);
                        }
                    }
                    if (M2C_FIELD(var_s0, s32 *, 0x3C) & 2) {
                        temp_v1_2 = M2C_FIELD(var_s0, void **, 0x1C);
                        temp_v0_2 = M2C_FIELD(temp_v1_2, u8 *, 0x1B5);
                        if (temp_v0_2 != 0) {
                            M2C_FIELD(temp_v1_2, u8 *, 0x1B5) = (u8) (temp_v0_2 - 1);
                        }
                    }
                }
                M2C_FIELD(var_s0, void **, 0x1C) = NULL;
                temp_f1 = M2C_FIELD(M2C_FIELD(var_s0, void **, 8), f32 *, 4);
                if (M2C_FIELD(var_s0, f32 *, 0x24) < temp_f1) {
                    M2C_FIELD(var_s0, f32 *, 0x24) = temp_f1;
                }
                func_0026D474(arg0 + 0x7514, var_s0);
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}
