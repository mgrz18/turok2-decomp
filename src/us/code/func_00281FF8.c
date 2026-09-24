#include "common.h"
#include "m2c_macros.h"

s32 *func_00204EDC(M2C_UNK, s32, M2C_UNK, M2C_UNK *); /* extern */
M2C_UNK func_00266C5C(s32, M2C_UNK, s32);           /* extern */
extern M2C_UNK D_800A9A08;

void func_00281FF8(void *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_a0;
    s32 temp_s1;

    temp_s1 = arg1 << 6;
    M2C_FIELD(arg0, s32 *, 0xF08) = arg1;
    temp_v0 = func_00204EDC(0, temp_s1, 0x23, &D_800A9A08);
    M2C_FIELD(arg0, s32 **, 0xF00) = temp_v0;
    temp_a0 = *temp_v0;
    M2C_FIELD(arg0, s32 *, 0xF04) = temp_a0;
    func_00266C5C(temp_a0, 0, temp_s1);
}
