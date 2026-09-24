#include "common.h"
#include "m2c_macros.h"

s32 func_002026EC(s32, s32);
s32 func_002062F8(s32, s32);
s32 func_002065F8(s32, s32);
s32 func_0029B6F0(s32, s32, s32);
s32 func_0029B820(s32, s32, s32);
s32 func_0029DFF0(void);
void func_0029E010(s32);

extern M2C_UNK D_800D1CC0;
extern M2C_UNK D_800D8DB0;
extern s32 D_800D8DCC;

void func_002057B0(s32 arg0, void **arg1, void *arg2) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;
    void *temp_s0;

    temp_a0 = func_0029DFF0();
    temp_v1 = D_800D8DCC + 1;
    D_800D8DCC = temp_v1;
    if (temp_v1 != 1) {
        func_0029E010(temp_a0);
        func_0029B6F0((s32) &D_800D8DB0, 0, 1);
    } else {
        func_0029E010(temp_a0);
    }
    temp_s0 = *arg1;
    M2C_FIELD(temp_s0, s32 *, 0xC) = (s32) (M2C_FIELD(temp_s0, s32 *, 0xC) & ~2);
    if (M2C_FIELD(temp_s0, s32 *, 8) != 0) {
loop_4:
        do {
            temp_v0 = M2C_FIELD(temp_s0, s32 *, 8) - 1;
            M2C_FIELD(temp_s0, s32 *, 8) = temp_v0;
            if (temp_v0 != 0) {
                goto loop_4;
            }
            M2C_FIELD(temp_s0, s32 *, 0xC) = (s32) (M2C_FIELD(temp_s0, s32 *, 0xC) & ~0x100);
        } while (M2C_FIELD(temp_s0, s32 *, 8) != 0);
    }
    if (!(M2C_FIELD(temp_s0, s32 *, 0xC) & 0x702)) {
        func_002065F8(0, (s32) temp_s0);
        func_002026EC((s32) &D_800D1CC0, M2C_FIELD(temp_s0, s32 *, 0));
        func_002062F8(0, (s32) temp_s0);
    }
    *arg1 = arg2;
    temp_v0_2 = func_0029DFF0();
    temp_v1_2 = D_800D8DCC - 1;
    D_800D8DCC = temp_v1_2;
    if (temp_v1_2 != 0) {
        func_0029E010(temp_v0_2);
        func_0029B820((s32) &D_800D8DB0, 0, 1);
        return;
    }
    func_0029E010(temp_v0_2);
}
