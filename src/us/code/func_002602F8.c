#include "common.h"
#include "m2c_macros.h"

s32 func_002602F8(void *arg0, void *arg1) {
    void *temp_a0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x14) + 0x14;
    if (((M2C_FIELD(arg1, s8 *, 0x130) != 0) && (M2C_FIELD(temp_a0, u16 *, 0x74) & 0x1F)) || ((M2C_FIELD(arg1, s8 *, 0x131) != 0) && (M2C_FIELD(temp_a0, u16 *, 0x74) & 0x3E0)) || ((M2C_FIELD(arg1, s8 *, 0x132) != 0) && (M2C_FIELD(temp_a0, u16 *, 0x74) & 0x7C00))) {
        return 1;
    }
    return 0;
}
