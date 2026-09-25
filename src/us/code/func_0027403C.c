#include "common.h"
#include "m2c_macros.h"

s32 func_0027314C(s32);

extern s32 D_800B2A5C;
extern s32 D_800B2A60;

void func_0027403C(void **arg0) {
    s32 var_s0;
    s32 var_s2;
    void *var_s1;

    var_s2 = 0;
    var_s1 = ((__typeof__(arg0))((s8 *)arg0 + 4));
    var_s0 = 0;
    do {
        if ((M2C_FIELD(var_s1, s32 *, 8) != -1) && (M2C_FIELD(*arg0, s16 *, 0x102) != var_s0)) {
            func_0027314C((s32) var_s1);
            var_s2 += 1;
        }
        var_s0 += 1;
        var_s1 += 0xCC;
    } while (var_s0 < 0x10);
    D_800B2A5C = var_s2;
    if (D_800B2A60 < var_s2) {
        D_800B2A60 = var_s2;
    }
}
