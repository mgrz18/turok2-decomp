#include "common.h"
#include "m2c_macros.h"
#include "globals/libultra.h"


sptr __osGetCurrFaultedThread(void) {
    return __osFaultedThread;
}
