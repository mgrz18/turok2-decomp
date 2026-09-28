#include "common.h"
#include "m2c_macros.h"

s32 func_002627EC(void *arg0) {
    s32 v = M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), s32 *, 0x1C);

    if (v != 0) {
        return v;
    }
    return 0x2F44;
}
