#include "common.h"
#include "m2c_macros.h"


f32 AI_LerpByAggression(sptr arg0, u8 *arg1, f32 arg2, f32 arg3) {
    return arg2 + arg1[0xC8] * 0.003921569f * (arg3 - arg2);
}
