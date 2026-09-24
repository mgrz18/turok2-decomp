#include "common.h"
#include "m2c_macros.h"

s32 func_00220260(s32, s32, s32);
s32 func_00412314(s32, s32);

extern M2C_UNK D_800F7078;

void func_00266A30(s32 arg0, s32 arg1, s32 arg2) {
    func_00412314(arg0, arg2);
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2100);
    func_00220260((s32) &D_800F7078, arg0, 0);
}
