#include "common.h"
#include "m2c_macros.h"

s32 func_00267F04(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x48) & 1;
}
