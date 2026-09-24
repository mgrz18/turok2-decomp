#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00243414(s32, M2C_UNK, M2C_UNK);       /* extern */
s32 func_002623EC();                                /* extern */

void func_00262830(s32 arg0, M2C_UNK arg1) {
    if (func_002623EC() != 0) {
        func_00243414(arg0, arg1, 0x3F);
    }
}
