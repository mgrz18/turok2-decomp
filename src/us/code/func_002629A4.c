#include "common.h"
#include "m2c_macros.h"

s32 func_00220408(M2C_UNK *, s32);                  /* extern */
M2C_UNK func_00243414(s32, void *, M2C_UNK);        /* extern */
M2C_UNK func_00262960(s32, void *, M2C_UNK);        /* extern */
extern M2C_UNK D_800F7078;

void func_002629A4(s32 arg0, void *arg1, M2C_UNK arg2) {
    if ((func_00220408(&D_800F7078, arg0) == 0) && (M2C_FIELD(arg1, s8 *, 0x34) == 0)) {
        func_00262960(arg0, arg1, arg2);
        func_00243414(arg0, arg1, 1);
    }
}
