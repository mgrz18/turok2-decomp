#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00275544(s16, s32, s32, s32, s32, s32); /* extern */

void func_0025E798(void *arg0) {
    u16 temp_a0;

    temp_a0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), u16 *, 0x42);
    if (temp_a0 != 0) {
        func_00275544((s16) temp_a0, M2C_FIELD(arg0, s32 *, 4), M2C_FIELD(arg0, s32 *, 8), M2C_FIELD(arg0, s32 *, 0xC), 0, -1);
    }
}
