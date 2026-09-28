#include "common.h"
#include "m2c_macros.h"

extern s32 func_002017D4(s32, s32);

void *func_00226160(void *arg0, s32 arg1, s32 arg2) {
    s32 *t = (s32 *) func_002017D4(func_002017D4(M2C_FIELD(arg0, s32 *, 0x68), arg1), 1);

    return (s8 *)t + (arg2 * *t + 8);
}
