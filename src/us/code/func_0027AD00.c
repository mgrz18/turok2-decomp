#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00277328();                            /* extern */

void func_0027AD00(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 4) = arg1;
    M2C_FIELD(arg0, s32 *, 8) = 2;
    func_00277328();
}
