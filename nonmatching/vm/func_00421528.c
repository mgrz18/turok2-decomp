/* NON-MATCHING: 88 of 88 words differ: m2c draft, not worked on (time) */
#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800C201C;

extern u8 D_800EBAC0;
extern M2C_UNK D_800F7078;
extern s32 D_80110060;
extern s32 D_8011AAD8;
extern u8 D_8011AC58;
extern M2C_UNK D_8011ACB0;

void func_00421528(s32 arg0) {
    s32 *var_a0_2;
    s32 *var_a1;
    s32 var_a2;
    u8 *var_a3;
    u8 *var_v1_2;
    u8 temp_v0;
    void *var_a0;
    void *var_v1;

    var_a0 = arg0 + 0x520;
    var_v1 = D_800C201C + 0x18;
    do {
        M2C_FIELD(var_a0, s32 *, 0) = (s32) M2C_FIELD(var_v1, s32 *, 0);
        M2C_FIELD(var_a0, s32 *, 4) = (s32) M2C_FIELD(var_v1, s32 *, 4);
        M2C_FIELD(var_a0, s32 *, 8) = (s32) M2C_FIELD(var_v1, s32 *, 8);
        M2C_FIELD(var_a0, s32 *, 0xC) = (s32) M2C_FIELD(var_v1, s32 *, 0xC);
        var_v1 += 0x10;
        var_a0 += 0x10;
    } while (var_v1 != (D_800C201C + 0x4F8));
    var_a1 = &D_8011AAD8;
    M2C_FIELD(var_a0, s32 *, 0) = (s32) M2C_FIELD(var_v1, s32 *, 0);
    M2C_FIELD(var_a0, s32 *, 4) = (s32) M2C_FIELD(var_v1, s32 *, 4);
    var_a0_2 = D_800C201C + 0x500;
    do {
        M2C_FIELD(var_a1, s32 *, 0) = M2C_FIELD(var_a0_2, s32 *, 0);
        M2C_FIELD(var_a1, s32 *, 4) = (s32) M2C_FIELD(var_a0_2, s32 *, 4);
        M2C_FIELD(var_a1, s32 *, 8) = (s32) M2C_FIELD(var_a0_2, s32 *, 8);
        M2C_FIELD(var_a1, s32 *, 0xC) = (s32) M2C_FIELD(var_a0_2, s32 *, 0xC);
        var_a0_2 = (__typeof__(var_a0_2))((s8 *)((__typeof__(var_a0_2))((s8 *)var_a0_2 + 0x10)));
        var_a1 = (__typeof__(var_a1))((s8 *)((__typeof__(var_a1))((s8 *)var_a1 + 0x10)));
    } while (var_a0_2 != (D_800C201C + 0x680));
    *var_a1 = *var_a0_2;
    M2C_FIELD(&D_8011ACB0, f32 *, 0) = (f32) M2C_FIELD(D_800C201C, f32 *, 8);
    var_a3 = &D_800EBAC0;
    M2C_FIELD(&D_8011ACB0, f32 *, 4) = (f32) M2C_FIELD(D_800C201C, f32 *, 0xC);
    var_a2 = 0;
    M2C_FIELD(&D_8011ACB0, f32 *, 8) = (f32) M2C_FIELD(D_800C201C, f32 *, 0x10);
    var_v1_2 = D_800C201C + 0x684;
    D_80110060 = M2C_FIELD(&D_8011ACB0, s32 *, -0x1D4);
    M2C_FIELD(&D_8011ACB0, u8 *, -0x58) = (u8) D_8011AC58;
    M2C_FIELD(&D_8011ACB0, f32 *, 0xC) = (f32) M2C_FIELD(D_800C201C, f32 *, 0x14);
    if (M2C_FIELD(&D_800F7078, s32 *, 0x30) > 0) {
        do {
            temp_v0 = *var_v1_2;
            var_v1_2 += 1;
            var_a2 += 1;
            *var_a3 = temp_v0;
            var_a3 += 1;
        } while (var_a2 < M2C_FIELD(&D_800F7078, s32 *, 0x30));
    }
}
