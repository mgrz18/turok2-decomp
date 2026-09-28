#include "common.h"
#include "m2c_macros.h"

s32 func_00298A0C(void);
s32 func_00298BE0(s32);
s32 func_00298C98(s32, s32);
s32 func_00299FE4(void);
s32 func_0029A080(s32, s32);
s32 func_0029B6F0(s32, s32, s32);

M2C_UNK func_0029A050();                            /* extern */
extern M2C_UNK D_801213E0;
extern u8 D_80121420[];

s32 func_00298900(s32 arg0, s32 arg1) {
    M2C_UNK sp10;
    s32 temp_s0;

    func_00299FE4();
    if (D_80121420[0] != 0xFF) {
        func_00298A0C();
        func_0029A080(1, (s32) &D_801213E0);
        func_0029B6F0(arg0, 0, 1);
        func_0029A080(0, (s32) &D_801213E0);
        func_0029B6F0(arg0, 0, 1);
        func_00298BE0(0xFF);
        func_0029A080(1, (s32) &D_801213E0);
        func_0029B6F0(arg0, 0, 1);
        D_80121420[0] = 0xFF;
    }
    temp_s0 = func_0029A080(0, (s32) &D_801213E0);
    func_0029B6F0(arg0, 0, 1);
    func_00298C98((s32) &sp10, arg1);
    func_0029A050();
    return temp_s0;
}
