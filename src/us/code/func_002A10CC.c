#include "common.h"
#include "m2c_macros.h"

s32 func_002A09E8(s32, s32, s32, s32, s32, s32);

void func_002A10CC(s32 arg0, void *arg1, s32 arg2) {
    func_002A09E8(arg0, M2C_FIELD(arg1, s32 *, 0xC), arg2, M2C_FIELD(arg1, s32 *, 0), M2C_FIELD(arg1, s32 *, 4), M2C_FIELD(arg1, s32 *, 8));
}
