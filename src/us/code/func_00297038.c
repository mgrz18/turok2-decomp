#include "common.h"
#include "m2c_macros.h"

s32 alFxParam(sptr *arg0, s32 arg1, sptr arg2) {
    if (arg1 == 1) {
        *arg0 = arg2;
    }
    return 0;
}
