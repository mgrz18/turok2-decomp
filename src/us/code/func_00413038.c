#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800C0AE0;

f32 func_00413038(f32 arg0, f32 arg1, f32 arg2) {
    return (arg0 * (M2C_FIELD(&D_800C0AE0, f32 *, 4) - arg2)) + (arg1 * arg2);
}
