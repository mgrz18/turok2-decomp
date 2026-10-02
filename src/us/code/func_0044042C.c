#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];
extern M2C_UNK D_8012F978;

void func_0044042C(s32 arg0, void *arg1) {
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        func_00243414(M2C_FIELD(&D_8012F978, s32 *, 0), M2C_FIELD(&D_8012F978, s32 *, 0) + 0x140, 1);
        func_00243414(M2C_FIELD(&D_8012F978, s32 *, -4), M2C_FIELD(&D_8012F978, s32 *, -4) + 0x140, 1);
        M2C_FIELD(&D_8012F978, s32 *, 0x64) = 8;
        D_800F1C40[0] |= 0x300;
        D_800F1F50[0] |= 0x300;
        func_00243414(arg0, (s32) arg1, 0x12);
    }
}
