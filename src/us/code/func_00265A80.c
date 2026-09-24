#include "common.h"
#include "m2c_macros.h"

s32 func_00220408(M2C_UNK *, void *);               /* extern */
M2C_UNK func_00243414(void *, M2C_UNK, M2C_UNK);    /* extern */
extern M2C_UNK D_800F7078;

void func_00265A80(void *arg0, M2C_UNK arg1) {
    if (func_00220408(&D_800F7078, arg0) == 1) {
        func_00243414(arg0, arg1, 1);
        return;
    }
    func_00243414(arg0, arg1, 0);
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x10000);
}
