#include "common.h"
#include "m2c_macros.h"

s32 func_0022425C(s32, s32);

M2C_UNK func_0026CE44(s32, s32);                    /* extern */
extern M2C_UNK D_800F7078;

void func_0026C5C4(void *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_v0;
    s32 temp_s0;
    u16 temp_v1;

    if (arg0 != NULL) {
        temp_v1 = M2C_FIELD(arg0, u16 *, 2);
        if (!(temp_v1 & 1)) {
            temp_a0 = M2C_FIELD(arg0, s32 *, 0x10);
            temp_s0 = M2C_FIELD(arg0, u16 *, 0);
            M2C_FIELD(arg0, u16 *, 2) = (u16) (temp_v1 | 1);
            if (temp_a0 != 0) {
                func_0026CE44(temp_a0, temp_s0);
            }
            temp_a0_2 = M2C_FIELD(arg0, s32 *, 0x14);
            if (temp_a0_2 != 0) {
                func_0026CE44(temp_a0_2, temp_s0);
            }
            temp_a0_3 = M2C_FIELD(arg0, s32 *, 0x18);
            if (temp_a0_3 != 0) {
                func_0026CE44(temp_a0_3, temp_s0);
            }
        }
        temp_v0 = func_0022425C((s32) &D_800F7078, (s32) arg0);
        if (temp_v0 != 0) {
            M2C_FIELD(temp_v0, s8 *, 0x5C) = 2;
            M2C_FIELD(temp_v0, s32 *, 0x44) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x44) | 1);
            M2C_FIELD(temp_v0, u8 *, 0x5B) = (u8) (M2C_FIELD(temp_v0, u8 *, 0x5B) | 2);
        }
    }
}
