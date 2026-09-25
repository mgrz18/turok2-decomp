#include "common.h"
#include "m2c_macros.h"

s32 func_0028582C(s32);

extern u8 D_800B6CF0;
extern M2C_UNK D_800F5C78;
extern M2C_UNK D_800F5EC0;

void func_002857C8(void) {
    M2C_UNK *var_s0;
    s32 var_s1;

    if (D_800B6CF0 != 0) {
        var_s1 = 0;
        var_s0 = &D_800F5EC0;
        do {
            func_0028582C((s32) var_s0);
            var_s1 += 1;
            var_s0 = (__typeof__(var_s0))((s8 *)var_s0 + 0x224);
        } while (var_s1 < 4);
        func_0028582C((s32) &D_800F5C78);
    }
}
