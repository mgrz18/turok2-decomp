#include "common.h"
#include "m2c_macros.h"

extern u8 D_8011AAEF;

s32 func_00231778(void *arg0) {
    s32 var_a1;

    var_a1 = 0;
    if (*M2C_FIELD(arg0, s32 **, 0xF8) & 0x02000000) {
        if (M2C_FIELD(arg0, s32 *, 0x40) & 2) {
            var_a1 = 2;
        } else if (D_8011AAEF == 2) {
            var_a1 = 1;
        }
    }
    return var_a1;
}
