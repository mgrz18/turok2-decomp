#include "common.h"
#include "m2c_macros.h"

s32 Cinema_IsPending();                                /* extern */
s32 Cinema_IsActive();                                /* extern */

s32 Cinema_IsIdle(void) {
    s32 var_v0;

    var_v0 = 0;
    if (Cinema_IsActive() == 0) {
        var_v0 = Cinema_IsPending() == 0;
    }
    return var_v0;
}
