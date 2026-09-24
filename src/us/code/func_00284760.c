#include "common.h"
#include "m2c_macros.h"

s16 func_002261A8(M2C_UNK *);                       /* extern */
extern M2C_UNK D_800F7078;

void func_00284760(void *arg0, s8 arg1) {
    M2C_FIELD(arg0, s8 *, 0) = 2;
    M2C_FIELD(arg0, s8 *, 1) = arg1;
    M2C_FIELD(arg0, s16 *, 2) = func_002261A8(&D_800F7078);
}
