#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

void func_0025E658(void *arg0, s32 arg1) {
    void *temp_a2;

    temp_a2 = M2C_FIELD(arg0, void **, 0x14);
    if ((M2C_FIELD(temp_a2, s32 *, 0) == 1) && !(M2C_FIELD(temp_a2, s32 *, 0x14) & 0x800)) {
        func_00243414((s32) arg0, arg1, 2);
    }
}
