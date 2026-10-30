/* NON-MATCHING: 235 of 245 words differ: m2c draft of the whole function, not worked on (time) */
/* WRONG CUT: one function 0x0042085C-0x00420C30 (size 0x3D4); splat cut it into func_0042085C, func_00420878 and func_004208E0. */
#include "common.h"
#include "m2c_macros.h"

s32 func_00426480(s32);

extern M2C_UNK D_800C2028;

s32 func_002858CC(s32);                             /* extern */
M2C_UNK func_00285A80(s32);                         /* extern */
M2C_UNK func_00285AE0(void *);                      /* extern */
M2C_UNK func_00285BD0(void *, M2C_UNK);             /* extern */
void func_00416644();
s32 func_0042655C(s32);                             /* extern */
M2C_UNK func_0042C700(M2C_UNK *, s32, M2C_UNK);     /* extern */
M2C_UNK func_004303AC(M2C_UNK *);                   /* extern */
extern M2C_UNK D_00439190;
extern M2C_UNK D_0043B6CC;
extern M2C_UNK D_0043B6F0;
extern M2C_UNK D_0043B714;
extern M2C_UNK D_0043BED0;
extern M2C_UNK D_0043BEF4;
extern M2C_UNK D_800F6CB0;
extern M2C_UNK D_800F7078;
extern s32 D_80110054;
extern M2C_UNK D_80119E2C;
extern s32 D_8011F148;
extern s32 D_80130940;
extern s32 D_8013098C;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_80130998;
extern s32 D_801309B4;
extern s32 D_801309C0;

s32 func_0042085C(s32 arg0, void *arg1) {
    M2C_UNK *temp_v1;
    M2C_UNK *temp_v1_3;
    M2C_UNK *var_a0;
    M2C_UNK *var_a0_2;
    M2C_UNK *var_a1;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1_2;
    s32 var_s1;
    s32 var_s1_2;
    void *temp_v0_3;
    void *temp_v0_4;

    if (D_801309C0 == 8) {
        return 0;
    }
    temp_v1 = M2C_FIELD(arg1, M2C_UNK **, 0x24);
    if ((temp_v1 == &D_0043BEF4) || (temp_v1 == &D_0043BED0)) {
        D_80130940 = 0;
    }
    temp_v1_2 = D_801309C0;
    if ((temp_v1_2 == 9) || (temp_v1_2 == 5)) {
        func_00285AE0(M2C_FIELD(arg1, void **, 0x20));
        func_00285BD0(M2C_FIELD(arg1, void **, 0x20), 1);
    }
    if (temp_v1_2 == 9) {
        if ((D_80130994 != 0) && (D_801309B4 == 0) && (D_8013098C == 0)) {
            if (D_80130990 != 0) {
                var_s1 = D_800C2028;
            } else {
                var_s1 = (s32) M2C_FIELD(M2C_FIELD(arg1, void **, 0x20), s8 *, 4);
            }
            func_00285A80(var_s1);
            func_0042C700(&D_800F7078, M2C_FIELD(M2C_FIELD(arg1, void **, 0x1C), s32 *, 0x92C), 2);
            return 1;
        }
        func_004303AC(&D_800F6CB0);
        return 1;
    }
    if (D_80130990 != 0) {
        var_s1_2 = D_800C2028;
    } else {
        var_s1_2 = (s32) M2C_FIELD(M2C_FIELD(arg1, void **, 0x20), s8 *, 4);
    }
    if (D_801309C0 == 5) {
        func_00285A80(var_s1_2);
        func_0042C700(&D_800F7078, D_80110054, 2);
    }
    if (D_80130990 != 0) {
        if (D_801309C0 == 6) {
            D_8011F148 = 1;
        }
    }
    if ((D_801309C0 == 4) && (D_80130994 != 0) && (D_801309B4 == 0)) {
        func_00426480(var_s1_2);
        temp_s0 = func_0042655C(var_s1_2);
        temp_v0 = func_002858CC(var_s1_2);
        if (((temp_s0 == 0) || (temp_v0 != 0)) && (temp_s0 != -2) && (M2C_FIELD(arg1, M2C_UNK **, 0x24) == (M2C_UNK *)1)) {
            var_a0 = &D_80119E2C;
            var_a1 = &D_0043B6F0;
        } else {
            var_a0 = &D_80119E2C;
            var_a1 = &D_00439190;
        }
        goto block_54;
    }
    if ((D_801309C0 == 3) && (D_80130998 != 0)) {
        func_00426480(var_s1_2);
        temp_s0_2 = func_0042655C(var_s1_2);
        temp_v0_2 = func_002858CC(var_s1_2);
        if (((temp_s0_2 == 0) || (temp_v0_2 != 0)) && (temp_s0_2 != -2) && (M2C_FIELD(arg1, M2C_UNK **, 0x24) == (M2C_UNK *)1)) {
            var_a0 = &D_80119E2C;
            var_a1 = &D_0043B714;
            goto block_54;
        }
        temp_v0_3 = M2C_FIELD(arg1, void **, 0x1C);
        if (temp_v0_3 != NULL) {
            var_a0_2 = M2C_FIELD(temp_v0_3, s32 *, 0x51C) + 0x53C;
        } else {
            var_a0_2 = &D_80119E2C;
        }
        func_00416644(var_a0_2, &D_0043BEF4, M2C_FIELD(arg1, void **, 0x1C), M2C_FIELD(arg1, void **, 0x20), 0);
        D_80130940 = 0;
        return 1;
    }
    if (M2C_FIELD(arg1, M2C_UNK **, 0x24) == (M2C_UNK *)1) {
        M2C_FIELD(arg1, M2C_UNK **, 0x24) = NULL;
    }
    temp_v1_3 = M2C_FIELD(arg1, M2C_UNK **, 0x24);
    if (temp_v1_3 != NULL) {
        if ((temp_v1_3 != &D_0043B6CC) && (temp_v0_4 = M2C_FIELD(arg1, void **, 0x1C), (temp_v0_4 != NULL))) {
            var_a0 = M2C_FIELD(temp_v0_4, s32 *, 0x51C) + 0x53C;
        } else {
            var_a0 = &D_80119E2C;
        }
        var_a1 = M2C_FIELD(arg1, M2C_UNK **, 0x24);
block_54:
        func_00416644(var_a0, var_a1, M2C_FIELD(arg1, void **, 0x1C), M2C_FIELD(arg1, void **, 0x20));
    }
    return 1;
}
