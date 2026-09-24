#include "common.h"
#include "m2c_macros.h"

void func_00218490(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0) = (s32) (M2C_FIELD(arg0, s32 *, 0) + (arg1 * M2C_FIELD(arg0, s32 *, 4)));
}
