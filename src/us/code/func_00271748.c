#include "common.h"
#include "m2c_macros.h"

void func_00271748(void *arg0, s32 arg1) {
    f64 var_f1;

    var_f1 = (f64) arg1;
    if (arg1 < 0) {
        var_f1 += 4294967296.0;
    }
    M2C_FIELD(arg0, f32 *, 0x2BA4) = (f32) var_f1;
}
