#include "common.h"
#include "m2c_macros.h"

s32 func_0028591C(s32);
s32 func_0041DD90(s32, s32, s32);

extern s32 D_800C2028;
extern s32 D_80130990;
extern s32 D_801309A4;

s32 func_00423964(void *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 var_v1;

    if (D_801309A4 == 0) {
        if (D_80130990 != 0) {
            var_v1 = D_800C2028;
        } else {
            var_v1 = (s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x20), s8 *, 4);
        }
        if (var_v1 != -1) {
            if (D_80130990 != 0) {
                var_a0 = D_800C2028;
            } else {
                var_a0 = (s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x20), s8 *, 4);
            }
            if (func_0028591C(var_a0) != 0) {
                if (D_80130990 != 0) {
                    var_a1 = D_800C2028;
                } else {
                    var_a1 = (s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x20), s8 *, 4);
                }
                if (func_0041DD90((s32) arg0, var_a1, 0) != 0) {
                    D_801309A4 = 1;
                }
            }
        }
    }
    return D_801309A4;
}
