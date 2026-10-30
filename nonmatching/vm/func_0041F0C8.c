/* NON-MATCHING: 133 of 153 words differ: m2c draft, not worked on (time) */
#include "common.h"
#include "m2c_macros.h"

s32 func_00412438(s32);
s32 func_00426164(s32, s32, s32, s32, s32, s32);
s32 func_0042670C(s32, s32);
s32 func_00426A94(s32, s32);

extern M2C_UNK D_800C2010;
extern M2C_UNK D_800C2014;
extern M2C_UNK D_800C2018;
extern M2C_UNK D_800C201C;
extern M2C_UNK D_800C2028;

M2C_UNK func_002052D8(M2C_UNK, s32);                /* extern */
M2C_UNK func_002053A8(M2C_UNK);                     /* extern */
M2C_UNK func_00412B14();                            /* extern */
M2C_UNK func_00416644(M2C_UNK *, M2C_UNK *, void *, s32, M2C_UNK *); /* extern */
s32 func_004267A0(s32, s32 *);                      /* extern */
s32 func_00426E6C(s32, void *);                     /* extern */
extern M2C_UNK D_0043BD68;
extern M2C_UNK D_0043BDF8;
extern M2C_UNK D_0043BEF4;
extern M2C_UNK D_0043C1C4;
extern M2C_UNK D_800B45CC;
extern M2C_UNK D_800B45D8;
extern s32 D_800F70A8;
extern M2C_UNK D_80119E2C;
extern s32 D_80130984;
extern s32 D_80130990;
static s32 D_800C2010 = 0;                          /* const */
static s32 D_800C2014 = 0;                          /* const */
static s32 D_800C2018 = 0;                          /* const */
static s32 D_800C201C = 0;                          /* const */
static s32 D_800C2028 = -1;                         /* const */

s32 func_0041F0C8(s32 arg0, void *arg1) {
    M2C_UNK sp18;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    M2C_UNK *var_a1;
    s32 temp_s1;
    s32 temp_s3;
    s32 var_s0;
    s32 var_s5;
    s32 var_s6;
    s32 var_v0;
    void *var_a2;

    var_s0 = 0;
    var_s5 = 1;
    var_s6 = 0;
    if (D_80130990 != 0) {
        var_v0 = D_800C2028;
        var_a2 = NULL;
    } else {
        var_a2 = M2C_FIELD(arg1, void **, 0x1C);
        var_v0 = (s32) M2C_FIELD(M2C_FIELD(arg1, void **, 0x20), s8 *, 4);
    }
    sp20 = var_v0;
    temp_s3 = D_800F70A8 + 0x688;
    if (D_80130984 != 0) {
        temp_s1 = func_00426E6C(temp_s3, var_a2);
        var_s0 = func_004267A0(sp20, &sp24);
        if (var_s0 == 0) {
            if (sp24 >= temp_s1) {
                var_s0 = func_0042670C(sp20, (s32) &sp28);
                if ((var_s0 == 0) && (sp28 != 0)) {
                    var_s5 = 0;
                    var_s0 = func_00426A94(sp20, (s32) &sp18);
                    if (var_s0 == 0) {
                        var_s6 = 1;
                        var_s0 = func_00426164(sp20, temp_s3, D_800C2018, (s32) &D_800B45CC, (s32) &sp18, (s32) &D_800B45D8);
                    }
                }
            }
            if (var_s5 != 0) {
                func_00416644(&D_80119E2C, &D_0043C1C4, var_a2, M2C_FIELD(var_a2, s32 *, 0xA50), M2C_FIELD(arg1, M2C_UNK **, 0x14));
                return 0;
            }
            goto block_12;
        }
    }
block_12:
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
    if (var_s6 != 0) {
        if ((var_s0 == 0) && (func_00412B14(), func_00412438(0x191), (var_s0 == 0))) {
            var_a1 = &D_0043BD68;
        } else {
            var_a1 = &D_0043BDF8;
        }
        func_00416644(&D_80119E2C, var_a1, var_a2, M2C_FIELD(var_a2, s32 *, 0xA50), &D_0043BEF4);
    }
    return 1;
}
