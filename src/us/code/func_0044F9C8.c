#include "common.h"
#include "m2c_macros.h"

s32 func_00200738(s32, s32);
s32 func_002052D8(s32, s32);
s32 func_0041531C(s32, s32);

extern M2C_UNK D_80119E2C;

void func_0044F9C8(void **arg0) {
    M2C_UNK (*temp_v0)(void *, void **);
    void *temp_s2;
    void *temp_v0_2;
    void *var_s0;

    var_s0 = *arg0;
    if (var_s0 != NULL) {
        do {
            temp_s2 = M2C_FIELD(var_s0, void **, 0x1D4);
            if ((M2C_FIELD(var_s0, s16 *, 0x28) == 2) && (arg0 != &D_80119E2C)) {
                if (M2C_FIELD(&D_80119E2C, s32 *, 0x10) == 0) {
                    goto block_6;
                }
            } else {
block_6:
                if (func_0041531C((s32) var_s0, (s32) arg0) != 0) {
                    temp_v0 = M2C_FIELD(M2C_FIELD(var_s0, void **, 0x14), M2C_UNK (**)(void *, void **), 0xC);
                    if (temp_v0 != NULL) {
                        temp_v0(var_s0, arg0);
                    }
                    temp_v0_2 = M2C_FIELD(var_s0, void **, 0x20);
                    M2C_FIELD(temp_v0_2, s32 *, 0xB0) = 0;
                    M2C_FIELD(temp_v0_2, s32 *, 0xB4) = 0;
                    M2C_FIELD(temp_v0_2, s32 *, 0xBC) = 0;
                    func_00200738((s32) arg0, (s32) var_s0);
                    func_002052D8(0, M2C_FIELD(var_s0, s32 *, 8));
                }
            }
            var_s0 = temp_s2;
        } while (var_s0 != NULL);
    }
}
