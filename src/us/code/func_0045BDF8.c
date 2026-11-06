#include "common.h"

s32 Cinema_IsActive(void);
s32 Cinema_IsFinished(void);

/* True when the first check passes and the second one fails. */
s32 func_0045BDF8(void) {
    if (Cinema_IsActive() != 0) {
        if (Cinema_IsFinished() == 0) {
            return 1;
        }
    }
    return 0;
}
