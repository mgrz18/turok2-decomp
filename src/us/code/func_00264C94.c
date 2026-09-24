#include "common.h"
#include "m2c_macros.h"

s32 func_00264994(s32, s32);
s32 func_00264CEC(s32, s32);
s32 func_00264D98(s32, s32);

extern M2C_UNK D_800B21E0;

void func_00264C94(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, M2C_UNK **, 0x2C) = &D_800B21E0;
    M2C_FIELD(arg1, s32 (**)(s32, s32), 0xF0) = func_00264D98;
    M2C_FIELD(arg1, s32 (**)(s32, s32), 0xFC) = func_00264CEC;
    M2C_FIELD(arg1, void **, 0x104) = (void *) (func_00264994 + 0xC);
}
