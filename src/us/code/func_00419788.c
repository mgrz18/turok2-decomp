#include "common.h"
#include "m2c_macros.h"

s32 func_002859DC();                                /* extern */

s32 func_00419788(s32 arg0, void *arg1) {
    void *temp_a0;
    void *temp_v0;

    if (func_002859DC() < 2) {
        temp_a0 = M2C_FIELD(arg1, void **, 0xC);
        M2C_FIELD(temp_a0, s32 *, 0x58) = (s32) (M2C_FIELD(temp_a0, s32 *, 0x58) & 0xFEFFFFFF);
        if (M2C_FIELD(arg1, s16 *, 0) == 2) {
            M2C_FIELD(arg1, s16 *, 0) = 1;
        }
    } else {
        temp_v0 = M2C_FIELD(arg1, void **, 0xC);
        M2C_FIELD(temp_v0, s32 *, 0x58) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x58) | 0x01000000);
    }
    return 0;
}
