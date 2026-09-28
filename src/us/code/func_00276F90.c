#include "common.h"
#include "m2c_macros.h"

s32 func_002017D4(s32, s32);

void func_00276F90(void *arg0, s32 arg1) {
    f64 var_f2;
    s32 temp_s1;
    s32 temp_v0_2;
    void *temp_v0;

    temp_s1 = (arg1 << 1);
    temp_v0 = func_002017D4(M2C_FIELD(M2C_FIELD(arg0, void **, 0), s32 *, 0x2B50), temp_s1);
    temp_v0_2 = M2C_FIELD(temp_v0, s32 *, 0);
    var_f2 = (f64) temp_v0_2;
    if (temp_v0_2 < 0) {
        var_f2 += 4294967296.0;
    }
    M2C_FIELD(arg0, f32 *, 0x20) = (f32) ((f32) var_f2 * 0.01f);
    M2C_FIELD(arg0, f32 *, 0x24) = (f32) M2C_FIELD(temp_v0, u16 *, 4);
    func_002017D4(M2C_FIELD(M2C_FIELD(arg0, void **, 0), s32 *, 0x2B50), temp_s1 | 1);
}
