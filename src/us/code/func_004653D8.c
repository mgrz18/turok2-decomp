#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

void func_0042C620(void *, int, int);

void func_004653D8(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x18FA8)) {
        M2C_FIELD(arg0, s32 *, 0x18FB8) = 0;
        func_0042C620(arg0, ~arg1, 0);
    }
}
