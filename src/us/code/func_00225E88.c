#include "common.h"
#include "m2c_macros.h"

extern s32 func_002017D4(s32, s32);

s32 func_00225E88(void *arg0, s32 arg1) {
    if (arg1 == -1) {
        return 0;
    }
    return func_002017D4(M2C_FIELD(arg0, s32 *, 0x64), arg1);
}
