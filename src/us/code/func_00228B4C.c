#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200518(s32, void *);                 /* extern */
M2C_UNK func_00200738();                            /* extern */

void func_00228B4C(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, s16 *, 0x16) = 0;
    func_00200738();
    func_00200518(arg0 + 0x14, arg1);
}
