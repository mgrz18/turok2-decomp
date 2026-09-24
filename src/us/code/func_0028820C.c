#include "common.h"
#include "m2c_macros.h"

s32 func_0029B060(s32, s32, s32, s32, s32, s32);
s32 func_0029BB10(s32);

M2C_UNK func_00266C5C(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
extern s32 D_800B6E74;
extern M2C_UNK D_800C7C00;
extern M2C_UNK D_800F6840;
extern M2C_UNK func_00288838;

void func_0028820C(void) {
    func_00266C5C(&D_800C7C00, 0, 0x80);
    func_0029B060((s32) &D_800F6840, 0, (s32) &func_00288838, 0, (s32) ((void *)((s8 *)&D_800C7C00 + 0x80)), D_800B6E74);
    func_0029BB10((s32) &D_800F6840);
}
