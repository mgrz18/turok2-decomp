#include "common.h"
#include "m2c_macros.h"

s32 func_00408BFC(void *arg0) {
    return M2C_FIELD(M2C_FIELD(arg0, void **, 4), s16 *, 0x46) - M2C_FIELD(arg0, s32 *, 0x10);
}
