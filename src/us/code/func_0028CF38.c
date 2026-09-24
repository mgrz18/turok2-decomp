#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0028B430(u8);                          /* extern */

void func_0028CF38(u8 *arg0) {
    u8 *var_s0;
    u8 temp_a0;

    var_s0 = arg0;
    if (*var_s0 != 0) {
        do {
            temp_a0 = *var_s0;
            var_s0 += 1;
            func_0028B430(temp_a0);
        } while (*var_s0 != 0);
    }
}
