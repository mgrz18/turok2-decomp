/* NON-MATCHING: 674 of 702 words differ: raw m2c draft of the whole function (0xAE0), not worked on. */
/* WRONG CUT: REAL func_00429FA0 size 0xAE0 (splat cut it into func_00429FA0 + func_0042A9A0). */
#include "common.h"
#include "m2c_macros.h"

s32 func_0042AC0C(s32, s32, s32);
s32 func_0042C3BC(s32);
s32 func_0042CBE4(s32, s32);


s32 D_00200AE0(s32, s32);                           /* extern */
void *func_002017D4(void *, M2C_UNK);               /* extern */
s32 func_00201848(void *, s32, M2C_UNK);            /* extern */
s32 func_0020185C(void *, s32, M2C_UNK, s32 *);     /* extern */
void **func_0020367C(M2C_UNK, s32, s32, M2C_UNK, s32, M2C_UNK *, M2C_UNK *, s32); /* extern */
M2C_UNK func_002052D8(M2C_UNK, void **);            /* extern */
M2C_UNK func_002053A8(M2C_UNK);                     /* extern */
M2C_UNK func_00205B94(M2C_UNK, void **, s32, M2C_UNK *); /* extern */
s32 *func_00205D0C(M2C_UNK, void *, M2C_UNK, s32, void *, s32 (*)(s32, s32), M2C_UNK *); /* extern */
s32 func_00224180(void *, M2C_UNK);                 /* extern */
s32 func_00224F84(void *, s32);                     /* extern */
M2C_UNK func_00226390();                            /* extern */
M2C_UNK func_0042C4F4(void *);                      /* extern */
s32 func_0042CAB4(s32, s32);                        /* extern */
M2C_UNK func_0042E894(M2C_UNK *);                   /* extern */
extern M2C_UNK D_800A5D70;
extern M2C_UNK D_800A5D80;
extern M2C_UNK D_800A5D8C;
extern M2C_UNK D_800A5D9C;
extern M2C_UNK D_800A5DAC;
extern M2C_UNK D_800A5DC0;
extern M2C_UNK D_800A5DD0;
extern M2C_UNK D_800A5DE0;
extern M2C_UNK D_800A5DF0;
extern M2C_UNK D_800A5E04;
extern M2C_UNK D_800A5E14;
extern M2C_UNK D_800A5E28;
extern M2C_UNK D_800A5E38;
extern M2C_UNK D_800A5E40;
extern M2C_UNK D_800A5E48;
extern M2C_UNK D_800A5E58;
extern M2C_UNK D_800A5E68;
extern M2C_UNK D_800A5E7C;
extern M2C_UNK D_800A5E8C;
extern f32 D_800A5EA0;
extern s32 D_800B6D54;
extern M2C_UNK D_800E6AC0;
extern M2C_UNK D_800EBAC0;
extern M2C_UNK D_80119870;
extern M2C_UNK func_0020B490;

void func_00429FA0(void *arg0, s32 arg1) {
    void *sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 temp_v0_13;
    s32 temp_v0_14;
    s32 temp_v0_18;
    s32 temp_v0_19;
    s32 temp_v0_23;
    s32 temp_v0_24;
    s32 temp_v0_28;
    s32 temp_v0_29;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    s32 temp_v0_8;
    s32 temp_v0_9;
    s32 temp_v1;
    void **temp_v0_10;
    void **temp_v0_11;
    void **temp_v0_15;
    void **temp_v0_16;
    void **temp_v0_20;
    void **temp_v0_21;
    void **temp_v0_25;
    void **temp_v0_26;
    void **temp_v0_30;
    void **temp_v0_31;
    void *temp_s4;
    void *temp_s4_2;
    void *temp_s4_3;
    void *temp_s4_4;
    void *temp_s5;
    void *temp_v0;
    void *temp_v0_12;
    void *temp_v0_17;
    void *temp_v0_22;
    void *temp_v0_27;
    void *temp_v0_32;

    func_00226390();
    M2C_FIELD(arg0, s32 *, 0) = 0;
    func_0042C4F4(arg0);
    M2C_FIELD(arg0, s32 *, 4) = 0;
    M2C_FIELD(arg0, s32 *, 8) = 0;
    M2C_FIELD(arg0, s32 *, 0x18FA8) = -1;
    M2C_FIELD(arg0, s32 *, 0x18EB8) = 0;
    M2C_FIELD(arg0, s32 *, 0x18EA4) = 0;
    func_0042E894(&D_80119870);
    temp_v0 = arg0 + 0x18EA8;
    M2C_FIELD(temp_v0, f32 *, 8) = 0.0f;
    M2C_FIELD(temp_v0, f32 *, 4) = 0.0f;
    M2C_FIELD(arg0, f32 *, 0x18EA8) = 0.0f;
    M2C_FIELD(arg0, f32 *, 0x18EB4) = (f32) D_800A5EA0;
    func_002053A8(0);
    func_00205B94(0, &sp20, arg1, &D_800A5D8C);
    temp_v0_2 = func_00201848(sp20, arg1, 5);
    M2C_FIELD(arg0, s32 *, 0xC) = temp_v0_2;
    func_00205B94(0, arg0 + 0x38, temp_v0_2, &D_800A5D9C);
    temp_v0_3 = func_00201848(sp20, arg1, 3);
    M2C_FIELD(arg0, s32 *, 0x10) = temp_v0_3;
    func_00205B94(0, arg0 + 0x3C, temp_v0_3, &D_800A5DAC);
    M2C_FIELD(arg0, s32 *, 0x74) = (s32) *func_00205D0C(0, sp20, 2, arg1, arg0, NULL, &D_800A5DC0);
    temp_v0_4 = func_00201848(sp20, arg1, 0);
    M2C_FIELD(arg0, s32 *, 0x14) = temp_v0_4;
    func_00205B94(0, arg0 + 0x40, temp_v0_4, &D_800A5DD0);
    M2C_FIELD(arg0, s32 *, 0x64) = (s32) *func_00205D0C(0, sp20, 1, arg1, arg0, func_0042CBE4, &D_800A5DE0);
    M2C_FIELD(arg0, s32 **, 0x94) = func_00205D0C(0, sp20, 4, arg1, arg0, func_0042CAB4, &D_800A5DF0);
    M2C_FIELD(arg0, s32 *, 0x78) = (s32) *func_00205D0C(0, sp20, 0xA, arg1, arg0, NULL, &D_800A5DC0);
    temp_v0_5 = func_00201848(sp20, arg1, 9);
    M2C_FIELD(arg0, s32 *, 0x18) = temp_v0_5;
    func_00205B94(0, arg0 + 0x44, temp_v0_5, &D_800A5E04);
    M2C_FIELD(arg0, s32 *, 0x5C) = (s32) *func_00205D0C(0, sp20, 0xB, arg1, arg0, NULL, &D_800A5E14);
    temp_v1 = *func_00205D0C(0, sp20, 0xC, arg1, arg0, NULL, &D_800A5E28);
    M2C_FIELD(arg0, M2C_UNK **, 0x6C) = &D_800EBAC0;
    M2C_FIELD(arg0, s32 *, 0x80) = temp_v1;
    M2C_FIELD(arg0, s32 *, 0x28) = func_0020185C(sp20, arg1, 6, arg0 + 0x30);
    M2C_FIELD(arg0, M2C_UNK **, 0x70) = &D_800E6AC0;
    M2C_FIELD(arg0, s32 *, 0x2C) = func_0020185C(sp20, arg1, 7, arg0 + 0x34);
    M2C_FIELD(arg0, s32 *, 0x84) = (s32) *func_00205D0C(0, sp20, 0xD, arg1, arg0, D_00200AE0, &D_800A5E38);
    M2C_FIELD(arg0, s32 *, 0x8C) = (s32) *func_00205D0C(0, sp20, 0xE, arg1, arg0, D_00200AE0, &D_800A5E40);
    M2C_FIELD(arg0, s32 *, 0x88) = (s32) *func_00205D0C(0, sp20, 0xF, arg1, arg0, D_00200AE0, &D_800A5E48);
    M2C_FIELD(arg0, s32 *, 0x7C) = (s32) *func_00205D0C(0, sp20, 0x11, arg1, arg0, NULL, &D_800A5E58);
    temp_v0_6 = func_00201848(sp20, arg1, 0x10);
    M2C_FIELD(arg0, s32 *, 0x1C) = temp_v0_6;
    func_00205B94(0, arg0 + 0x48, temp_v0_6, &D_800A5E68);
    temp_v0_7 = func_00201848(sp20, arg1, 0x12);
    M2C_FIELD(arg0, s32 *, 0x24) = temp_v0_7;
    func_00205B94(0, arg0 + 0x60, temp_v0_7, &D_800A5E7C);
    M2C_FIELD(arg0, s32 *, 0x90) = (s32) *func_00205D0C(0, sp20, 0x13, arg1, arg0, NULL, &D_800A5E8C);
    func_0042C3BC((s32) arg0);
    D_800B6D54 = 1;
    temp_s5 = arg0 + 0x18FEC;
    temp_v0_8 = func_00224180(arg0, 0xDAC);
    if (temp_v0_8 == -1) {
        M2C_FIELD(temp_s5, s32 *, 8) = 0;
    } else {
        temp_v0_9 = func_00224F84(arg0, temp_v0_8);
        temp_v0_10 = func_0020367C(0, temp_v0_9, 0x18, 0, 0, NULL, &D_800A5D70, 1);
        if (temp_v0_10 != NULL) {
            temp_v0_11 = func_0020367C(0, func_0020185C(*temp_v0_10, temp_v0_9, 1, &sp24), sp24, 0, 0, &func_0020B490, &D_800A5D80, 1);
            if (temp_v0_11 != NULL) {
                temp_v0_12 = func_002017D4(func_002017D4(func_002017D4(func_002017D4(*temp_v0_11, 2), 0), 0), 0);
                M2C_FIELD(arg0, s32 *, 0x18FEC) = (s32) M2C_FIELD(temp_v0_12, s32 *, 0);
                M2C_FIELD(temp_s5, s32 *, 4) = (s32) M2C_FIELD(temp_v0_12, s32 *, 4);
                M2C_FIELD(temp_s5, s32 *, 8) = (s32) M2C_FIELD(temp_v0_12, s32 *, 8);
                M2C_FIELD(temp_s5, s32 *, 0xC) = (s32) M2C_FIELD(temp_v0_12, s32 *, 0xC);
                M2C_FIELD(temp_s5, s32 *, 0x10) = (s32) M2C_FIELD(temp_v0_12, s32 *, 0x10);
                M2C_FIELD(temp_s5, s32 *, 0x14) = (s32) M2C_FIELD(temp_v0_12, s32 *, 0x14);
                M2C_FIELD(temp_s5, s32 *, 0x18) = (s32) M2C_FIELD(temp_v0_12, s32 *, 0x18);
                func_002052D8(0, temp_v0_11);
            }
            func_002052D8(0, temp_v0_10);
        }
    }
    temp_s4 = arg0 + 0x19008;
    temp_v0_13 = func_00224180(arg0, 0xDAD);
    if (temp_v0_13 == -1) {
        M2C_FIELD(temp_s4, s32 *, 8) = 0;
    } else {
        temp_v0_14 = func_00224F84(arg0, temp_v0_13);
        temp_v0_15 = func_0020367C(0, temp_v0_14, 0x18, 0, 0, NULL, &D_800A5D70, 1);
        if (temp_v0_15 != NULL) {
            temp_v0_16 = func_0020367C(0, func_0020185C(*temp_v0_15, temp_v0_14, 1, &sp28), sp28, 0, 0, &func_0020B490, &D_800A5D80, 1);
            if (temp_v0_16 != NULL) {
                temp_v0_17 = func_002017D4(func_002017D4(func_002017D4(func_002017D4(*temp_v0_16, 2), 0), 0), 0);
                M2C_FIELD(arg0, s32 *, 0x19008) = (s32) M2C_FIELD(temp_v0_17, s32 *, 0);
                M2C_FIELD(temp_s4, s32 *, 4) = (s32) M2C_FIELD(temp_v0_17, s32 *, 4);
                M2C_FIELD(temp_s4, s32 *, 8) = (s32) M2C_FIELD(temp_v0_17, s32 *, 8);
                M2C_FIELD(temp_s4, s32 *, 0xC) = (s32) M2C_FIELD(temp_v0_17, s32 *, 0xC);
                M2C_FIELD(temp_s4, s32 *, 0x10) = (s32) M2C_FIELD(temp_v0_17, s32 *, 0x10);
                M2C_FIELD(temp_s4, s32 *, 0x14) = (s32) M2C_FIELD(temp_v0_17, s32 *, 0x14);
                M2C_FIELD(temp_s4, s32 *, 0x18) = (s32) M2C_FIELD(temp_v0_17, s32 *, 0x18);
                func_002052D8(0, temp_v0_16);
            }
            func_002052D8(0, temp_v0_15);
        }
    }
    temp_s4_2 = arg0 + 0x19024;
    temp_v0_18 = func_00224180(arg0, 0xDAE);
    if (temp_v0_18 == -1) {
        M2C_FIELD(temp_s4_2, s32 *, 8) = 0;
    } else {
        temp_v0_19 = func_00224F84(arg0, temp_v0_18);
        temp_v0_20 = func_0020367C(0, temp_v0_19, 0x18, 0, 0, NULL, &D_800A5D70, 1);
        if (temp_v0_20 != NULL) {
            temp_v0_21 = func_0020367C(0, func_0020185C(*temp_v0_20, temp_v0_19, 1, &sp2C), sp2C, 0, 0, &func_0020B490, &D_800A5D80, 1);
            if (temp_v0_21 != NULL) {
                temp_v0_22 = func_002017D4(func_002017D4(func_002017D4(func_002017D4(*temp_v0_21, 2), 0), 0), 0);
                M2C_FIELD(arg0, s32 *, 0x19024) = (s32) M2C_FIELD(temp_v0_22, s32 *, 0);
                M2C_FIELD(temp_s4_2, s32 *, 4) = (s32) M2C_FIELD(temp_v0_22, s32 *, 4);
                M2C_FIELD(temp_s4_2, s32 *, 8) = (s32) M2C_FIELD(temp_v0_22, s32 *, 8);
                M2C_FIELD(temp_s4_2, s32 *, 0xC) = (s32) M2C_FIELD(temp_v0_22, s32 *, 0xC);
                M2C_FIELD(temp_s4_2, s32 *, 0x10) = (s32) M2C_FIELD(temp_v0_22, s32 *, 0x10);
                M2C_FIELD(temp_s4_2, s32 *, 0x14) = (s32) M2C_FIELD(temp_v0_22, s32 *, 0x14);
                M2C_FIELD(temp_s4_2, s32 *, 0x18) = (s32) M2C_FIELD(temp_v0_22, s32 *, 0x18);
                func_002052D8(0, temp_v0_21);
            }
            func_002052D8(0, temp_v0_20);
        }
    }
    temp_s4_3 = arg0 + 0x19040;
    temp_v0_23 = func_00224180(arg0, 0xDAF);
    if (temp_v0_23 == -1) {
        M2C_FIELD(temp_s4_3, s32 *, 8) = 0;
    } else {
        temp_v0_24 = func_00224F84(arg0, temp_v0_23);
        temp_v0_25 = func_0020367C(0, temp_v0_24, 0x18, 0, 0, NULL, &D_800A5D70, 1);
        if (temp_v0_25 != NULL) {
            temp_v0_26 = func_0020367C(0, func_0020185C(*temp_v0_25, temp_v0_24, 1, &sp30), sp30, 0, 0, &func_0020B490, &D_800A5D80, 1);
            if (temp_v0_26 != NULL) {
                temp_v0_27 = func_002017D4(func_002017D4(func_002017D4(func_002017D4(*temp_v0_26, 2), 0), 0), 0);
                M2C_FIELD(arg0, s32 *, 0x19040) = (s32) M2C_FIELD(temp_v0_27, s32 *, 0);
                M2C_FIELD(temp_s4_3, s32 *, 4) = (s32) M2C_FIELD(temp_v0_27, s32 *, 4);
                M2C_FIELD(temp_s4_3, s32 *, 8) = (s32) M2C_FIELD(temp_v0_27, s32 *, 8);
                M2C_FIELD(temp_s4_3, s32 *, 0xC) = (s32) M2C_FIELD(temp_v0_27, s32 *, 0xC);
                M2C_FIELD(temp_s4_3, s32 *, 0x10) = (s32) M2C_FIELD(temp_v0_27, s32 *, 0x10);
                M2C_FIELD(temp_s4_3, s32 *, 0x14) = (s32) M2C_FIELD(temp_v0_27, s32 *, 0x14);
                M2C_FIELD(temp_s4_3, s32 *, 0x18) = (s32) M2C_FIELD(temp_v0_27, s32 *, 0x18);
                func_002052D8(0, temp_v0_26);
            }
            func_002052D8(0, temp_v0_25);
        }
    }
    temp_s4_4 = arg0 + 0x1905C;
    temp_v0_28 = func_00224180(arg0, 0xDB0);
    if (temp_v0_28 == -1) {
        M2C_FIELD(temp_s4_4, s32 *, 8) = 0;
    } else {
        temp_v0_29 = func_00224F84(arg0, temp_v0_28);
        temp_v0_30 = func_0020367C(0, temp_v0_29, 0x18, 0, 0, NULL, &D_800A5D70, 1);
        if (temp_v0_30 != NULL) {
            temp_v0_31 = func_0020367C(0, func_0020185C(*temp_v0_30, temp_v0_29, 1, &sp34), sp34, 0, 0, &func_0020B490, &D_800A5D80, 1);
            if (temp_v0_31 != NULL) {
                temp_v0_32 = func_002017D4(func_002017D4(func_002017D4(func_002017D4(*temp_v0_31, 2), 0), 0), 0);
                M2C_FIELD(arg0, s32 *, 0x1905C) = (s32) M2C_FIELD(temp_v0_32, s32 *, 0);
                M2C_FIELD(temp_s4_4, s32 *, 4) = (s32) M2C_FIELD(temp_v0_32, s32 *, 4);
                M2C_FIELD(temp_s4_4, s32 *, 8) = (s32) M2C_FIELD(temp_v0_32, s32 *, 8);
                M2C_FIELD(temp_s4_4, s32 *, 0xC) = (s32) M2C_FIELD(temp_v0_32, s32 *, 0xC);
                M2C_FIELD(temp_s4_4, s32 *, 0x10) = (s32) M2C_FIELD(temp_v0_32, s32 *, 0x10);
                M2C_FIELD(temp_s4_4, s32 *, 0x14) = (s32) M2C_FIELD(temp_v0_32, s32 *, 0x14);
                M2C_FIELD(temp_s4_4, s32 *, 0x18) = (s32) M2C_FIELD(temp_v0_32, s32 *, 0x18);
                func_002052D8(0, temp_v0_31);
            }
            func_002052D8(0, temp_v0_30);
        }
    }
    func_0042AC0C((s32) arg0, 0, 0);
}

