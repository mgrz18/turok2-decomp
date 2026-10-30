/* NON-MATCHING: 113 of 131 words differ: m2c draft, not worked on (time) */
#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800C1720;
extern M2C_UNK D_800C1BD0;

s32 func_00253748(void *, s16);                     /* extern */
M2C_UNK func_00279028(s32);                         /* extern */
M2C_UNK func_0027B320(s32, M2C_UNK, s16, s16, f32, f32, s32); /* extern */
extern void *D_800D8E24;
static s32 D_800C1BD0 = 0xFFFFFFFF;                 /* const */

void func_00424780(void *arg0, void *arg1, s32 arg2, void *arg3) {
    s16 temp_a1;
    s16 temp_s0;
    s32 var_s1;
    s32 var_v0;
    s32 var_v1;
    void *temp_a0;
    void *temp_a1_2;
    void *temp_a2;
    void *temp_v0;
    void *temp_v1;

    temp_v0 = M2C_FIELD(arg0, void **, 0x18);
    temp_a0 = M2C_FIELD(arg3, void **, 0x1C);
    temp_a1 = M2C_FIELD(temp_v0, s16 *, 0x20);
    temp_s0 = M2C_FIELD(temp_v0, s16 *, 0x22);
    var_s1 = 0;
    if (temp_a1 != 0x10FE) {
        if (temp_a1 != 0x1194) {
            var_s1 = temp_s0 < func_00253748(temp_a0, temp_a1);
        } else {
            var_s1 = M2C_FIELD((temp_a0 + temp_s0), u8 *, 0x9FC) != 0;
        }
    } else if (temp_s0 >= 0) {
        if (temp_s0 >= 3) {
            if (temp_s0 < 6) {
                var_s1 = (temp_s0 - 3) < (s32) M2C_FIELD(temp_a0, u8 *, 0xA06);
            }
        } else {
            var_s1 = temp_s0 < (s32) M2C_FIELD(temp_a0, u8 *, 0xA05);
        }
    }
    if (D_800C1BD0 != 0x14) {
        D_800C1BD0 = 0x14;
        func_00279028((s32) (M2C_FIELD(arg3, f32 *, 0x30) * (M2C_FIELD(arg3, f32 *, 0x34) * 255.0f)));
    }
    temp_v1 = D_800D8E24;
    temp_a1_2 = D_800D8E24 + 8;
    temp_a2 = temp_a1_2 + 8;
    D_800D8E24 = temp_a1_2;
    M2C_FIELD(temp_v1, s32 *, 0) = 0xE7000000;
    M2C_FIELD(temp_v1, s32 *, 4) = 0;
    D_800D8E24 = temp_a2;
    M2C_FIELD(D_800D8E24, s32 *, 8) = 0xE3000A01;
    M2C_FIELD(temp_a1_2, s32 *, 4) = 0x100000;
    if (var_s1 != 0) {
        M2C_FIELD(temp_a1_2, s32 *, 8) = 0xFB000000;
        D_800D8E24 = temp_a1_2 + 0x10;
        var_v0 = (s32) (M2C_FIELD(arg3, f32 *, 0x30) * (M2C_FIELD(arg3, f32 *, 0x34) * M2C_FIELD(&D_800C1720, f32 *, 4)));
        var_v1 = -0x100;
    } else {
        M2C_FIELD(temp_a1_2, s32 *, 8) = 0xFB000000;
        var_v1 = 0x32323200;
        D_800D8E24 = temp_a1_2 + 0x10;
        var_v0 = (s32) (M2C_FIELD(arg3, f32 *, 0x30) * (M2C_FIELD(arg3, f32 *, 0x34) * 255.0f)) & 0xFF;
    }
    M2C_FIELD(temp_a2, s32 *, 4) = (s32) (var_v0 | var_v1);
    func_0027B320(M2C_FIELD(arg0, s32 *, 0x14), 0, M2C_FIELD(arg1, s16 *, 0x16), M2C_FIELD(arg1, s16 *, 0x1E), M2C_FIELD(arg1, f32 *, 0xC), M2C_FIELD(arg1, f32 *, 0x10), 1);
}
