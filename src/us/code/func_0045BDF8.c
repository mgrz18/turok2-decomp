#include "common.h"

s32 func_00284174(void);
s32 func_00284240(void);

/* True when the first check passes and the second one fails. */
s32 func_0045BDF8(void) {
    if (func_00284174() != 0) {
        if (func_00284240() == 0) {
            return 1;
        }
    }
    return 0;
}
