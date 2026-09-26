#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800AF618;

s32 func_00255048(void *arg0, s32 *arg1) {
    s16 temp_a2;
    s32 *var_a1;
    s32 var_a3;
    s32 var_t0;
    s32 var_t1;
    s32 var_v0;
    s32 var_v1;
    void *temp_v1;

    var_a1 = arg1;
    var_t1 = 0;
    var_a3 = 0;
    var_t0 = 0;
    do {
        temp_v1 = *(((__typeof__(&D_800AF618))((s8 *)&D_800AF618 + var_t0)));
        if (M2C_FIELD((arg0 + var_a3), s8 *, 0x972) == 0) {
            var_v1 = 0;
        } else {
            if (M2C_FIELD(arg0, s32 *, 0x3C8) & 0x80) {
                var_v0 = M2C_FIELD(temp_v1, s32 *, 0x14) & 2;
            } else {
                var_v0 = M2C_FIELD(temp_v1, s32 *, 0x14) & 1;
            }
            if (var_v0 == 0) {
                var_v1 = 0;
            } else {
                temp_a2 = M2C_FIELD(temp_v1, s16 *, 0xC);
                var_v1 = 1;
                if (M2C_FIELD((arg0 + temp_a2), s8 *, 0x972) != 0) {
                    var_v1 = (var_a3 < temp_a2) ^ 1;
                }
            }
        }
        *var_a1 = var_v1;
        if (var_v1 != 0) {
            var_t1 += 1;
        }
        var_a1 = (__typeof__(var_a1))((s8 *)((__typeof__(var_a1))((s8 *)var_a1 + 4)));
        var_a3 += 1;
        var_t0 += 4;
    } while (var_a3 < 0x23);
    return var_t1;
}
