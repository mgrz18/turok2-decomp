#include "common.h"
#include "m2c_macros.h"

s32 CList__AddHead(s32, s32);
s32 CList__Remove(s32, s32);

extern M2C_UNK D_800D8D7C;

void func_002066B0(s32 arg0, s32 arg1) {
    CList__Remove((s32) &D_800D8D7C, arg1);
    M2C_FIELD(arg1, s32 *, 0x10) = 0;
    CList__AddHead((s32) (((__typeof__(&D_800D8D7C))((s8 *)&D_800D8D7C - 0x14))), arg1);
}
