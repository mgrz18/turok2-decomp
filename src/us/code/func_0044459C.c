#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

void func_0044459C(s32 arg0, void *arg1) {
    M2C_FIELD(M2C_FIELD(M2C_FIELD(arg1, void **, 0x30), void **, 0xC), s8 *, 4) = 7;
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        func_00243414(arg0, (s32) arg1, 0xB);
    }
}
