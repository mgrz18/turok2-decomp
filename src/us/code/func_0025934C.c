#include "common.h"
#include "m2c_macros.h"

s32 func_00219F74(void *, M2C_UNK, M2C_UNK);        /* extern */
M2C_UNK func_0021E9F8(void *, s32, M2C_UNK);        /* extern */

void func_0025934C(void *arg0) {
    void *temp_s0;

    temp_s0 = M2C_FIELD(arg0, void **, 0x1A8);
    if (M2C_FIELD(temp_s0, s32 *, 0xA64) & 0x2000) {
        M2C_FIELD(temp_s0, s32 *, 0xC04) = 0x1130;
        func_0021E9F8(temp_s0, func_00219F74(temp_s0, 0x1130, -1), 0);
        M2C_FIELD(temp_s0, s8 *, 0xE2) = 0;
    }
}
