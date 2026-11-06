#include "common.h"
#include "m2c_macros.h"

extern s32 D_800B8924;

s32 __osGetCurrFaultedThread(void) {
    return D_800B8924;
}
