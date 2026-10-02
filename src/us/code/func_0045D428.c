#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00423D30();                            /* extern */
extern s32 D_800C2038;

void func_0045D428(s32 arg0) {
    D_800C2038 = arg0;
    func_00423D30();
}
