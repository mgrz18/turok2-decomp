#include "common.h"
#include "m2c_macros.h"

f32 Bits_Normalize(f32 arg0, s32 arg1) {
    return arg0 / (f32) ((1 << arg1) - 1);
}
