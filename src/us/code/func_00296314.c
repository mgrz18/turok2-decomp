#include "common.h"
#include "m2c_macros.h"

f64 func_00296314(f64 arg0, s32 arg2) {
    f64 var_f12;

    var_f12 = arg0;
    if (arg2 != 0) {
        var_f12 *= (f64) (1 << arg2);
    }
    return var_f12;
}
