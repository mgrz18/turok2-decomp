#include "common.h"
#include "m2c_macros.h"

s32 func_002065CC(s32, s32);
s32 func_0029B6F0(s32, s32, s32);
s32 func_0029B820(s32, s32, s32);
s32 func_0029DFF0(void);
void func_0029E010(s32);

extern M2C_UNK D_800D8DB0;
extern s32 D_800D8DCC;

void func_00205110(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = func_0029DFF0();
    temp_v1 = D_800D8DCC + 1;
    D_800D8DCC = temp_v1;
    if (temp_v1 != 1) {
        func_0029E010(temp_a0);
        func_0029B6F0((s32) &D_800D8DB0, 0, 1);
    } else {
        func_0029E010(temp_a0);
    }
    func_002065CC(0, arg1);
    M2C_FIELD(arg1, s32 *, 8) = (s32) (M2C_FIELD(arg1, s32 *, 8) + 1);
    M2C_FIELD(arg1, s32 *, 0xC) = (s32) (M2C_FIELD(arg1, s32 *, 0xC) | 0x100);
    temp_v0 = func_0029DFF0();
    temp_v1_2 = D_800D8DCC - 1;
    D_800D8DCC = temp_v1_2;
    if (temp_v1_2 != 0) {
        func_0029E010(temp_v0);
        func_0029B820((s32) &D_800D8DB0, 0, 1);
        return;
    }
    func_0029E010(temp_v0);
}
