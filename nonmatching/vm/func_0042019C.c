/* NON-MATCHING: 164 of 174 words differ: m2c draft of the whole function, not worked on (time) */
/* WRONG CUT: one function 0x0042019C-0x00420454 (size 0x2B8); splat cut its tail into func_004203B0. */
#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800C2010;
extern M2C_UNK D_800C2014;
extern M2C_UNK D_800C2018;
extern M2C_UNK D_800C201C;
extern M2C_UNK D_800C2028;

M2C_UNK func_002052D8(M2C_UNK, s32);                /* extern */
M2C_UNK func_002053A8(M2C_UNK);                     /* extern */
void *func_0025342C(M2C_UNK *, void *);             /* extern */
void func_00416644();
extern M2C_UNK D_00438808;
extern M2C_UNK D_00439190;
extern M2C_UNK D_0043AC80;
extern M2C_UNK D_0043ACA4;
extern M2C_UNK D_0043ACC8;
extern M2C_UNK D_0043ACEC;
extern M2C_UNK D_0043B738;
extern M2C_UNK D_0043BEF4;
extern M2C_UNK D_80119870;
extern M2C_UNK D_80119E2C;
extern s32 D_8011B108;
extern s32 D_8011B114;
extern s32 D_8011F144;
extern s32 D_8011F148;
extern s32 D_80130940;
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_801309BC;

s32 func_0042019C(s32 arg0, void *arg1) {
    s32 sp18;
    M2C_UNK *var_a0;
    M2C_UNK *var_a1;
    s8 temp_v1;
    void *var_a2;
    void *var_a3;

    if ((D_800C2010 != 0) || (D_800C2014 != 0)) {
        func_002053A8(0);
    }
    if (D_800C2010 != 0) {
        func_002052D8(0, D_800C2010);
    }
    if (D_800C2014 != 0) {
        func_002052D8(0, D_800C2014);
    }
    D_800C2010 = 0;
    D_800C2014 = 0;
    D_800C2018 = 0;
    D_800C201C = 0;
    D_80130940 = 0;
    if (D_80130990 != 0) {
        sp18 = D_800C2028;
    } else {
        sp18 = (s32) M2C_FIELD(M2C_FIELD(arg1, void **, 0x20), s8 *, 4);
    }
    if (D_80130990 != 0) {
        var_a0 = &D_80119E2C;
        var_a1 = &D_0043B738;
        var_a3 = M2C_FIELD(arg1, void **, 0x20);
        D_8011F148 = 0;
        D_8011B108 = 0;
        D_8011F144 = 1;
        D_8011B114 = 0;
        var_a2 = NULL;
        goto block_27;
    }
    if (D_801309B4 != 0) {
        if (D_80130994 != 0) {
            D_801309BC = 1;
            var_a2 = func_0025342C(&D_80119870, M2C_FIELD(arg1, void **, 0x20));
            var_a1 = &D_00438808;
            var_a3 = M2C_FIELD(var_a2, void **, 0xA50);
            var_a0 = M2C_FIELD(var_a2, s32 *, 0x51C) + 0x53C;
            goto block_27;
        }
        if (D_80130984 != 0) {
            D_801309BC = 1;
            temp_v1 = M2C_FIELD(M2C_FIELD(arg1, void **, 0x20), s8 *, 4);
            var_a2 = M2C_FIELD(arg1, void **, 0x1C);
            sp18 = (s32) temp_v1;
            switch (temp_v1) {                      /* irregular */
            default:
                var_a1 = &D_0043AC80;
                break;
            case 1:
                var_a1 = &D_0043ACA4;
                break;
            case 2:
                var_a1 = &D_0043ACC8;
                break;
            case 3:
                var_a1 = &D_0043ACEC;
                break;
            }
            var_a3 = M2C_FIELD(var_a2, void **, 0xA50);
            var_a0 = M2C_FIELD(var_a2, s32 *, 0x51C) + 0x53C;
            goto block_27;
        }
        goto block_30;
    }
    var_a2 = func_0025342C(&D_80119870, M2C_FIELD(arg1, void **, 0x20));
    if (D_80130994 != 0) {
        var_a0 = ((__typeof__(&D_80119870))((s8 *)&D_80119870 + 0x5BC));
        var_a1 = &D_00439190;
        var_a3 = M2C_FIELD(var_a2, void **, 0xA50);
        D_801309BC = 1;
block_27:
        func_00416644(var_a0, var_a1, var_a2, var_a3);
        return 1;
    }
    if (D_80130984 != 0) {
        D_801309BC = 1;
        func_00416644(M2C_FIELD(var_a2, s32 *, 0x51C) + 0x53C, &D_0043BEF4, var_a2, M2C_FIELD(var_a2, void **, 0xA50), sp18);
        D_80130940 = 0;
        return 1;
    }
block_30:
    return 0;
}
