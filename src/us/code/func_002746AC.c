#include "common.h"
#include "m2c_macros.h"

s32 func_00293E70(s32, s32);

M2C_UNK func_00293E60(s32, s16);                    /* extern */

void func_002746AC(void *arg0, s32 arg1) {
    s32 temp_s0;
    s32 temp_s0_2;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0xB0);
    temp_s0_2 = temp_s0 + 0x84;
    func_00293E60(temp_s0_2, M2C_FIELD((temp_s0 + (M2C_FIELD(arg0, s32 *, 0) << 1)), s16 *, 0xDC));
    func_00293E70(temp_s0_2, (s32) (s16) (s32) ((f32) arg1 * M2C_FIELD(arg0, f32 *, 0xC8)));
}
