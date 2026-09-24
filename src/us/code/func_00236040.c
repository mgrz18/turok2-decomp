#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0026D288(s32, void *);                 /* extern */
M2C_UNK func_0026D2D0(void *, void *);              /* extern */

void func_00236040(s32 arg0, void *arg1, void *arg2) {
    M2C_FIELD(arg1, f32 *, 0x4C) = (f32) (M2C_FIELD(arg1, f32 *, 0x4C) - M2C_FIELD(arg2, f32 *, 0xAC));
    func_0026D2D0(arg1 + 0x40, arg2);
    func_0026D288(arg0 + 0x6A88, arg2);
}
