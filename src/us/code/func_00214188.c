#include "common.h"
#include "m2c_macros.h"

void *func_002017D4(s32 *, s32);                    /* extern */

s32 func_00214188(s32 *arg0) {
    s32 var_s0;
    s32 var_s1;
    void *temp_v0;

    var_s0 = 0;
    var_s1 = 0;
    if (*arg0 > 0) {
        do {
            temp_v0 = func_002017D4(arg0, var_s0);
            var_s0 += 1;
            var_s1 += M2C_FIELD(temp_v0, s32 *, 4);
        } while (var_s0 < *arg0);
    }
    return var_s1;
}
