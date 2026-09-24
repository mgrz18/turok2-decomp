#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0024E5F0(void *, M2C_UNK, M2C_UNK);    /* extern */
M2C_UNK func_00251A2C();                            /* extern */

void func_002563FC(void *arg0, M2C_UNK arg1) {
    func_00251A2C();
    if (M2C_FIELD(arg0, f32 *, 0xA78) != 0.0f) {
        func_0024E5F0(arg0, arg1, 0x22);
    }
}
