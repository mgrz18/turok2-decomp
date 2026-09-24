#include "common.h"
#include "m2c_macros.h"

void func_00266A98(void *arg0, s32 arg1, void *arg2) {
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), u8 *, 0xE) = (u8) M2C_FIELD(arg2, u8 *, 7);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), u8 *, 0x10) = (u8) M2C_FIELD(arg2, u8 *, 7);
}
