#include "common.h"
#include "m2c_macros.h"

void func_002368B0(void *arg0) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    void *temp_v1;
    void *temp_v1_2;

    temp_v1 = M2C_FIELD(arg0, void **, 0x1C);
    if (temp_v1 != NULL) {
        if (M2C_FIELD(arg0, s32 *, 0x3C) & 1) {
            temp_v0 = M2C_FIELD(temp_v1, u8 *, 0x10F);
            if (temp_v0 != 0) {
                M2C_FIELD(temp_v1, u8 *, 0x10F) = (u8) (temp_v0 - 1);
            }
        }
        if (M2C_FIELD(arg0, s32 *, 0x3C) & 2) {
            temp_v1_2 = M2C_FIELD(arg0, void **, 0x1C);
            temp_v0_2 = M2C_FIELD(temp_v1_2, u8 *, 0x1B5);
            if (temp_v0_2 != 0) {
                M2C_FIELD(temp_v1_2, u8 *, 0x1B5) = (u8) (temp_v0_2 - 1);
            }
        }
    }
    M2C_FIELD(arg0, void **, 0x1C) = NULL;
    temp_f1 = M2C_FIELD(M2C_FIELD(arg0, void **, 8), f32 *, 4);
    if (M2C_FIELD(arg0, f32 *, 0x24) < temp_f1) {
        M2C_FIELD(arg0, f32 *, 0x24) = temp_f1;
    }
}
