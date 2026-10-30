/* NON-MATCHING: 62 of 105 words differ: m2c draft, not worked on (time) */
#include "common.h"
#include "m2c_macros.h"

s32 func_00425E74(s32, s32, s32, s32, s32, s32, s32, s32);

extern M2C_UNK D_800C1404;
extern M2C_UNK D_800C2028;

M2C_UNK func_0029E3E0(u8 *, M2C_UNK *, s32);        /* extern */
M2C_UNK func_00426E04(M2C_UNK *, M2C_UNK *);        /* extern */
s32 func_00426E3C(M2C_UNK *, M2C_UNK);              /* extern */
extern M2C_UNK D_800B48D4;
extern M2C_UNK D_800B48E0;
extern s32 D_80130990;
extern s32 D_801309BC;
static M2C_UNK D_800C1404;                          /* unable to generate initializer: unknown type; const */
static s32 D_800C2028 = -1;                         /* const */

s32 func_0041FB50(void *arg0, void *arg1) {
    M2C_UNK sp20;
    M2C_UNK sp30;
    M2C_UNK sp38;
    M2C_UNK sp40;
    s32 sp48;
    s32 sp4C;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_v1;
    s32 var_v1_2;
    u8 *var_a0_2;
    u8 *var_a0_3;
    u8 var_s1;

    var_s1 = 0x20;
    if (D_801309BC == 0) {
        if (D_80130990 != 0) {
            var_a0 = D_800C2028;
        } else {
            var_a0 = (s32) M2C_FIELD(M2C_FIELD(arg1, void **, 0x20), s8 *, 4);
        }
        temp_v0 = func_00425E74(var_a0, M2C_FIELD(arg0, s32 *, 0) - 3, (s32) &sp48, (s32) &sp20, (s32) &sp30, (s32) &sp4C, (s32) &sp38, (s32) &sp40);
        if ((sp48 != 0) && (temp_v0 == 0)) {
            if (func_00426E3C(&sp20, 0x10) != 0) {
                func_00426E04(&sp20, &D_800B48D4);
            }
        } else {
            func_00426E04(&sp20, &D_800B48E0);
            func_00426E04(&sp30, &D_800B48E0);
            sp4C = 0;
        }
        var_a1 = 1;
        var_v1 = 0;
        var_a0_2 = M2C_FIELD(arg0, s32 *, 0x14) + 3;
        do {
            if (var_a1 != 0) {
                var_s1 = *(((__typeof__(&sp20))((s8 *)&sp20 + var_v1)));
            }
            if (var_s1 == 0) {
                var_a1 = 0;
                var_s1 = 0x20;
            }
            *var_a0_2 = var_s1;
            var_v1 += 1;
            var_a0_2 += 1;
        } while (var_v1 < 0x10);
        var_a1_2 = 1;
        var_a0_3 = var_a0_2 + 1;
        var_v1_2 = 0;
        do {
            if (var_a1_2 != 0) {
                var_s1 = *(((__typeof__(&sp30))((s8 *)&sp30 + var_v1_2)));
            }
            if (var_s1 == 0) {
                var_a1_2 = 0;
                var_s1 = 0x20;
            }
            *var_a0_3 = var_s1;
            var_v1_2 += 1;
            var_a0_3 += 1;
        } while (var_v1_2 < 4);
        *var_a0_3 += 2;
        func_0029E3E0(var_a0_3, &D_800C1404, sp4C);
    }
    return 0;
}
