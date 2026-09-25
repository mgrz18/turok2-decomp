#include "common.h"
#include "m2c_macros.h"

s32 func_00200518(s32, s32);
s32 func_00200738(s32, s32);

extern M2C_UNK D_800D8D7C;

void func_002066B0(s32 arg0, s32 arg1) {
    func_00200738((s32) &D_800D8D7C, arg1);
    M2C_FIELD(arg1, s32 *, 0x10) = 0;
    func_00200518((s32) (((__typeof__(&D_800D8D7C))((s8 *)&D_800D8D7C - 0x14))), arg1);
}
