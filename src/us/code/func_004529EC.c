#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00275A74();                            /* extern */
M2C_UNK func_00275ABC();                            /* extern */
M2C_UNK func_00275E1C(s32);                         /* extern */
s32 func_00275F7C();                                /* extern */

s32 func_004529EC(void) {
    func_00275E1C(func_00275F7C());
    func_00275A74();
    func_00275ABC();
    return 1;
}
