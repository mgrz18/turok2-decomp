#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_801206BC;
extern u32 D_80120FCC;
extern void *D_80121290;
extern M2C_UNK D_80121378;

void func_0028E53C(void) {
    M2C_UNK *var_v1;
    s32 var_a0_2;
    s32 var_a1;
    u16 temp_v0;
    u32 *var_a0;
    u32 temp_v1;
    u32 var_a2;
    u32 var_a3;

    var_a2 = 0;
    var_a3 = 0x80000000;
    var_a1 = 0;
    var_a0 = &D_80120FCC;
    do {
        temp_v1 = *var_a0;
        if (var_a2 < temp_v1) {
            var_a2 = temp_v1;
        }
        if (temp_v1 < var_a3) {
            var_a3 = temp_v1;
        }
        *var_a0 = temp_v1 + 1;
        var_a1 += 1;
        var_a0 = (__typeof__(var_a0))((s8 *)var_a0 + 0x10);
    } while (var_a1 < 0x18);
    var_v1 = &D_80121378;
    var_a0_2 = 0;
    if (&D_80121378 != NULL) {
        do {
            temp_v0 = M2C_FIELD(var_v1, u16 *, 6);
            var_v1 = M2C_FIELD(var_v1, M2C_UNK **, 0);
            var_a0_2 += temp_v0 << 0xC;
        } while (var_v1 != NULL);
    }
    M2C_FIELD(&D_801206BC, s32 *, 0) = var_a0_2;
    M2C_FIELD(&D_801206BC, u32 *, -0xC) = var_a2;
    M2C_FIELD(&D_801206BC, u32 *, -8) = var_a3;
    M2C_FIELD(&D_801206BC, s32 *, -4) = (s32) M2C_FIELD(D_80121290, s32 *, 0xC);
}
