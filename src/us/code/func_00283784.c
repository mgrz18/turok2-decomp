#include "common.h"
#include "m2c_macros.h"

/* qsort-style comparator on the float at 0x210 of the pointed-to objects. */
s32 func_00283784(void **arg0, void **arg1) {
    s32 r = 1;

    if (M2C_FIELD(*arg0, f32 *, 0x210) > M2C_FIELD(*arg1, f32 *, 0x210)) {
        r = -1;
    }
    return r;
}
