#include "common.h"
#include "m2c_macros.h"

s32 func_002052D8(s32, s32);

s32 func_002532A8(M2C_UNK *);                       /* extern */
M2C_UNK func_002758DC(M2C_UNK);                     /* extern */
M2C_UNK func_00275B58();                            /* extern */
M2C_UNK func_00275EFC(s32);                         /* extern */
s32 func_00275F7C();                                /* extern */
M2C_UNK func_0042EA24(s32);                         /* extern */
extern M2C_UNK D_80119870;

void func_004658FC(void *arg0) {
    s32 temp_v0;

    temp_v0 = func_002532A8(&D_80119870);
    if (temp_v0 != 0) {
        func_0042EA24(temp_v0);
    }
    func_00275EFC(func_00275F7C());
    func_00275B58();
    func_002758DC(0x1000);
    func_002052D8(0, M2C_FIELD(arg0, s32 *, 0x98));
    func_002052D8(0, M2C_FIELD(arg0, s32 *, 0x9C));
    func_002052D8(0, M2C_FIELD(arg0, s32 *, 0xA0));
    func_002052D8(0, M2C_FIELD(arg0, s32 *, 0xA4));
    func_002052D8(0, M2C_FIELD(arg0, s32 *, 0xA8));
    func_002052D8(0, M2C_FIELD(arg0, s32 *, 0xB0));
    func_002052D8(0, M2C_FIELD(arg0, s32 *, 0xB4));
    func_002052D8(0, M2C_FIELD(arg0, s32 *, 0xCC));
    func_002052D8(0, M2C_FIELD(arg0, s32 *, 0xAC));
    func_002052D8(0, M2C_FIELD(arg0, s32 *, 0xC8));
}
