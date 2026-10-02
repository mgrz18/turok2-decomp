#include "common.h"
#include "m2c_macros.h"

extern void *D_800C1BB0;

void func_00412B14(void) {
    M2C_FIELD(D_800C1BB0, s32 *, 0xB4) = 1;
}
