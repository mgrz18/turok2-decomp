#include "common.h"
#include "m2c_macros.h"

s32 func_00265B2C(void *arg0) {
    s32 v = M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), s32 *, 0x24);

    if (v != 0) {
        return v;
    }
    return 0x5334;
}
