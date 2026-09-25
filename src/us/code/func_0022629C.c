#include "common.h"
#include "m2c_macros.h"

s32 func_00201848(s32, s32, s32);
s32 func_002052D8(s32, s32);
s32 func_00205B94(s32, s32, s32, s32);
s32 func_00205D0C(s32, s32, s32, s32, s32, s32, s32);

M2C_UNK func_0029E460(M2C_UNK, s32, s32);           /* extern */
extern M2C_UNK D_800A5EC0;
extern M2C_UNK D_800A5FD0;

void func_0022629C(void *arg0, s32 arg1, M2C_UNK arg2, s32 arg3) {
    s32 sp20;
    s32 temp_s2;
    s32 temp_v0;
    s32 var_a2;
    void *temp_v0_2;

    temp_v0 = func_00201848(M2C_FIELD(arg0, s32 *, 0x38), M2C_FIELD(arg0, s32 *, 0xC), arg1);
    temp_s2 = func_00205B94(0, (s32) &sp20, temp_v0, (s32) &D_800A5EC0);
    temp_v0_2 = func_00205D0C(0, sp20, 8, temp_v0, (s32) arg0, 0, (s32) &D_800A5FD0);
    var_a2 = M2C_FIELD(temp_v0_2, s32 *, 4);
    if (arg3 < var_a2) {
        var_a2 = arg3;
    }
    func_0029E460(arg2, M2C_FIELD(temp_v0_2, s32 *, 0), var_a2);
    func_002052D8(0, (s32) temp_v0_2);
    if (M2C_FIELD(arg0, s32 *, 0x18FA8) != arg1) {
        func_002052D8(0, temp_s2);
    }
}
