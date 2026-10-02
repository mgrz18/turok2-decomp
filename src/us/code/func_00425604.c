#include "common.h"
#include "m2c_macros.h"

void func_00425604(void *arg0) {
    s32 var_v1;
    void *var_v0;

    var_v1 = 2;
    var_v0 = arg0 + 8;
    M2C_FIELD(arg0, s32 *, 0) = 0;
    M2C_FIELD(arg0, s32 *, 4) = 0;
    M2C_FIELD(arg0, s32 *, 8) = 0;
    M2C_FIELD(arg0, s32 *, 0x18) = 0;
    M2C_FIELD(arg0, s32 *, 0x1C) = 0;
    do {
        M2C_FIELD(var_v0, s32 *, 0xC) = 0;
        var_v1 -= 1;
        var_v0 -= 4;
    } while (var_v1 >= 0);
}
