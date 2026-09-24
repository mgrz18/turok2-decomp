#include "common.h"
#include "m2c_macros.h"

s32 func_00204D90(s32);                             /* extern */

s32 func_00219968(void *arg0) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x10);
    if (temp_a0 == 0) {
        return 0;
    }
    return func_00204D90(temp_a0) != 0;
}
