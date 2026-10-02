#include "common.h"
#include "m2c_macros.h"

s32 func_00426480(s32);

M2C_UNK func_00285A68(s8);                          /* extern */

void func_00419560(void *arg0) {
    s8 temp_s0;

    temp_s0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x20), s8 *, 4);
    func_00285A68(temp_s0);
    func_00426480((s32) temp_s0);
}
