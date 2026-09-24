#include "common.h"
#include "m2c_macros.h"

extern u8 D_8011AAEF;

void func_002316A0(void *arg0, s32 *arg1) {
    s32 var_a2;

    var_a2 = 0;
    if (*M2C_FIELD(arg0, s32 **, 0xF8) & 0x02000000) {
        if (M2C_FIELD(arg0, s32 *, 0x40) & 2) {
            var_a2 = 2;
        } else if (D_8011AAEF == 2) {
            var_a2 = 1;
        }
    }
    *arg1 |= var_a2 << 0x1E;
}
