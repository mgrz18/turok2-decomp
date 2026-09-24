#include "common.h"
#include "m2c_macros.h"

extern f32 D_800B6D28;

void func_00254704(void *arg0) {
    f32 temp_f0;
    f32 temp_f1;

    temp_f1 = M2C_FIELD(arg0, f32 *, 0xA30);
    if (temp_f1 > 0.0f) {
        temp_f0 = temp_f1 - D_800B6D28;
        M2C_FIELD(arg0, f32 *, 0xA30) = temp_f0;
        if (temp_f0 < 0.0f) {
            M2C_FIELD(arg0, f32 *, 0xA30) = 0.0f;
        }
    }
}
