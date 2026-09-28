#include "common.h"
#include "m2c_macros.h"

void *func_00268A2C(M2C_UNK);                       /* extern */

void func_002545F0(void *arg0, M2C_UNK arg1) {
    f32 temp_f1;
    void *temp_v0;

    temp_v0 = func_00268A2C(arg1);
    if (temp_v0 != NULL) {
        M2C_FIELD(arg0, f32 *, 0xB24) += (M2C_FIELD(temp_v0, f32 *, 0x2C) - M2C_FIELD(arg0, f32 *, 0xB24)) * 0.2f;
    }
}
