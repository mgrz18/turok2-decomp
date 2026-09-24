#include "common.h"
#include "m2c_macros.h"

void func_0028E9CC(void *arg0, void *arg1) {
    M2C_FIELD(arg0, s32 **, 4) = (s32 *) M2C_FIELD(arg1, s32 **, 4);
    *M2C_FIELD(arg1, s32 **, 4) = 0;
}
