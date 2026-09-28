#include "common.h"
#include "m2c_macros.h"

s32 func_00245E20(void *arg0) {
    if ((M2C_FIELD(arg0, s32 *, 0x27C) & 2) && M2C_FIELD(arg0, u16 *, 0xB8) != 0xCA) {
        return 1;
    }
    return 0;
}
