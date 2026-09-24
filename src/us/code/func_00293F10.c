#include "common.h"
#include "m2c_macros.h"

s32 func_0029E230(s32);                             /* extern */

void func_00293F10(void *arg0, void *arg1) {
    s32 temp_v0;

    temp_v0 = func_0029E230(1);
    M2C_FIELD(arg1, s32 *, 0x10) = (s32) M2C_FIELD(arg0, s32 *, 0x20);
    M2C_FIELD(arg1, void **, 0) = (void *) M2C_FIELD(arg0, void **, 0);
    M2C_FIELD(arg0, void **, 0) = arg1;
    func_0029E230(temp_v0);
}
