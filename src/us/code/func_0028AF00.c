#include "common.h"
#include "m2c_macros.h"

s32 func_0029E230(s32);                             /* extern */

void func_0028AF00(void *arg0, void *arg1, s32 arg2) {
    s32 temp_v0;

    temp_v0 = func_0029E230(1);
    M2C_FIELD(arg1, s32 *, 4) = arg2;
    M2C_FIELD(arg1, void **, 0) = (void *) M2C_FIELD(arg0, void **, 0x2E0);
    M2C_FIELD(arg0, void **, 0x2E0) = arg1;
    func_0029E230(temp_v0);
}
