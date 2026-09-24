#include "common.h"
#include "m2c_macros.h"

s32 func_00220408(M2C_UNK *, void *);               /* extern */
s32 func_00284188();                                /* extern */
extern M2C_UNK D_800F7078;

void func_0025EE38(void *arg0, s32 *arg1) {
    if (func_00220408(&D_800F7078, arg0) == 0) {
        M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2100);
    }
    if (func_00284188() != 0) {
        *arg1 |= 0x200;
    }
}
