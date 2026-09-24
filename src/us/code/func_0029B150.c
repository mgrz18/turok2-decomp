#include "common.h"
#include "m2c_macros.h"

extern void *D_800B8920;

s32 func_0029B150(void *arg0) {
    void *var_a0;

    var_a0 = arg0;
    if (var_a0 == NULL) {
        var_a0 = D_800B8920;
    }
    return M2C_FIELD(var_a0, s32 *, 4);
}
