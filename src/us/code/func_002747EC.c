#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00293580(s32);                         /* extern */
M2C_UNK func_00293E60(s32, s16);                    /* extern */

void func_002747EC(void *arg0) {
    s32 temp_s0;
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0xB0);
    temp_s0 = temp_v1 + 0x84;
    func_00293E60(temp_s0, M2C_FIELD((temp_v1 + (M2C_FIELD(arg0, s32 *, 0) << 1)), s16 *, 0xDC));
    func_00293580(temp_s0);
}
