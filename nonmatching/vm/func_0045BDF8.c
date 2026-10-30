/* NON-MATCHING: 8 of 15 words differ: the ROM moves the second call's result to $v1 and returns 1/0 by a branch; GCC always folds it into sltiu. */
#include "common.h"

s32 func_00284174(void);
s32 func_00284240(void);

s32 func_0045BDF8(void) {
    s32 result;

    if (func_00284174() == 0) {
        result = 0;
    } else if (func_00284240() == 0) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}
