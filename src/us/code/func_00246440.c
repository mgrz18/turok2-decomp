#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00210EF0(void *);                      /* extern */

void func_00246440(void *arg0, f32 arg1) {
    M2C_FIELD(arg0, f32 *, 0x50) = (f32) (M2C_FIELD(arg0, f32 *, 0x50) + arg1);
    func_00210EF0(arg0 + 0x50);
}
