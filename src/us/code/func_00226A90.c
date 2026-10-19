#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

char *func_002017D4(s32 *, s32);
s32 func_00201818(s32, s32);

s32 func_00226A90(void *arg0, s32 arg1, s32 arg2) {
    s32 *temp_v0;
    s32 temp_v1;
    s32 var_s1;
    s8 *temp_a0;

    var_s1 = arg2;
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(M2C_FIELD(arg0, s32 **, 0x6C), 0), arg1), 0);
    func_002017D4(temp_v0, 0);
    func_00201818((s32) temp_v0, 1);
    temp_a0 = func_002017D4(temp_v0, 1);
    temp_v1 = 1 << (var_s1 & 7);
    if (var_s1 < 0) {
        var_s1 += 7;
    }
    return ((u8) temp_a0[var_s1 >> 3] & temp_v1) != 0;
}
