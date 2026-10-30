/* NON-MATCHING: 258 of 268 words differ: m2c draft, not worked on (time) */
#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800C1810;
extern M2C_UNK D_800C181C;
extern M2C_UNK D_800C182C;
extern M2C_UNK D_800C184C;
extern M2C_UNK D_800C185C;
extern M2C_UNK D_800C1864;
extern M2C_UNK D_800C2210;
extern M2C_UNK D_800C2214;
extern M2C_UNK D_800C2218;

s32 *func_00204EDC(M2C_UNK, s32, M2C_UNK, M2C_UNK *); /* extern */
s32 *func_00204FD0(M2C_UNK, s32, s32, M2C_UNK *);   /* extern */
M2C_UNK func_002052D8(M2C_UNK, s32 *);              /* extern */
u32 func_00266B80();                                /* extern */
M2C_UNK func_00266C5C(s32, M2C_UNK, M2C_UNK);       /* extern */
extern s32 D_80130A40;
extern s32 D_80130A44;
extern s32 D_80130A48;
extern s32 D_80130A4C;
extern s32 D_80130A50;
extern s32 D_80130A78;
extern s32 D_80130A7C;
extern s32 D_80130A80;
extern s32 D_80130A84;
extern M2C_UNK D_80130A88;

void func_00425220(void *arg0) {
    M2C_UNK var_s2;
    s32 *temp_a1;
    s32 *temp_a1_2;
    s32 *temp_a1_3;
    s32 *temp_a1_4;
    s32 *temp_a1_5;
    s32 *temp_s0;
    s32 *temp_v0;
    s32 *temp_v0_2;
    s32 temp_a1_6;
    s32 temp_lo;
    s32 temp_s1;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_s1;
    s32 var_v1;
    u32 var_s0_2;
    u32 var_s0_3;
    u32 var_s0_4;
    u32 var_s0_5;
    void *var_s0;
    void *var_v0;

    temp_a1 = M2C_FIELD(arg0, s32 **, 4);
    if (temp_a1 != NULL) {
        func_002052D8(0, temp_a1);
    }
    temp_a1_2 = M2C_FIELD(arg0, s32 **, 8);
    if (temp_a1_2 != NULL) {
        func_002052D8(0, temp_a1_2);
    }
    temp_a1_3 = M2C_FIELD(arg0, s32 **, 0x18);
    if (temp_a1_3 != NULL) {
        func_002052D8(0, temp_a1_3);
    }
    temp_a1_4 = M2C_FIELD(arg0, s32 **, 0x1C);
    var_s1 = 0;
    if (temp_a1_4 != NULL) {
        func_002052D8(0, temp_a1_4);
        var_s1 = 0;
    }
    var_s0 = arg0;
    do {
        temp_a1_5 = M2C_FIELD(var_s0, s32 **, 0xC);
        if (temp_a1_5 != NULL) {
            func_002052D8(0, temp_a1_5);
        }
        var_s1 += 1;
        var_s0 += 4;
    } while (var_s1 < 3);
    var_v1 = 2;
    var_v0 = arg0 + 8;
    M2C_FIELD(arg0, s32 *, 0) = 0;
    M2C_FIELD(arg0, s32 **, 4) = NULL;
    M2C_FIELD(arg0, s32 **, 8) = NULL;
    M2C_FIELD(arg0, s32 **, 0x18) = NULL;
    M2C_FIELD(arg0, s32 **, 0x1C) = NULL;
    do {
        M2C_FIELD(var_v0, s32 *, 0xC) = 0;
        var_v1 -= 1;
        var_v0 -= 4;
    } while (var_v1 >= 0);
    if (func_00266B80() >= (u32) (0x100000 * 6)) {
        temp_s1 = func_00266B80() | 0x80000000;
        temp_s0 = func_00204FD0(0, temp_s1 - 0x100000, 0x54600, &D_800C1810);
        temp_v0 = func_00204FD0(0, temp_s1 - (0x100000 * 2), 0xFD200, &D_800C181C);
        func_00266C5C(*temp_v0, 0, 0xFD200);
        func_00266C5C(*temp_s0, 0, 0x54600);
        M2C_FIELD(arg0, s32 **, 0x18) = temp_s0;
        var_s2 = 0x54600;
        M2C_FIELD(arg0, s32 **, 0xC) = temp_v0;
        D_80130A78 = *temp_s0;
        var_s0_2 = 0;
        if (D_800C2210 != 0) {
            var_a0 = 0;
            do {
                temp_v1 = var_s0_2 * 4;
                var_s0_2 += 1;
                *(((__typeof__(&D_80130A88))((s8 *)&D_80130A88 + temp_v1))) = *temp_v0 + var_a0;
                var_a0 += 0x54600;
            } while (var_s0_2 < (u32) D_800C2210);
        }
    } else {
        var_s2 = 0x1ECC0;
        temp_lo = (D_800C2210 + 1) * 0x1ECC0;
        var_s0_3 = 0;
        temp_v0_2 = func_00204FD0(0, 0x80400000 - temp_lo, temp_lo, &D_800C182C);
        M2C_FIELD(arg0, s32 **, 0xC) = temp_v0_2;
        if (D_800C2210 != 0) {
            var_a0_2 = 0;
            do {
                temp_v1_2 = var_s0_3 * 4;
                var_s0_3 += 1;
                *(((__typeof__(&D_80130A88))((s8 *)&D_80130A88 + temp_v1_2))) = *temp_v0_2 + var_a0_2;
                var_a0_2 += 0x1ECC0;
            } while (var_s0_3 < (u32) D_800C2210);
        }
        D_80130A78 = *temp_v0_2 + (0x1ECC0 * var_s0_3);
    }
    var_s0_4 = 0;
    if (D_800C2210 != 0) {
        do {
            func_00266C5C(*(((__typeof__(&D_80130A88))((s8 *)&D_80130A88 + (var_s0_4 * 4)))), 0, var_s2);
            var_s0_4 += 1;
        } while (var_s0_4 < (u32) D_800C2210);
    }
    func_00266C5C(D_80130A78, 0, var_s2);
    M2C_FIELD(arg0, s32 **, 4) = func_00204EDC(0, (D_800C2214 + D_800C2218) * 0x10, 0x23, &D_800C184C);
    M2C_FIELD(arg0, s32 **, 8) = func_00204EDC(0, 0x8000, 0x23, &D_800C185C);
    M2C_FIELD(arg0, s32 **, 0x1C) = func_00204EDC(0, 0x22000, 0x23, &D_800C1864);
    D_80130A44 = 0x8000;
    D_80130A7C = 0x22000;
    temp_a1_6 = D_800C2214 * 8;
    temp_v1_3 = *M2C_FIELD(arg0, s32 **, 4);
    D_80130A80 = temp_v1_3;
    temp_v1_4 = temp_v1_3 + temp_a1_6;
    D_80130A4C = temp_v1_4;
    temp_v1_5 = temp_v1_4 + (D_800C2218 * 8);
    D_80130A84 = temp_v1_5;
    D_80130A50 = temp_v1_5 + temp_a1_6;
    D_80130A40 = *M2C_FIELD(arg0, s32 **, 0x1C);
    D_80130A48 = *M2C_FIELD(arg0, s32 **, 8);
    if (D_800C2210 != 0) {
        var_s0_5 = 1;
        do {
            var_s0_5 += 1;
        } while (var_s0_5 < (u32) D_800C2210);
    }
    M2C_FIELD(arg0, s32 *, 0) = 1;
}
