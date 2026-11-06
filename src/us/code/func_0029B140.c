#include "common.h"
#include "m2c_macros.h"

extern s32 __osFaultedThread;

s32 __osGetCurrFaultedThread(void) {
    return __osFaultedThread;
}
