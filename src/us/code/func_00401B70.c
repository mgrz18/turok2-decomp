#include "common.h"
#include "m2c_macros.h"

s32 func_00400000(s32);
s32 func_00401CE8(s32);

void func_00401B70(void *arg0) {
    s32 temp_s0;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;

    func_00400000(M2C_FIELD(arg0, s32 *, 0xC));
    temp_s0 = M2C_FIELD(arg0, s32 *, 0xC);
    if (temp_s0 != 0) {
        temp_a0 = M2C_FIELD(temp_s0, void **, 0x10);
        M2C_FIELD(temp_s0, u16 *, 2) = (u16) (M2C_FIELD(temp_s0, u16 *, 2) & 0x7FFF);
        if ((temp_a0 != NULL) && (M2C_FIELD(temp_a0, u16 *, 2) & 0x8000)) {
            func_00401CE8((s32) temp_a0);
        }
        temp_a0_2 = M2C_FIELD(temp_s0, void **, 0x14);
        if ((temp_a0_2 != NULL) && (M2C_FIELD(temp_a0_2, u16 *, 2) & 0x8000)) {
            func_00401CE8((s32) temp_a0_2);
        }
        temp_a0_3 = M2C_FIELD(temp_s0, void **, 0x18);
        if ((temp_a0_3 != NULL) && (M2C_FIELD(temp_a0_3, u16 *, 2) & 0x8000)) {
            func_00401CE8((s32) temp_a0_3);
        }
    }
}
