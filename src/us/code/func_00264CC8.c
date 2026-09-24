#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

void func_00264CC8(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, s32 *, 0x110) = 0;
    M2C_FIELD(arg1, s32 *, 0x114) = 0;
    func_00243414(arg0, (s32) arg1, 0);
}
