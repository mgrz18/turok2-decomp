#include "common.h"
#include "m2c_macros.h"

s32 func_0042EF2C(s32);

extern M2C_UNK D_004384E8;
extern M2C_UNK D_800F56B0;

void func_0042F590(void) {
    s32 temp_v0;

    if (M2C_FIELD(&D_800F56B0, s32 *, 0) != 0) {
        func_0042EF2C(*(((__typeof__(&D_004384E8))((s8 *)&D_004384E8 + (M2C_FIELD(&D_800F56B0, s32 *, 0x350) * 4)))));
        temp_v0 = M2C_FIELD(&D_800F56B0, s32 *, 0x350) + 1;
        M2C_FIELD(&D_800F56B0, s32 *, 0x350) = temp_v0;
        if (*(((__typeof__(&D_004384E8))((s8 *)&D_004384E8 + (temp_v0 * 4)))) == 0) {
            M2C_FIELD(&D_800F56B0, s32 *, 0x350) = 0;
        }
    }
}
