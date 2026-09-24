#include "common.h"
#include "m2c_macros.h"

s32 func_002017D4(s32, s32);
s32 func_00205A2C(s32, s32);

void func_0020BEE8(s32 *arg0) {
    s32 *temp_v0;
    s32 *temp_v0_2;
    s32 temp_a1;
    s32 temp_s2;
    s32 temp_s4;
    s32 var_s0;
    s32 var_s1;

    temp_v0 = func_002017D4(*arg0, 2);
    temp_s4 = *temp_v0;
    var_s1 = 0;
    if (temp_s4 > 0) {
        do {
            temp_v0_2 = func_002017D4((s32) temp_v0, var_s1);
            temp_s2 = *temp_v0_2;
            var_s0 = 0;
            if (temp_s2 > 0) {
                do {
                    temp_a1 = M2C_FIELD(func_002017D4(func_002017D4((s32) temp_v0_2, var_s0), 0), s32 *, 8);
                    if (temp_a1 != 0) {
                        func_00205A2C(0, temp_a1);
                    }
                    var_s0 += 1;
                } while (var_s0 < temp_s2);
            }
            var_s1 += 1;
        } while (var_s1 < temp_s4);
    }
}
