#include "common.h"
#include "m2c_macros.h"

s32 func_0021A7EC(s32);
s32 func_00220260(s32, s32, s32);
s32 func_002666B0(s32, s32, s32);

extern M2C_UNK D_800F7078;

void func_002668D4(s32 arg0, s32 arg1, void *arg2) {
    func_002666B0(arg0, arg1, (s32) M2C_FIELD(arg2, u16 *, 6));
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2100);
    func_0021A7EC(arg0);
    func_00220260((s32) &D_800F7078, arg0, 0);
}
