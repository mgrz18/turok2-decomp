#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00220530(s32);                         /* extern */
M2C_UNK func_0027ADA8(s32);                         /* extern */
M2C_UNK func_00281354(s32);                         /* extern */
M2C_UNK func_00288B1C(s32);                         /* extern */
M2C_UNK func_004001D0(s32);                         /* extern */
s32 func_00401C1C(s32);                             /* extern */

void func_0028908C(s32 arg0) {
    s32 temp_s0;

    temp_s0 = arg0 + 0x23DF0;
    func_004001D0(temp_s0);
    func_00220530(arg0 + 0x3C8);
    func_00281354(arg0 + 0x22C00);
    func_0027ADA8(arg0 + 0x19440);
    if (func_00401C1C(temp_s0) == 0) {
        func_00288B1C(arg0);
    }
}
