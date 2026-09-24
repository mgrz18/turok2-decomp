#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

void func_0025F380(s32 arg0, void *arg1) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(arg1, s32 *, 0x114);
    func_00243414(temp_a0, temp_a0 + 0x140, 1);
}
