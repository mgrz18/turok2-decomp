/* NON-MATCHING: 1408 of 1430 words differ: raw m2c draft only (with the two jump tables jtbl_800C0268/jtbl_800C0298 fed to m2c), not worked on; display-list renderer, 0x1658 bytes. */
#include "common.h"
#include "m2c_macros.h"

void *func_002017D4();   /* extern */
M2C_UNK func_0020C208(M2C_UNK *, M2C_UNK *);        /* extern */
M2C_UNK func_0020D274(M2C_UNK *, void *);           /* extern */
M2C_UNK func_0020F85C(M2C_UNK *, f32 *, f32 *);     /* extern */
M2C_UNK func_0020FC24(M2C_UNK *, f32, f32, M2C_UNK); /* extern */
M2C_UNK func_0020FC74(M2C_UNK *, f32, f32, M2C_UNK); /* extern */
M2C_UNK func_00210318(M2C_UNK *, f32, f32, M2C_UNK); /* extern */
M2C_UNK func_00210478(M2C_UNK *, f32, f32, M2C_UNK); /* extern */
M2C_UNK func_00210B38(M2C_UNK *, f32);              /* extern */
f32 func_002974C0(f32);                             /* extern */
M2C_UNK func_00297D50(M2C_UNK *, M2C_UNK, f32, M2C_UNK, f32, f32, f32, f32); /* extern */
f32 func_002982F0(f32); /* extern */
extern M2C_UNK D_800AB100;
extern M2C_UNK D_800AB130;
extern s32 D_800B6D1C;
extern s32 D_800B6D54;
extern M2C_UNK D_800C0230;
extern M2C_UNK D_800C0238;
extern M2C_UNK D_800C0240;
extern M2C_UNK D_800C0248;
extern f32 D_800C0250;
extern f32 D_800C0254;
extern M2C_UNK D_800C0258;
extern f32 D_800C0260;
extern f32 D_800C0294;
extern M2C_UNK D_800C02C0;
extern M2C_UNK D_800C02C8;
extern M2C_UNK D_800C02D0;
extern f32 D_800C02D8;
extern f32 D_800C02DC;
extern M2C_UNK D_800C02E0;
extern M2C_UNK D_800C02E8;
extern f32 D_800C02F0;
extern f32 D_800C02F4;
extern f32 D_800C02F8;
extern f32 D_800C02FC;
extern f32 D_800C0300;
extern f32 D_800C0304;
extern f32 D_800C0308;
extern f32 D_800C030C;
extern f32 D_800C0310;
extern s32 D_800C2030;
extern s32 D_800C2034;
extern s32 *D_800D8E24;
extern void *D_800F7070;
extern M2C_UNK D_800F7078;
extern s32 D_8011AAD8;
extern s32 D_8011AC8C;
extern M2C_UNK D_8011B0A8;
extern M2C_UNK D_80129100;
extern M2C_UNK D_80129180;
extern M2C_UNK D_801291FE;
extern M2C_UNK D_80129200;
extern u16 D_80129202;
extern M2C_UNK D_80129A00;
extern M2C_UNK D_80129A40;
extern M2C_UNK D_8012DA40;
extern M2C_UNK D_8012F340;

void func_004004D8(void *arg0) {
    s32 sp20;
    s32 sp28;
    s32 sp24;
    s32 sp134;
    M2C_UNK sp30;
    M2C_UNK sp70;
    M2C_UNK spB0;
    f32 spF0;
    f32 spF4;
    f32 spF8;
    f32 spFC;
    f32 sp100;
    f32 sp104;
    f32 sp108;
    f32 sp10C;
    f32 sp110;
    f32 sp114;
    f32 sp118;
    f32 sp11C;
    f32 sp120;
    f32 sp124;
    f32 sp128;
    f32 sp12C;
    f32 sp130;
    M2C_UNK sp13C;
    s32 sp160;
    s32 sp164;
    void *sp168;
    s32 sp170;
    M2C_UNK *var_v1;
    f32 *var_s2;
    f32 *var_s3;
    f32 *var_v0_2;
    f32 *var_v1_3;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f1_3;
    f32 temp_f1_4;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f20_3;
    f32 temp_f20_4;
    f32 temp_f21;
    f32 temp_f21_2;
    f32 temp_f21_3;
    f32 temp_f22;
    f32 temp_f22_2;
    f32 temp_f22_3;
    f32 temp_f23;
    f32 temp_f25;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f3;
    f32 temp_f3_2;
    f32 temp_f4;
    f32 temp_f4_2;
    f32 temp_f5;
    f32 var_f0;
    f32 var_f0_10;
    f32 var_f0_2;
    f32 var_f0_3;
    f32 var_f0_4;
    f32 var_f0_5;
    f32 var_f0_6;
    f32 var_f0_7;
    f32 var_f0_8;
    f32 var_f0_9;
    f32 var_f1;
    s16 *var_t2;
    s16 *var_t2_2;
    s16 temp_v1_7;
    s16 var_a2_4;
    s16 var_t1;
    s16 var_t2_3;
    s32 *temp_a0;
    s32 *temp_a0_2;
    s32 *temp_a0_5;
    s32 *temp_a1;
    s32 *temp_a1_3;
    s32 *temp_a1_4;
    s32 *temp_a1_5;
    s32 *temp_a1_6;
    s32 *temp_a2;
    s32 *temp_a2_2;
    s32 *temp_a3;
    s32 *temp_a3_2;
    s32 *temp_t0;
    s32 *temp_t0_2;
    s32 *temp_t0_5;
    s32 *temp_t1;
    s32 *temp_t1_2;
    s32 *temp_t1_3;
    s32 *temp_t2;
    s32 *temp_t2_2;
    s32 *temp_t3;
    s32 *temp_t4;
    s32 *temp_v0;
    s32 *temp_v0_5;
    s32 *temp_v1;
    s32 *temp_v1_11;
    s32 *temp_v1_12;
    s32 *temp_v1_13;
    s32 *temp_v1_2;
    s32 *temp_v1_3;
    s32 *temp_v1_6;
    s32 *var_v1_2;
    s32 temp_a1_2;
    s32 temp_f8;
    s32 temp_f8_2;
    s32 temp_t9;
    s32 temp_v0_4;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_a1;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_a2_3;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_condition_bit;
    s32 var_condition_bit_2;
    s32 var_condition_bit_3;
    s32 var_condition_bit_4;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_4;
    s32 var_s3_2;
    s32 var_s5;
    s32 var_s7;
    s32 var_t1_2;
    s32 var_t3;
    s32 var_t4_2;
    s32 var_t5;
    s32 var_t6;
    s32 var_t6_2;
    s32 var_v0;
    s32 var_v0_3;
    s32 var_v1_10;
    s32 var_v1_11;
    s32 var_v1_12;
    s32 var_v1_6;
    s32 var_v1_7;
    s32 var_v1_8;
    s32 var_v1_9;
    s8 temp_v1_10;
    s8 temp_v1_5;
    s8 temp_v1_8;
    s8 temp_v1_9;
    s8 var_v0_4;
    s8 var_v0_5;
    s8 var_v1_4;
    s8 var_v1_5;
    u16 *temp_v0_3;
    u16 *var_t0;
    u16 temp_v1_4;
    u32 var_s1_3;
    u8 temp_a0_3;
    u8 temp_a0_4;
    void *temp_s2;
    void *temp_s2_2;
    void *temp_s2_3;
    void *temp_s4;
    void *temp_t0_3;
    void *temp_t0_4;
    void *temp_t3_2;
    void *temp_v0_2;
    void *var_a0;
    void *var_a1_2;
    void *var_s0;
    void *var_t4;
    void *var_t7;

    sp168 = (D_800B6D1C * 0x10) + &D_8011B0A8;
    if (D_8011AC8C != 1) {
        func_00297D50(&D_80129A00, 0, (f32) (D_800C2030 - 1), 0, (f32) (D_800C2034 - 1), M2C_FIELD(&D_800C0230, f32 *, 0), M2C_FIELD(&D_800C0230, f32 *, 4), M2C_FIELD(&D_800C0238, f32 *, 0));
        if ((M2C_FIELD(arg0, s32 *, 0xC) != 0) && (D_800B6D54 != 0)) {
            sp170 = 0xDB04001C;
            temp_v1 = D_800D8E24;
            temp_a0 = D_800D8E24 + 8;
            temp_a1 = temp_a0 + 8;
            D_800D8E24 = temp_a0;
            M2C_FIELD(temp_v1, s32 *, 0) = 0xDB060000;
            M2C_FIELD(temp_v1, s32 *, 4) = 0;
            temp_t0 = temp_a0 + 0x10;
            D_800D8E24 = temp_a1;
            M2C_FIELD(temp_a0, M2C_UNK **, 4) = &D_800AB100;
            temp_t4 = temp_a0 + 0x18;
            M2C_FIELD(D_800D8E24, s32 *, 8) = 0xDE000000;
            D_800D8E24 = temp_t0;
            M2C_FIELD(temp_a0, s32 *, 8) = 0xDE000000;
            M2C_FIELD(temp_a1, M2C_UNK **, 4) = &D_800AB130;
            temp_t2 = temp_a0 + 0x20;
            temp_t3 = temp_a0 + 0x28;
            temp_t1 = temp_a0 + 0x30;
            D_800D8E24 = temp_t4;
            M2C_FIELD(temp_a0, s32 *, 0x10) = 0xED000000;
            D_800D8E24 = temp_t2;
            temp_a3 = temp_a0 + 0x38;
            D_800D8E24 = temp_t3;
            temp_a1_2 = D_800C2030 - 1;
            D_800D8E24 = temp_t1;
            D_800D8E24 = temp_a3;
            M2C_FIELD(temp_t0, s32 *, 4) = (s32) ((((s32) ((f32) temp_a1_2 * M2C_FIELD(&D_800C0238, f32 *, 4)) & 0xFFF) << 0xC) | ((s32) ((f32) (D_800C2034 - 1) * M2C_FIELD(&D_800C0238, f32 *, 4)) & 0xFFF));
            temp_v1_2 = temp_a0 + 0x40;
            M2C_FIELD(temp_a0, s32 *, 0x18) = (s32) ((temp_a1_2 & 0xFFF) | 0xFF100000);
            temp_a1_3 = temp_a0 + 0x48;
            D_800D8E24 = temp_v1_2;
            temp_t0_2 = temp_a0 + 0x60;
            D_800D8E24 = temp_a1_3;
            M2C_FIELD(temp_t4, s32 *, 4) = (s32) M2C_FIELD(D_800F7070, s32 *, 0x110);
            M2C_FIELD(temp_a0, s32 *, 0x20) = 0xDC080008;
            M2C_FIELD(temp_t2, void **, 4) = sp168;
            M2C_FIELD(temp_a0, s32 *, 0x28) = 0xE7000000;
            temp_t2_2 = temp_a0 + 0x50;
            M2C_FIELD(temp_t3, s32 *, 4) = 0;
            M2C_FIELD(temp_a0, s32 *, 0x30) = 0xE3000A01;
            M2C_FIELD(temp_t1, s32 *, 4) = 0;
            M2C_FIELD(temp_a0, s32 *, 0x38) = 0xE200001C;
            M2C_FIELD(temp_a3, s32 *, 4) = 0x507048;
            M2C_FIELD(temp_a0, s32 *, 0x40) = 0xFCFFFFFF;
            M2C_FIELD(temp_v1_2, s32 *, 4) = 0xFFFE793C;
            M2C_FIELD(temp_a0, s32 *, 0x48) = 0xD9000000;
            temp_t1_2 = temp_a0 + 0x58;
            M2C_FIELD(temp_a1_3, s32 *, 4) = 0;
            M2C_FIELD(temp_a0, s32 *, 0x50) = 0xD9FFFFFF;
            M2C_FIELD(temp_t2_2, s32 *, 4) = 0x200004;
            M2C_FIELD(temp_a0, s32 *, 0x58) = 0xD7000000;
            temp_a3_2 = temp_a0 + 0x68;
            temp_a1_4 = temp_a0 + 0x70;
            M2C_FIELD(temp_t1_2, s32 *, 4) = 0;
            M2C_FIELD(temp_a0, s32 *, 0x60) = 0xDB040004;
            M2C_FIELD(temp_t0_2, s32 *, 4) = 6;
            M2C_FIELD(temp_a0, s32 *, 0x68) = 0xDB04000C;
            M2C_FIELD(temp_a3_2, s32 *, 4) = 6;
            M2C_FIELD(temp_a0, s32 *, 0x70) = 0xDB040014;
            M2C_FIELD(temp_a1_4, s32 *, 4) = 0xFFFA;
            temp_v1_3 = temp_a0 + 0x78;
            M2C_FIELD(temp_a0, s32 *, 0x78) = 0xDB04001C;
            M2C_FIELD(temp_v1_3, s32 *, 4) = 0xFFFA;
            temp_v0 = temp_a0 + 0x80;
            M2C_FIELD(temp_a0, s32 *, 0x80) = 0xDA380007;
            M2C_FIELD(temp_v0, M2C_UNK **, 4) = &D_80129A00;
            temp_f21 = M2C_FIELD(arg0, f32 *, 0x14);
            temp_f20 = M2C_FIELD(arg0, f32 *, 0x1C);
            temp_a0_2 = temp_a0 + 0x88;
            D_800D8E24 = temp_t2_2;
            D_800D8E24 = temp_t1_2;
            D_800D8E24 = temp_t0_2;
            D_800D8E24 = temp_a3_2;
            D_800D8E24 = temp_a1_4;
            D_800D8E24 = temp_v1_3;
            D_800D8E24 = temp_v0;
            D_800D8E24 = temp_a0_2;
            temp_f25 = func_002982F0(M2C_FIELD(arg0, f32 *, 0x10));
            temp_f0 = func_002974C0(M2C_FIELD(arg0, f32 *, 0x10));
            temp_f2 = M2C_FIELD(arg0, f32 *, 0x28);
            temp_f1 = M2C_FIELD(arg0, f32 *, 0x20);
            temp_f21_2 = temp_f21 + ((temp_f2 * temp_f25) - (temp_f1 * temp_f0));
            temp_f22 = M2C_FIELD(&D_800C0240, f32 *, 0);
            temp_f20_2 = temp_f20 + ((temp_f2 * temp_f0) + (temp_f1 * temp_f25));
            var_f1 = M2C_FIELD(&D_800C0240, f32 *, 4);
            temp_f23 = temp_f22;
            if ((var_f1 < temp_f21_2) || (var_f0 = M2C_FIELD(&D_800C0248, f32 *, 0), !(temp_f21_2 < var_f0))) {
                var_f0 = temp_f21_2;
                if (var_f1 < var_f0) {
                    var_f0 = var_f1;
                }
                var_f1 = M2C_FIELD(&D_800C0248, f32 *, 4);
            }
            if ((var_f1 < temp_f20_2) || (var_f0_2 = D_800C0250, !(temp_f20_2 < var_f0_2))) {
                var_f0_2 = temp_f20_2;
                if (var_f1 < var_f0_2) {
                    var_f0_2 = var_f1;
                }
            }
            func_0020FC24(&sp30, -var_f0, -var_f0_2, 0);
            func_00210478(&sp30, temp_f23, temp_f22, 0x3F800000);
            var_s1 = 0;
            var_s3 = &sp130;
            var_s2 = &sp100;
            func_00210B38(&sp30, M2C_FIELD(arg0, f32 *, 0x10) + D_800C0254);
            func_00210318(&sp30, (f32) ((s32) D_800C2030 / 2), (f32) ((s32) D_800C2034 / 2) - M2C_FIELD(&D_800C0258, f32 *, 0), 0);
            func_0020C208(&sp70, &sp30);
            temp_f0_2 = (f32) D_800C2030;
            sp10C = temp_f0_2;
            sp118 = temp_f0_2;
            temp_f0_3 = (f32) D_800C2034;
            sp100 = 0.0f;
            sp104 = 0.0f;
            sp108 = 0.0f;
            sp110 = 0.0f;
            sp114 = 0.0f;
            sp11C = temp_f0_3;
            sp120 = 0.0f;
            sp124 = 0.0f;
            sp128 = temp_f0_3;
            sp12C = 0.0f;
            do {
                func_0020F85C(&sp70, var_s2, var_s3);
                var_s3 += 0xC;
                var_s1 += 1;
                var_s2 += 0xC;
            } while (var_s1 < 4);
            var_s1_2 = 1;
            var_v1 = &sp13C;
            spF8 = sp130;
            spF0 = sp130;
            spFC = sp134;
            spF4 = sp134;
            do {
                var_f0_3 = M2C_FIELD(var_v1, f32 *, 0);
                if (!(var_f0_3 <= spF0)) {
                    var_f0_3 = spF0;
                }
                spF0 = var_f0_3;
                var_f0_4 = M2C_FIELD(var_v1, f32 *, 4);
                if (!(var_f0_4 <= spF4)) {
                    var_f0_4 = spF4;
                }
                spF4 = var_f0_4;
                var_f0_5 = M2C_FIELD(var_v1, f32 *, 0);
                if (!(spF8 <= var_f0_5)) {
                    var_f0_5 = spF8;
                }
                spF8 = var_f0_5;
                var_f0_6 = M2C_FIELD(var_v1, f32 *, 4);
                if (!(spFC <= var_f0_6)) {
                    var_f0_6 = spFC;
                }
                spFC = var_f0_6;
                var_s1_2 += 1;
                var_v1 += 0xC;
            } while (var_s1_2 < 4);
            func_0020D274(&sp30, (D_800B6D1C << 6) + &D_80129100);
            temp_a2 = D_800D8E24 + 8;
            M2C_FIELD(D_800D8E24, s32 *, 0) = 0xDA380003;
            M2C_FIELD(D_800D8E24, void **, 4) = (void *) ((D_800B6D1C << 6) + &D_80129100);
            temp_s2 = (D_800B6D1C << 0xD) + &D_80129A40;
            D_800D8E24 = temp_a2;
            sp160 = func_002017D4(M2C_FIELD(&D_800F7078, s32 *, 0x58), 2, temp_a2, &D_800D8E24) + 8;
            temp_v0_2 = func_002017D4(M2C_FIELD(&D_800F7078, s32 *, 0x58), 3);
            var_s0 = temp_v0_2 + 8;
            sp164 = M2C_FIELD(temp_v0_2, s32 *, 4);
            var_t6 = 0;
            var_s5 = 0;
            var_s7 = 0;
            var_t1 = 0;
            temp_t9 = func_002017D4(M2C_FIELD(&D_800F7078, s32 *, 0x58), 0) + 8;
            sp170 = (u32) (D_8011AAD8 & 0x2000) > 0U;
            if (sp164 > 0) {
loop_27:
                if ((spF0 < M2C_FIELD(var_s0, f32 *, 8)) && (M2C_FIELD(var_s0, f32 *, 0) < spF8) && (spF4 < M2C_FIELD(var_s0, f32 *, 0xC)) && (M2C_FIELD(var_s0, f32 *, 4) < spFC)) {
                    var_s1_3 = 0;
                    if (M2C_FIELD(var_s0, u32 *, 0x10) != 0) {
                        var_t4 = (var_s5 << 5) + sp160;
loop_33:
                        temp_s4 = temp_t9 + (M2C_FIELD(var_t4, u16 *, 0) * 0x64);
                        if (!(M2C_FIELD(temp_s4, s32 *, 0x44) & 0x200)) {
                            var_a1 = 0;
                            var_v1_2 = &sp20;
                            var_a0 = var_t4;
                            do {
                                temp_t0_3 = M2C_FIELD(var_a0, void **, 4);
                                temp_f1_2 = M2C_FIELD(temp_t0_3, f32 *, 0);
                                if (temp_f1_2 < spF0) {
                                    *var_v1_2 = 1;
                                } else if (spF8 <= temp_f1_2) {
                                    *var_v1_2 = 4;
                                } else {
                                    *var_v1_2 = 0;
                                }
                                temp_f1_3 = M2C_FIELD(temp_t0_3, f32 *, 8);
                                if (temp_f1_3 < spF4) {
                                    var_v0 = *var_v1_2 | 8;
                                    goto block_44;
                                }
                                if (spFC <= temp_f1_3) {
                                    var_v0 = *var_v1_2 | 2;
block_44:
                                    *var_v1_2 = var_v0;
                                }
                                var_v1_2 += 4;
                                var_a1 += 1;
                                var_a0 += 4;
                            } while (var_a1 < 3);
                            if (!(sp20 & sp24 & sp28) && ((var_t5 = 0, (sp170 != 0)) || (M2C_FIELD(var_t4, u16 *, 2) & 0x400))) {
                                var_t7 = var_t4;
                                var_s3_2 = 0;
                                var_a1_2 = (var_t1 * 0x10) + temp_s2;
                                var_t2 = (var_t6 * 2) + &D_80129200;
loop_50:
                                temp_v0_3 = M2C_FIELD(var_t7, u16 **, 0x10);
                                var_a0_2 = 0;
                                if (temp_v0_3 == NULL) {
                                    var_a0_2 = 1;
                                } else if ((u16) M2C_FIELD(var_t4, u16 *, 0) < (u16) *temp_v0_3) {
                                    var_a0_2 = 1;
                                }
                                if (var_a0_2 != 0) {
                                    var_a0_3 = 0;
                                    if (var_t5 != 2) {
                                        var_a0_3 = var_t5 + 1;
                                    }
                                    if (!(*(&sp20 + var_s3_2) & (&sp20)[var_a0_3])) {
                                        var_a2 = 0;
                                        var_v1_3 = &sp130;
                                        var_a3 = 0xC;
                                        temp_t0_4 = M2C_FIELD(var_t7, void **, 4);
                                        temp_t3_2 = M2C_FIELD((var_t4 + (var_a0_3 * 4)), void **, 4);
loop_59:
                                        var_v0_2 = &sp130;
                                        if (var_a2 != 3) {
                                            var_v0_2 += var_a3;
                                        }
                                        temp_f3 = M2C_FIELD(var_v1_3, f32 *, 4);
                                        temp_f2_2 = M2C_FIELD(var_v1_3, f32 *, 0);
                                        temp_f5 = M2C_FIELD(var_v0_2, f32 *, 4) - temp_f3;
                                        temp_f4 = temp_f2_2 - M2C_FIELD(var_v0_2, f32 *, 0);
                                        var_v1_3 += 0xC;
                                        if (!(((temp_f5 * (M2C_FIELD(temp_t0_4, f32 *, 0) - temp_f2_2)) + (temp_f4 * (M2C_FIELD(temp_t0_4, f32 *, 8) - temp_f3))) > 0.0f) || (var_v0_3 = 0, !(((temp_f5 * (M2C_FIELD(temp_t3_2, f32 *, 0) - temp_f2_2)) + (temp_f4 * (M2C_FIELD(temp_t3_2, f32 *, 8) - temp_f3))) > 0.0f))) {
                                            var_a2 += 1;
                                            var_a3 += 0xC;
                                            if (var_a2 >= 4) {
                                                var_v0_3 = 1;
                                            } else {
                                                goto loop_59;
                                            }
                                        }
                                        if (var_v0_3 != 0) {
                                            var_a0_4 = 1;
                                            if ((var_t1 != 0) && (M2C_FIELD(var_a1_2, s16 *, -0x10) == (s16) (s32) M2C_FIELD(temp_t0_4, f32 *, 0))) {
                                                var_a0_4 = M2C_FIELD((var_a1_2 - 0x10), s16 *, 2) != (s16) (s32) M2C_FIELD(temp_t0_4, f32 *, 8);
                                            }
                                            if (var_a0_4 != 0) {
                                                *var_t2 = var_t1;
                                                M2C_FIELD(var_a1_2, s16 *, 0) = (s16) (s32) M2C_FIELD(temp_t0_4, f32 *, 0);
                                                M2C_FIELD(var_a1_2, s16 *, 2) = (s16) (s32) M2C_FIELD(temp_t0_4, f32 *, 8);
                                                var_t2_2 = var_t2 + 2;
                                                var_t6_2 = var_t6 + 1;
                                                temp_f8 = (s32) ((M2C_FIELD(temp_t0_4, f32 *, 4) - M2C_FIELD(arg0, f32 *, 0x18)) * D_800C0260);
                                                var_v1_4 = temp_f8 + 0xC8;
                                                var_a2_2 = 0xFF;
                                                if (var_v1_4 < 0x96) {
                                                    var_v1_4 = 0x96;
                                                } else if (var_v1_4 >= 0x100) {
                                                    var_v1_4 = -1;
                                                }
                                                if (temp_f8 >= 0x33) {
                                                    var_a2_2 = 0x131 - temp_f8;
                                                    goto block_78;
                                                }
                                                if (temp_f8 < -0x32) {
                                                    var_a2_2 = temp_f8 + 0xCD;
block_78:
                                                    if (var_a2_2 < 0x4B) {
                                                        var_a2_2 = 0x4B;
                                                    }
                                                }
                                                temp_a0_3 = M2C_FIELD(temp_s4, u8 *, 0x5C);
                                                switch (temp_a0_3) { /* switch 1 */
                                                case 0: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_4;
block_93:
                                                    M2C_FIELD(var_a1_2, s8 *, 0xD) = var_v1_4;
                                                    M2C_FIELD(var_a1_2, s8 *, 0xE) = var_v1_4;
                                                    break;
                                                case 1: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = 0;
                                                    M2C_FIELD(var_a1_2, s8 *, 0xD) = var_v1_4;
block_98:
                                                    M2C_FIELD(var_a1_2, s8 *, 0xE) = 0;
                                                    break;
                                                case 2: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = 0;
block_88:
                                                    M2C_FIELD(var_a1_2, s8 *, 0xD) = 0;
                                                    M2C_FIELD(var_a1_2, s8 *, 0xE) = var_v1_4;
                                                    break;
                                                case 3: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_4;
block_97:
                                                    M2C_FIELD(var_a1_2, s8 *, 0xD) = 0;
                                                    goto block_98;
                                                case 4: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_4;
                                                    M2C_FIELD(var_a1_2, s8 *, 0xD) = 0;
                                                    M2C_FIELD(var_a1_2, s8 *, 0xE) = (s8) (var_v1_4 / 2);
                                                    break;
                                                case 5: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_4;
                                                    goto block_88;
                                                case 6: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = (s8) (var_v1_4 / 2);
                                                    var_v0_4 = var_v1_4;
                                                    if (var_v1_4 < 0) {
                                                        var_v0_4 = var_v1_4 + 3;
                                                    }
                                                    M2C_FIELD(var_a1_2, s8 *, 0xD) = (s8) (var_v0_4 >> 2);
                                                    goto block_98;
                                                case 7: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = 0;
                                                    goto block_93;
                                                case 8: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_4;
                                                    M2C_FIELD(var_a1_2, s8 *, 0xD) = var_v1_4;
                                                    goto block_98;
                                                case 9: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_4;
                                                    M2C_FIELD(var_a1_2, s8 *, 0xD) = (s8) (var_v1_4 / 2);
                                                    goto block_98;
                                                case 10: /* switch 1 */
                                                    M2C_FIELD(var_a1_2, s8 *, 0xC) = 0;
                                                    goto block_97;
                                                }
                                                temp_f0_4 = (f32) var_a2_2 * M2C_FIELD(arg0, f32 *, 8);
                                                if (!(M2C_FIELD(&D_800C0258, f32 *, 4) <= temp_f0_4)) {
                                                    M2C_FIELD(var_a1_2, s8 *, 0xF) = (s8) (s32) temp_f0_4;
                                                } else {
                                                    M2C_FIELD(var_a1_2, s8 *, 0xF) = (s8) ((s32) (temp_f0_4 - M2C_FIELD(&D_800C0258, f32 *, 4)) | 0x80000000);
                                                }
                                                var_a1_2 += 0x10;
                                                var_t1 += 1;
                                            } else {
                                                *var_t2 = var_t1 - 1;
                                                var_t2_2 = var_t2 + 2;
                                                var_t6_2 = var_t6 + 1;
                                            }
                                            *var_t2_2 = var_t1;
                                            M2C_FIELD(var_a1_2, s16 *, 0) = (s16) (s32) M2C_FIELD(temp_t3_2, f32 *, 0);
                                            M2C_FIELD(var_a1_2, s16 *, 2) = (s16) (s32) M2C_FIELD(temp_t3_2, f32 *, 8);
                                            var_t2 = var_t2_2 + 2;
                                            var_t6 = var_t6_2 + 1;
                                            temp_f8_2 = (s32) ((M2C_FIELD(temp_t0_4, f32 *, 4) - M2C_FIELD(arg0, f32 *, 0x18)) * D_800C0294);
                                            var_v1_5 = temp_f8_2 + 0xC8;
                                            var_a2_3 = 0xFF;
                                            if (var_v1_5 < 0x96) {
                                                var_v1_5 = 0x96;
                                            } else if (var_v1_5 >= 0x100) {
                                                var_v1_5 = -1;
                                            }
                                            if (temp_f8_2 >= 0x33) {
                                                var_a2_3 = 0x131 - temp_f8_2;
                                                goto block_112;
                                            }
                                            if (temp_f8_2 < -0x32) {
                                                var_a2_3 = temp_f8_2 + 0xCD;
block_112:
                                                if (var_a2_3 < 0x4B) {
                                                    var_a2_3 = 0x4B;
                                                }
                                            }
                                            temp_a0_4 = M2C_FIELD(temp_s4, u8 *, 0x5C);
                                            switch (temp_a0_4) { /* switch 2 */
                                            case 0: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_5;
block_127:
                                                M2C_FIELD(var_a1_2, s8 *, 0xD) = var_v1_5;
                                                M2C_FIELD(var_a1_2, s8 *, 0xE) = var_v1_5;
                                                break;
                                            case 1: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = 0;
                                                M2C_FIELD(var_a1_2, s8 *, 0xD) = var_v1_5;
block_132:
                                                M2C_FIELD(var_a1_2, s8 *, 0xE) = 0;
                                                break;
                                            case 2: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = 0;
block_122:
                                                M2C_FIELD(var_a1_2, s8 *, 0xD) = 0;
                                                M2C_FIELD(var_a1_2, s8 *, 0xE) = var_v1_5;
                                                break;
                                            case 3: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_5;
block_131:
                                                M2C_FIELD(var_a1_2, s8 *, 0xD) = 0;
                                                goto block_132;
                                            case 4: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_5;
                                                M2C_FIELD(var_a1_2, s8 *, 0xD) = 0;
                                                M2C_FIELD(var_a1_2, s8 *, 0xE) = (s8) (var_v1_5 / 2);
                                                break;
                                            case 5: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_5;
                                                goto block_122;
                                            case 6: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = (s8) (var_v1_5 / 2);
                                                var_v0_5 = var_v1_5;
                                                if (var_v1_5 < 0) {
                                                    var_v0_5 = var_v1_5 + 3;
                                                }
                                                M2C_FIELD(var_a1_2, s8 *, 0xD) = (s8) (var_v0_5 >> 2);
                                                goto block_132;
                                            case 7: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = 0;
                                                goto block_127;
                                            case 8: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_5;
                                                M2C_FIELD(var_a1_2, s8 *, 0xD) = var_v1_5;
                                                goto block_132;
                                            case 9: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = var_v1_5;
                                                M2C_FIELD(var_a1_2, s8 *, 0xD) = (s8) (var_v1_5 / 2);
                                                goto block_132;
                                            case 10: /* switch 2 */
                                                M2C_FIELD(var_a1_2, s8 *, 0xC) = 0;
                                                goto block_131;
                                            }
                                            temp_f0_5 = (f32) var_a2_3 * M2C_FIELD(arg0, f32 *, 8);
                                            if (!(M2C_FIELD(&D_800C0258, f32 *, 4) <= temp_f0_5)) {
                                                M2C_FIELD(var_a1_2, s8 *, 0xF) = (s8) (s32) temp_f0_5;
                                            } else {
                                                M2C_FIELD(var_a1_2, s8 *, 0xF) = (s8) ((s32) (temp_f0_5 - M2C_FIELD(&D_800C0258, f32 *, 4)) | 0x80000000);
                                            }
                                            var_t1 += 1;
                                            var_a1_2 += 0x10;
                                            if (var_t1 < 0x1FF) {
                                                goto block_137;
                                            }
                                        } else {
                                            goto block_137;
                                        }
                                    } else {
                                        goto block_137;
                                    }
                                } else {
block_137:
                                    var_t7 += 4;
                                    var_t5 += 1;
                                    var_s3_2 += 4;
                                    if (var_t5 >= 3) {
                                        goto block_138;
                                    }
                                    goto loop_50;
                                }
                            } else {
                                goto block_138;
                            }
                        } else {
block_138:
                            var_t4 += 0x20;
                            var_s1_3 += 1;
                            var_s5 += 1;
                            if (var_s1_3 >= (u32) M2C_FIELD(var_s0, u32 *, 0x10)) {
                                var_s0 += 0x14;
                                goto block_142;
                            }
                            goto loop_33;
                        }
                    } else {
                        goto block_141;
                    }
                } else {
                    var_s5 += M2C_FIELD(var_s0, u32 *, 0x10);
block_141:
                    var_s0 += 0x14;
block_142:
                    var_s7 += 1;
                    if (var_s7 < sp164) {
                        goto loop_27;
                    }
                }
            }
            var_t2_3 = var_t1;
            var_t1_2 = 0;
            var_s1_4 = 0;
            if (var_t2_3 >= 2) {
                var_t0 = &D_80129202;
                var_t4_2 = 1;
                do {
                    var_a2_4 = 0x20;
                    if (var_t2_3 < 0x21) {
                        var_a2_4 = var_t2_3;
                    }
                    var_t3 = var_t4_2;
                    M2C_FIELD(D_800D8E24, s32 *, 0) = ((var_a2_4 & 0xFF) << 0xC) | (((var_a2_4 & 0x7F) * 2) | 0x01000000);
                    M2C_FIELD(D_800D8E24, void **, 4) = (void *) (temp_s2 + (var_t1_2 * 0x10));
                    D_800D8E24 += 8;
                    if ((*var_t0 - var_t1_2) < var_a2_4) {
                        var_a3_2 = (var_s1_4 * 2) + 2;
loop_149:
                        temp_v0_4 = var_t3 * 2;
                        if (var_s1_4 < (var_t6 - 1)) {
                            temp_v1_4 = *(&D_801291FE + var_a3_2);
                            var_a3_2 += 4;
                            var_t0 += 4;
                            var_t4_2 += 2;
                            var_s1_4 += 2;
                            var_t3 = var_s1_4 + 1;
                            M2C_FIELD(D_800D8E24, s32 *, 0) = ((((temp_v1_4 - var_t1_2) * 2) & 0xFF) << 0x10) | (((*(&D_80129200 + temp_v0_4) - var_t1_2) << 9) & 0xFE00) | 0x08000000;
                            M2C_FIELD(D_800D8E24, void **, 4) = NULL;
                            D_800D8E24 += 8;
                            if ((*(&D_80129200 + var_a3_2) - var_t1_2) < var_a2_4) {
                                goto loop_149;
                            }
                        }
                    }
                    if ((*var_t0 - var_t1_2) == var_a2_4) {
                        var_a2_4 -= 1;
                    }
                    var_t2_3 -= var_a2_4;
                    var_t1_2 += var_a2_4;
                } while (var_t2_3 >= 2);
            }
            temp_f1_4 = temp_f25 * M2C_FIELD(&D_800C02C0, f32 *, 4);
            temp_s2_2 = (D_800B6D1C * 0x30) + &D_8012F340;
            temp_f3_2 = temp_f0 * M2C_FIELD(&D_800C02C0, f32 *, 4);
            M2C_FIELD(temp_s2_2, s16 *, 0) = (s16) (s32) (M2C_FIELD(arg0, f32 *, 0x14) - temp_f1_4);
            temp_f4_2 = temp_f0 * M2C_FIELD(&D_800C02C8, f32 *, 0);
            M2C_FIELD(temp_s2_2, s16 *, 2) = (s16) (s32) (M2C_FIELD(arg0, f32 *, 0x1C) - temp_f3_2);
            temp_f2_3 = temp_f25 * M2C_FIELD(&D_800C02C8, f32 *, 0);
            M2C_FIELD(temp_s2_2, s16 *, 0x10) = (s16) (s32) (M2C_FIELD(arg0, f32 *, 0x14) + temp_f4_2 + temp_f1_4);
            M2C_FIELD(temp_s2_2, s16 *, 0x12) = (s16) (s32) ((M2C_FIELD(arg0, f32 *, 0x1C) - temp_f2_3) + temp_f3_2);
            temp_f22_2 = M2C_FIELD(arg0, f32 *, 8) * M2C_FIELD(&D_800C02C8, f32 *, 4);
            M2C_FIELD(temp_s2_2, s16 *, 0x20) = (s16) (s32) ((M2C_FIELD(arg0, f32 *, 0x14) - temp_f4_2) + temp_f1_4);
            M2C_FIELD(temp_s2_2, s16 *, 0x22) = (s16) (s32) (M2C_FIELD(arg0, f32 *, 0x1C) + temp_f2_3 + temp_f3_2);
            if (!(M2C_FIELD(&D_800C02D0, f32 *, 0) <= temp_f22_2)) {
                var_v1_6 = (s32) temp_f22_2;
            } else {
                var_v1_6 = (s32) (temp_f22_2 - M2C_FIELD(&D_800C02D0, f32 *, 0)) | 0x80000000;
            }
            var_f0_7 = M2C_FIELD(&D_800C02D0, f32 *, 4);
            var_condition_bit = var_f0_7 <= temp_f22_2;
            M2C_FIELD(temp_s2_2, s8 *, 0xF) = (s8) var_v1_6;
            if (!(var_f0_7 <= temp_f22_2)) {
                M2C_FIELD(temp_s2_2, s8 *, 0x1F) = (s8) (s32) temp_f22_2;
            } else {
                temp_v1_5 = (s32) (temp_f22_2 - var_f0_7) | 0x80000000;
                var_f0_7 = D_800C02D8;
                M2C_FIELD(temp_s2_2, s8 *, 0x1F) = temp_v1_5;
                var_condition_bit = var_f0_7 <= temp_f22_2;
            }
            if (var_condition_bit) {
                var_v1_7 = (s32) (temp_f22_2 - var_f0_7) | 0x80000000;
            } else {
                var_v1_7 = (s32) temp_f22_2;
            }
            M2C_FIELD(temp_s2_2, s8 *, 0x2F) = (s8) var_v1_7;
            temp_t1_3 = D_800D8E24;
            temp_t0_5 = D_800D8E24 + 8;
            M2C_FIELD(temp_t1_3, void **, 4) = temp_s2_2;
            temp_s2_3 = (D_800B6D1C << 7) + &D_8012DA40;
            temp_v1_6 = temp_t0_5 + 8;
            D_800D8E24 = temp_t0_5;
            M2C_FIELD(temp_t1_3, s32 *, 0) = 0x01003006;
            D_800D8E24 = temp_v1_6;
            M2C_FIELD(D_800D8E24, s32 *, 8) = 0x08000200;
            M2C_FIELD(temp_t0_5, s32 *, 4) = 0;
            D_800D8E24 = temp_t0_5 + 0x10;
            M2C_FIELD(temp_t0_5, s32 *, 8) = 0x08000400;
            M2C_FIELD(temp_v1_6, s32 *, 4) = 0;
            func_0020FC74(&spB0, D_800C02DC, D_800C02DC, 0x3F800000);
            func_00210318(&spB0, (f32) ((s32) D_800C2030 / 2), 32.0f, 0);
            func_0020D274(&spB0, (D_800B6D1C << 6) + &D_80129180);
            temp_a0_5 = D_800D8E24;
            M2C_FIELD(temp_a0_5, s32 *, 0) = 0xDA380003;
            M2C_FIELD(temp_a0_5, void **, 4) = (void *) ((D_800B6D1C << 6) + &D_80129180);
            temp_f21_3 = M2C_FIELD(&D_800C02E0, f32 *, 4);
            temp_f22_3 = M2C_FIELD(arg0, f32 *, 8) * M2C_FIELD(&D_800C02E0, f32 *, 0);
            D_800D8E24 += 8;
            M2C_FIELD(temp_s2_3, s16 *, 0) = (s16) (s32) (func_002982F0(M2C_FIELD(arg0, f32 *, 0x10)) * temp_f21_3);
            M2C_FIELD(temp_s2_3, s16 *, 2) = (s16) (s32) (func_002974C0(M2C_FIELD(arg0, f32 *, 0x10)) * M2C_FIELD(&D_800C02E8, f32 *, 0));
            temp_f20_3 = M2C_FIELD(&D_800C02E8, f32 *, 4);
            M2C_FIELD(temp_s2_3, s16 *, 0x10) = (s16) (s32) ((f32) M2C_FIELD(temp_s2_3, s16 *, 0) - (func_002982F0(-M2C_FIELD(arg0, f32 *, 0x10)) * temp_f20_3));
            M2C_FIELD(temp_s2_3, s16 *, 0x12) = (s16) (s32) ((f32) M2C_FIELD(temp_s2_3, s16 *, 2) - (func_002974C0(-M2C_FIELD(arg0, f32 *, 0x10)) * temp_f20_3));
            temp_f20_4 = D_800C02F0;
            M2C_FIELD(temp_s2_3, s16 *, 0x20) = (s16) (s32) ((f32) M2C_FIELD(temp_s2_3, s16 *, 0x10) + (func_002982F0(-M2C_FIELD(arg0, f32 *, 0x10) - temp_f20_4) * temp_f21_3));
            M2C_FIELD(temp_s2_3, s16 *, 0x22) = (s16) (s32) ((f32) M2C_FIELD(temp_s2_3, s16 *, 0x12) + (func_002974C0(-M2C_FIELD(arg0, f32 *, 0x10) - temp_f20_4) * temp_f21_3));
            M2C_FIELD(temp_s2_3, s16 *, 0x30) = (s16) (s32) ((f32) M2C_FIELD(temp_s2_3, s16 *, 0x10) + (func_002982F0(temp_f20_4 - M2C_FIELD(arg0, f32 *, 0x10)) * temp_f21_3));
            temp_f0_6 = func_002974C0(temp_f20_4 - M2C_FIELD(arg0, f32 *, 0x10));
            M2C_FIELD(temp_s2_3, s16 *, 0x40) = -7;
            M2C_FIELD(temp_s2_3, s16 *, 0x42) = -7;
            M2C_FIELD(temp_s2_3, s16 *, 0x50) = -7;
            M2C_FIELD(temp_s2_3, u16 *, 0x52) = 8U;
            M2C_FIELD(temp_s2_3, s16 *, 0x60) = 8;
            M2C_FIELD(temp_s2_3, s16 *, 0x70) = 8;
            temp_v1_7 = M2C_FIELD(temp_s2_3, u16 *, 0x52) - 0xF;
            M2C_FIELD(temp_s2_3, s16 *, 0x62) = temp_v1_7;
            M2C_FIELD(temp_s2_3, s16 *, 0x72) = (s16) (temp_v1_7 + 0xF);
            M2C_FIELD(temp_s2_3, s16 *, 0x32) = (s16) (s32) ((f32) M2C_FIELD(temp_s2_3, s16 *, 0x12) + (temp_f0_6 * temp_f21_3));
            if (!(D_800C02F4 <= temp_f22_3)) {
                var_v1_8 = (s32) temp_f22_3;
            } else {
                var_v1_8 = (s32) (temp_f22_3 - D_800C02F4) | 0x80000000;
            }
            var_f0_8 = D_800C02F8;
            var_condition_bit_2 = var_f0_8 <= temp_f22_3;
            M2C_FIELD(temp_s2_3, s8 *, 0xF) = (s8) var_v1_8;
            if (!(var_f0_8 <= temp_f22_3)) {
                M2C_FIELD(temp_s2_3, s8 *, 0x1F) = (s8) (s32) temp_f22_3;
            } else {
                temp_v1_8 = (s32) (temp_f22_3 - var_f0_8) | 0x80000000;
                var_f0_8 = D_800C02FC;
                M2C_FIELD(temp_s2_3, s8 *, 0x1F) = temp_v1_8;
                var_condition_bit_2 = var_f0_8 <= temp_f22_3;
            }
            if (var_condition_bit_2) {
                var_v1_9 = (s32) (temp_f22_3 - var_f0_8) | 0x80000000;
            } else {
                var_v1_9 = (s32) temp_f22_3;
            }
            var_f0_9 = D_800C0300;
            var_condition_bit_3 = var_f0_9 <= temp_f22_3;
            M2C_FIELD(temp_s2_3, s8 *, 0x2F) = (s8) var_v1_9;
            if (!(var_f0_9 <= temp_f22_3)) {
                M2C_FIELD(temp_s2_3, s8 *, 0x3F) = (s8) (s32) temp_f22_3;
            } else {
                temp_v1_9 = (s32) (temp_f22_3 - var_f0_9) | 0x80000000;
                var_f0_9 = D_800C0304;
                M2C_FIELD(temp_s2_3, s8 *, 0x3F) = temp_v1_9;
                var_condition_bit_3 = var_f0_9 <= temp_f22_3;
            }
            if (var_condition_bit_3) {
                var_v1_10 = (s32) (temp_f22_3 - var_f0_9) | 0x80000000;
            } else {
                var_v1_10 = (s32) temp_f22_3;
            }
            var_f0_10 = D_800C0308;
            var_condition_bit_4 = var_f0_10 <= temp_f22_3;
            M2C_FIELD(temp_s2_3, s8 *, 0x4F) = (s8) var_v1_10;
            if (!(var_f0_10 <= temp_f22_3)) {
                M2C_FIELD(temp_s2_3, s8 *, 0x5F) = (s8) (s32) temp_f22_3;
            } else {
                temp_v1_10 = (s32) (temp_f22_3 - var_f0_10) | 0x80000000;
                var_f0_10 = D_800C030C;
                M2C_FIELD(temp_s2_3, s8 *, 0x5F) = temp_v1_10;
                var_condition_bit_4 = var_f0_10 <= temp_f22_3;
            }
            if (var_condition_bit_4) {
                var_v1_11 = (s32) (temp_f22_3 - var_f0_10) | 0x80000000;
            } else {
                var_v1_11 = (s32) temp_f22_3;
            }
            M2C_FIELD(temp_s2_3, s8 *, 0x6F) = (s8) var_v1_11;
            if (!(D_800C0310 <= temp_f22_3)) {
                var_v1_12 = (s32) temp_f22_3;
            } else {
                var_v1_12 = (s32) (temp_f22_3 - D_800C0310) | 0x80000000;
            }
            M2C_FIELD(temp_s2_3, s8 *, 0x7F) = (s8) var_v1_12;
            temp_v1_11 = D_800D8E24;
            temp_v0_5 = D_800D8E24 + 8;
            D_800D8E24 = temp_v0_5;
            M2C_FIELD(temp_v1_11, s32 *, 0) = 0x01008010;
            M2C_FIELD(temp_v1_11, void **, 4) = temp_s2_3;
            temp_v1_12 = temp_v0_5 + 8;
            temp_a1_5 = temp_v0_5 + 0x10;
            D_800D8E24 = temp_v1_12;
            M2C_FIELD(D_800D8E24, s32 *, 8) = 0x08000200;
            M2C_FIELD(temp_v0_5, s32 *, 4) = 0;
            D_800D8E24 = temp_a1_5;
            M2C_FIELD(temp_v0_5, s32 *, 8) = 0x08020400;
            M2C_FIELD(temp_v1_12, s32 *, 4) = 0;
            temp_v1_13 = temp_v0_5 + 0x18;
            D_800D8E24 = temp_v1_13;
            M2C_FIELD(temp_v0_5, s32 *, 0x10) = 0x08020600;
            M2C_FIELD(temp_a1_5, s32 *, 4) = 0;
            temp_a1_6 = temp_v0_5 + 0x20;
            temp_a2_2 = temp_v0_5 + 0x28;
            D_800D8E24 = temp_a1_6;
            M2C_FIELD(temp_v0_5, s32 *, 0x18) = 0x08080A00;
            M2C_FIELD(temp_v1_13, s32 *, 4) = 0;
            D_800D8E24 = temp_a2_2;
            M2C_FIELD(temp_v0_5, s32 *, 0x20) = 0x080A0C00;
            M2C_FIELD(temp_a1_6, s32 *, 4) = 0;
            D_800D8E24 = temp_v0_5 + 0x30;
            M2C_FIELD(temp_v0_5, s32 *, 0x28) = 0x080C0E00;
            M2C_FIELD(temp_a2_2, s32 *, 4) = 0;
        }
    }
}
