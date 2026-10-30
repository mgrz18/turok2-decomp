/* NON-MATCHING: m2c draft of the whole function (0x7D8); does not compile yet (m2c could not rebuild its two irregular switches). */
/* WRONG CUT: REAL func_0042AC0C size 0x7D8 (splat cut it into func_0042AC0C + D_0042B304). */
#include "common.h"
#include "m2c_macros.h"

s32 func_0042AA80(s32);
s32 func_0042B6B0(s32, s32);
s32 func_0042BA40(s32, s32);
s32 func_0042BF48(s32);
s32 func_0042CD04(s32, s32);
void func_0042CF90(s32);
void func_0042D120(s32);
s32 func_0042D220(s32, s32);
s32 func_0042D410(s32);
void func_0042EBBC(s32);
s32 func_0042FEEC(s32);

M2C_UNK CCollisionInfo__Reset();                    /* extern */
s32 D_00200AE0(s32, s32);                           /* extern */
s32 func_00201848(s32 *, s32, s32);                 /* extern */
M2C_UNK func_00201C08(M2C_UNK *);                   /* extern */
void **func_00204EDC(M2C_UNK, s32, M2C_UNK, M2C_UNK *); /* extern */
M2C_UNK func_002052D8(M2C_UNK, void **);            /* extern */
M2C_UNK func_002053A8(M2C_UNK);                     /* extern */
void **func_00205B94(M2C_UNK, void *, s32, M2C_UNK *); /* extern */
void **func_00205D0C(M2C_UNK, s32 *, M2C_UNK, s32, void *, s32 (*)(s32, s32), M2C_UNK *); /* extern */
M2C_UNK func_00206068(M2C_UNK);                     /* extern */
M2C_UNK func_0021A344(s32);                         /* extern */
M2C_UNK func_0021E5D4(void *);                      /* extern */
M2C_UNK func_0021F1B4(s32);                         /* extern */
M2C_UNK func_0022629C(void *, s32, void *, M2C_UNK); /* extern */
M2C_UNK func_00226390(void *);                      /* extern */
M2C_UNK func_00226724(void *);                      /* extern */
s32 func_002532A8(M2C_UNK *);                       /* extern */
M2C_UNK func_002758DC(M2C_UNK);                     /* extern */
M2C_UNK func_00275B58();                            /* extern */
M2C_UNK func_00275BE8(M2C_UNK *, M2C_UNK *, M2C_UNK); /* extern */
M2C_UNK func_00275EFC(s32);                         /* extern */
s32 func_00275F7C();                                /* extern */
s32 func_0027BC34();                                /* extern */
M2C_UNK func_0027ECF0(void *);                      /* extern */
s32 func_00284174();                                /* extern */
M2C_UNK func_00412B28(M2C_UNK);                     /* extern */
M2C_UNK func_00429F20(void *, void *, s32);         /* extern */
s32 func_0042B3E4(s32, s32);                        /* extern */
M2C_UNK func_0042C79C(void *);                      /* extern */
M2C_UNK func_0042C840(void *);                      /* extern */
M2C_UNK func_0042C96C();                            /* extern */
M2C_UNK func_0042D0A0(void *);                      /* extern */
M2C_UNK func_0042E894(M2C_UNK *);                   /* extern */
M2C_UNK func_0042EB04(s32);                         /* extern */
extern M2C_UNK D_800A5EB4;
extern M2C_UNK D_800A5EC0;
extern M2C_UNK D_800A5ECC;
extern M2C_UNK D_800A5ED8;
extern M2C_UNK D_800A5EE4;
extern M2C_UNK D_800A5EF8;
extern M2C_UNK D_800A5F00;
extern M2C_UNK D_800A5F0C;
extern M2C_UNK D_800A5F18;
extern M2C_UNK D_800A5F24;
extern s32 D_800AB8A0;
extern s32 D_800B6D20;
extern M2C_UNK D_800D6D68;
extern M2C_UNK D_800E68B0;
extern M2C_UNK D_800E6A90;
extern M2C_UNK D_801102B8;
extern M2C_UNK D_80119870;
extern void *D_80119890;
extern u8 D_8011AAF1;

void func_0042AC0C(void *arg0, s32 arg1, s32 arg2) {
    M2C_UNK var_a0;
    s32 temp_s0;
    s32 temp_s4;
    s32 temp_s4_2;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 temp_v0_6;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_4;
    s32 var_s2;
    s32 var_s2_2;
    void **temp_v0_2;
    void **temp_v0_4;
    void **temp_v0_5;
    void **temp_v0_7;
    void **temp_v0_8;
    void *temp_s0_2;
    void *temp_v1_5;
    void *var_s0_3;
    void *var_s1_3;

    var_s2 = arg1;
    var_s1 = arg2;
    if (M2C_FIELD(arg0, s32 *, 0x18FA8) != -1) {
        func_0042C96C();
    }
    if ((func_00284174() == 0) && (func_0027BC34() == 0) && (var_s1 != 0) && (M2C_FIELD(arg0, s32 *, 0x18FB8) != 0)) {
        temp_v1 = M2C_FIELD(arg0, s32 *, 0x18FD0);
        switch (temp_v1) {                          /* switch 1; irregular */
        case 0xBB8:                                 /* switch 1 */
            var_a0 = 0x8F;
block_21:
            func_00412B28(var_a0);
            break;
        case 0x1770:                                /* switch 1 */
            var_a0 = 0x92;
            goto block_21;
        case 0x7D0:                                 /* switch 1 */
            var_a0 = 0x8E;
            goto block_21;
        case 0xFA0:                                 /* switch 1 */
            var_a0 = 0x90;
            goto block_21;
        case 0x1388:                                /* switch 1 */
            var_a0 = 0x91;
            goto block_21;
        }
    }
    func_00226724(arg0);
    M2C_FIELD(arg0, s32 *, 0) = 0;
    func_00201C08(&D_800D6D68);
    func_002053A8(0);
    if (M2C_FIELD(arg0, s32 *, 0x18FA8) != -1) {
        temp_v0 = func_002532A8(&D_80119870);
        if (temp_v0 != 0) {
            func_0042EB04(temp_v0);
        }
        func_00275EFC(func_00275F7C());
        func_00275B58();
        func_002758DC(0x1000);
        func_002052D8(0, M2C_FIELD(arg0, void ***, 0x98));
        func_002052D8(0, M2C_FIELD(arg0, void ***, 0x9C));
        func_002052D8(0, M2C_FIELD(arg0, void ***, 0xA0));
        func_002052D8(0, M2C_FIELD(arg0, void ***, 0xA4));
        func_002052D8(0, M2C_FIELD(arg0, void ***, 0xA8));
        func_002052D8(0, M2C_FIELD(arg0, void ***, 0xB0));
        func_002052D8(0, M2C_FIELD(arg0, void ***, 0xB4));
        func_002052D8(0, M2C_FIELD(arg0, void ***, 0xCC));
        func_002052D8(0, M2C_FIELD(arg0, void ***, 0xAC));
        func_002052D8(0, M2C_FIELD(arg0, void ***, 0xC8));
    }
    func_002053A8(0);
    D_800B6D20 = 0;
    func_00206068(8);
    func_00226390(arg0);
    func_0042D220((s32) (arg0 + 0x18B0), *M2C_FIELD(arg0, s32 **, 0x94));
    func_0042CF90((s32) (arg0 + 0x10518));
    func_0042D0A0(arg0 + 0x13B28);
    func_0042D410((s32) &D_801102B8);
    CCollisionInfo__Reset();
    func_0042D120((s32) (arg0 + 0x18EBC));
    func_00275BE8(&D_800E6A90, &D_800E68B0, 0xF);
    func_0042EBBC((s32) (((__typeof__(&D_801102B8))((s8 *)&D_801102B8 - 0x1C8))));
    temp_v1_2 = *M2C_FIELD(arg0, s32 **, 0x38);
    if (temp_v1_2 < var_s2) {
        var_s2 = temp_v1_2 - 1;
        var_s1 = 0;
    }
    M2C_FIELD(arg0, s32 *, 0x18FA8) = var_s2;
    if (var_s1 == 0) {
        M2C_FIELD(arg0, s32 *, 0x18FB8) = 0;
    }
    func_0022629C(arg0, var_s2, arg0 + 0x18E5C, 0x3F);
    temp_v1_3 = M2C_FIELD(arg0, s32 *, 0x18FD0);
    if (temp_v1_3 < 0x1CEA) {
        switch (temp_v1_3);                         /* switch 2; irregular */
    } else if (temp_v1_3 >= 0x1D4C) {
        if (temp_v1_3 >= 0x1D4E) {
            if ((temp_v1_3 < 0x1DB3) && (temp_v1_3 >= 0x1DB1)) {
            case 0x1BBC:                            /* switch 2 */
            case 0x1C20:                            /* switch 2 */
                goto block_44;
            }
            goto block_45;
        }
    case 0x1C84:                                    /* switch 2 */
    case 0x123A:                                    /* switch 2 */
block_44:
        M2C_FIELD(arg0, s32 *, 0x18FA4) = 1;
    } else {
block_45:
        M2C_FIELD(arg0, s32 *, 0x18FA4) = 0;
    }
    temp_v1_4 = M2C_FIELD(arg0, s32 *, 0x18FD0);
    if (temp_v1_4 < 0x1CEA) {
        if ((temp_v1_4 < 0x1CE8) && (temp_v1_4 != 0x1BBC)) {
            if (temp_v1_4 < 0x1BBD) {
                var_s0 = 0x320;
                if (temp_v1_4 != 0x123A) {

                } else {
                    goto block_61;
                }
            } else if ((temp_v1_4 != 0x1C20) && (var_s0 = 0x320, (temp_v1_4 != 0x1C84))) {

            } else {
                goto block_61;
            }
        } else {
            goto block_61;
        }
    } else if (temp_v1_4 != 0x1DB1) {
        if (temp_v1_4 < 0x1DB2) {
            if (temp_v1_4 < 0x1D4E) {
                var_s0 = 0x320;
                if (temp_v1_4 >= 0x1D4C) {
                    var_s0 = 0xC80;
                }
            } else {
                var_s0 = 0x320;
            }
        } else {
            var_s0 = 0x320;
            if (temp_v1_4 == 0x1DB2) {
block_61:
                var_s0 = 0xC80;
            }
        }
    } else {
        var_s0 = 0x4B0;
    }
    if (D_8011AAF1 != 0) {
        var_s0 += 0x200;
    }
    temp_v0_2 = func_00204EDC(0, var_s0 << 6, 3, &D_800A5EB4);
    M2C_FIELD(arg0, void ***, 0xC8) = temp_v0_2;
    func_00429F20(arg0 + 0xD0, *temp_v0_2, var_s0);
    temp_v0_3 = func_00201848(M2C_FIELD(arg0, s32 **, 0x38), M2C_FIELD(arg0, s32 *, 0xC), var_s2);
    M2C_FIELD(arg0, void ***, 0x98) = func_00205B94(0, arg0 + 0x4C, temp_v0_3, &D_800A5EC0);
    temp_v0_4 = func_00205D0C(0, M2C_FIELD(arg0, s32 **, 0x4C), 1, temp_v0_3, arg0, func_0042B6B0, &D_800A5ECC);
    M2C_FIELD(arg0, void ***, 0x9C) = temp_v0_4;
    M2C_FIELD(arg0, void **, 0x58) = (void *) *temp_v0_4;
    temp_v0_5 = func_00205D0C(0, M2C_FIELD(arg0, s32 **, 0x4C), 4, temp_v0_3, arg0, D_00200AE0, &D_800A5ED8);
    M2C_FIELD(arg0, void ***, 0xA0) = temp_v0_5;
    M2C_FIELD(arg0, void **, 0x54) = (void *) *temp_v0_5;
    temp_v0_6 = func_00201848(M2C_FIELD(arg0, s32 **, 0x4C), temp_v0_3, 5);
    M2C_FIELD(arg0, s32 *, 0x20) = temp_v0_6;
    M2C_FIELD(arg0, void ***, 0xA4) = func_00205B94(0, arg0 + 0x50, temp_v0_6, &D_800A5EE4);
    M2C_FIELD(arg0, void ***, 0xB4) = func_00205D0C(0, M2C_FIELD(arg0, s32 **, 0x4C), 7, temp_v0_3, arg0, func_0042B3E4, &D_800A5EF8);
    M2C_FIELD(arg0, void ***, 0xCC) = func_00205D0C(0, M2C_FIELD(arg0, s32 **, 0x4C), 2, temp_v0_3, arg0, func_0042CD04, &D_800A5F00);
    temp_v0_7 = func_00205D0C(0, M2C_FIELD(arg0, s32 **, 0x4C), 0, temp_v0_3, arg0, NULL, &D_800A5F0C);
    M2C_FIELD(arg0, void ***, 0xA8) = temp_v0_7;
    M2C_FIELD(arg0, void **, 0x18E50) = (void *) *temp_v0_7;
    func_0042AA80((s32) arg0);
    D_800AB8A0 = ((u8) M2C_FIELD(M2C_FIELD(arg0, void **, 0x18E50), u8 *, 0x10) >> 2) & 1;
    temp_v0_8 = func_00205D0C(0, M2C_FIELD(arg0, s32 **, 0x4C), 6, temp_v0_3, arg0, D_00200AE0, &D_800A5F18);
    M2C_FIELD(arg0, void ***, 0xAC) = temp_v0_8;
    temp_v1_5 = *temp_v0_8;
    M2C_FIELD(arg0, void **, 0x18E9C) = (void *) (temp_v1_5 + 8);
    M2C_FIELD(arg0, s32 *, 0) = 1;
    M2C_FIELD(arg0, s32 *, 0xE0) = 0;
    M2C_FIELD(arg0, s32 *, 0xE8) = 0;
    M2C_FIELD(arg0, s32 *, 0x18EA0) = (s32) M2C_FIELD(temp_v1_5, s32 *, 4);
    M2C_FIELD(arg0, void ***, 0xB0) = func_00205D0C(0, M2C_FIELD(arg0, s32 **, 0x4C), 3, temp_v0_3, arg0, func_0042BA40, &D_800A5F24);
    M2C_FIELD(arg0, s32 *, 0) = 2;
    func_0042C79C(arg0);
    temp_s4 = M2C_FIELD(arg0, s32 *, 0xE8);
    var_s1_2 = M2C_FIELD(arg0, s32 *, 0xE4);
    M2C_FIELD(arg0, s32 *, 0) = 3;
    if (var_s1_2 < temp_s4) {
        var_s0_2 = var_s1_2 * 0x288;
        do {
            var_s1_2 += 1;
            func_0021E5D4(M2C_FIELD(arg0, s32 *, 0xE0) + var_s0_2);
            var_s0_2 += 0x288;
        } while (var_s1_2 < temp_s4);
    }
    var_s1_3 = D_80119890;
    var_s0_3 = var_s1_3;
    if (var_s1_3 != NULL) {
        do {
            func_0021E5D4(var_s0_3);
            func_0021E5D4(var_s0_3 + 0x288);
            var_s1_3 = M2C_FIELD(var_s1_3, void **, 0x1320);
            var_s0_3 = var_s1_3;
        } while (var_s1_3 != NULL);
    }
    func_0042C840(arg0);
    temp_s4_2 = M2C_FIELD(arg0, s32 *, 0xE8);
    var_s1_4 = M2C_FIELD(arg0, s32 *, 0xE4);
    if (var_s1_4 < temp_s4_2) {
        var_s2_2 = var_s1_4 * 0x288;
        do {
            var_s1_4 += 1;
            temp_s0 = M2C_FIELD(arg0, s32 *, 0xE0) + var_s2_2;
            func_0021F1B4(temp_s0);
            func_0021A344(temp_s0);
            var_s2_2 += 0x288;
        } while (var_s1_4 < temp_s4_2);
    }
    M2C_FIELD(arg0, s32 *, 0) = 4;
    func_0042E894(&D_80119870);
    temp_s0_2 = ((__typeof__(&D_80119870))((s8 *)&D_80119870 + 0x40));
    func_0042FEEC((s32) temp_s0_2);
    func_0027ECF0(temp_s0_2);
    if ((M2C_FIELD(&D_80119870, u8 *, 0x1281) != 0) || (M2C_FIELD(arg0, s32 *, 0x18FA4) != 0)) {
        func_0042BF48((s32) arg0);
    }
}

