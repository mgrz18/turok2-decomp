/* NON-MATCHING: 340 of 344 words differ: raw m2c draft of the whole function (0x508); Turok 2 variant of Rage Wars func_8044C7B8_de (MENU/ITEMS/8044ACCC_13.CPP), not worked on. */
/* WRONG CUT: REAL func_0042BA40 size 0x508 (splat cut it into func_0042BA40 + func_0042BE68). */
#include "common.h"
#include "m2c_macros.h"

s32 func_0042C194(s32, s32, s32, s32, s32);
s32 func_0042C560(s32, s32, s32, s32);

s32 func_00200B00(void ***, M2C_UNK);               /* extern */
M2C_UNK func_0020192C(void *, void *, M2C_UNK);     /* extern */
M2C_UNK func_002057B0(M2C_UNK, void ***, void **);  /* extern */
void **func_00205D7C(M2C_UNK, void ***, s32);       /* extern */
M2C_UNK func_00210EF0(void *);                      /* extern */
M2C_UNK func_0021E6AC(void *, void *, s32, s32);    /* extern */
s32 func_00220A1C(void *, void *);                  /* extern */
s32 func_002242F8(void *, M2C_UNK);                 /* extern */
s32 *func_00225E88(void *, u16);                    /* extern */
f32 func_002465F0(void *, s32, s32, s32);           /* extern */
s32 func_00267D94(void *);                          /* extern */
s32 func_00267DBC(void *);                          /* extern */
M2C_UNK func_0042DA60(s32, void *, s32, s32);       /* extern */
extern f32 D_800A5F54;
extern M2C_UNK D_800A5F58;
extern f32 D_800A5F60;
extern f32 D_800A5F64;
extern s32 D_80119874;
extern s32 D_80119878;
extern M2C_UNK D_80119928;
extern f32 D_8011992C;
extern s32 D_80119948;

void func_0042BA40(void *arg0, void ***arg1) {
    M2C_UNK sp18;
    void *sp68;
    s32 sp6C;
    void **sp70;
    s32 sp74;
    M2C_UNK *sp78;
    void *sp7C;
    M2C_UNK *var_s0;
    f32 temp_f0;
    f32 temp_f1;
    f32 temp_f20;
    f32 var_f20;
    s16 var_v0;
    s32 temp_fp;
    s32 temp_s7;
    s32 temp_v0_3;
    s32 var_s2;
    s32 var_s2_2;
    s32 var_s3;
    s32 var_s3_2;
    s32 var_s3_3;
    s32 var_s4;
    s32 var_s5;
    void **temp_v0;
    void *temp_s0;
    void *temp_s0_2;
    void *temp_s2;
    void *temp_v0_2;
    void *var_a1;
    void *var_s0_2;
    void *var_s1;
    void *var_s6;
    void *var_v1;

    if (func_00200B00(arg1, 1) != 0) {
        temp_s0 = **arg1;
        temp_fp = M2C_FIELD(temp_s0, s32 *, 4);
        sp6C = func_002242F8(arg0, 0);
        temp_v0 = func_00205D7C(0, arg1, (temp_fp * 0x288) + 8);
        sp70 = temp_v0;
        if (temp_v0 != NULL) {
            func_0020192C(temp_s0, *temp_v0, 1);
            temp_v0_2 = *sp70;
            M2C_FIELD(temp_v0_2, s32 *, 0) = 0x288;
            M2C_FIELD(temp_v0_2, s32 *, 4) = temp_fp;
            temp_s2 = temp_s0 + 8;
            sp68 = temp_v0_2 + 8;
            if (M2C_FIELD(arg0, s32 *, 0x18FB8) == 0) {
                temp_v0_3 = func_0042C560((s32) arg0, (s32) (temp_s0 + 0xC), M2C_FIELD(arg0, s32 *, 0x18FA8), (s32) (arg0 + 0x18FBC));
                M2C_FIELD(arg0, s32 *, 0x18FD0) = temp_v0_3;
                if (temp_v0_3 != 0) {
                    M2C_FIELD(arg0, s32 *, 0x18FB8) = 1;
                }
            }
            var_s5 = 0;
            sp74 = 0;
            if (M2C_FIELD(arg0, s32 *, 0x18FB8) != 0) {
                sp74 = func_0042C194((s32) arg0, M2C_FIELD(arg0, s32 *, 0x18FA8), M2C_FIELD(arg0, s32 *, 0x18FD0), (s32) &sp18, 4);
            }
            var_s3 = 0;
            M2C_FIELD(arg0, void **, 0xE0) = sp68;
            M2C_FIELD(arg0, s32 *, 0xE8) = temp_fp;
            if (temp_fp > 0) {
                var_v1 = sp68;
                var_a1 = temp_s2;
                do {
                    var_s3 += 1;
                    temp_s0_2 = var_v1;
                    var_v1 = temp_s0_2 + 0x288;
                    M2C_FIELD(temp_s0_2, s32 **, 0x14) = func_00225E88(arg0, M2C_FIELD(var_a1, u16 *, 0x22));
                    var_a1 += 0x28;
                } while (var_s3 < temp_fp);
            }
            var_s3_2 = 0;
            M2C_FIELD(arg0, s32 *, 0xE4) = 0;
            if (temp_fp > 0) {
                temp_f20 = M2C_FIELD(&D_800A5F58, f32 *, 0);
                var_s6 = temp_s2;
                sp78 = &sp18;
                sp7C = sp68;
                do {
                    func_0021E6AC(sp7C, var_s6, sp6C, var_s3_2);
                    if ((*func_00225E88(arg0, M2C_FIELD(var_s6, u16 *, 0x22)) == 0xB) || (var_s3_2 == 0)) {
                        M2C_FIELD(arg0, s32 *, 0xE4) = (s32) (M2C_FIELD(arg0, s32 *, 0xE4) + 1);
                        M2C_FIELD(&D_80119928, s32 *, 0) = (s32) M2C_FIELD(var_s6, s32 *, 4);
                        M2C_FIELD(&D_80119928, s32 *, 4) = (s32) M2C_FIELD(var_s6, s32 *, 8);
                        M2C_FIELD(&D_80119928, s32 *, 8) = (s32) M2C_FIELD(var_s6, s32 *, 0xC);
                        D_8011992C += M2C_FIELD(&D_800A5F58, f32 *, 4);
                        D_80119948 = func_00220A1C(arg0, var_s6 + 4);
                        var_s2 = 0;
                        if (D_80119878 > 0) {
                            var_s4 = 0;
                            var_s0 = (var_s5 * 0x14) + sp78;
                            do {
                                temp_s7 = D_80119874 + var_s4;
                                if (sp74 != 0) {
                                    M2C_FIELD(var_s6, u16 *, 0x20) = (u16) M2C_FIELD(var_s0, u16 *, 0x12);
                                    func_00210EF0(((__typeof__(var_s0))((s8 *)var_s0 + 0xC)));
                                    temp_f1 = M2C_FIELD(var_s0, f32 *, 0xC);
                                    var_v0 = 0x7FFF;
                                    if (!(temp_f20 < temp_f1) && (var_v0 = -0x7FFF, !(temp_f1 < D_800A5F60)) && (var_v0 = 0x7FFF, !(temp_f20 < temp_f1))) {
                                        M2C_FIELD(var_s6, s16 *, 0x24) = (s16) (s32) (temp_f1 * D_800A5F64);
                                    } else {
                                        M2C_FIELD(var_s6, s16 *, 0x24) = var_v0;
                                    }
                                    M2C_FIELD(var_s6, s32 *, 4) = (s32) M2C_FIELD(var_s0, s32 *, 0);
                                    M2C_FIELD(var_s6, s32 *, 8) = (s32) M2C_FIELD(var_s0, s32 *, 4);
                                    M2C_FIELD(var_s6, s32 *, 0xC) = (s32) M2C_FIELD(var_s0, s32 *, 8);
                                    var_s5 += 1;
                                    var_s0 = (__typeof__(var_s0))((s8 *)((__typeof__(var_s0))((s8 *)var_s0 + 0x14)));
                                    if (var_s5 >= sp74) {
                                        var_s0 = sp78;
                                        var_s5 = 0;
                                    }
                                }
                                func_0042DA60(temp_s7, var_s6, sp6C, var_s3_2);
                                var_s2 += 1;
                                var_s4 += 0x1328;
                            } while (var_s2 < D_80119878);
                        }
                    }
                    var_s6 += 0x28;
                    var_s3_2 += 1;
                    sp7C += 0x288;
                } while (var_s3_2 < temp_fp);
            }
            var_s3_3 = M2C_FIELD(arg0, s32 *, 0xE4);
            if (var_s3_3 < temp_fp) {
                var_s1 = (var_s3_3 * 0x288) + sp68;
loop_28:
                var_f20 = D_800A5F54;
                M2C_FIELD(var_s1, void **, 0x1A8) = NULL;
                if (func_00267DBC(var_s1) != 0) {
                    var_s2_2 = M2C_FIELD(arg0, s32 *, 0xE4);
                    if (var_s2_2 < temp_fp) {
                        var_s0_2 = (var_s2_2 * 0x288) + sp68;
loop_31:
                        if ((*M2C_FIELD(var_s1, s32 **, 0x14) == *M2C_FIELD(var_s0_2, s32 **, 0x14)) && (func_00267D94(var_s0_2) != 0) && (var_s1 != var_s0_2)) {
                            if (M2C_FIELD(var_s1, void **, 0x1A8) != NULL) {
                                temp_f0 = func_002465F0(var_s1, M2C_FIELD(var_s0_2, s32 *, 4), M2C_FIELD(var_s0_2, s32 *, 8), M2C_FIELD(var_s0_2, s32 *, 0xC));
                                if (temp_f0 < var_f20) {
                                    M2C_FIELD(var_s1, void **, 0x1A8) = var_s0_2;
                                    var_f20 = temp_f0;
                                    var_s2_2 += 1;
                                    var_s0_2 += 0x288;
                                    if (var_s2_2 >= temp_fp) {
                                        goto block_39;
                                    }
                                    goto loop_31;
                                }
                                goto block_52;
                            }
                            M2C_FIELD(var_s1, void **, 0x1A8) = var_s0_2;
                            var_f20 = func_002465F0(var_s1, M2C_FIELD(var_s0_2, s32 *, 4), M2C_FIELD(var_s0_2, s32 *, 8), M2C_FIELD(var_s0_2, s32 *, 0xC));
                            goto block_52;
                        }
block_52:
                        var_s2_2 += 1;
                        var_s0_2 += 0x288;
                        if (var_s2_2 >= temp_fp) {
                            var_s3_3 += 1;
                            var_s1 += 0x288;
                            if (var_s3_3 >= temp_fp) {
                                func_002057B0(0, arg1, sp70);
                                M2C_FIELD(arg0, s32 *, 0xF00) = 0;
                                return;
                            }
                            goto loop_28;
                        }
                        goto loop_31;
                    }
                }
block_39:
                var_s3_3 += 1;
                var_s1 += 0x288;
                if (var_s3_3 >= temp_fp) {
                    goto block_40;
                }
                goto loop_28;
            }
            goto block_40;
        }
block_40:
        func_002057B0(0, arg1, sp70);
        M2C_FIELD(arg0, s32 *, 0xF00) = 0;
    }
}

