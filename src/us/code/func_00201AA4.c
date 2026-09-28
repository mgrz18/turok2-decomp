#include "common.h"
#include "m2c_macros.h"

extern void func_0029B030(void *, void *, s32);

void func_00201AA4(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x1C) = 0;
    func_0029B030(arg0, (s8 *)arg0 + 0x18, 1);
}
