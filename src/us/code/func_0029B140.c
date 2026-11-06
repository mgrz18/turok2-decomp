#include "common.h"
#include "m2c_macros.h"
#include "globals/libultra.h"


s32 __osGetCurrFaultedThread(void) {
    return __osFaultedThread;
}
