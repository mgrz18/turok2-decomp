#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00290EF0(s32);                         /* extern */

void func_00275248(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x28) = -1;
    func_00290EF0(M2C_FIELD(arg0, s32 *, 0x14));
}
