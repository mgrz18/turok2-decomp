#include "common.h"
#include "m2c_macros.h"

s32 func_0020185C(s32, s32, s32, s32);
s32 func_0020367C(s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_00266D24(s32, s32, s32);

extern M2C_UNK D_00200AE0;
extern M2C_UNK D_800A5F38;

s32 func_00224D54(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp20;
    s32 temp_v0;
    void *temp_a0;

    temp_a0 = M2C_FIELD(arg0, void **, 0x78);
    temp_v0 = func_00266D24((s32) (temp_a0 + 8), M2C_FIELD(temp_a0, s32 *, 4), arg1);
    if (temp_v0 != -1) {
        return func_0020367C(0, func_0020185C(M2C_FIELD(arg0, s32 *, 0x44), M2C_FIELD(arg0, s32 *, 0x18), temp_v0, (s32) &sp20), sp20, arg2, 0, (s32) &D_00200AE0, (s32) &D_800A5F38, arg3);
    }
    return 0;
}
