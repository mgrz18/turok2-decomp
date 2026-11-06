#include "common.h"
#include "m2c_macros.h"

void CISet__GetSetAndSize(s32 *arg0, s32 **arg1, s32 *arg2) {
    *arg1 = arg0;
    *arg2 = *(s32 *)((s8 *)arg0 + (*arg0 << 2) + 4);
}
