#include "common.h"
#include "m2c_macros.h"

s32 func_00226D30(void *arg0, void *arg1) {
    s32 r = 1;

    if (M2C_FIELD(arg0, f32 *, 4) > M2C_FIELD(arg1, f32 *, 4)) {
        r = -1;
    }
    return r;
}
