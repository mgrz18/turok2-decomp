#include "common.h"
#include "m2c_macros.h"

s16 __alSeqReadShort(u8 **arg0) {
    s32 hi = *arg0[2]++;
    s32 lo = *arg0[2]++;

    return (hi << 8) | lo;
}
