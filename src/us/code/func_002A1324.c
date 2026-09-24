#include "common.h"
#include "m2c_macros.h"

s32 func_002A31E4();                                /* extern */
M2C_UNK func_002A31F0(s32);                         /* extern */

void func_002A1324(s32 arg0) {
    func_002A31F0(func_002A31E4() | arg0);
}
