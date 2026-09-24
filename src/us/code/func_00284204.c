#include "common.h"
#include "m2c_macros.h"

s32 func_0028415C();                                /* extern */
s32 func_00284174();                                /* extern */

s32 func_00284204(void) {
    s32 var_v0;

    var_v0 = 0;
    if (func_00284174() == 0) {
        var_v0 = func_0028415C() == 0;
    }
    return var_v0;
}
