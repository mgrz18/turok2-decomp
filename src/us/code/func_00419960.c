#include "common.h"
#include "m2c_macros.h"

s32 func_002759C4(s32);
s32 func_00430330(s32);

M2C_UNK func_00275A2C();                            /* extern */
M2C_UNK func_00275A74();                            /* extern */
extern M2C_UNK D_800F6CB0;

s32 func_00419960(void) {
    func_002759C4(-1);
    func_00275A74();
    func_00275A2C();
    func_00430330((s32) &D_800F6CB0);
    return 1;
}
