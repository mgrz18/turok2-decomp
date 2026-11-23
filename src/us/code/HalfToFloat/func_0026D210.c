#include "common.h"
#include "m2c_macros.h"

f32 HalfToFloat(s32 arg0) {
    u32 v = arg0 << 16;

    return *(f32 *)&v;
}
