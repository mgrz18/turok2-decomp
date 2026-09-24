#include "common.h"
#include "m2c_macros.h"

s32 func_00223F70(M2C_UNK *);                       /* extern */
M2C_UNK func_00228AEC(s32, s32);                    /* extern */
extern M2C_UNK D_800F7078;

void func_00228AB0(s32 arg0) {
    func_00228AEC(arg0, func_00223F70(&D_800F7078));
}
