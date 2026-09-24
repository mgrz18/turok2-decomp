#include "common.h"
#include "m2c_macros.h"

s32 func_00276898(void *arg0) {
    return M2C_FIELD(arg0, u16 *, 6) & 0x3FFF;
}
