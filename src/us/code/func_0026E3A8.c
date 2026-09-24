#include "common.h"
#include "m2c_macros.h"

s32 func_002759C4(s32);

M2C_UNK func_0027580C(u16);                         /* extern */

void func_0026E3A8(s32 arg0, s32 arg1, void *arg2) {
    if ((u16) M2C_FIELD(arg2, u16 *, 6) >= 0x100U) {
        func_0027580C(M2C_FIELD(arg2, u16 *, 6));
        return;
    }
    func_002759C4((s32) M2C_FIELD(arg2, u16 *, 6));
}
