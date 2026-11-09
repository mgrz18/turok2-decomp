#include "common.h"
#include "m2c_macros.h"

void memset(s8 *arg0, s8 arg1, sptr arg2) {
    sptr var_v0;
    s8 *var_a0;

    var_a0 = arg0;
    var_v0 = arg2 - 1;
    if (arg2 != 0) {
        do {
            *var_a0 = arg1;
            var_v0 -= 1;
            var_a0 += 1;
        } while (var_v0 != -1);
    }
}
