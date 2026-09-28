#include "common.h"
#include "m2c_macros.h"

f64 func_00296280(f64 arg0, s32 *arg2) {
    f64 var_f1;

    var_f1 = fabs(arg0);
    if (var_f1 >= 1.0) {
        do {
            var_f1 *= 0.5;
            *arg2 += 1;
        } while (var_f1 >= 1.0);
    }
    if (var_f1 < 0.5) {
        do {
            var_f1 *= 2.0;
            *arg2 -= 1;
        } while (var_f1 < 0.5);
    }
    if (!(arg0 > 0.0)) {
        var_f1 = -var_f1;
    }
    return var_f1;
}
