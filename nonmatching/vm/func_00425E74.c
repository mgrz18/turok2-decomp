/* NON-MATCHING: 92 of 132 words differ: m2c draft of the whole function, not worked on (time) */
/* WRONG CUT: one function 0x00425E74-0x00426064 (size 0x1F0); splat cut its tail into func_00425F84. */
#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800C2224;
extern M2C_UNK D_800C222C;

M2C_UNK func_0029E460(void *, s32, M2C_UNK);        /* extern */
extern M2C_UNK D_80130C60;
static s32 D_800C2224 = 0;                          /* const */
static M2C_UNK D_800C222C;                          /* unable to generate initializer: unknown type; const */

s32 func_00425E74(s32 arg0, s32 arg1, s32 *arg2, u8 *arg3, u8 *arg4, u32 *arg5, void *arg6, void *arg7) {
    s32 temp_s0;
    s32 temp_s1;
    s32 temp_s2;
    s32 var_a0;
    s32 var_a2;
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 *var_a1;
    u8 *var_a3;

    if (*(((__typeof__(&D_80130C60))((s8 *)&D_80130C60 + (arg0 * 4)))) != 3) {
        return -2;
    }
    temp_s1 = arg0 * 0x204;
    temp_s2 = arg1 << 5;
    if (M2C_FIELD((temp_s1 + D_800C2224 + temp_s2), s32 *, 4) == 0) {
        *arg2 = 0;
    } else {
        *arg2 = 1;
        *arg5 = (u32) M2C_FIELD((temp_s1 + D_800C2224 + temp_s2), u32 *, 4) >> 8;
        temp_s0 = temp_s2 + 4;
        func_0029E460(arg6, D_800C2224 + temp_s1 + temp_s0 + 8, 2);
        func_0029E460(arg7, D_800C2224 + temp_s1 + temp_s0 + 4, 4);
        var_a0 = 0;
        var_a3 = arg3;
        do {
            temp_v1 = temp_s2 + (temp_s1 + D_800C2224) + 0x12 + var_a0;
            if ((u8) *temp_v1 >= 0x42U) {
                *var_a3 = 0x7E;
            } else {
                *var_a3 = *(&D_800C222C + *temp_v1);
            }
            var_a0 += 1;
            var_a3 += 1;
        } while (var_a0 < 0x10);
        var_a2 = 0;
        var_a1 = arg4;
        do {
            temp_v1_2 = (arg1 << 5) + ((arg0 * 0x204) + D_800C2224) + 0xE + var_a2;
            if ((u8) *temp_v1_2 >= 0x42U) {
                *var_a1 = 0x7E;
            } else {
                *var_a1 = *(&D_800C222C + *temp_v1_2);
            }
            var_a2 += 1;
            var_a1 += 1;
        } while (var_a2 < 4);
        M2C_FIELD(arg7, s8 *, 4) = 0;
        M2C_FIELD(arg6, s8 *, 2) = 0;
    }
    return 0;
}
