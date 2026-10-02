#include "common.h"
#include "m2c_macros.h"

s32 func_002052D8(s32, s32);

void func_00412EF4(void *arg0) {
    if (arg0 != NULL) {
        func_002052D8(0, M2C_FIELD(arg0, s32 *, -0x10));
    }
}
