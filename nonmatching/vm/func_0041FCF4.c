/* NON-MATCHING: 111 of 116 words differ: m2c draft, not worked on (time) */
#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800C1404;
extern M2C_UNK D_800C1408;
extern M2C_UNK D_800C140C;
extern M2C_UNK D_800C2028;

M2C_UNK func_0029E3E0(u8 *, M2C_UNK *, s32);        /* extern */
s32 func_004267A0(s8, s32 *);                       /* extern */
s32 func_00426E6C(s32);                             /* extern */
extern M2C_UNK D_800B48B4;
extern M2C_UNK D_800B4914;
extern s32 D_800F70A8;
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_801309B4;
static M2C_UNK D_800C1404;                          /* unable to generate initializer: unknown type; const */
static M2C_UNK D_800C1408;                          /* unable to generate initializer: unknown type; const */
static M2C_UNK D_800C140C;                          /* unable to generate initializer: unknown type; const */
static s32 D_800C2028 = 0xFFFFFFFF;                 /* const */

s32 func_0041FCF4(void *arg0, void *arg1) {
    s32 sp11;
    s32 sp12;
    u8 sp10;
    s32 sp18;
    M2C_UNK *var_v0;
    s32 temp_s0;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_s2;
    void *temp_s1;
    void *temp_s1_2;
    void *temp_s1_3;
    void *var_s1;

    if (D_80130990 != 0) {
        var_a0 = D_800C2028;
    } else {
        var_a0 = (s32) M2C_FIELD(M2C_FIELD(arg1, void **, 0x20), s8 *, 4);
    }
    sp10 = M2C_UNALIGNED32(D_800C1408);
    if (D_80130984 != 0) {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B48B4;
        if (func_004267A0((s8) var_a0, &sp18) == 0) {
            var_a0_2 = 0x18;
            if (D_801309B4 == 0) {
                var_a0_2 = D_800F70A8 + 0x688;
            }
            var_s2 = func_00426E6C(var_a0_2);
            var_s1 = M2C_FIELD(arg0, M2C_UNK **, 0x14) + 0xC;
            func_0029E3E0(&sp10, &D_800C1404, sp18);
            goto block_14;
        }
    } else {
        if (D_80130990 != 0) {
            var_v0 = &D_800B4914;
            goto block_12;
        }
        if (D_80130994 != 0) {
            var_v0 = &D_800B48B4;
block_12:
            M2C_FIELD(arg0, M2C_UNK **, 0x14) = var_v0;
            temp_s0 = func_004267A0((s8) var_a0, &sp18);
            var_s2 = func_00426E6C(D_800F70A8 + 0x688);
            if (temp_s0 == 0) {
                var_s1 = M2C_FIELD(arg0, M2C_UNK **, 0x14) + 0xC;
                func_0029E3E0(&sp10, &D_800C1404, sp18);
block_14:
                M2C_FIELD(var_s1, u8 *, 0) = sp10;
                temp_s1 = var_s1 + 1;
                M2C_FIELD(var_s1, u8 *, 1) = sp11;
                temp_s1_2 = temp_s1 + 1;
                M2C_FIELD(temp_s1, u8 *, 1) = sp12;
                func_0029E3E0(&sp10, &D_800C1404, var_s2);
                temp_s1_3 = temp_s1_2 + 0xC;
                M2C_FIELD(temp_s1_2, u8 *, 0xC) = sp10;
                M2C_FIELD(temp_s1_3, u8 *, 1) = sp11;
                M2C_FIELD((temp_s1_3 + 1), u8 *, 1) = sp12;
            }
        } else {
            M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800C140C;
        }
    }
    return 0;
}
