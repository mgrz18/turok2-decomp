#include "common.h"
#include "m2c_macros.h"

extern s32 __osViCurr;

s32 __osViGetCurrentContext(void) {
    return __osViCurr;
}
