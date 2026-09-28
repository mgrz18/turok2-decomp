#include "common.h"
#include "m2c_macros.h"

s32 func_002927E8(void *arg0, s32 arg1) {
    s32 v = M2C_FIELD(arg0, s32 *, 0x24) - arg1;

    if (v >= 0) {
        return v;
    }
    return 1000;
}
