#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00275A98();                            /* extern */
M2C_UNK func_00275ABC();                            /* extern */
extern s32 D_800F2B34;

void func_00275950(s32 arg0) {
    D_800F2B34 = arg0;
    if (arg0 != 0) {
        func_00275ABC();
        return;
    }
    func_00275A98();
}
