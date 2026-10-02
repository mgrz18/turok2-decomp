#include "common.h"
#include "m2c_macros.h"

s32 func_00200738(s32, s32);
s32 func_002052D8(s32, s32);

void func_00417418(s32 arg0, void *arg1) {
    M2C_UNK (*temp_v0)(void *, s32);
    void *temp_v0_2;

    temp_v0 = M2C_FIELD(M2C_FIELD(arg1, void **, 0x14), M2C_UNK (**)(void *, s32), 0xC);
    if (temp_v0 != NULL) {
        temp_v0(arg1, arg0);
    }
    temp_v0_2 = M2C_FIELD(arg1, void **, 0x20);
    M2C_FIELD(temp_v0_2, s32 *, 0xB0) = 0;
    M2C_FIELD(temp_v0_2, s32 *, 0xB4) = 0;
    M2C_FIELD(temp_v0_2, s32 *, 0xBC) = 0;
    func_00200738(arg0, (s32) arg1);
    func_002052D8(0, M2C_FIELD(arg1, s32 *, 8));
}
