#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002052D8(M2C_UNK, s32);                /* extern */
M2C_UNK func_002053A8(M2C_UNK);                     /* extern */

void func_002820B4(void *arg0) {
    s32 temp_a1;

    func_002053A8(0);
    temp_a1 = M2C_FIELD(arg0, s32 *, 0xF00);
    if (temp_a1 != 0) {
        func_002052D8(0, temp_a1);
        M2C_FIELD(arg0, s32 *, 0xF00) = 0;
        M2C_FIELD(arg0, s32 *, 0xF04) = 0;
        M2C_FIELD(arg0, s32 *, 0xF08) = 0;
    }
}
