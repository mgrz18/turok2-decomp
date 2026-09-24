#include "common.h"
#include "m2c_macros.h"

s32 *func_002017D4(s32 *, M2C_UNK);                 /* extern */

void func_002079D8(s32 **arg0, void *arg1) {
    s32 *temp_s0;

    temp_s0 = *arg0;
    if (*temp_s0 != 0) {
        M2C_FIELD(arg1, s32 **, 8) = func_002017D4(temp_s0, 0);
        M2C_FIELD(arg1, s32 *, 0) = (s32) *func_002017D4(temp_s0, 1);
        M2C_FIELD(arg1, s32 *, 4) = (s32) *func_002017D4(temp_s0, 2);
        return;
    }
    M2C_FIELD(arg1, s32 **, 8) = NULL;
    M2C_FIELD(arg1, s32 *, 0) = 0;
    M2C_FIELD(arg1, s32 *, 4) = 0;
}
