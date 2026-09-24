#include "common.h"
#include "m2c_macros.h"

s32 func_00273A34(s32, s32, s32);

void func_002763AC(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, void **, 4) = arg0;
    M2C_FIELD(arg0, void **, 0) = arg0;
    func_00273A34((s32) (arg0 + 8), arg1, -1);
}
