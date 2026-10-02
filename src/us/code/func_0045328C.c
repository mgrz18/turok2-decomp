#include "common.h"
#include "m2c_macros.h"

void func_0026EDA8(s32);
s32 func_00285304(void);

extern M2C_UNK D_800F5A50;
extern M2C_UNK D_800F5F8C;
extern M2C_UNK D_80130910;

void func_0045328C(void) {
    s32 temp_v0;
    s32 var_a0;
    s32 var_v1;

    func_00285304();
    var_v1 = 0;
    var_a0 = 0;
    do {
        temp_v0 = *(((__typeof__(&D_80130910))((s8 *)&D_80130910 + ((var_v1 << 2)))));
        var_v1 += 1;
        *(((__typeof__(&D_800F5F8C))((s8 *)&D_800F5F8C + var_a0))) = temp_v0;
        var_a0 += 0x224;
    } while (var_v1 < 4);
    func_0026EDA8((s32) &D_800F5A50);
}
