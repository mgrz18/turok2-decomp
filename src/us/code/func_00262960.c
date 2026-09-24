#include "common.h"
#include "m2c_macros.h"

s32 func_00220408(M2C_UNK *, void *);               /* extern */
extern M2C_UNK D_800F7078;

void func_00262960(void *arg0) {
    if (func_00220408(&D_800F7078, arg0) == 0) {
        M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2100);
    }
}
