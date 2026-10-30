/* NON-MATCHING: 165 of 186 words differ: m2c draft of the whole function, not worked on (time) */
/* WRONG CUT: one function 0x00425BC0-0x00425E74 (size 0x2B4); splat cut its tail into func_00425D94. */
#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800C222C;

M2C_UNK func_002847E0();                            /* extern */
M2C_UNK func_00285878(M2C_UNK);                     /* extern */
M2C_UNK func_002858A4();                            /* extern */
M2C_UNK func_0029E460(s8 *, u8 *, M2C_UNK, s8 *);   /* extern */
M2C_UNK func_004258E8(s32);                         /* extern */
s32 func_00427180(void *, s32, s32, s8 *, s8 *, s32, s32 *); /* extern */
M2C_UNK osPfsDeleteFile(void *, s32, s32, s8 *, s8 *); /* extern */
s32 osPfsReadWriteFile(void *, s32, M2C_UNK, M2C_UNK, s32, s32); /* extern */
extern s8 D_800F6750;
extern M2C_UNK D_80130C60;
extern M2C_UNK D_80130C70;
extern M2C_UNK D_80130C80;
static M2C_UNK D_800C222C;                          /* unable to generate initializer: unknown type; const */

s32 func_00425BC0(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 *arg4, u8 *arg5, void *arg6, void *arg7) {
    s8 sp20;
    s8 sp30;
    s32 temp_s0;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s5;
    s32 temp_t1;
    s32 var_s0;
    s8 *temp_a3;
    s8 *var_t0;
    s8 *var_t0_2;
    s8 var_a0;
    s8 var_a0_2;
    u8 *temp_t1_2;
    u8 *var_a1;
    u8 *var_a1_2;
    u8 var_v1;
    u8 var_v1_2;
    void *temp_s1;

    temp_s0 = arg0 * 4;
    if (*(((__typeof__(&D_80130C60))((s8 *)&D_80130C60 + temp_s0))) != 3) {
        return -2;
    }
    func_00285878(1);
    func_002847E0();
    var_s0 = *(((__typeof__(&D_80130C70))((s8 *)&D_80130C70 + temp_s0)));
    D_800F6750 = 2;
    if (var_s0 == 0) {
        var_t0 = &sp30;
        var_a1 = arg5;
        temp_t1 = var_a1 + 4;
        do {
            var_v1 = *var_a1;
            if ((u32) (var_v1 - 0x61) < 0x1AU) {
                var_v1 -= 0x20;
            }
            *var_a1 = var_v1;
            var_a0 = 0;
            temp_a3 = var_t0;
loop_7:
            if (*var_a1 != *(((__typeof__(&D_800C222C))((s8 *)&D_800C222C + var_a0)))) {
                var_a0 += 1;
                if (var_a0 < 0x42) {
                    goto loop_7;
                }
            } else {
                *temp_a3 = var_a0;
            }
            if (var_a0 == 0x42) {
                *var_t0 = 0;
            }
            var_a1 += 1;
            var_t0 += 1;
        } while ((s32) var_a1 < temp_t1);
        temp_s5 = (arg1 + 0xFF) & ~0xFF;
        func_0029E460(&sp20, arg3, 0x10, temp_a3);
        var_t0_2 = &sp20;
        var_a1_2 = arg3;
        temp_t1_2 = var_a1_2 + 0x10;
        do {
            var_v1_2 = *var_a1_2;
            if ((u32) (var_v1_2 - 0x61) < 0x1AU) {
                var_v1_2 -= 0x20;
            }
            *var_a1_2 = var_v1_2;
            var_a0_2 = 0;
loop_19:
            if (*var_a1_2 != *(((__typeof__(&D_800C222C))((s8 *)&D_800C222C + var_a0_2)))) {
                var_a0_2 += 1;
                if (var_a0_2 < 0x42) {
                    goto loop_19;
                }
            } else {
                *var_t0_2 = var_a0_2;
            }
            if (var_a0_2 == 0x42) {
                *var_t0_2 = 0;
            }
            var_a1_2 += 1;
            var_t0_2 += 1;
        } while ((s32) var_a1_2 < (s32) temp_t1_2);
        temp_s1 = (arg0 * 0x68) + &D_80130C80;
        temp_s3 = M2C_FIELD(arg6, u8 *, 1) | (M2C_FIELD(arg6, u8 *, 0) << 8);
        temp_s2 = (M2C_FIELD(arg7, u8 *, 0) << 0x18) | (M2C_FIELD(arg7, u8 *, 1) << 0x10) | (M2C_FIELD(arg7, u8 *, 2) << 8) | M2C_FIELD(arg7, u8 *, 3);
        var_s0 = -1;
        if ((func_00427180(temp_s1, temp_s3, temp_s2, &sp20, &sp30, temp_s5, arg4) == 0) || (-1 == 0)) {
            var_s0 = osPfsReadWriteFile(temp_s1, *arg4, 1, 0, temp_s5, arg2);
            if (var_s0 != 0) {
                osPfsDeleteFile(temp_s1, temp_s3, temp_s2, &sp20, &sp30);
                var_s0 = -1;
            }
        }
        func_004258E8(arg0);
    }
    func_002858A4();
    return var_s0;
}
