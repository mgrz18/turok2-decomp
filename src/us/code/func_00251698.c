#include "common.h"
#include "m2c_macros.h"

s32 func_0022425C(s32, s32);
s32 func_00225EBC(s32, s32, s32);

M2C_UNK func_00283FF8(s32, M2C_UNK, M2C_UNK *);     /* extern */
extern s32 D_800ACDE4;
extern s32 D_800AE340;
extern void *D_800F1FD0;
extern u8 D_800F1FE4[];
extern M2C_UNK D_800F7078;
extern s32 D_8011001C;
extern s32 D_80119878;
extern M2C_UNK func_00257784;

void func_00251698(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_s0;
    s32 var_s1;
    s32 var_v1;

    var_s0 = 1;
    temp_a1 = M2C_FIELD(arg0, s32 *, 0x10);
    var_s1 = 0;
    if (temp_a1 != 0) {
        temp_v0 = func_0022425C((s32) &D_800F7078, temp_a1);
        if (temp_v0 != 0) {
            var_s1 = M2C_FIELD(temp_v0, s32 *, 0x44);
        }
    }
    if (D_8011001C != 0) {
        var_s0 = 0;
    }
    temp_a0 = M2C_FIELD(arg0, s32 *, 0xA1C) & 0x8000;
    if (temp_a0 != 0) {
        var_s0 = 0;
    }
    if (D_8011001C != 0) {
        var_s0 = 0;
    }
    if (D_80119878 >= 2) {
        var_s0 = 0;
    }
    if (arg2 == 0xC) {
        var_s0 = 0;
    }
    if (temp_a0 != 0) {
        var_s0 = 0;
    }
    if (var_s1 & 0x01000000) {
        var_s0 = 0;
    }
    if ((var_s1 & 0x80000) && (arg2 != 0x14) && (arg2 != 0x28) && (arg2 != 0x1E)) {
        var_s0 = 0;
    }
    M2C_FIELD(arg0, s32 *, 0xBF0) = var_s0;
    if (var_s0 != 0) {
        if (arg2 == 0x32) {
            D_800AE340 = 1;
            var_v1 = 0;
            do {
                D_800F1FE4[var_v1] = M2C_FIELD((M2C_FIELD(arg0, s32 *, 0x51C) + var_v1), u8 *, 0x508);
                var_v1 += 1;
            } while (var_v1 < 4);
        }
        D_800ACDE4 = 0;
        D_800F1FD0 = arg0;
        func_00283FF8(arg2, 0, &func_00257784);
    }
    temp_v0_2 = func_00225EBC((s32) &D_800F7078, -1, 0xC45);
    if (temp_v0_2 != 0) {
        M2C_FIELD(arg0, f32 *, 0x34) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x38);
        M2C_FIELD(arg0, f32 *, 0x38) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x3C);
        M2C_FIELD(arg0, f32 *, 0x3C) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x40);
    }
}
