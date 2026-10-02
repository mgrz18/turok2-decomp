#include "common.h"
#include "m2c_macros.h"

s32 func_0042C700(s32, s32, s32);

void func_0042C4B8(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x18FA8)) {
        M2C_FIELD(arg0, s32 *, 0x18FB8) = 0;
        func_0042C700((s32) arg0, ~arg1, 0);
    }
}
