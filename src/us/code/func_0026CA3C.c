#include "common.h"
#include "m2c_macros.h"

void func_0026CA3C(void *arg0, f32 arg1, s32 arg2) {
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;

    if (M2C_FIELD(arg0, u16 *, 0) == (arg2 & 0xFFFF)) {
        if (M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 4) == arg1) {
            if ((M2C_FIELD(M2C_FIELD(arg0, void **, 8), f32 *, 4) != arg1) || (M2C_FIELD(M2C_FIELD(arg0, void **, 0xC), f32 *, 4) != arg1)) {
                goto block_5;
            }
        } else {
block_5:
            M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 4) = arg1;
            M2C_FIELD(M2C_FIELD(arg0, void **, 8), f32 *, 4) = arg1;
            M2C_FIELD(M2C_FIELD(arg0, void **, 0xC), f32 *, 4) = arg1;
            temp_a0 = M2C_FIELD(arg0, void **, 0x10);
            if (temp_a0 != NULL) {
                func_0026CA3C(temp_a0, arg1, arg2 & 0xFFFF);
            }
            temp_a0_2 = M2C_FIELD(arg0, void **, 0x14);
            if (temp_a0_2 != NULL) {
                func_0026CA3C(temp_a0_2, arg1, arg2 & 0xFFFF);
            }
            temp_a0_3 = M2C_FIELD(arg0, void **, 0x18);
            if (temp_a0_3 != NULL) {
                func_0026CA3C(temp_a0_3, arg1, arg2 & 0xFFFF);
            }
        }
    }
}
