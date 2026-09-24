#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0021A7EC();                            /* extern */

void func_0026E318(void *arg0) {
    func_0021A7EC();
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2100);
}
