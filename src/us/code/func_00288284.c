#include "common.h"
#include "m2c_macros.h"

s32 func_0029B060(s32, s32, s32, s32, s32, s32);
s32 func_0029B9B0(s32, s32);
s32 func_0029BB10(s32);

M2C_UNK func_00266C5C(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
extern s32 D_800B6E70;
extern s32 D_800B6E84;
extern M2C_UNK D_800C7C80;
extern M2C_UNK D_800CFC80;
extern M2C_UNK D_800F6A70;
extern M2C_UNK func_00288860;

void func_00288284(void *arg0, s32 arg1) {
    func_00266C5C(&D_800C7C80, 1, 0x8000);
    func_0029B060((s32) &D_800F6A70, 1, (s32) &func_00288860, arg1, (s32) &D_800CFC80, D_800B6E84);
    func_0029BB10((s32) &D_800F6A70);
    {
        s32 v = D_800B6E70;

        M2C_FIELD(arg0, s32 *, 0x23FE8) = 0;
        func_0029B9B0(0, v);
    }
loop_1:
    goto loop_1;
}
