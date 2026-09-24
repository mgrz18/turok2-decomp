#include "common.h"
#include "m2c_macros.h"

s32 func_00200518(s32, s32);
s32 func_00200738(s32, s32);
s32 func_00202A08(s32);

extern M2C_UNK D_800D8D68;
extern M2C_UNK D_800D8D7C;

void func_002065F8(s32 arg0, void *arg1) {
    void *temp_s0;
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg1, void **, 0x14);
    if (temp_v0 != NULL) {
        func_00202A08(M2C_FIELD(temp_v0, s32 *, 8));
        temp_s0 = M2C_FIELD(arg1, void **, 0x14);
        func_00200738((s32) &D_800D8D7C, (s32) temp_s0);
        M2C_FIELD(temp_s0, s32 *, 0x10) = 0;
        func_00200518((s32) &D_800D8D68, (s32) temp_s0);
    }
}
