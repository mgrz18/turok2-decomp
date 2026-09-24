#include "common.h"
#include "m2c_macros.h"

s32 *func_00204EDC(M2C_UNK, s32, M2C_UNK, M2C_UNK *); /* extern */
M2C_UNK func_00266C5C(s32, M2C_UNK, s32);           /* extern */
extern M2C_UNK D_800A7080;

void func_00257830(void *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_a0;
    s32 temp_s0;

    temp_s0 = arg1 * 0x1328;
    M2C_FIELD(arg0, s32 *, 8) = arg1;
    temp_v0 = func_00204EDC(0, temp_s0, 0x23, &D_800A7080);
    M2C_FIELD(arg0, s32 **, 0) = temp_v0;
    temp_a0 = *temp_v0;
    M2C_FIELD(arg0, s32 *, 4) = temp_a0;
    func_00266C5C(temp_a0, 0, temp_s0);
}
