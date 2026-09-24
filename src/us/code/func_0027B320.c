#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002051F4(M2C_UNK, s32);                /* extern */
s32 func_00224DF4(M2C_UNK *, s32, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_0027A7A0(s32, M2C_UNK, s16, s16, f32, f32, s32); /* extern */
extern s32 D_800B6D54;
extern s32 D_800C2214;
extern s32 D_800D8E24;
extern void *D_800F7070;
extern M2C_UNK D_800F7078;

s32 func_0027B320(s32 arg0, M2C_UNK arg1, s16 arg2, s16 arg3, f32 arg4, f32 arg5, s32 arg6) {
    s32 temp_v0;

    if (((u32) (D_800C2214 - ((u32) (D_800D8E24 - M2C_FIELD(D_800F7070, s32 *, 0x114)) >> 3)) < 0x3E8U) || (D_800B6D54 == 0) || (temp_v0 = func_00224DF4(&D_800F7078, arg0, 0, 1), (temp_v0 == 0))) {
        return 0;
    }
    func_0027A7A0(temp_v0, arg1, arg2, arg3, arg4, arg5, arg6);
    func_002051F4(0, temp_v0);
    return 1;
}
